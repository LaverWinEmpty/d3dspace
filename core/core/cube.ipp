Cube::Cube(): back(list + 0), front(list + 2), left(list + 4), right(list + 6), down(list + 8), up(list + 10) {}

void Cube::DrawPrepare(void* vertices)
{
    memcpy(vertices, list, sizeof(CustomPolygon) * POLYGON_COUNT);
}

void Cube::MovePosition(float x, float y, float z)
{
    for(int i = 0; i < POLYGON_COUNT; ++i) {
        for(int j = 0; j < 3; ++j) {
            list[i].vertex[j].position.x += x;
            list[i].vertex[j].position.y += y;
            list[i].vertex[j].position.z += z;
        }
    }
}

void Cube::NormalMapping()
{
    D3DXVECTOR3 p, n; // positive, negative

    static auto fpGetNormal = [](CustomVertex* polygon) {
        D3DXVECTOR3 u, v, res;
        u = polygon[0].position - polygon[1].position;
        v = polygon[0].position - polygon[2].position;
        D3DXVec3Cross(&res, &u, &v);
        D3DXVec3Normalize(&res, &res);
        return res;
    };

    p = fpGetNormal(up->vertex); //
    n = p * -1.f;                // -
    for(int i = 0; i < 3; ++i) {
        up[0].vertex[i].normal = up[1].vertex[i].normal = p;     // up is positive
        down[0].vertex[i].normal = down[1].vertex[i].normal = n; // down id negative
    }

    p = fpGetNormal(right->vertex); // +
    n = p * -1.f;                   // -
    for(int i = 0; i < 3; ++i) {
        right[0].vertex[i].normal = right[1].vertex[i].normal = n; // left is negative
        left[0].vertex[i].normal = left[1].vertex[i].normal = p;   // right is positive
    }

    p = fpGetNormal(front->vertex); // +
    n = p * -1.f;                   // -
    for(int i = 0; i < 3; ++i) {
        front[0].vertex[i].normal = front[1].vertex[i].normal = n; // front is negative
        back[0].vertex[i].normal = back[1].vertex[i].normal = p;   // back is positive
    }
}

// Size (abs)
void Cube::SetShape(IN D3DXVECTOR3 ruf, IN D3DXVECTOR3 rdf, IN D3DXVECTOR3 luf, IN D3DXVECTOR3 ldf, IN D3DXVECTOR3 rub,
                    IN D3DXVECTOR3 rdb, IN D3DXVECTOR3 lub, IN D3DXVECTOR3 ldb)
{
    // front
    rdf.y = -rdf.y;                 // down
    luf.x = -luf.x;                 // left
    ldf.x = -ldf.x, ldf.y = -ldf.y; // left, down

    // back
    rub.z = -rub.z;
    rdb.z = -rdb.z, rdb.y = -rdb.y;                 // down
    lub.z = -lub.z, lub.x = -lub.x;                 // left
    ldb.z = -ldb.z, ldb.x = -ldb.x, ldb.y = -ldb.y; // left, down

    back[0].vertex[0].position = ldb;
    back[0].vertex[1].position = lub;
    back[0].vertex[2].position = rdb;

    back[1].vertex[0].position = rub;
    back[1].vertex[1].position = lub;
    back[1].vertex[2].position = rdb;

    front[0].vertex[0].position = ldf;
    front[0].vertex[1].position = luf;
    front[0].vertex[2].position = rdf;

    front[1].vertex[0].position = ruf;
    front[1].vertex[1].position = luf;
    front[1].vertex[2].position = rdf;

    left[0].vertex[0].position = ldf;
    left[0].vertex[1].position = luf;
    left[0].vertex[2].position = ldb;

    left[1].vertex[0].position = lub;
    left[1].vertex[1].position = luf;
    left[1].vertex[2].position = ldb;

    right[0].vertex[0].position = rdf;
    right[0].vertex[1].position = ruf;
    right[0].vertex[2].position = rdb;

    right[1].vertex[0].position = rub;
    right[1].vertex[1].position = ruf;
    right[1].vertex[2].position = rdb;

    up[0].vertex[0].position = lub;
    up[0].vertex[1].position = luf;
    up[0].vertex[2].position = rub;

    up[1].vertex[0].position = ruf;
    up[1].vertex[1].position = luf;
    up[1].vertex[2].position = rub;

    down[0].vertex[0].position = ldf;
    down[0].vertex[1].position = rdf;
    down[0].vertex[2].position = ldb;

    down[1].vertex[0].position = rdb;
    down[1].vertex[1].position = rdf;
    down[1].vertex[2].position = ldb;

    NormalMapping();
}