#pragma once
#include <laygui.hpp>
#include <glm/glm.hpp>

namespace Gengine
{
    enum GUI_alignment {
        GUI_DEFAULT = 0x00,
        GUI_HORIZONTAL = 0x01,
        GUI_VERTICAL = 0x02,
    };

    class GUI_slider {
    private:
        Glayout* referenceLayout = NULL;
        G_UIelement* railElement = NULL;
        G_UIelement* knobElement = NULL;
        G_UIelementAttribute* sliderAttribute = NULL;
        glm::vec4 railColour = glm::vec4(1.0);
        glm::vec4 knobColour = glm::vec4(1.0);
        GUI_alignment orientation = GUI_DEFAULT;
        float minValue = 0.0;
        float maxValue = 1.0;
        glm::vec2 size = glm::vec2(0.0);
        glm::vec2 knobSize = glm::vec2(0.0);
        float value = 0.0;
        void updateKnobPosition();
        void updateKnobColour();
        void updateValue();
        static void onHoverInCallback(void* slider);
        static void onHoverOutCallback(void* slider);
        static void onPressCallback(void* slider);
        static void onReleaseCallback(void* slider);
        static void onStateChangeCallback(void* slider, G_UIattribSlider laststate);
        static void onPositionUpdate(void* slider);
    public:
        int Create(float minValue, float maxValue, GUI_alignment orientation, glm::vec2 size, glm::vec2 knobSize, glm::vec2 position, glm::vec4 railColour, glm::vec4 knobColour);
        int Resize(glm::vec2 size, glm::vec2 knobSize);
        int Recolour(glm::vec4 railColour, glm::vec4 knobColour);
        int ValueAmplitude(float minValue, float maxValue);
        int UpdateBounds();
        int UpdateMesh();
        int Precalculate();
        float Value();
        void SetReferenceLayout(Glayout* layout);
        void AddToLayout();
        void RemoveFromLayout();
        void AddAsChild(G_UIelement* parent);
        G_UIelement* RailElement();
        G_UIelement* KnobElement();
        void Delete();
    };
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
        int Create(glm::vec2 size, glm::vec2 position, glm::vec4 buttonColour);
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
    class GUI_elementList {

    };
    class GUIcreator
    {

    };
}