#include "GLwindow.h"

/* ------------------------------------------- GLFW Window -----------------------------------------------*/

// create GLFWwindow struct pointer
GLFWwindow *create_window(const int16_t width, const int16_t height, bool fullscreen, const char *title, GLFWwindow *shared)
{

    // Initialize GLFW library
    if (!glfwInit())
    {
        log_error("glfwInit() Failed To Initialize.\n");
        return NULL;
    }

    // Set OPENGL version 4.6 and CORE profile as window hints (based on GLAD loader information)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // monitor structs are not freed by programmer as per glfw documentation
    GLFWmonitor *monitor = NULL;

    //turn fullscreen on if fullscreen is true
    if (fullscreen) monitor = glfwGetPrimaryMonitor();

    //create window with GLFW
    GLFWwindow *window = glfwCreateWindow(width, height, title, monitor, shared);
    if (!window)
    {
        log_error("glfwCreateWindow(), failed to open window.\n");
        glfwTerminate();
        return NULL;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // initialize GLAD opengl function loader
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        log_error("gladLoadGLLoader() Failed To Initialize.\n");
        return NULL;
    }

    // set viewport for drawing
    glViewport(0, 0, width, height);

    return window;
}

// setup window callbacks, use NULL in place of user pointer and function pointers if you do not want to use them in your program
void gl_set_window_callbacks(GLFWwindow *window, void *user_pointer, GLFWkeyfun key_callback, GLFWmousebuttonfun mouse_callback, GLFWscrollfun scroll_callback, GLFWjoystickfun controller_callback)
{
    //set window user pointer for later callback usage
    glfwSetWindowUserPointer(window, user_pointer);

    // key callback set to personal function key_pressed_callback
    glfwSetKeyCallback(window, key_callback);
    // mouse callback set to personal function mouse_callback
    glfwSetMouseButtonCallback(window, mouse_callback);
    // scroll callback set to personal function scroll_callback
    glfwSetScrollCallback(window, scroll_callback);

    //set joystick user pointer
    glfwSetJoystickUserPointer(GLFW_JOYSTICK_1, user_pointer);
    // controller callback set to personal function controller_callback
    glfwSetJoystickCallback(controller_callback);
}

// set resolution of window
void set_resolution(GLFWwindow *window, int16_t width, int16_t height)
{
    if (window == NULL) return;
    GLFWmonitor *monitor = glfwGetWindowMonitor(window);
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    glfwSetWindowMonitor(window, monitor, 0, 0, width, height, mode->refreshRate);
}

// set refresh rate of window
void set_refresh_rate(GLFWwindow *window, int16_t refresh_rate)
{
    if (window == NULL) return;
    GLFWmonitor *monitor = glfwGetWindowMonitor(window);
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, refresh_rate);
}

// returns whether or not window is fullscreen
bool is_fullscreen(GLFWwindow *window)
{
    if(window == NULL) return false;
    bool state = glfwGetWindowMonitor(window) != NULL;
    return state;
}

//toggles fullscreen for window
void toggle_fullscreen(GLFWwindow *window)
{
    if(window == NULL) return;

    //Toggle Fullscreen on
    if(!is_fullscreen(window))
    {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        log_info("Fullscreen toggled on.");
        return;
    }
    else // Toggle fullscreen off
    {
        int16_t window_w = WIDTH;
        int16_t window_h = HEIGHT;

        // get primary monitor size in pixels
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        // unfullscreen window and set window in middle of primary monitor
        glfwSetWindowMonitor(window, NULL, (mode->width / 2) - (window_w / 2), (mode->height / 2) - (window_h / 2), window_w, window_h, mode->refreshRate);
        log_info("Fullscreen toggled off.");
        return;
    }
}

// Calls glfw terminate and removes unnecessary files (come back to this later)
void gl_terminate()
{

    glfwTerminate();

    return;
}