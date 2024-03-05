#ifndef LWE__D3D_SPACE__CORE_HPP__
#define LWE__D3D_SPACE__CORE_HPP__

#include "player.hpp"
#include "planet.hpp"
#include "input.hpp"

class Core
{
private:
    Core() {}
    ~Core() {}

public:
    static Core* Instance();
    static void  CreateInstance();
    static void  DestroyInstance();

public:
    void Cleanup();

public:
    HRESULT InitD3D();
    HRESULT InitGeometry();
    HRESULT SetupLight();
    HRESULT SetupCamera();

public:
    void Render();

public:
    static LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    INT                   Run();

private:
    HWND                    hWnd;
    LPDIRECT3DDEVICE9       device;
    LPDIRECT3D9             d3d;
    LPDIRECT3DVERTEXBUFFER9 vbuf;

private:
    static Core* instance;
};

#include "core.ipp"
#endif