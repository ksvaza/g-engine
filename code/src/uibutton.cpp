#include "../include/uibutton.hpp"
#include <uielement.hpp>
#include <stdio.h>

namespace Gengine
{
    int GUI_button::Create(glm::vec2 size, glm::vec3 position, glm::vec4 buttonColour)
    {
        element = (G_UIelement*)malloc(sizeof(G_UIelement));

        Mesh buttonMesh;
        MeshGenerator::RegularShape(&buttonMesh, G_RECTANGLE);
        buttonMesh.transform = NewTransform();
        buttonMesh.transform.scale = glm::vec3(size.x, size.y, 1.0);
        this->size = size;
        buttonMesh.SetColour(buttonColour);
        this->buttonColour = buttonColour;

        Glayout::CreateElement(element, G_BUTTON);
        G_UIelementAttribute bAttribute;
        G_UIattribButton button;
        for (int i = 0; i < _MAX_MOUSE_BUTTON_COUNT; i++) { button.pressedWith[i] = 0; }
        button.type = G_BUTTON_ATTRIB;
        button.bounds = { 0.0, 0.0, 0.0, 0.0 };
        button.onStateChange = onStateChangeCallback;
        button.onHoverIn = onHoverInCallback;
        button.onHoverOut = onHoverOutCallback;
        button.onPress = onPressCallback;
        button.onRelease = onReleaseCallback;
        bAttribute.button = button;
        bAttribute.button.button = this;
        Glayout::AddAttribute(element, bAttribute);
        attribute = Glayout::GetAttributeByType(element, G_BUTTON_ATTRIB);
        element->mesh = buttonMesh;
        element->transform = NewTransform();
        element->transform.position = position;

        return 0;
    }
    int GUI_button::UpdateBounds()
    {
        AABox buttonBounds = Glayout::CalculateRelativeBounds(element, -1);
        if (!attribute)
        {
            printf("Attribute not found\n");
            return -1;
        }
        attribute->button.bounds = buttonBounds;
        return 0;
    }
    void GUI_button::updateButtonColour()
    {
        UpdateBounds();
        Glayout::RecalculateSupermesh(element);
        return;
    }
    void GUI_button::updateValue()
    {
        this->hovered = attribute->button.isHovered;
        this->pressed = attribute->button.isPressed;
        for (int i = 0; i < _MAX_MOUSE_BUTTON_COUNT; i++)
        {
            this->pressedWith[i] = attribute->button.pressedWith[i];
        }
        return;
    }
    int GUI_button::Precalculate()
    {
        Glayout::RecalculateSupermesh(element);
        UpdateBounds();
        return 0;
    }
    char GUI_button::IsPressed()
    {
        return pressed;
    }
    char GUI_button::PressedWith(int index)
    {
        return pressedWith[index];
    }
    char GUI_button::IsHovered()
    {
        return hovered;
    }

    void GUI_button::onHoverInCallback(void *element)
    {
        GUI_button* buttonPtr = (GUI_button*)element;
        G_UIelement* buttonElementPtr = buttonPtr->element;
        buttonElementPtr->mesh.colour *= 9.0 / 10.0;
        //printf("Hovered\n");
    }
    void GUI_button::onHoverOutCallback(void *element)
    {
        GUI_button* buttonPtr = (GUI_button*)element;
        G_UIelement* buttonElementPtr = buttonPtr->element;
        buttonElementPtr->mesh.colour *= 10.0 / 9.0;
        //printf("Unhovered\n");
    }
    void GUI_button::onPressCallback(void *element)
    {
        GUI_button* buttonPtr = (GUI_button*)element;
        G_UIelement* buttonElementPtr = buttonPtr->element;
        buttonElementPtr->mesh.colour *= 8.0 / 9.0;
        //printf("Pressed\n");
    }
    void GUI_button::onReleaseCallback(void *element)
    {
        GUI_button* buttonPtr = (GUI_button*)element;
        G_UIelement* buttonElementPtr = buttonPtr->element;
        buttonElementPtr->mesh.colour *= 9.0 / 8.0;
        //printf("Released\n");
    }
    void GUI_button::onStateChangeCallback(void *element, G_UIattribButton laststate)
    {
        // Your callback implementation here
        GUI_button* buttonPtr = (GUI_button*)element;
        buttonPtr->updateButtonColour();
        buttonPtr->updateValue();
    }

    void GUI_button::SetReferenceLayout(Glayout *layout)
    {
        referenceLayout = layout;
    }
    void GUI_button::AddToLayout()
    {
        if (referenceLayout)
        {
            referenceLayout->AddElement(element);
        }
    }
    void GUI_button::AddAsChild(G_UIelement *parent)
    {
        Glayout::AddChild(parent, element);
    }
    G_UIelement* GUI_button::Element()
    {
        return element;
    }
    void GUI_button::RemoveFromLayout()
    {
        if (referenceLayout)
        {
            referenceLayout->RemoveElement(element);
        }
    }
    void GUI_button::Delete()
    {
        Glayout::DeleteElement(element);
    }
}
