#include "GLheader.h"

#define WIDTH 1920
#define HEIGHT 1080

/* ------------------------------------------- GLFW Window Functions -----------------------------------------------*/

// Creates and returns GLFW Window Structure Pointer
/*!
    @brief
    @param[in] width width of window in pixels
    @param[in] height height of window in pixels
    @param[in] fullscreen true or false
    @param[in] title name of window
    @param[in] shared GLFWwindow pointer to shared window (leave NULL)
    @return: GLFWwindow *window
*/
GLFWwindow *create_window(const int16_t width, const int16_t height, bool fullscreen, const char *title, GLFWwindow *shared);

// setup window callbacks, use NULL in place of function pointers if you do not want to use
/*!
    @brief
    @param[in] window GLFWwindow pointer (can't be NULL)
    @param[in] user_pointer void pointer to struct of user_pointer (can be NULL)
    @param[in] key_callback GLFWkeyfun of keyboard callback (can be NULL for no callback)
    @param[in] mouse_callback GLFWmousebuttonfun of mouse button callback (can be NULL for no callback)
    @param[in] scroll_callback GLFWscrollfun of scroll callback (can be NULL for no callback)
    @param[in] controller_callback GLFWjoystickfun of joystick callback (can be NULL for no callback)
    @return:
*/
void gl_set_window_callbacks(GLFWwindow *window, void *user_pointer, GLFWkeyfun key_callback, GLFWmousebuttonfun mouse_callback, GLFWscrollfun scroll_callback, GLFWjoystickfun controller_callback);

// set resolution of GLFWwindow pointer
void set_resolution(GLFWwindow *window, int16_t width, int16_t height);

// set refresh rate of GLFWwindow pointer
void set_refresh_rate(GLFWwindow *window, int16_t refresh_rate);

// returns whether or not window is fullscreen
bool is_fullscreen(GLFWwindow *window);

// toggles fullscreen on GLFWwindow pointer
void toggle_fullscreen(GLFWwindow *window);

// Calls glfw terminate and removes unnecessary files
void gl_terminate();