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
        G_UIelement* railElement = NULL;
        G_UIelement* knobElement = NULL;
        G_UIelementAttribute* sliderAttribute = NULL;
        glm::vec4 railColour = glm::vec4(1.0);
        glm::vec4 knobColour = glm::vec4(1.0);
        GUI_alignment orientation = GUI_DEFAULT;
        static void onHoverInCallback(void* element);
        static void onHoverOutCallback(void* element);
        static void onPressCallback(void* element);
        static void onReleaseCallback(void* element);
        static void onStateChangeCallback(void* element, G_UIattribSlider laststate);
    public:
        int Create(float minValue, float maxValue, GUI_alignment orientation, glm::vec2 size, glm::vec2 knobSize, glm::vec2 position, glm::vec4 railColour, glm::vec4 knobColour);
        int CalculateBounds();
        float Value();
    };
    class GUIcreator
    {

    };
}