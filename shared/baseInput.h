#pragma once



class BaseInput 
{
    public:
    enum class KeyBoardKey 
    {
        // Letters
        KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G,
        KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N,
        KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U,
        KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

        // Numbers
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
        KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

        // Arrow keys
        KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN,

        // Unknown key
        CURRENT_KEY_UNKNOWN,
    };


    struct MouseInfo 
    {
        int x, y;          
        int dx, dy;        
        int scroll;        
        int buttonState;   
    };

    virtual ~BaseInput() = default;

    // Keyboard input
    virtual KeyBoardKey PollKeyBoardInput() = 0;
    virtual bool IsKeyPressed(KeyBoardKey key) const = 0;

    // Mouse input
    virtual MouseInfo PollMouse() = 0;
    virtual bool IsMouseButtonPressed(int button) const = 0;

    protected:
    BaseInput() = default;
};
