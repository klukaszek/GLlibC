#include "GLheader.h"

/* ------------------------------------------- GLFW Window Input Callbacks -----------------------------------------------*/

// simple key_pressed callback for glfwSetKeyCallback(window, callback);
/*!
    @brief
    @param[in] window GLFW Window Pointer
    @param[in] key Key pressed's char value
    @param[in] scancode Scancode int
    @param[in] action Action GLFW int,
    @param[in] mods Modification GLFW int
    @return: in this case it prints the key that is pressed
*/
void key_pressed_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// simple mouse callback for glfwSetMouseCallback(window, callback);
/*!
    @brief
    @param[in] window GLFW Window Pointer
    @param[in] button MouseButton pressed as int (0=last??, 1=left, 2=right, 3=middle)
    @param[in] action Action GLFW int,
    @param[in] mods Modification GLFW int
    @return:
*/
void mouse_callback(GLFWwindow *window, int button, int action, int mods);

// simple scroll callback for glfwSetScrollCallback(window, callback);
/*!
    @brief
    @param[in] window GLFW Window Pointer
    @param[in] xoffset double representing amount scrolled horizontally
    @param[in] yoffset double representing amount scrolled vertically
    @return:
*/
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
