#include "GLheader.h"

#define THUMB_THRESH 0.05

typedef struct key_event
{

    uint16_t key;
    uint16_t scancode;
    uint8_t action;
    uint8_t mods;

} KeyEvent;

typedef struct key_event_queue
{

    KeyEvent *curr_e;
    KeyEvent *prev_e;

} KeyEventQueue;

typedef struct mouse_event
{
    GLFWwindow *window;
    uint8_t button;
    uint8_t action;
    uint8_t mods;

} MouseEvent;

typedef struct mouse_event_queue
{
    MouseEvent *curr_e;
    MouseEvent *prev_e;

} MouseEventQueue;

typedef struct controller_event
{
    unsigned char *buttons;
    float *axes;
    int jid;

} ControllerEvent;

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

//simple controller callback for glfwSetControllerCallback(window, callback);
/*!
    @brief
    @param[in] jid joystick id
    @param[in] event controller event
    @return:
*/
void controller_callback(int jid, int event);

/*-------------------------------------------- Key Event Functions --------------------------------------------------*/

// returns a key event passed to window (GLFW implementation, see directX for windows)
KeyEvent *create_key_event(int key, int scancode, int action, int mods);

//log key event
void log_key_event(KeyEvent *e);

/*--------------------------------------------- Mouse Event Functions -----------------------------------------------*/

//mouse button event
MouseEvent *create_mouse_event(GLFWwindow *window, int button, int action, int mods);

//logs mouse event to console
void log_mouse_event(MouseEvent *e);

/*--------------------------------------------- Controller Event Functions -----------------------------------------------*/
//returns controller event
ControllerEvent *get_controller_event(int jid);

//logs controller event information
void log_controller_event(ControllerEvent *e);

/* -------------------------------------------- Handle Key Event ---------------------------------------*/

//COMPLETE LATER, USE HASHMAP OR SOME DATA STRUCTURE TO PASS KEYBINDINGS AS PARAMETERS TO FUNCTION
KeyEvent *get_key_event(GLFWwindow *window);

/* -------------------------------------------- Handle Controller Event ---------------------------------------*/

//Handle controller events for main function
void poll_controller_events();