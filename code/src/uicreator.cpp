#include "../include/uicreator.hpp"

namespace Gengine
{
    int GUI_slider::Create(float minValue, float maxValue, GUI_alignment orientation, glm::vec2 size, glm::vec2 knobSize, glm::vec2 position, glm::vec4 railColour, glm::vec4 knobColour)
    {
        railElement = (G_UIelement*)malloc(sizeof(G_UIelement));
        knobElement = (G_UIelement*)malloc(sizeof(G_UIelement));

        Mesh railMesh;
        MeshGenerator::RegularShape(&railMesh, G_RECTANGLE);
        railMesh.transform = NewTransform();
        railMesh.transform.scale = glm::vec3(size.x, size.y, 1.0);
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
        knobMesh.SetColour(knobColour);

        Glayout::CreateElement(knobElement, G_SLIDER);
        G_UIelementAttribute attribute;
        G_UIattribSlider slider;
        for (int i = 0; i < _MAX_MOUSE_BUTTON_COUNT; i++) { slider.pressedWith[i] = 0; }
        slider.type = G_SLIDER_ATTRIB;
        slider.bounds = { 0.0, 0.0, 0.0, 0.0 };
        slider.onStateChange = onStateChangeCallback;
        slider.onPress = onPressCallback;
        slider.onRelease = onReleaseCallback;
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
    int GUI_slider::CalculateBounds()
    {
        AABox knobBounds = Glayout::CalculateRelativeBounds(knobElement, -1);
        sliderAttribute->slider.bounds = knobBounds;
        return 0;
    }
    void GUI_slider::updateKnobPosition()
    {
        glm::vec2 windowSize = glm::vec2(referenceLayout->Gwindow->Width, referenceLayout->Gwindow->Height);
        glm::vec3 deltaMouseWorldPos = glm::vec3(HWInputs::ConvertPixelToWorldSpace(HWInputs::Mouse.MouseDeltaPosition, windowSize, referenceLayout->UIviewMatrix, referenceLayout->UIprojectionMatrix), 0.0);
        knobElement->transform.position += deltaMouseWorldPos;
        return;
    }
    void GUI_slider::updateKnobColour()
    {
        return;
    }
    
    void GUI_slider::onHoverInCallback(void *element)
    {
        GUI_slider* sliderPtr = (GUI_slider*)element;
        G_UIelement* knobElementPtr = sliderPtr->knobElement;
        knobElementPtr->mesh.colour *= 9.0 / 10.0;
    }
    void GUI_slider::onHoverOutCallback(void *element)
    {
        GUI_slider* sliderPtr = (GUI_slider*)element;
        G_UIelement* knobElementPtr = sliderPtr->knobElement;
        knobElementPtr->mesh.colour *= 10.0 / 9.0;
    }
    void GUI_slider::onPressCallback(void *element)
    {
        GUI_slider* sliderPtr = (GUI_slider*)element;
        G_UIelement* knobElementPtr = sliderPtr->knobElement;
        knobElementPtr->mesh.colour *= 9.0 / 10.0;
        sliderPtr->updateKnobPosition();
    }
    void GUI_slider::onReleaseCallback(void *element)
    {
        GUI_slider* sliderPtr = (GUI_slider*)element;
        G_UIelement* knobElementPtr = sliderPtr->knobElement;
        knobElementPtr->mesh.colour *= 10.0 / 9.0;
    }
    void GUI_slider::onStateChangeCallback(void *element, G_UIattribSlider laststate)
    {
        // Your callback implementation here
        GUI_slider* sliderPtr = (GUI_slider*)element;
        sliderPtr->updateKnobColour();
    }
    void GUI_slider::SetReferenceLayout(Glayout *layout)
    {
        referenceLayout = layout;
    }
    void GUI_slider::AddToLayout()
    {
        referenceLayout->AddElement(railElement);
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
}
