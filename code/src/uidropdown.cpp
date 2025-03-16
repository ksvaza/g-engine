#include "../include/uidropdown.hpp"
#include <uielement.hpp>
#include <stdio.h>

namespace Gengine
{
    int GUI_dropdown::Create(int optionCount, int defaultValue, glm::vec2 singleSize, glm::vec3 position, glm::vec4 mainColour, glm::vec4 dropColour)
    {
        element = (G_UIelement*)malloc(sizeof(G_UIelement));
        this->singleSize = singleSize;
        this->mainColour = mainColour;
        this->dropColour = dropColour;
        this->value = defaultValue;

        Mesh mainMesh;
        MeshGenerator::Rectangle(&mainMesh, singleSize);
        mainMesh.transform = NewTransform();
        mainMesh.SetColour(mainColour);

        Glayout::CreateElement(element, G_BUTTON);
        G_UIelementAttribute mAttribute;
        G_UIattribButton mButton;
        for (int i = 0; i < _MAX_MOUSE_BUTTON_COUNT; i++) { mButton.pressedWith[i] = 0; }
        mButton.type = G_BUTTON_ATTRIB;
        mButton.bounds = { 0.0, 0.0, 0.0, 0.0 };
        mButton.onStateChange = onStateChangeTopCallback;
        mButton.onHoverIn = onHoverInTopCallback;
        mButton.onHoverOut = onHoverOutTopCallback;
        mButton.onPress = onPressTopCallback;
        mButton.onRelease = onReleaseTopCallback;
        mAttribute.button = mButton;
        mAttribute.button.button = this;
        Glayout::AddAttribute(element, mAttribute);
        attribute = Glayout::GetAttributeByType(element, G_BUTTON_ATTRIB);
        element->mesh = mainMesh;
        element->transform = NewTransform();
        element->transform.position = position;

        Mesh dropMesh;
        MeshGenerator::Rectangle(&dropMesh, singleSize);
        dropMesh.transform = NewTransform();
        dropMesh.SetColour(dropColour);

        dropAttributes = (G_UIelementAttribute**)malloc(sizeof(G_UIelementAttribute*) * optionCount);
        dropElements = (G_UIelement**)malloc(sizeof(G_UIelement*) * optionCount);

        for (int i = 0; i < optionCount; i++)
        {
            GUI_DROPDOWN_FORWARDER* forwarder = (GUI_DROPDOWN_FORWARDER*)malloc(sizeof(GUI_DROPDOWN_FORWARDER));
            forwarder->forward = this;
            forwarder->source = i;

            G_UIelement* dropElement = (G_UIelement*)malloc(sizeof(G_UIelement));
            Glayout::CreateElement(dropElement, G_BUTTON);
            G_UIelementAttribute dAttribute;
            G_UIattribDropdown dropdown;
            for (int i = 0; i < _MAX_MOUSE_BUTTON_COUNT; i++) { dropdown.pressedWith[i] = 0; }
            dropdown.type = G_DROPDOWN_ATTRIB;
            dropdown.bounds = { 0.0, 0.0, 0.0, 0.0 };
            dropdown.onStateChange = onStateChangeDropCallback;
            dropdown.onHoverIn = onHoverInDropCallback;
            dropdown.onHoverOut = onHoverOutDropCallback;
            dropdown.onPress = onPressDropCallback;
            dropdown.onRelease = onReleaseDropCallback;
            dAttribute.dropdown = dropdown;
            dAttribute.dropdown.forwarder = forwarder;
            Glayout::AddAttribute(dropElement, dAttribute);
            dropAttributes[i] = Glayout::GetAttributeByType(dropElement, G_DROPDOWN_ATTRIB);
            MeshGenerator::CopyMesh(&dropElement->mesh, &dropMesh);
            dropElement->transform = NewTransform();
            dropElement->transform.position = glm::vec3(position.x, -singleSize.y * (i + 1), 0.1);
            dropElement->visible = 0;
            dropElements[i] = dropElement;
            Glayout::AddChild(element, dropElement);
        }
        dropElementCount = optionCount;
        dropMesh.Delete();

        return 0;
    }
    int GUI_dropdown::UpdateBounds()
    {
        AABox buttonBounds = Glayout::CalculateRelativeBounds(element, -1);
        if (!attribute)
        {
            printf("Attribute not found\n");
            return -1;
        }
        attribute->button.bounds = buttonBounds;
        for (int i = 0; i < dropElementCount; i++)
        {
            AABox dropBounds = Glayout::CalculateRelativeBounds(dropElements[i], -1);
            dropAttributes[i]->button.bounds = dropBounds;
        }
        return 0;
    }
    void GUI_dropdown::updateMesh(G_UIelement* element)
    {
        for (int i = 0; i < dropElementCount; i++)
        {
            Glayout::CalculateSupermesh(dropElements[i], 0);
        }
        Glayout::RecalculateSupermesh(element);
        if (!isOpen) UpdateBounds();
        return;
    }
    void GUI_dropdown::toggleState()
    {
        if (isOpen)
        {
            for (int i = 0; i < dropElementCount; i++)
            {
                dropElements[i]->visible = 0;
            }
            isOpen = 0;
            element->mesh.colour *= 8.0 / 7.0;
        }
        else
        {
            for (int i = 0; i < dropElementCount; i++)
            {
                dropElements[i]->visible = 1;
            }
            isOpen = 1;
            element->mesh.colour *= 7.0 / 8.0;
        }
        updateMesh(element);
        return;
    }
    void GUI_dropdown::updateValue(int source)
    {
        G_UIelement* lastElement = dropElements[value];
        for (int i = 0; i < lastElement->childCount; i++)
        {
            Glayout::RemoveChild(element, lastElement->children[i]);
        }
        this->value = source;
        G_UIelement* newElement = dropElements[value];
        for (int i = 0; i < newElement->childCount; i++)
        {
            Glayout::AddChild(element, newElement->children[i]);
        }
        printf("Value: %d, Childrencount: %d\n", value, element->childCount);

        Glayout::SortChildren(element);
        toggleState();
        return;
    }
    int GUI_dropdown::Precalculate()
    {
        Glayout::RecalculateSupermesh(element);
        for (int i = 0; i < dropElementCount; i++)
        {
            Glayout::RecalculateSupermesh(dropElements[i]);
        }
        return 0;
    }
    int GUI_dropdown::Value()
    {
        return value;
    }

    void GUI_dropdown::onPressTopCallback(void* dropdown)
    {
        GUI_dropdown* dropdownPtr = (GUI_dropdown*)dropdown;
        G_UIelement* elementPtr = dropdownPtr->element;
        elementPtr->mesh.colour *= 8.0 / 9.0;
        dropdownPtr->toggleState();
        return;
    }
    void GUI_dropdown::onReleaseTopCallback(void* dropdown)
    {
        GUI_dropdown* dropdownPtr = (GUI_dropdown*)dropdown;
        G_UIelement* elementPtr = dropdownPtr->element;
        elementPtr->mesh.colour *= 9.0 / 8.0;
        return;
    }
    void GUI_dropdown::onHoverInTopCallback(void* dropdown)
    {
        GUI_dropdown* dropdownPtr = (GUI_dropdown*)dropdown;
        G_UIelement* elementPtr = dropdownPtr->element;
        elementPtr->mesh.colour *= 9.0 / 10.0;
        return;
    }
    void GUI_dropdown::onHoverOutTopCallback(void* dropdown)
    {
        GUI_dropdown* dropdownPtr = (GUI_dropdown*)dropdown;
        G_UIelement* elementPtr = dropdownPtr->element;
        elementPtr->mesh.colour *= 10.0 / 9.0;
        return;
    }
    void GUI_dropdown::onStateChangeTopCallback(void* dropdown, G_UIattribButton laststate)
    {
        GUI_dropdown* dropdownPtr = (GUI_dropdown*)dropdown;
        dropdownPtr->updateMesh(dropdownPtr->element);
        return;
    }

    void GUI_dropdown::onHoverInDropCallback(void* forwarder)
    {
        GUI_DROPDOWN_FORWARDER* forwardPtr = (GUI_DROPDOWN_FORWARDER*)forwarder;
        GUI_dropdown* dropdownPtr = (GUI_dropdown*)forwardPtr->forward;
        int source = forwardPtr->source;
        G_UIelement* elementPtr = dropdownPtr->dropElements[source];
        elementPtr->mesh.colour *= 9.0 / 10.0;
        return;
    }
    void GUI_dropdown::onHoverOutDropCallback(void* forwarder)
    {
        GUI_DROPDOWN_FORWARDER* forwardPtr = (GUI_DROPDOWN_FORWARDER*)forwarder;
        GUI_dropdown* dropdownPtr = (GUI_dropdown*)forwardPtr->forward;
        int source = forwardPtr->source;
        G_UIelement* elementPtr = dropdownPtr->dropElements[source];
        elementPtr->mesh.colour *= 10.0 / 9.0;
        return;
    }
    void GUI_dropdown::onPressDropCallback(void* forwarder)
    {
        GUI_DROPDOWN_FORWARDER* forwardPtr = (GUI_DROPDOWN_FORWARDER*)forwarder;
        GUI_dropdown* dropdownPtr = (GUI_dropdown*)forwardPtr->forward;
        int source = forwardPtr->source;
        G_UIelement* elementPtr = dropdownPtr->dropElements[source];
        elementPtr->mesh.colour *= 8.0 / 9.0;
        return;
    }
    void GUI_dropdown::onReleaseDropCallback(void* forwarder)
    {
        GUI_DROPDOWN_FORWARDER* forwardPtr = (GUI_DROPDOWN_FORWARDER*)forwarder;
        GUI_dropdown* dropdownPtr = (GUI_dropdown*)forwardPtr->forward;
        int source = forwardPtr->source;
        G_UIelement* elementPtr = dropdownPtr->dropElements[source];
        elementPtr->mesh.colour *= 9.0 / 8.0;
        //onHoverOutDropCallback(forwarder);
        dropdownPtr->updateValue(source);
        return;
    }
    void GUI_dropdown::onStateChangeDropCallback(void* forwarder, G_UIattribDropdown laststate)
    {
        GUI_DROPDOWN_FORWARDER* forwardPtr = (GUI_DROPDOWN_FORWARDER*)forwarder;
        GUI_dropdown* dropdownPtr = (GUI_dropdown*)forwardPtr->forward;
        dropdownPtr->updateMesh(dropdownPtr->element);
        return;
    }

    void GUI_dropdown::SetReferenceLayout(Glayout* layout)
    {
        referenceLayout = layout;
        return;
    }
    void GUI_dropdown::AddToLayout()
    {
        if (referenceLayout)
        {
            referenceLayout->AddElement(element);
        }
        return;
    }
    void GUI_dropdown::RemoveFromLayout()
    {
        if (referenceLayout)
        {
            referenceLayout->RemoveElement(element);
        }
        return;
    }
    void GUI_dropdown::AddAsChild(G_UIelement* parent)
    {
        Glayout::AddChild(parent, element);
        return;
    }
    G_UIelement* GUI_dropdown::Element()
    {
        return element;
    }
    G_UIelement* GUI_dropdown::DropElement(int index)
    {
        if (index < 0 || index >= dropElementCount) { return NULL; }
        return dropElements[index];
    }
    void GUI_dropdown::Delete()
    {
        Glayout::DeleteElement(element);
        free(dropElements);
        free(dropAttributes);
        for (int i = 0; i < dropElementCount; i++)
        {
            GUI_DROPDOWN_FORWARDER* forwarder = (GUI_DROPDOWN_FORWARDER*)Glayout::GetAttributeByType(dropElements[i], G_BUTTON_ATTRIB)->button.button;
            free(forwarder);
        }
        return;
    }
}