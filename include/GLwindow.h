#include "GLheader.h"

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

// set resolution of GLFWwindow pointer
void set_resolution(GLFWwindow *window, int16_t width, int16_t height);

// set refresh rate of GLFWwindow pointer
void set_refresh_rate(GLFWwindow *window, int16_t refresh_rate);

// returns whether or not window is fullscreen
bool is_fullscreen(GLFWwindow *window);

// toggles fullscreen on GLFWwindow pointer
void toggle_fullscreen(GLFWwindow *window);