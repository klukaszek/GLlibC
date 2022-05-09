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

/* ------------------------------------------ Get Input Events ---------------------------------------------*/

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

/* ------------------------------------------- Input Events -----------------------------------------------*/

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

/* -------------------------------------------- Handle Key Event ---------------------------------------*/