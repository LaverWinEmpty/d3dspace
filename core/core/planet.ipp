Planet SolarSystem::list[EIndex::COUNT];

HRESULT SolarSystem::Create(LPDIRECT3DDEVICE9 device)
{
    HRESULT res;

    res = D3DXCreateSphere(device, 0.25f, SPHERE_SLICE, SPHERE_STACK, &list[SUN].mesh, NULL);
    if(FAILED(res)) {
        return res;
    }
    res = D3DXCreateSphere(device, 0.05f, SPHERE_SLICE, SPHERE_STACK, &list[MERCURY].mesh, NULL);
    if(FAILED(res)) {
        return res;
    }
    res = D3DXCreateSphere(device, 0.11f, SPHERE_SLICE, SPHERE_STACK, &list[VENUS].mesh, NULL);
    if(FAILED(res)) {
        return res;
    }
    res = D3DXCreateSphere(device, 0.12f, SPHERE_SLICE, SPHERE_STACK, &list[EARTH].mesh, NULL);
    if(FAILED(res)) {
        return res;
    }
    res = D3DXCreateSphere(device, 0.08f, SPHERE_SLICE, SPHERE_STACK, &list[MARS].mesh, NULL);
    if(FAILED(res)) {
        return res;
    }
    res = D3DXCreateSphere(device, 0.03f, SPHERE_SLICE, SPHERE_STACK, &list[MOON].mesh, NULL);
    if(FAILED(res)) {
        return res;
    }

    list[SUN].color     = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
    list[MERCURY].color = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
    list[VENUS].color   = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);
    list[EARTH].color   = D3DXCOLOR(0.0f, 0.8f, 0.0f, 1.0f);
    list[MARS].color    = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f);
    list[MOON].color    = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

    // location
    for(int i = 0; i < MOON; ++i) {
        list[i].position = D3DXVECTOR3(i * 0.5f, 0.0f, 0.0f);
    }
    list[MOON].position = D3DXVECTOR3(0.2f, 0.0f, 0.0f); // based on parent

    // orbit speed
    // rotate around the x and y axes
    // rotation angle per speed
    list[MERCURY].orbitSpeed.x = 180.f;
    list[VENUS].orbitSpeed.x   = 160.f;
    list[EARTH].orbitSpeed.x   = 140.f;
    list[MARS].orbitSpeed.x    = 120.f;
    list[MOON].orbitSpeed.x    = 240.f;

    // rotation speed
    // apply all at once
    float rotX = 30.f;
    float rotY = 20.f;
    float rotZ = 10.f;
    for(int i = 0; i < MOON; ++i) {
        list[i].rotSpeed = D3DXVECTOR3{ rotX, rotY, rotZ };
    }

    // speed digree to radian
    for(int i = 0; i < COUNT; ++i) {
        D3DXVECTOR3* ptr = &list[i].orbitSpeed;
        ptr->x           = DEGREE_TO_RADIAN(ptr->x);
        ptr->y           = DEGREE_TO_RADIAN(ptr->y);
        ptr->z           = DEGREE_TO_RADIAN(ptr->z);

        ptr    = &list[i].rotSpeed;
        ptr->x = DEGREE_TO_RADIAN(ptr->x);
        ptr->y = DEGREE_TO_RADIAN(ptr->y);
        ptr->z = DEGREE_TO_RADIAN(ptr->z);
    }

    // set parents
    list[MOON].parent = &list[EARTH];
    return S_OK;
}

void SolarSystem::DrawWithUpdate(const LPDIRECT3DDEVICE9 device)
{
    float delta = Timer::DeltaTime();

    D3DXMATRIXA16 world, tr, rot, orbit, allRot;

    D3DXMatrixRotationZ(&allRot, DEGREE_TO_RADIAN(20)); // tilted diagonally

    for(int i = 0; i < COUNT; ++i) {
        Planet* cur = &list[i];
        D3DXMatrixIdentity(&world);

        // material
        D3DMATERIAL9 mtrl;
        ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
        mtrl.Diffuse = mtrl.Ambient = cur->color;
        if(i == SUN) {
            mtrl.Emissive = cur->color;
        }
        device->SetMaterial(&mtrl);

        // angle over 360 => set 0
        cur->rotCurrent += cur->rotSpeed * delta;
        if(cur->rotCurrent.x >= ROTATE_MAX) cur->rotCurrent.x -= ROTATE_MAX;
        if(cur->rotCurrent.y >= ROTATE_MAX) cur->rotCurrent.y -= ROTATE_MAX;
        if(cur->rotCurrent.z >= ROTATE_MAX) cur->rotCurrent.z -= ROTATE_MAX;

        D3DXMatrixRotationX(&rot, cur->rotCurrent.x);
        world = world * rot;
        D3DXMatrixRotationY(&rot, cur->rotCurrent.y);
        world = world * rot;
        D3DXMatrixRotationZ(&rot, cur->rotCurrent.z);
        world = world * rot;

        D3DXMatrixTranslation(&tr, cur->position.x, cur->position.y, cur->position.z);
        world = world * tr;

        cur->orbitCurrent += cur->orbitSpeed * delta;
        if(cur->orbitCurrent.x >= ROTATE_MAX) {
            cur->orbitCurrent.x -= ROTATE_MAX;
        }

        D3DXMatrixRotationY(&orbit, cur->orbitCurrent.x);
        world = world * orbit;

        // additional work for satellites
        if(cur->parent) {
            Planet*      parent    = cur->parent;
            D3DXVECTOR3* parentPos = &parent->position;

            D3DXMatrixTranslation(&tr, parentPos->x, parentPos->y, parentPos->z); // move to parent
            world = world * tr;                                                   // apply

            D3DXMatrixRotationY(&orbit, parent->orbitCurrent.x); // rotate exactly like parent
            world = world * orbit;                               // apply
        }

        world = world * allRot; // apply

        device->SetTransform(D3DTS_WORLD, &world); // apply
        list[i].mesh->DrawSubset(0);               // draw
    }
}
