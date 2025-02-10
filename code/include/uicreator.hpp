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
        float value = 0.0;
        void updateKnobPosition();
        void updateKnobColour();
        static void onHoverInCallback(void* slider);
        static void onHoverOutCallback(void* slider);
        static void onPressCallback(void* slider);
        static void onReleaseCallback(void* slider);
        static void onStateChangeCallback(void* slider, G_UIattribSlider laststate);
    public:
        int Create(float minValue, float maxValue, GUI_alignment orientation, glm::vec2 size, glm::vec2 knobSize, glm::vec2 position, glm::vec4 railColour, glm::vec4 knobColour);
        int CalculateBounds();
        float Value();
        void SetReferenceLayout(Glayout* layout);
        void AddToLayout();
        void RemoveFromLayout();
        void Delete();
    };
    class GUIcreator
    {

    };
}