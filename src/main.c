//MAKE SHARED LIBRARY OR STATIC LIBRARY USING MAKEFILE AND LINK IT TO YOUR PROJECT, INCLUDE HEADER FILES AT TOP

#include "GLheader.h"
#include "GLwindow.h"
#include "GLinput.h"

int main(void)
{
    const char *platform;

    //get platform name and print
    #if PLATFORM_NAME == windows
        platform = "windows";
    #elif PLATFORM_NAME == linux
        platform = "linux";
    #elif PLATFORM_NAME == osx
        platform = "osx";
    #endif
    log_debug("Compilation platform: %s", platform);
    
    const int16_t window_w = WIDTH;
    const int16_t window_h = HEIGHT;
    const bool fullscreen = false;
    const char *title = "My Window"; 
    
    //width, height, fullscreen, window title, shared window
    GLFWwindow *window = create_window(window_w, window_h, fullscreen, title, NULL);

    //if window initialization fails, terminate glfw and return
    if (window == NULL)
    {
        gl_terminate();
        return -1;
    }

    int num_controllers = count_controllers();
    log_debug("%d controller(s) connected.", num_controllers);

    //Set callbacks for window
    gl_set_window_callbacks(window, NULL, key_pressed_callback, mouse_callback, scroll_callback, controller_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(250.0f / 255.0f, 119.0f / 255.0f, 110.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        gl_poll_events(num_controllers);
    }
    
    //free all data associated with GLFW
    gl_terminate();
    return 0;
}