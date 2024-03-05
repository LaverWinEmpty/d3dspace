#ifndef LWE__D3D_SPACE__INPUT_HPP__
#define LWE__D3D_SPACE__INPUT_HPP__

#include "unordered_map"
#include "string"
#include "player.hpp"
#include "command.hpp"

#define DECLARE_COMMAND(name)                                                                                          \
    class name: public ICommand                                                                                        \
    {                                                                                                                  \
    public:                                                                                                            \
        void Execute() override;                                                                                       \
    };

class InputManager
{
public:
    DECLARE_COMMAND(MoveLeft);
    DECLARE_COMMAND(MoveRight);
    DECLARE_COMMAND(MoveFoward);
    DECLARE_COMMAND(MoveBackward);
    DECLARE_COMMAND(MoveUp);
    DECLARE_COMMAND(MoveDown);
    DECLARE_COMMAND(Exit);

public:
    static const char INPUT_SETTING_FILE_PATH[];

public:
    static constexpr float MOUSE_SPEED = 20.f;

private:
    std::unordered_map<int, ICommand*> dic;
    int                                mouseViewToggle;

public:
    void KeyBinding();
    void Input();

private:
    InputManager() {}
    ~InputManager() {}

public:
    void Initialize(IN HWND);

public:
    /**
     * @brief cursor off
     */
    void CursorOff();

    /**
     * @brief cursor on
     */
    void CursorOn();

    /**
     * @brief cursor move to center
     */
    void SetCursorToCenter();

    /**
     * @brief get the distnace of mouse moved
     *
     * @return POINTFLOAT
     */
    POINTFLOAT GetMouseMoved();

public:
    /**
     * @brief get instance
     */
    static InputManager* Instance();

    /**
     * @brief create a instance
     */
    static void CreateInstance();

    /**
     * @brief destroy a instance
     */
    static void DestroyInstance();

private:
    /**
     * @brief signleton instance
     */
    static InputManager* instance;

private:
    /**
     * @brief center pos
     */
    POINT center = { 0 };

    /**
     * @brief input direction
     */
    D3DXVECTOR3 dir;

private:
    /**
     * @brief about check cursor show
     */
    bool isShowCursor = true;

    /**
     * @brief about check cursor show
     */
    bool isPressedMouseShowButton = true;

private:
    HWND hWnd;
};

#include "input.ipp"

#undef DECLARE_COMMAND
#endif