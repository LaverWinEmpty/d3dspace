#ifndef LWE__D3D_SPACE__CUBE_HPP__
#define LWE__D3D_SPACE__CUBE_HPP__

#include "common.hpp"

class Cube
{
public:
    static const int POLYGON_COUNT = 12;

public:
    Cube();

    /**
     * @brief create
     *
     * @param vertices vertex buffer allocated
     */
    void DrawPrepare(IN void* vertices);

    /**
     * @brief set position
     *
     * @param float [in] x y z
     */
    void MovePosition(IN float x, IN float y, IN float z);

    /**
     * @brief normal mapping
     */
    void NormalMapping();

    /**
     * @brief Set the Shape object
     *
     * @param ruf [in] position of | Right - Up   - Front |
     * @param rdf [in] position of | Right - Down - Front |
     * @param luf [in] position of | Left  - Up   - Front |
     * @param ldf [in] position of | Left  - Down - Front |
     * @param rub [in] position of | Right - Up   - Back  |
     * @param rdb [in] position of | Right - Down - Back  |
     * @param lub [in] position of | Left  - Up   - Back  |
     * @param ldb [in] position of | Left  - Down - Back  |
     */
    void SetShape(IN D3DXVECTOR3 ruf, IN D3DXVECTOR3 rdf, IN D3DXVECTOR3 luf, IN D3DXVECTOR3 ldf, IN D3DXVECTOR3 rub,
                  IN D3DXVECTOR3 rdb, IN D3DXVECTOR3 lub, IN D3DXVECTOR3 ldb);

private:
    /**
     * @brief polygons
     *
     */
    CustomPolygon list[POLYGON_COUNT];

    /**
     * @brief list pointer
     */
    CustomPolygon *back, *front, *left, *right, *down, *up;
};

#include "cube.ipp"
#endif