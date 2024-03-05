#ifndef LWE__D3D_SPACE__COMMON_HPP__
#define LWE__D3D_SPACE__COMMON_HPP__

#include "d3d/d3d/inc/d3dx9.h"
#include "mmsystem.h"
#include "strsafe.h"

#ifdef _WIN64
#    pragma comment(lib, "d3d/d3d/lib/x64/d3d9")
#    pragma comment(lib, "d3d/d3d/lib/x64/d3dx9")
#else
#    pragma comment(lib, "d3d/d3d/lib/x86/d3d9")
#    pragma comment(lib, "d3d/d3d/lib/x86/d3dx9")
#endif

#define DEGREE_TO_RADIAN(value) value * 0.017453f
#define RADIAN_TO_DEGREE(value) value * 57.29577f

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL)

#define SPHERE_SLICE 16
#define SPHERE_STACK 16

static constexpr float ROTATE_MAX = DEGREE_TO_RADIAN(360.f);

struct CustomVertex
{
    /**
     * @brief the 3d position for the vertex
     */
    D3DXVECTOR3 position;

    /**
     * @brief the surface normal for the vertex
     */
    D3DXVECTOR3 normal;
};

struct CustomPolygon
{
    static constexpr int VERTEX_COUNT = 3;

    /**
     * @brief vertex
     */
    CustomVertex vertex[VERTEX_COUNT];
};

class Timer
{
public:
    Timer() = delete;

private:
    static LONGLONG frecuency;
    static LONGLONG last;
    static LONGLONG curr;
    static float    delta;
    static float    timeScale;

public:
    /**
     * @brief init
     *
     * @param float time scale
     */
    static void SetTimeScale(IN float);

public:
    /**
     * @brief calculate delta time
     */
    static void Update();

public:
    /**
     * @brief getter
     *
     * @return sec
     */
    static float DeltaTime();
};

#include "common.ipp"
#endif