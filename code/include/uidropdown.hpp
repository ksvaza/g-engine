#pragma once
#include <stdint.h>
#include <laygui.hpp>
#include <uielement.hpp>
#include <glm/glm.hpp>

namespace Gengine
{
    struct GUI_DROPDOWN_FORWARDER
    {
        void* forward = NULL;
        int source = 0;
    };
    class GUI_dropdown : public GUI_element
    {
    private:
        G_UIelement** dropElements = NULL;
        G_UIelementAttribute** dropAttributes = NULL;
        uint16_t dropElementCount = 0;

        glm::vec4 mainColour = glm::vec4(1.0);
        glm::vec4 dropColour = glm::vec4(1.0);
        
        glm::vec2 singleSize = glm::vec2(0.0);
        int value = 0;
        char isOpen = 0;
        void updateMesh(G_UIelement* element);
        void toggleState();
        void updateValue(int source);

        static void onHoverInTopCallback(void* dropdown);
        static void onHoverOutTopCallback(void* dropdown);
        static void onPressTopCallback(void* dropdown);
        static void onReleaseTopCallback(void* dropdown);
        static void onStateChangeTopCallback(void* dropdown, G_UIattribButton laststate);

        static void onHoverInDropCallback(void* forwarder);
        static void onHoverOutDropCallback(void* forwarder);
        static void onPressDropCallback(void* forwarder);
        static void onReleaseDropCallback(void* forwarder);
        static void onStateChangeDropCallback(void* forwarder, G_UIattribDropdown laststate);
    public:
        int Create(int optionCount, int defaultValue, glm::vec2 singleSize, glm::vec3 position, glm::vec4 mainColour, glm::vec4 dropColour);
        int UpdateBounds();
        int Precalculate();
        int Value();
        void SetReferenceLayout(Glayout* layout) override;
        void AddToLayout() override;
        void RemoveFromLayout() override;
        void AddAsChild(G_UIelement* parent) override;
        G_UIelement* Element() override;
        G_UIelement* DropElement(int index);
        void Delete() override;
    };
}