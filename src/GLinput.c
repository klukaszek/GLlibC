#include "GLinput.h"
#include "GLwindow.h"

/* ------------------------------------------- GLFW Callbacks (WILL PROBABLY MAKE AN INPUT.C FILE WITH HEADER CONTAINING DIFFERENT CALLBACKS) -----------------------------------------------*/

// simple key_pressed callback for glfwSetKeyCallback(window, callback);
void key_pressed_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
    {
        if
            (is_fullscreen(window)) toggle_fullscreen_off(window);
        else 
            toggle_fullscreen_on(window);

        log_info("Fullscreen toggled.");
    }
}

// simple mouse callback for glfwSetMouseCallback(window, callback);
void mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
    // check if mouse button pressed has event, return if no event
    if (button != GLFW_MOUSE_BUTTON_LEFT && button != GLFW_MOUSE_BUTTON_RIGHT && button != GLFW_MOUSE_BUTTON_MIDDLE)
        return;

    // cursor position within window
    double x, y;

    // check for lmb pressed
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        glfwGetCursorPos(window, &x, &y);
        if (action == GLFW_PRESS)
            log_info("LMB Pressed at (%.1f, %.1f)", x, y);
        if (action == GLFW_RELEASE)
            log_info("LMB Released at (%.1f, %.1f)", x, y);

        return;
    }

    // check for rmb pressed
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        glfwGetCursorPos(window, &x, &y);
        if (action == GLFW_PRESS)
            log_trace("RMB Pressed at (%.1f, %.1f)", x, y);
        if (action == GLFW_RELEASE)
            log_trace("RMB Released at (%.1f, %.1f)", x, y);

        return;
    }

    // check for mmb pressed
    if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        glfwGetCursorPos(window, &x, &y);
        if (action == GLFW_PRESS)
            log_trace("MMB Pressed at (%.1f, %.1f)", x, y);
        if (action == GLFW_RELEASE)
            log_trace("MMB Released at (%.1f, %.1f)", x, y);
    }
}

// simple scroll callback for glfwSetScrollCallback(window, callback);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    printf("xOffset: %.1f, yOffset: %.1f\n", xoffset, yoffset);
}