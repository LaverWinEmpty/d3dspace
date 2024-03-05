#ifndef LWE__D3D_SPACE__PLAYER_HPP__
#define LWE__D3D_SPACE__PLAYER_HPP__

#include "airplane.hpp"

class Player
{
public:
    static constexpr float SPEED = 5.f;
    static constexpr float SIZE  = 0.5f;

public:
    Player() = delete;

public:
    /**
     * @brief init
     *
     * @param LPDIRECT3DDEVICE9       [in]  device
     * @param LPDIRECT3DVERTEXBUFFER9 [out] vertex buffer
     * @return HRESULT
     */
    static HRESULT Create(IN const LPDIRECT3DDEVICE9, OUT LPDIRECT3DVERTEXBUFFER9*);

    /**
     * @brief
     *
     * @param LPDIRECT3DDEVICE9       [in]
     * @param LPDIRECT3DVERTEXBUFFER9 [in]
     * @return int next vertex index of DrawPrimitive
     */
    static int Draw(IN const LPDIRECT3DDEVICE9, IN const LPDIRECT3DVERTEXBUFFER9);

public:
    /**
     * @brief get instance
     */
    static Airplane* Instance();

private:
    /**
     * @brief instance
     */
    static Airplane player;
};

#include "player.ipp"
#endif