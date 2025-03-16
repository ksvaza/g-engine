#pragma once
#include <laygui.hpp>
#include <uielement.hpp>
#include <glm/glm.hpp>

namespace Gengine
{
    class GUI_slider : public GUI_element
    {
    private:
        //Glayout* referenceLayout = NULL;
        //G_UIelement* element = NULL;
        G_UIelement* knobElement = NULL;
        //G_UIelementAttribute* attribute = NULL;
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
        int Create(float minValue, float maxValue, float defaultValue, GUI_alignment orientation, glm::vec2 size, glm::vec2 knobSize, glm::vec3 position, glm::vec4 railColour, glm::vec4 knobColour);
        int Resize(glm::vec2 size, glm::vec2 knobSize);
        int Recolour(glm::vec4 railColour, glm::vec4 knobColour);
        int ValueAmplitude(float minValue, float maxValue);
        int UpdateBounds();
        int UpdateMesh();
        int Precalculate();
        float Value();
        void SetReferenceLayout(Glayout* layout) override;
        void AddToLayout() override;
        void RemoveFromLayout() override;
        void AddAsChild(G_UIelement* parent) override;
        G_UIelement* Element() override;
        G_UIelement* KnobElement();
        void Delete() override;
    };
}