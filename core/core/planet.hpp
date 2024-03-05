#ifndef LWE__D3D_SPACE__PLANET_HPP__
#define LWE__D3D_SPACE__PLANET_HPP__

#include "common.hpp"

struct Planet
{
    LPD3DXMESH  mesh = nullptr;
    D3DXCOLOR   color;
    D3DXVECTOR3 position;
    D3DXVECTOR3 rotSpeed;
    D3DXVECTOR3 rotCurrent;
    D3DXVECTOR3 orbitSpeed;
    D3DXVECTOR3 orbitCurrent;
    Planet*     parent = nullptr;
};

class SolarSystem
{
private:
    SolarSystem();
    ~SolarSystem();

public:
    enum EIndex
    {
        SUN,
        MERCURY,
        VENUS,
        EARTH,
        MARS,
        MOON,
        COUNT
    };

public:
    /**
     * @brief init
     *
     * @param  LPDIRECT3DDEVICE9 device
     * @return HRESULT
     */
    static HRESULT Create(const LPDIRECT3DDEVICE9);

public:
    /**
     * @brief draw and update
     *
     * @param  LPDIRECT3DDEVICE9 device
     */
    static void DrawWithUpdate(const LPDIRECT3DDEVICE9);

private:
    static Planet list[COUNT];
};

#include "planet.ipp"
#endif