#ifndef LWE__D3D_SPACE__AIRPLANE_HPP__
#define LWE__D3D_SPACE__AIRPLANE_HPP__

#include "cube.hpp"

class Airplane
{
public:
    static constexpr int   PARTS_COUNT = 4;
    static constexpr float SPEED       = 5.f;

    Airplane()
    {
        body.SetShape(
            // front
            D3DXVECTOR3{ 0.05f, 0.05f, 0.2f },
            D3DXVECTOR3{ 0.05f, 0.05f, 0.2f },
            D3DXVECTOR3{ 0.05f, 0.05f, 0.2f },
            D3DXVECTOR3{ 0.05f, 0.05f, 0.2f },
            // back
            D3DXVECTOR3{ 0.05f, 0.05f, 0.f },
            D3DXVECTOR3{ 0.05f, 0.05f, 0.f },
            D3DXVECTOR3{ 0.05f, 0.05f, 0.f },
            D3DXVECTOR3{ 0.05f, 0.05f, 0.f });
        body.MovePosition(0.f, 0.f, -0.1f);

        head.SetShape(
            // front
            D3DXVECTOR3{ 0.025f, 0.0f, 0.2f },
            D3DXVECTOR3{ 0.025f, 0.05f, 0.2f },
            D3DXVECTOR3{ 0.025f, 0.0f, 0.2f },
            D3DXVECTOR3{ 0.025f, 0.05f, 0.2f },
            // back
            D3DXVECTOR3{ 0.05f, 0.05f, 0.f },
            D3DXVECTOR3{ 0.05f, 0.05f, 0.f },
            D3DXVECTOR3{ 0.05f, 0.05f, 0.f },
            D3DXVECTOR3{ 0.05f, 0.05f, 0.f });
        head.MovePosition(0.f, 0.f, 0.1f);

        left.SetShape(
            // front
            D3DXVECTOR3{ 0.2f, 0.005f, 0.2f },
            D3DXVECTOR3{ 0.2f, 0.005f, 0.2f },
            D3DXVECTOR3{ 0.0f, 0.005f, 0.1f },
            D3DXVECTOR3{ 0.0f, 0.005f, 0.1f },
            // back
            D3DXVECTOR3{ 0.2f, 0.005f, 0.f },
            D3DXVECTOR3{ 0.2f, 0.005f, 0.f },
            D3DXVECTOR3{ 0.0f, 0.005f, 0.f },
            D3DXVECTOR3{ 0.0f, 0.005f, 0.f });
        left.MovePosition(-0.25f, 0.f, -0.1f);

        right.SetShape(
            // front
            D3DXVECTOR3{ 0.1f, 0.005f, 0.1f },
            D3DXVECTOR3{ 0.1f, 0.005f, 0.1f },
            D3DXVECTOR3{ 0.1f, 0.005f, 0.2f },
            D3DXVECTOR3{ 0.1f, 0.005f, 0.2f },

            // back
            D3DXVECTOR3{ 0.1f, 0.005f, 0.f },
            D3DXVECTOR3{ 0.1f, 0.005f, 0.f },
            D3DXVECTOR3{ 0.1f, 0.005f, 0.f },
            D3DXVECTOR3{ 0.1f, 0.005f, 0.f });
        right.MovePosition(0.15f, 0.f, -0.1f);
    }

    void DrawPrepare(IN void* vertices)
    {
        CustomPolygon* ptr = static_cast<CustomPolygon*>(vertices);
        body.DrawPrepare(ptr);
        ptr += Cube::POLYGON_COUNT;
        head.DrawPrepare(ptr);
        ptr += Cube::POLYGON_COUNT;
        left.DrawPrepare(ptr);
        ptr += Cube::POLYGON_COUNT;
        right.DrawPrepare(ptr);
    }

    void               SetPosition(const D3DXVECTOR3& param) { pos = param; }
    void               SetRadianAngle(const D3DXVECTOR3& param) { angle = param; }
    const D3DXVECTOR3& GetPosition() const { return pos; }
    const D3DXVECTOR3& GetRadianAngle() const { return angle; }

private:
    Cube        body, head, left, right;
    D3DXVECTOR3 pos, angle;
};

#endif