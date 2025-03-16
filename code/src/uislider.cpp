#include "../include/uislider.hpp"
#include <uielement.hpp>
#include <stdio.h>

namespace Gengine
{
    int GUI_slider::Create(float minValue, float maxValue, float defaultValue, GUI_alignment orientation, glm::vec2 size, glm::vec2 knobSize, glm::vec3 position, glm::vec4 railColour, glm::vec4 knobColour)
    {
        element = (G_UIelement*)malloc(sizeof(G_UIelement));
        knobElement = (G_UIelement*)malloc(sizeof(G_UIelement));

        Mesh railMesh;
        MeshGenerator::RegularShape(&railMesh, G_RECTANGLE);
        railMesh.transform = NewTransform();
        railMesh.transform.scale = glm::vec3(size.x, size.y, 1.0);
        this->size = size;
        railMesh.SetColour(railColour);
        this->railColour = railColour;

        Glayout::CreateElement(element, G_PARENT);
        element->mesh = railMesh;
        element->transform = NewTransform();
        element->transform.position = position;

        Mesh knobMesh;
        MeshGenerator::RegularShape(&knobMesh, G_RECTANGLE);
        knobMesh.transform = NewTransform();
        knobMesh.transform.position = glm::vec3(0.0, 0.0, 0.1);
        knobMesh.transform.scale = glm::vec3(knobSize.x, knobSize.y, 1.0);
        this->knobSize = knobSize;
        knobMesh.SetColour(knobColour);

        Glayout::CreateElement(knobElement, G_SLIDER);
        G_UIelementAttribute sAttribute;
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
        this->value = defaultValue;
        this->orientation = orientation;
        sAttribute.slider = slider;
        sAttribute.slider.slider = this;
        Glayout::AddAttribute(knobElement, sAttribute);
        attribute = Glayout::GetAttributeByType(knobElement, G_SLIDER_ATTRIB);
        knobElement->mesh = knobMesh;
        knobElement->transform = NewTransform();
        if (orientation == GUI_HORIZONTAL)
        {
            //knobElement->transform.position = glm::vec3(-(size.x - knobSize.x) / 2.0, 0.0, 0.0);
            knobElement->transform.position.x = -(size.x - knobSize.x) / 2.0 + (defaultValue - minValue) / (maxValue - minValue) * (size.x - knobSize.x);
        }
        else if (orientation == GUI_VERTICAL)
        {
            //knobElement->transform.position = glm::vec3(0.0, -(size.y - knobSize.y) / 2.0, 0.0);
            knobElement->transform.position.y = -(size.y - knobSize.y) / 2.0 + (defaultValue - minValue) / (maxValue - minValue) * (size.y - knobSize.y);
        }
        Glayout::AddChild(element, knobElement);

        return 0;
    }
    int GUI_slider::UpdateBounds()
    {
        AABox knobBounds = Glayout::CalculateRelativeBounds(knobElement, -1);
        if (!attribute)
        {
            printf("Attribute not found!\n");
            return -1;
        }
        attribute->slider.bounds = knobBounds;
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
        if (referenceLayout)
        {
            referenceLayout->AddElement(element);
        }
    }
    void GUI_slider::AddAsChild(G_UIelement *parent)
    {
        Glayout::AddChild(parent, element);
    }
    G_UIelement* GUI_slider::Element()
    {
        return element;
    }
    G_UIelement* GUI_slider::KnobElement()
    {
        return knobElement;
    }
    void GUI_slider::RemoveFromLayout()
    {
        if (referenceLayout)
        {
            referenceLayout->RemoveElement(element);
        }
    }
    void GUI_slider::Delete()
    {
        Glayout::DeleteElement(element);
    }
}
