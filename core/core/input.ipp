const char InputManager::INPUT_SETTING_FILE_PATH[] = "./input.ini";

InputManager* InputManager::instance = nullptr;

void InputManager::MoveLeft::Execute()
{
    InputManager::instance->dir.x -= Player::SPEED * Timer::DeltaTime();
}

void InputManager::MoveRight::Execute()
{
    InputManager::instance->dir.x += Player::SPEED * Timer::DeltaTime();
}

void InputManager::MoveFoward::Execute()
{
    InputManager::instance->dir.z += Player::SPEED * Timer::DeltaTime();
}

void InputManager::MoveBackward::Execute()
{
    InputManager::instance->dir.z -= Player::SPEED * Timer::DeltaTime();
}

void InputManager::MoveUp::Execute()
{
    InputManager::instance->dir.y += Player::SPEED * Timer::DeltaTime();
}

void InputManager::MoveDown::Execute()
{
    InputManager::instance->dir.y -= Player::SPEED * Timer::DeltaTime();
}

void InputManager::Exit::Execute()
{
    SendMessage(InputManager::instance->hWnd, WM_DESTROY, 0, 0);
}

void InputManager::Initialize(HWND param)
{
    hWnd = param;

    RECT rt;
    GetWindowRect(hWnd, &rt);
    center.x = ((rt.right - rt.left) >> 1) + rt.left;
    center.y = ((rt.bottom - rt.top) >> 1) + rt.top;
    SetCursorPos(center.x, center.y);
    CursorOff();
}

InputManager* InputManager::Instance()
{
    return instance;
}

void InputManager::CreateInstance()
{
    if(!instance) {
        instance = new InputManager;
    }
}

void InputManager::DestroyInstance()
{
    if(instance) {
        delete instance;
        instance = nullptr;
    }
}

void InputManager::CursorOff()
{
    if(isShowCursor == false) {
        return;
    }
    isShowCursor = false;
    ShowCursor(isShowCursor);
}

void InputManager::CursorOn()
{
    if(isShowCursor == true) {
        return;
    }
    isShowCursor = true;
    ShowCursor(isShowCursor);
}

POINTFLOAT InputManager::GetMouseMoved()
{
    POINT current;
    GetCursorPos(&current);

    float x = (current.x - center.x) * MOUSE_SPEED;
    float y = (current.y - center.y) * MOUSE_SPEED;

    return { x, y };
}

void InputManager::SetCursorToCenter()
{
    SetCursorPos(center.x, center.y);
}

void InputManager::KeyBinding()
{
    auto fpGetValue = [](const char* key) {
        // key list
        static const std::unordered_map<std::string, int> list = {
            {  "SPACE",   VK_SPACE},
            {  "SHIFT",   VK_SHIFT},
            {   "CTRL", VK_CONTROL},
            {    "ALT",    VK_MENU},
            {  "ENTER",  VK_RETURN},
            {"LBUTTON", VK_LBUTTON},
            {"RBUTTON", VK_RBUTTON},
            {   "LEFT",    VK_LEFT},
            {  "RIGHT",   VK_RIGHT},
            {     "UP",      VK_UP},
            {   "DOWN",    VK_DOWN},
            {    "ESC",  VK_ESCAPE},
            { "ESCAPE",  VK_ESCAPE},
        };

        // key
        char value[MAX_PATH] = { 0 };

        // input
        GetPrivateProfileStringA("Input", key, "", &value[0], MAX_PATH, const_cast<LPSTR>(INPUT_SETTING_FILE_PATH));

        // to uppercase
        for(int i = 0; value[i]; ++i) {
            value[i] &= ~0x20;
        }

        auto it = list.find(value);
        if(it != list.end()) {
            return it->second;
        }
        else {
            return static_cast<int>(value[0]); // 'A' => 65
        }
    };

    dic.insert({ fpGetValue("Up"), new MoveUp() });
    dic.insert({ fpGetValue("Down"), new MoveDown() });
    dic.insert({ fpGetValue("Left"), new MoveLeft() });
    dic.insert({ fpGetValue("Right"), new MoveRight() });
    dic.insert({ fpGetValue("Forward"), new MoveFoward() });
    dic.insert({ fpGetValue("Backward"), new MoveBackward() });
    dic.insert({ fpGetValue("Exit"), new Exit() });

    mouseViewToggle = fpGetValue("MouseView");
}

void InputManager::Input()
{
    float         delta = Timer::DeltaTime();
    D3DXMATRIXA16 mat;
    dir = { 0, 0, 0 };

    // check press
    if(GetAsyncKeyState(mouseViewToggle)) {
        isPressedMouseShowButton = true;
        CursorOn();
        return; // ignore
    }

    else if(isPressedMouseShowButton) {
        isPressedMouseShowButton = false;
        SetCursorToCenter();
        CursorOff();
    }

    for(auto i = dic.begin(); i != dic.end(); ++i) {
        if(GetAsyncKeyState(i->first)) {
            (i->second)->Execute();
        }
    }

    // get value of mouse moved
    auto mouse  = GetMouseMoved();
    auto angle  = Player::Instance()->GetRadianAngle();
    angle.x    += DEGREE_TO_RADIAN(mouse.x * Timer::DeltaTime());

    // player rotate from the value
    D3DXMatrixRotationY(&mat, angle.x);
    D3DXVec3TransformCoord(&dir, &dir, &mat);

    // moveta
    auto moved  = Player::Instance()->GetPosition();
    moved      += dir;

    // mouse y => camera only
    angle.y += DEGREE_TO_RADIAN(mouse.y * Timer::DeltaTime());

    // mouse to center
    SetCursorToCenter();

    // value correct
    if(angle.x >= ROTATE_MAX) {
        angle.x -= ROTATE_MAX;
    }
    if(angle.x <= ROTATE_MAX) {
        angle.x += ROTATE_MAX;
    }
    if(angle.y <= DEGREE_TO_RADIAN(-50)) {
        angle.y = DEGREE_TO_RADIAN(-50);
    }
    if(angle.y >= DEGREE_TO_RADIAN(50)) {
        angle.y = DEGREE_TO_RADIAN(50);
    }

    // update
    Player::Instance()->SetRadianAngle(angle);
    Player::Instance()->SetPosition(moved);
}