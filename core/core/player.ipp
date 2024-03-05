Airplane Player::player;

Airplane* Player::Instance()
{
    return &player;
}

HRESULT Player::Create(const LPDIRECT3DDEVICE9 device, LPDIRECT3DVERTEXBUFFER9* vb)
{
    if(FAILED(device->CreateVertexBuffer(sizeof(CustomPolygon) * Cube::POLYGON_COUNT * Airplane::PARTS_COUNT,
                                         0,
                                         D3DFVF_CUSTOMVERTEX,
                                         D3DPOOL_DEFAULT,
                                         vb,
                                         NULL))) {
        return E_FAIL;
    }

    VOID* pVertices;
    if(FAILED((*vb)->Lock(0,
                          sizeof(CustomPolygon) * Cube::POLYGON_COUNT * Airplane::PARTS_COUNT,
                          (void**)&pVertices,
                          0))) {
        return E_FAIL;
    }
    player.SetPosition(D3DXVECTOR3{ 0, 0, -1 });
    player.DrawPrepare(pVertices);
    (*vb)->Unlock();
    return S_OK;
}

int Player::Draw(const LPDIRECT3DDEVICE9 device, const LPDIRECT3DVERTEXBUFFER9 vb)
{
    D3DXMATRIXA16 world, temp;
    D3DXMatrixIdentity(&world);

    D3DXMatrixRotationY(&temp, player.GetRadianAngle().x);
    world = world * temp;

    const D3DXVECTOR3& pos = player.GetPosition();
    D3DXMatrixTranslation(&temp, pos.x, pos.y, pos.z);
    world = world * temp;

    D3DXMatrixScaling(&temp, SIZE, SIZE, SIZE);
    world = world * temp;

    device->SetTransform(D3DTS_WORLD, &world);
    device->SetStreamSource(0, vb, 0, sizeof(CustomVertex));
    device->SetFVF(D3DFVF_CUSTOMVERTEX);

    int index = 0;

    // body / head material
    D3DMATERIAL9 mtrl;
    ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
    mtrl.Diffuse = mtrl.Ambient = D3DCOLORVALUE{ 1.0f, 0.0f, 1.0f, 1.0f };
    device->SetMaterial(&mtrl);

    device->DrawPrimitive(D3DPT_TRIANGLELIST, index, Cube::POLYGON_COUNT); // body
    index += CustomPolygon::VERTEX_COUNT * Cube::POLYGON_COUNT;

    device->DrawPrimitive(D3DPT_TRIANGLELIST, index, Cube::POLYGON_COUNT); // head
    index += CustomPolygon::VERTEX_COUNT * Cube::POLYGON_COUNT;

    mtrl.Diffuse = mtrl.Ambient = D3DCOLORVALUE{ 0.0f, 0.5f, 1.0f, 1.0f }; // wing material
    device->SetMaterial(&mtrl);
    device->DrawPrimitive(D3DPT_TRIANGLELIST, index, Cube::POLYGON_COUNT * 2); // wing

    return index + CustomPolygon::VERTEX_COUNT * Cube::POLYGON_COUNT * 2;
}