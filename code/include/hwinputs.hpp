#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Gengine
{
    // Constants
    #define _MAX_RESSETABLE_KEY_COUNT 20
    #define _MAX_KEY_COUNT 347 // 346 max keycode
    #define _MAX_MOUSE_BUTTON_COUNT 5

    // Structs
    typedef struct {
        glm::vec2 MousePosition;
        glm::vec2 MouseDeltaPosition;
        glm::vec2 MouseScrollOffset;
        char MouseButton[_MAX_MOUSE_BUTTON_COUNT];
        char MouseButtonDown[_MAX_MOUSE_BUTTON_COUNT];
        char MouseButtonUp[_MAX_MOUSE_BUTTON_COUNT];
    }MouseInputs;
    typedef struct {
        int Key[_MAX_KEY_COUNT];
        char KeyDown[_MAX_KEY_COUNT];
        char KeyUp[_MAX_KEY_COUNT];
    }KeyboardInputs;
    
    class HWInputs
    {
        public:
            // Variables
            static MouseInputs Mouse;
            static KeyboardInputs Keyboard;

            // Functions
            static void SetTestMode(char mode);
            static void Update();
            static void Initialise(GLFWwindow* window);

            HWInputs(){}
            ~HWInputs(){}
        private:
            // Variables
            static int RESETTABLE_KEYS[_MAX_RESSETABLE_KEY_COUNT];
            static int RESETTABLE_KEY_COUNT;
            static char RESETTABLE_MOUSE_BUTTONS[_MAX_MOUSE_BUTTON_COUNT];
            static char RESETTABLE_MOUSE_BUTTON_COUNT;
            static glm::vec2 LAST_MOUSE_POSITION;
            static char TEST_MODE;

            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
            static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
            static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    };
}