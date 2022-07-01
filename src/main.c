//MAKE SHARED LIBRARY OR STATIC LIBRARY USING MAKEFILE AND LINK IT TO YOUR PROJECT, INCLUDE HEADER FILES AT TOP

#include "GLheader.h"
#include "GLwindow.h"
#include "GLinput.h"
#include "GLshaders.h"

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

    char *shader_0 = malloc(sizeof(char) * 128);
    strcpy(shader_0, "bin/assets/shaders/vertex_shader.glsl");

    char *shader_1 = malloc(sizeof(char) * 128);
    strcpy(shader_1, "bin/assets/shaders/fragment_shader.glsl");

    char **shader_files = malloc((sizeof(char) * 128) * 2);

    shader_files[0] = shader_0;
    shader_files[1] = shader_1;

    size_t num_shaders = 2;
    GLuint program_id = gl_create_program(shader_files, num_shaders);

    float vertex_array[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f,  // right
        0.0f, 0.5f, 0.0f    // top
    };

    float *vertices = malloc(sizeof(float) * 9);

    for(int i = 0; i < 9; i++)
    {
        vertices[i] = vertex_array[i];
    }

    unsigned int vbo_id, vao_id;
    glGenVertexArrays(1, &vao_id);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(vao_id);

    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, (sizeof(vertices)+1)*sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    //Set callbacks for window
    gl_set_window_callbacks(window, NULL, key_pressed_callback, mouse_callback, scroll_callback, controller_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        //glClearColor(250.0f / 255.0f, 119.0f / 255.0f, 110.0f / 255.0f, 1.0f);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gl_render(program_id, vao_id);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        gl_poll_events(num_controllers);

    }

    //free all data associated with GLFW
    gl_terminate();
    return 0;
}