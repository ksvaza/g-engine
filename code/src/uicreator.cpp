#include "../include/uicreator.hpp"
#include <stdio.h>

namespace Gengine
{
    // GUI_slider functions
    // ----------------------------------------------------------------

    int GUI_slider::Create(float minValue, float maxValue, GUI_alignment orientation, glm::vec2 size, glm::vec2 knobSize, glm::vec2 position, glm::vec4 railColour, glm::vec4 knobColour)
    {
        railElement = (G_UIelement*)malloc(sizeof(G_UIelement));
        knobElement = (G_UIelement*)malloc(sizeof(G_UIelement));

        Mesh railMesh;
        MeshGenerator::RegularShape(&railMesh, G_RECTANGLE);
        railMesh.transform = NewTransform();
        railMesh.transform.scale = glm::vec3(size.x, size.y, 1.0);
        this->size = size;
        railMesh.SetColour(railColour);
        this->railColour = railColour;

        Glayout::CreateElement(railElement, G_PARENT);
        railElement->mesh = railMesh;
        railElement->transform = NewTransform();
        railElement->transform.position = glm::vec3(position.x, position.y, 0.0);

        Mesh knobMesh;
        MeshGenerator::RegularShape(&knobMesh, G_RECTANGLE);
        knobMesh.transform = NewTransform();
        knobMesh.transform.scale = glm::vec3(knobSize.x, knobSize.y, 1.0);
        this->knobSize = knobSize;
        knobMesh.SetColour(knobColour);

        Glayout::CreateElement(knobElement, G_SLIDER);
        G_UIelementAttribute attribute;
        G_UIattribSlider slider;
        for (int i = 0; i < _MAX_MOUSE_BUTTON_COUNT; i++) { slider.pressedWith[i] = 0; }
        slider.type = G_SLIDER_ATTRIB;
        slider.bounds = { 0.0, 0.0, 0.0, 0.0 };
        slider.onStateChange = onStateChangeCallback;
        slider.onHoverIn = onHoverInCallback;
        slider.onHoverOut = onHoverOutCallback;
        slider.onPress = onPressCallback;
        slider.onRelease = onReleaseCallback;
        slider.onPositionUpdate = onPositionUpdate;
        this->minValue = minValue;
        this->maxValue = maxValue;
        this->value = minValue;
        this->orientation = orientation;
        attribute.slider = slider;
        attribute.slider.slider = this;
        Glayout::AddAttribute(knobElement, attribute);
        sliderAttribute = Glayout::GetAttributeByType(knobElement, G_SLIDER_ATTRIB);
        knobElement->mesh = knobMesh;
        knobElement->transform = NewTransform();
        if (orientation == GUI_HORIZONTAL)
        {
            knobElement->transform.position = glm::vec3(-(size.x - knobSize.x) / 2.0, 0.0, 0.0);
        }
        else if (orientation == GUI_VERTICAL)
        {
            knobElement->transform.position = glm::vec3(0.0, -(size.y - knobSize.y) / 2.0, 0.0);
        }
        Glayout::AddChild(railElement, knobElement);

        return 0;
    }
    int GUI_slider::UpdateBounds()
    {
        AABox knobBounds = Glayout::CalculateRelativeBounds(knobElement, -1);
        if (!sliderAttribute)
        {
            printf("Attribute not found!\n");
            return -1;
        }
        sliderAttribute->slider.bounds = knobBounds;
        return 0;
    }
    int GUI_slider::UpdateMesh()
    {
        Glayout::RecalculateSupermesh(knobElement);
        return 0;
    }
    void GUI_slider::updateKnobPosition()
    {
        glm::vec2 windowSize = glm::vec2(referenceLayout->Gwindow->Width, referenceLayout->Gwindow->Height);
        glm::vec3 deltaMouseWorldPos = glm::vec3(HWInputs::ConvertPixelToWorldSpace(HWInputs::Mouse.MouseDeltaPosition, windowSize, referenceLayout->UIviewMatrix, referenceLayout->UIprojectionMatrix), 0.0);
        if (orientation == GUI_HORIZONTAL)
        {
            knobElement->transform.position.x += deltaMouseWorldPos.x;
            if (knobElement->transform.position.x < -(size.x - knobSize.x) / 2.0)
            {
                knobElement->transform.position.x = -(size.x - knobSize.x) / 2.0;
            }
            else if (knobElement->transform.position.x > (size.x - knobSize.x) / 2.0)
            {
                knobElement->transform.position.x = (size.x - knobSize.x) / 2.0;
            }
        }
        else if (orientation == GUI_VERTICAL)
        {
            knobElement->transform.position.y += deltaMouseWorldPos.y;
            if (knobElement->transform.position.y < -(size.y - knobSize.y) / 2.0)
            {
                knobElement->transform.position.y = -(size.y - knobSize.y) / 2.0;
            }
            else if (knobElement->transform.position.y > (size.y - knobSize.y) / 2.0)
            {
                knobElement->transform.position.y = (size.y - knobSize.y) / 2.0;
            }
        }
        
        UpdateMesh();
        updateValue();
        return;
    }
    void GUI_slider::updateKnobColour()
    {
        UpdateBounds();
        Glayout::RecalculateSupermesh(knobElement);
        return;
    }
    void GUI_slider::updateValue()
    {
        float knobPosition = 0.0;
        float knobAmpitude = 0.0;
        if (orientation == GUI_HORIZONTAL)
        {
            knobAmpitude = size.x - knobSize.x;
            knobPosition = knobElement->transform.position.x;
        }
        else if (orientation == GUI_VERTICAL)
        {
            knobAmpitude = size.y - knobSize.y;
            knobPosition = knobElement->transform.position.y;
        }
        float relativeValue = (knobPosition + knobAmpitude / 2.0) / knobAmpitude;
        this->value = minValue + (maxValue - minValue) * relativeValue;
        return;
    }
    int GUI_slider::Precalculate()
    {
        Glayout::RecalculateSupermesh(knobElement);
        UpdateBounds();
        return 0;
    }
    float GUI_slider::Value()
    {
        return value;
    }

    void GUI_slider::onHoverInCallback(void *element)
    {
        GUI_slider* sliderPtr = (GUI_slider*)element;
        G_UIelement* knobElementPtr = sliderPtr->knobElement;
        knobElementPtr->mesh.colour *= 9.0 / 10.0;
        //printf("Hovered\n");
    }
    void GUI_slider::onHoverOutCallback(void *element)
    {
        GUI_slider* sliderPtr = (GUI_slider*)element;
        G_UIelement* knobElementPtr = sliderPtr->knobElement;
        knobElementPtr->mesh.colour *= 10.0 / 9.0;
        //printf("Unhovered\n");
    }
    void GUI_slider::onPressCallback(void *element)
    {
        GUI_slider* sliderPtr = (GUI_slider*)element;
        G_UIelement* knobElementPtr = sliderPtr->knobElement;
        knobElementPtr->mesh.colour *= 8.0 / 9.0;
        //printf("Pressed\n");
    }
    void GUI_slider::onReleaseCallback(void *element)
    {
        GUI_slider* sliderPtr = (GUI_slider*)element;
        G_UIelement* knobElementPtr = sliderPtr->knobElement;
        knobElementPtr->mesh.colour *= 9.0 / 8.0;
        //printf("Released\n");
    }
    void GUI_slider::onStateChangeCallback(void *element, G_UIattribSlider laststate)
    {
        // Your callback implementation here
        GUI_slider* sliderPtr = (GUI_slider*)element;
        sliderPtr->updateKnobColour();
    }
    void GUI_slider::onPositionUpdate(void *element)
    {
        GUI_slider* sliderPtr = (GUI_slider*)element;
        sliderPtr->updateKnobPosition();
    }
    
    void GUI_slider::SetReferenceLayout(Glayout *layout)
    {
        referenceLayout = layout;
    }
    void GUI_slider::AddToLayout()
    {
        referenceLayout->AddElement(railElement);
    }
    void GUI_slider::AddAsChild(G_UIelement *parent)
    {
        Glayout::AddChild(parent, railElement);
    }
    G_UIelement* GUI_slider::RailElement()
    {
        return railElement;
    }
    G_UIelement* GUI_slider::KnobElement()
    {
        return knobElement;
    }
    void GUI_slider::RemoveFromLayout()
    {
        referenceLayout->RemoveElement(railElement);
    }
    void GUI_slider::Delete()
    {
        Glayout::DeleteElement(railElement);
        Glayout::DeleteElement(knobElement);
    }

    // GUI_button functions
    // ----------------------------------------------------------------

    int GUI_button::Create(glm::vec2 size, glm::vec2 position, glm::vec4 buttonColour)
    {
        buttonElement = (G_UIelement*)malloc(sizeof(G_UIelement));

        Mesh buttonMesh;
        MeshGenerator::RegularShape(&buttonMesh, G_RECTANGLE);
        buttonMesh.transform = NewTransform();
        buttonMesh.transform.scale = glm::vec3(size.x, size.y, 1.0);
        this->size = size;
        buttonMesh.SetColour(buttonColour);
        this->buttonColour = buttonColour;

        Glayout::CreateElement(buttonElement, G_BUTTON);
        G_UIelementAttribute attribute;
        G_UIattribButton button;
        for (int i = 0; i < _MAX_MOUSE_BUTTON_COUNT; i++) { button.pressedWith[i] = 0; }
        button.type = G_BUTTON_ATTRIB;
        button.bounds = { 0.0, 0.0, 0.0, 0.0 };
        button.onStateChange = onStateChangeCallback;
        button.onHoverIn = onHoverInCallback;
        button.onHoverOut = onHoverOutCallback;
        button.onPress = onPressCallback;
        button.onRelease = onReleaseCallback;
        attribute.button = button;
        attribute.button.button = this;
        Glayout::AddAttribute(buttonElement, attribute);
        buttonAttribute = Glayout::GetAttributeByType(buttonElement, G_BUTTON_ATTRIB);
        buttonElement->mesh = buttonMesh;
        buttonElement->transform = NewTransform();
        buttonElement->transform.position = glm::vec3(position.x, position.y, 0.0);

        return 0;
    }
    int GUI_button::UpdateBounds()
    {
        AABox buttonBounds = Glayout::CalculateRelativeBounds(buttonElement, -1);
        if (!buttonAttribute)
        {
            printf("Attribute not found\n");
            return -1;
        }
        buttonAttribute->button.bounds = buttonBounds;
        return 0;
    }
    int GUI_button::UpdateMesh()
    {
        Glayout::RecalculateSupermesh(buttonElement);
        return 0;
    }
    void GUI_button::updateButtonColour()
    {
        UpdateBounds();
        Glayout::RecalculateSupermesh(buttonElement);
        return;
    }
    void GUI_button::updateValue()
    {
        this->hovered = buttonAttribute->button.isHovered;
        this->pressed = buttonAttribute->button.isPressed;
        for (int i = 0; i < _MAX_MOUSE_BUTTON_COUNT; i++)
        {
            this->pressedWith[i] = buttonAttribute->button.pressedWith[i];
        }
        return;
    }
    int GUI_button::Precalculate()
    {
        Glayout::RecalculateSupermesh(buttonElement);
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
        G_UIelement* buttonElementPtr = buttonPtr->buttonElement;
        buttonElementPtr->mesh.colour *= 9.0 / 10.0;
        //printf("Hovered\n");
    }
    void GUI_button::onHoverOutCallback(void *element)
    {
        GUI_button* buttonPtr = (GUI_button*)element;
        G_UIelement* buttonElementPtr = buttonPtr->buttonElement;
        buttonElementPtr->mesh.colour *= 10.0 / 9.0;
        //printf("Unhovered\n");
    }
    void GUI_button::onPressCallback(void *element)
    {
        GUI_button* buttonPtr = (GUI_button*)element;
        G_UIelement* buttonElementPtr = buttonPtr->buttonElement;
        buttonElementPtr->mesh.colour *= 8.0 / 9.0;
        //printf("Pressed\n");
    }
    void GUI_button::onReleaseCallback(void *element)
    {
        GUI_button* buttonPtr = (GUI_button*)element;
        G_UIelement* buttonElementPtr = buttonPtr->buttonElement;
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
        referenceLayout->AddElement(buttonElement);
    }
    void GUI_button::AddAsChild(G_UIelement *parent)
    {
        Glayout::AddChild(parent, buttonElement);
    }
    G_UIelement* GUI_button::Element()
    {
        return buttonElement;
    }
    void GUI_button::RemoveFromLayout()
    {
        referenceLayout->RemoveElement(buttonElement);
    }
    void GUI_button::Delete()
    {
        Glayout::DeleteElement(buttonElement);
    }
}
