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
    }
    else if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        log_trace("Key Pressed: %c", key);
    }
}

// simple mouse callback for glfwSetMouseCallback(window, callback);
// this checks 
void mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
    // check if mouse button pressed has event, return if no event
    if (button != GLFW_MOUSE_BUTTON_LEFT && button != GLFW_MOUSE_BUTTON_RIGHT && button != GLFW_MOUSE_BUTTON_MIDDLE) return;

    // check for lmb pressed
    left_mouse_click_event(window, button, action);

    // check for rmb pressed
    right_mouse_click_event(window, button, action);

    // check for mmb pressed
    middle_mouse_click_event(window, button, action);
}

// simple scroll callback for glfwSetScrollCallback(window, callback);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    log_info("xOffset: %.1f, yOffset: %.1f", xoffset, yoffset);
}

/* ------------------------------------------- Input Events -----------------------------------------------*/

// left click event
void left_mouse_click_event(GLFWwindow *window, int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        // cursor position within window
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        if (action == GLFW_PRESS) log_info("LMB Pressed at (%.1f, %.1f)", x, y);
        if (action == GLFW_RELEASE) log_info("LMB Released at (%.1f, %.1f)", x, y);
        return;
    }
}

// right click event
void right_mouse_click_event(GLFWwindow *window, int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        // cursor position within window
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        if (action == GLFW_PRESS) log_info("RMB Pressed at (%.1f, %.1f)", x, y);
        if (action == GLFW_RELEASE) log_info("RMB Released at (%.1f, %.1f)", x, y);
        return;
    }
}

// middle mouse event
void middle_mouse_click_event(GLFWwindow *window, int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        // cursor position within window
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        if (action == GLFW_PRESS) log_info("MMB Pressed at (%.1f, %.1f)", x, y);
        if (action == GLFW_RELEASE) log_info("MMB Released at (%.1f, %.1f)", x, y);
        return;
    }
}