#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "../Mathe/komunals.h"

// Structures and defines

#define MAX_RESSETABLE_KEY_COUNT 20
#define MAX_KEY_COUNT 347 // 346 max keycode
#define MAX_MOUSE_BUTTON_COUNT 5

typedef struct {
    // keyboard inputs

    int key[MAX_KEY_COUNT];
    char keyDown[MAX_KEY_COUNT];
    char keyUp[MAX_KEY_COUNT];

    // mouse inputs

    Vector2f mousePosition;
    Vector2f mousePositionOffset;
    Vector2f mouseScrollOffset;
    char mouseButton[MAX_MOUSE_BUTTON_COUNT];
    char mouseButtonDown[MAX_MOUSE_BUTTON_COUNT];
    char mouseButtonUp[MAX_MOUSE_BUTTON_COUNT];
}TYPE_INPUTS;

// Global variables

TYPE_INPUTS Inputs;
int RESETTABLE_KEYS[MAX_RESSETABLE_KEY_COUNT];
char RESETTABLE_KEY_COUNT;
char RESETTABLE_MOUSE_BUTTONS[MAX_MOUSE_BUTTON_COUNT];
char RESETTABLE_MOUSE_BUTTON_COUNT;
Vector2f LAST_MOUSE_POSITION;

// Callbacks

void KEY_CALLBACK(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key >= MAX_KEY_COUNT) { return; }
    Inputs.key[key] = action;
    if (action == GLFW_PRESS) { Inputs.keyDown[key] = 1; }
    else if (!action) { Inputs.keyUp[key] = 1; }
    RESETTABLE_KEYS[RESETTABLE_KEY_COUNT] = key;
    RESETTABLE_KEY_COUNT++;
    if (RESETTABLE_KEY_COUNT >= MAX_RESSETABLE_KEY_COUNT) { printf("\nMAX_RESSETABLE_KEY_COUNT of %d exceded\n\n"); RESETTABLE_KEY_COUNT--; }
}
static void CURSOR_POSITION_CALLBACK(GLFWwindow* window, double xpos, double ypos)
{
    Inputs.mousePosition = (Vector2f){ (float)xpos, (float)ypos };
}
void MOUSE_BUTTON_CALLBACK(GLFWwindow* window, int button, int action, int mods)
{
    if (button >= MAX_MOUSE_BUTTON_COUNT) { return; }
    Inputs.mouseButton[button] = action;
    if (action) { Inputs.mouseButtonDown[button] = 1; }
    else { Inputs.mouseButtonUp[button] = 1; }
    RESETTABLE_MOUSE_BUTTONS[RESETTABLE_MOUSE_BUTTON_COUNT] = button;
    RESETTABLE_MOUSE_BUTTON_COUNT++;
    if (RESETTABLE_KEY_COUNT >= MAX_MOUSE_BUTTON_COUNT) { printf("\nMAX_MOUSE_BUTTON_COUNT of %d exceded\n\n"); RESETTABLE_MOUSE_BUTTON_COUNT--; }
}
void SCROLL_CALLBACK(GLFWwindow* window, double xoffset, double yoffset)
{
    Inputs.mouseScrollOffset = (Vector2f){ (float)xoffset, (float)yoffset };
}

// Functions

int initialiseInputs(GLFWwindow** window)
{
    glfwSetKeyCallback(*window, KEY_CALLBACK);
    glfwSetCursorPosCallback(*window, CURSOR_POSITION_CALLBACK);
    glfwSetMouseButtonCallback(*window, MOUSE_BUTTON_CALLBACK);
    glfwSetScrollCallback(*window, SCROLL_CALLBACK);
    
    // sussy
    //glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);

    // vajag
    if (glfwRawMouseMotionSupported()) { glfwSetInputMode(*window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE); }

    return 0;
}
void updateInputs()
{
    if (RESETTABLE_KEY_COUNT > 0)
    {
        for (char i = 0; i < RESETTABLE_KEY_COUNT + 1; i++)
        {
            Inputs.keyDown[RESETTABLE_KEYS[i]] = 0;
            Inputs.keyUp[RESETTABLE_KEYS[i]] = 0;
        }
        RESETTABLE_KEY_COUNT = 0;
    }
    if (RESETTABLE_MOUSE_BUTTON_COUNT > 0)
    {
        for (char i = 0; i < RESETTABLE_MOUSE_BUTTON_COUNT; i++)
        {
            Inputs.mouseButtonDown[i] = 0;
            Inputs.mouseButtonUp[i] = 0;
        }
        RESETTABLE_MOUSE_BUTTON_COUNT = 0;
    }
    Inputs.mousePositionOffset = subtractVector2f(LAST_MOUSE_POSITION, Inputs.mousePosition);
    LAST_MOUSE_POSITION = Inputs.mousePosition;
}
