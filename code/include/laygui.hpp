#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <mesh.hpp>
#include <render.hpp>
#include <shader.hpp>
#include <transform.hpp>
#include <hwinputs.hpp>
#include <winlib.hpp>
#include <render.hpp>
#include <vector>
#include <map>

namespace Gengine
{
    // Enumerators
    // ----------------------------------------------------------------

    enum G_UIelementType {
        G_EMPTY = 0x00,
        G_MESH = 0x01,
        G_PARENT = 0x02,
        G_BUTTON = 0x03,
        G_SLIDER = 0x04,
        G_TEXT = 0x05,
    };
    enum G_UIattribType {
        G_EMPTY_ATTRIB = 0x00,
        G_BUTTON_ATTRIB = 0x01,
        G_SLIDER_ATTRIB = 0x02,
        G_TEXT_ATTRIB = 0x03,
    };

    // Structs and Unions
    // ----------------------------------------------------------------

    struct G_UIattribButton {
        G_UIattribType type = G_EMPTY_ATTRIB;
        AABox bounds = { 0.0, 0.0, 0.0, 0.0 };
        void (*onStateChange)(void*, G_UIattribButton) = NULL;
        void (*onHoverIn)(void*) = NULL;
        void (*onHoverOut)(void*) = NULL;
        void (*onPress)(void*) = NULL;
        void (*onRelease)(void*) = NULL;
        char isActive = 1;
        char isHovered = 0;
        char isPressed = 0;
        char pressedWith[_MAX_MOUSE_BUTTON_COUNT];
        void* button = NULL;
    };
    struct G_UIattribSlider {
        G_UIattribType type = G_EMPTY_ATTRIB;
        AABox bounds = { 0.0, 0.0, 0.0, 0.0 };
        void (*onStateChange)(void*, G_UIattribSlider) = NULL;
        void (*onHoverIn)(void*) = NULL;
        void (*onHoverOut)(void*) = NULL;
        void (*onPress)(void*) = NULL;
        void (*onRelease)(void*) = NULL;
        void (*onPositionUpdate)(void*) = NULL;
        char isActive = 1;
        char isHovered = 0;
        char isPressed = 0;
        char pressedWith[_MAX_MOUSE_BUTTON_COUNT];
        void* slider = NULL;
    };
    struct G_UIattribText {
        G_UIattribType type = G_EMPTY_ATTRIB;
        char* textContent = NULL;
        uint16_t textLength = 0;
        void* font = NULL;
        uint16_t fontSize = 0;
        glm::vec4 textColour = glm::vec4(1.0);
        void* text = NULL;
    };

    union G_UIelementAttribute {
        G_UIattribType type = G_EMPTY_ATTRIB;
        G_UIattribButton button;
        G_UIattribSlider slider;
        G_UIattribText text;
        G_UIelementAttribute() {}
        ~G_UIelementAttribute() {}
    };
    struct G_UIelement{
        Mesh mesh;
        Mesh supermesh;
        int8_t visible = 1;
        intptr_t uniqueID = 0;
        G_UIelementType type = G_EMPTY;
        Transform transform;
        G_UIelement* parent;
        G_UIelement** children = NULL;
        G_UIelementAttribute** attributes = NULL;
        int16_t childCount;
        int16_t attribCount;
    };

    class Glayout
    {
    private:
        G_UIelement** UI_elementList = NULL;
        uint16_t UI_elementCount = 0;
        std::map<G_UIattribType, std::vector<G_UIelement*>> UI_attributeMap;

        void recursiveAddAttribute(G_UIelement* element, G_UIattribType type);
        void recursiveRemoveAttribute(G_UIelement* element, G_UIattribType type);
        static void recursiveTextureAtlasStich(G_UIelement* element, TextureAtlas* atlas);
    public:
        Shader UIshader;
        glm::mat4 UIviewMatrix;
        glm::mat4 UIprojectionMatrix;
        HWInputs* Input;
        Window* Gwindow;
        Renderer* Render;
    
        // G_UIelement basic functions
        static void CreateElement(G_UIelement* element, G_UIelementType type);
        static void DeleteElement(G_UIelement* element);
        static void AddAttribute(G_UIelement* element, G_UIelementAttribute attribute);
        static void AddChild(G_UIelement* parent, G_UIelement* child);
        static G_UIelementAttribute* GetAttributeByType(G_UIelement* element, G_UIattribType type);
        static int8_t HasAttribute(G_UIelement* element, G_UIattribType type);
        static G_UIelement* GetChildByType(G_UIelement* element, G_UIelementType type);
        static void CalculateSupermesh(G_UIelement* element, char all);
        static void RecalculateSupermesh(G_UIelement* element);
        static AABox CalculateRelativeBounds(G_UIelement* element, uint16_t depth);
        
        // Assembled functions
        static void CreateButton(G_UIelement* element);
        static int AddButtonCallbacks(G_UIelement* element, void (*onStateChange)(void*, G_UIattribButton), void (*onHoverIn)(void*), void (*onHoverOut)(void*), void (*onPress)(void*), void (*onRelease)(void*));
        static int AddButtonBounds(G_UIelement* element, AABox bounds);
        static void CreateUIWindow(G_UIelement* element, G_UIelementAttribute** buttonAttribute, glm::vec2 position, glm::vec2 size, float borderWidth, float buttonHeight);
        static int ResizeUIWindow(G_UIelement* element, glm::vec2 size, float borderWidth, float buttonHeight);
        static int AddUIWindowColours(G_UIelement* element, glm::vec4 colour, glm::vec4 borderColour, glm::vec4 buttonColour);
        static void SortChildren(G_UIelement* parent);

        // Atlas Management functions
        static int StichAtlas(G_UIelement* parent);
        static int MapAtlas(G_UIelement* parent);

        // UI system functions
        void SetUIshader(Shader shader);
        void SetUIviewMatrix(glm::mat4 viewMatrix);
        void SetUIprojectionMatrix(glm::mat4 projectionMatrix);
        void SetInput(HWInputs* input);
        void SetWindow(Window* window);
        void AddElement(G_UIelement* element);
        void RemoveElement(G_UIelement* element);
        void UpdateAttributeMap();
        void DrawElements();
        G_UIelement* GetElementByUniqueID(intptr_t uniqueID);
        int Compile();
        void Update();

        // Static functions
        static void DefaultButtonStateChange(void* element, G_UIattribButton laststate);
        static void DefaultButtonHoverIn(void* element);
        static void DefaultButtonHoverOut(void* element);
        static void DefaultButtonPress(void* element);
        static void DefaultButtonRelease(void* element);
    };
}