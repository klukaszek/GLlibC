#include "GLinput.h"
#include "GLwindow.h"

/* ------------------------------------------- GLFW Callbacks -----------------------------------------------*/

//Attempt to write key event address to file descriptor so that it can be accessed from outside of callback
void *stin(KeyEvent *e)
{
    #if PLATFORM_NAME == windows
        log_debug("%p", (void *)e);

        return (void *)e;
    #endif
}


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
        //original key event
        log_info("Key Event: %c, %d, %p", key, key, &e); //print letter, ascii val, and pointer to key event

        //obtainable key event from outside of callback
        void *p = stin(e);
        KeyEvent *c = malloc(sizeof(KeyEvent));
        memcpy(c, p, sizeof(KeyEvent));
        log_debug("Pulled from memory: %c", c->key);

        free(e);
        free(c);
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

// returns empty key event queue for when program starts execution
KeyEventQueue *init_key_queue()
{

    KeyEventQueue *eq = malloc(sizeof(KeyEventQueue));
    eq->curr_e = NULL;
    eq->prev_e = NULL;

    return eq;

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