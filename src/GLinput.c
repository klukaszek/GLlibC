#include "GLinput.h"
#include "GLwindow.h"
/* ------------------------------------------- GLFW Callbacks -----------------------------------------------*/

// simple key_pressed callback for glfwSetKeyCallback(window, callback);
void key_pressed_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    //Toggle Fullscreen with F3
    if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
    {
        toggle_fullscreen(window); //GLwindow.c function
        return;
    }
    else if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        KeyEvent *e = create_key_event(key, scancode, action, mods);

        log_key_event(e);
        free(e);
    }
}

// simple mouse callback for glfwSetMouseCallback(window, callback);
void mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
    // check if mouse button pressed has event, return if no event
    if (button != GLFW_MOUSE_BUTTON_LEFT && button != GLFW_MOUSE_BUTTON_RIGHT && button != GLFW_MOUSE_BUTTON_MIDDLE) return;

    // check for mouse button pressed
    MouseEvent *e = create_mouse_event(window, button, action, mods);
    log_mouse_event(e);

    free(e);
}

// simple scroll callback for glfwSetScrollCallback(window, callback);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    log_info("xOffset: %.1f, yOffset: %.1f", xoffset, yoffset);
}

void controller_callback(int jid, int event)
{
    if (event == GLFW_CONNECTED)
    {
        if(glfwJoystickIsGamepad(jid))
        {
            const char * name = glfwGetGamepadName(jid);
            log_debug("%s Connected", name);
        }
    }
    else if (event == GLFW_DISCONNECTED)
    {
        if (glfwJoystickIsGamepad(jid))
        {
            const char *name = glfwGetGamepadName(jid);
            log_debug("%s Disconnected", name);
        }
    }
}

/* ------------------------------------------ Key Event Functions ---------------------------------------------*/

// returns previous key event passed to window (GLFW implementation, see directX for windows)
KeyEvent *create_key_event(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_UNKNOWN) return NULL;
    KeyEvent *key_e = malloc(sizeof(KeyEvent));

    key_e->key = key;
    key_e->scancode = scancode;
    key_e->action = action;
    key_e->mods = mods;

    return key_e;
}

// key event to str
char *key_to_str(KeyEvent *e)
{
    char *str = malloc(sizeof(char) * 36);

    sprintf(str, "Key Event: %c, %d, %d, %d, %d", e->key, e->key, e->scancode, e->action, e->mods);
    return str;
}

//print key event
void log_key_event(KeyEvent *e)
{
    char *str = key_to_str(e);
    log_debug(str); // print letter, ascii val, and pointer to key event
    free(str);
}

/* ------------------------------------------- Mouse Event Functions -----------------------------------------------*/

// mouse button event
MouseEvent *create_mouse_event(GLFWwindow *window, int button, int action, int mods)
{
    if (window == NULL) return NULL;

    MouseEvent *mouse_e = malloc(sizeof(MouseEvent));

    mouse_e->window = window;
    mouse_e->button = button;
    mouse_e->action = action;
    mouse_e->mods = mods;

    return mouse_e;
}

// logs mouse event to console
void log_mouse_event(MouseEvent *e)
{
    const char *str;

    //check for button pressed string
    if(e->button == GLFW_MOUSE_BUTTON_LEFT) str = "LMB";
    else if(e->button == GLFW_MOUSE_BUTTON_RIGHT) str = "RMB";
    else str = "MMB";

    //get position of mouse in window
    double x, y;
    glfwGetCursorPos(e->window, &x, &y);

    //log mouse event
    if (e->action == GLFW_PRESS) log_info("%s Pressed at (%.1f, %.1f)", str, x, y);
    if (e->action == GLFW_RELEASE) log_info("%s Released at (%.1f, %.1f)", str, x, y);
}

/* ------------------------------------------- Controller Event Functions -----------------------------------------------*/

// returns controller event
ControllerEvent *get_controller_event(int jid)
{
    GLFWgamepadstate state;

    //function already checks if joystick is connected so it does not have to be done at the top
    if(glfwGetGamepadState(jid, &state))
    {

        ControllerEvent *e = malloc(sizeof(ControllerEvent));

        e->buttons = state.buttons;
        e->axes = state.axes;
        e->jid = jid;

        return e;
    }

    return NULL;
}

// logs controller event information
void log_controller_event(ControllerEvent *e)
{

    double t = THUMB_THRESH;

    if (e != NULL)
    {

        // handle utility buttons
        if ((e->buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_PRESS) | (e->buttons[GLFW_GAMEPAD_BUTTON_BACK] == GLFW_PRESS))
        {
            log_debug("Controller %d // Start: %s | Select: %s", e->jid, e->buttons[GLFW_GAMEPAD_BUTTON_START] ? "Pressed" : "Released", e->buttons[GLFW_GAMEPAD_BUTTON_BACK] ? "Pressed" : "Released");
        }

        // handle thumbstick buttons
        if ((e->buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] == GLFW_PRESS) | (e->buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] == GLFW_PRESS))
        {
            log_debug("Controller %d // L3: %s | R3: %s", e->jid, e->buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] ? "Pressed" : "Released", e->buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] ? "Pressed" : "Released");
        }

        // handle left thumbstick values
        if ((e->axes[GLFW_GAMEPAD_AXIS_LEFT_X] > t) | (e->axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -t) | (e->axes[GLFW_GAMEPAD_AXIS_LEFT_Y] > t) | (e->axes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -t))
        {
            log_debug("Controller %d // L Thumb X: %.2f | L Thumb Y: %.2f", e->jid, e->axes[GLFW_GAMEPAD_AXIS_LEFT_X], e->axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
        }

        // handle right thumbstick values
        if ((e->axes[GLFW_GAMEPAD_AXIS_RIGHT_X] > t) | (e->axes[GLFW_GAMEPAD_AXIS_RIGHT_X] < -t) | (e->axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] > t) | (e->axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] < -t))
        {
            log_debug("Controller %d // R Thumb X: %.2f | R Thumb Y: %.2f", e->jid, e->axes[GLFW_GAMEPAD_AXIS_RIGHT_X], e->axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
        }

        // handle shoulder buttons
        if ((e->buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] == GLFW_PRESS) | (e->buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] == GLFW_PRESS) | (e->axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > -1) | (e->axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > -1))
        {
            log_debug("Controller %d // R1: %s | L1: %s | R2: %.2f | L2: %.2f", e->jid, e->buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] ? "Pressed" : "Released", e->buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] ? "Pressed" : "Released", e->axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER], e->axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER]);
        }

        // handle face buttons
        if ((e->buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS) | (e->buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS) | (e->buttons[GLFW_GAMEPAD_BUTTON_Y] == GLFW_PRESS) | (e->buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_PRESS))
        {
            log_debug("Controller %d // A: %s | B: %s | Y: %s | X: %s", e->jid, e->buttons[GLFW_GAMEPAD_BUTTON_A] ? "Pressed" : "Released", e->buttons[GLFW_GAMEPAD_BUTTON_B] ? "Pressed" : "Released", e->buttons[GLFW_GAMEPAD_BUTTON_Y] ? "Pressed" : "Released", e->buttons[GLFW_GAMEPAD_BUTTON_X] ? "Pressed" : "Released");
        }

        // handle dpad
        if ((e->buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_PRESS) | (e->buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_PRESS) | (e->buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_PRESS) | (e->buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_PRESS))
        {
            log_debug("Controller %d // Down: %s | Right: %s | Up: %s | Left: %s", e->jid, e->buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] ? "Pressed" : "Released", e->buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] ? "Pressed" : "Released", e->buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] ? "Pressed" : "Released", e->buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] ? "Pressed" : "Released");
        }
    }
}

/* -------------------------------------------- Handle Key Event ---------------------------------------*/

//come back later
KeyEvent *get_key_event(GLFWwindow *window)
{

    return NULL;

}

/* -------------------------------------------- Handle Controller Event ---------------------------------------*/

// returns number of controllers connected
int count_controllers()
{
    int count = 0;
    int max_controllers = 2; //change this if you want more controllers

    for(int i = 0; i < max_controllers; i++)
    {
        if(glfwJoystickPresent(GLFW_JOYSTICK_1))
        {
            log_debug("controller %d is present", i + 1);
            count++;
        }
    }

    return count;
}

// Handle controller events for main function
void poll_controller_events(int num_controllers)
{
    for(int i = 0; i < num_controllers; i++)
    {
        ControllerEvent *e = get_controller_event(i);

        if (e == NULL)
            return;

        log_controller_event(e);

        free(e);
    }
}