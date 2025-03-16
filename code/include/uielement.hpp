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

    class GUI_element
    {
    public:
        Glayout* referenceLayout = NULL;
        G_UIelement* element = NULL;
        G_UIelementAttribute* attribute = NULL;
        virtual void SetReferenceLayout(Glayout* layout) = 0;
        virtual void AddToLayout() = 0;
        virtual void RemoveFromLayout() = 0;
        virtual void AddAsChild(G_UIelement* parent) = 0;
        virtual G_UIelement* Element() = 0;
        virtual void Delete() = 0;
    };
    
    class GUI_elementList {

    };
    class GUIcreator
    {

    };
}