#pragma once
#include <laygui.hpp>
#include <uicreator.hpp>
#include <glm/glm.hpp>

namespace Gengine
{
    class GUI_button {
    private:
        Glayout* referenceLayout = NULL;
        G_UIelement* buttonElement = NULL;
        G_UIelementAttribute* buttonAttribute = NULL;
        glm::vec4 buttonColour = glm::vec4(1.0);
        GUI_alignment orientation = GUI_DEFAULT;
        glm::vec2 size = glm::vec2(0.0);
        char pressed = 0;
        char hovered = 0;
        char pressedWith[_MAX_MOUSE_BUTTON_COUNT];
        void updateButtonColour();
        void updateValue();
        static void onHoverInCallback(void* button);
        static void onHoverOutCallback(void* button);
        static void onPressCallback(void* button);
        static void onReleaseCallback(void* button);
        static void onStateChangeCallback(void* button, G_UIattribButton laststate);
    public:
        int Create(glm::vec2 size, glm::vec3 position, glm::vec4 buttonColour);
        int Resize(glm::vec2 size);
        int Recolour(glm::vec4 buttonColour);
        int UpdateBounds();
        int UpdateMesh();
        int Precalculate();
        char IsPressed();
        char PressedWith(int mouseButton);
        char IsHovered();
        void SetReferenceLayout(Glayout* layout);
        void AddToLayout();
        void RemoveFromLayout();
        void AddAsChild(G_UIelement* parent);
        G_UIelement* Element();
        void Delete();
    };
}