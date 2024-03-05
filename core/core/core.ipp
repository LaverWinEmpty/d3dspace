Core* Core::instance = nullptr;

Core* Core::Instance()
{
    return instance;
}

void Core::CreateInstance()
{
    instance = new Core;
    InputManager::CreateInstance();
}

void Core::DestroyInstance()
{
    InputManager::DestroyInstance();
    instance->Cleanup();
    delete instance;
}

void Core::Cleanup()
{
    if(vbuf) {
        vbuf->Release();
    }

    if(device) {
        device->Release();
    }

    if(d3d) {
        d3d->Release();
    }
}

HRESULT Core::InitD3D()
{
    if(NULL == (d3d = Direct3DCreate9(D3D_SDK_VERSION))) {
        return E_FAIL;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed               = TRUE;
    d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat       = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    if(FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT,
                                D3DDEVTYPE_HAL,
                                hWnd,
                                D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                &d3dpp,
                                &device))) {
        return E_FAIL;
    }

    device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    device->SetRenderState(D3DRS_ZENABLE, TRUE);
    // g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    return S_OK;
}

HRESULT Core::InitGeometry()
{
    HRESULT res;
    res = Player::Create(device, &vbuf);
    if(FAILED(res)) {
        return res;
    }
    res = SolarSystem::Create(device);
    if(FAILED(res)) {
        return res;
    }
    return S_OK;
}

HRESULT Core::SetupCamera()
{
    HRESULT res;

    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity(&matWorld);

    res = device->SetTransform(D3DTS_WORLD, &matWorld);
    if(FAILED(res)) {
        return res;
    }

    D3DXMATRIXA16 mat;

    // pov
    D3DXVECTOR3 vEyePt    = { 0, 1, -2 };
    D3DXVECTOR3 vLookatPt = { 0, 0, 2 };
    D3DXVECTOR3 angle     = Player::Instance()->GetRadianAngle();

    // mouse move y -> rotate x
    D3DXMatrixRotationX(&mat, angle.y);
    D3DXVec3TransformNormal(&vLookatPt, &vLookatPt, &mat);
    D3DXVec3TransformNormal(&vEyePt, &vEyePt, &mat);

    // mouse move x -> rotate y
    D3DXMatrixRotationY(&mat, angle.x);
    D3DXVec3TransformNormal(&vLookatPt, &vLookatPt, &mat);
    D3DXVec3TransformNormal(&vEyePt, &vEyePt, &mat);

    // set position
    vLookatPt += Player::Instance()->GetPosition() * Player::SIZE;
    vEyePt    += Player::Instance()->GetPosition() * Player::SIZE;

    D3DXVECTOR3   vUpVec(0.0f, 1.0f, 0.0f);
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
    res = device->SetTransform(D3DTS_VIEW, &matView);
    if(FAILED(res)) {
        return res;
    }

    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
    res = device->SetTransform(D3DTS_PROJECTION, &matProj);
    if(FAILED(res)) {
        return res;
    }

    return S_OK;
}

HRESULT Core::SetupLight()
{
    D3DLIGHT9 light;
    ZeroMemory(&light, sizeof(D3DLIGHT9));
    light.Type         = D3DLIGHT_POINT;
    light.Diffuse.r    = 1.0f;
    light.Diffuse.g    = 0.5f;
    light.Diffuse.b    = 0.3f;
    light.Attenuation0 = 0.3f;
    light.Range        = 20.0f;

    HRESULT res;

    res = device->SetLight(0, &light);
    if(FAILED(res)) {
        return res;
    }
    res = device->LightEnable(0, TRUE);
    if(FAILED(res)) {
        return res;
    }
    res = device->SetRenderState(D3DRS_LIGHTING, TRUE);
    if(FAILED(res)) {
        return res;
    }
    res = device->SetRenderState(D3DRS_AMBIENT, 0x00202020);
    if(FAILED(res)) {
        return res;
    }

    return S_OK;
}

void Core::Render()
{
    device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x0F0F0FFF, 1.0f, 0);

    if(SUCCEEDED(device->BeginScene())) {
        SetupLight();
        SetupCamera();

        SolarSystem::DrawWithUpdate(device);
        Player::Draw(device, vbuf);

        device->EndScene();
    }

    device->Present(NULL, NULL, NULL, NULL);
}

LRESULT WINAPI Core::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg) {
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT Core::Run()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      L"D3DSpace",        NULL };
    RegisterClassEx(&wc);

    hWnd = CreateWindow(L"D3DSpace",
                        L"D3DSpace",
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        960,
                        960,
                        NULL,
                        NULL,
                        wc.hInstance,
                        NULL);

    if(SUCCEEDED(InitD3D())) {
        if(SUCCEEDED(InitGeometry())) {
            ShowWindow(hWnd, SW_SHOWDEFAULT);
            UpdateWindow(hWnd);

            Timer::SetTimeScale(1);
            InputManager::Instance()->Initialize(hWnd);
            InputManager::Instance()->KeyBinding();

            MSG msg;
            ZeroMemory(&msg, sizeof(msg));
            while(msg.message != WM_QUIT) {
                if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                else {
                    Timer::Update();
                    InputManager::Instance()->Input();
                    Render();
                }
            }
        }
    }

    UnregisterClass(L"D3DSpace", wc.hInstance);
    return 0;
}