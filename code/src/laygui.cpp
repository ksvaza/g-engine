#include "../include/laygui.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <render.hpp>
#include <shader.hpp>
#include <mesh.hpp>
#include <hwinputs.hpp>
#include <glm/glm.hpp>
#include <uicreator.hpp>

using namespace std;

namespace Gengine
{
    // Private functions
    // ----------------------------------------------------------------

    void Glayout::recursiveAddAttribute(G_UIelement* element, G_UIattribType type)
    {
        for (int i = 0; i < element->attribCount; i++)
        {
            if (element->attributes[i]->type == type)
            {
                UI_attributeMap[type].push_back(element);
            }
        }
        for (int j = 0; j < element->childCount; j++)
        {
            recursiveAddAttribute(element->children[j], type);
        }
    }
    void Glayout::recursiveRemoveAttribute(G_UIelement* element, G_UIattribType type)
    {
        for (int i = 0; i < element->attribCount; i++)
        {
            if (element->attributes[i]->type == type)
            {
                for (int j = 0; j < (int)UI_attributeMap[type].size(); j++)
                {
                    if (UI_attributeMap[type][j]->uniqueID == element->uniqueID)
                    {
                        UI_attributeMap[type].erase(UI_attributeMap[type].begin() + j);
                        break;
                    }
                }
            }
        }
        for (int i = 0; i < element->childCount; i++)
        {
            recursiveRemoveAttribute(element->children[i], type);
        }
    }

    // G_UIelement basic functions
    // ----------------------------------------------------------------

    void Glayout::CreateElement(G_UIelement* element, G_UIelementType type)
    {
        element->mesh = Mesh::Empty();
        element->supermesh = Mesh::Empty();
        element->visible = 1;
        element->uniqueID = (intptr_t)element;
        element->type = type;
        Transform transform;
        transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        element->transform = transform;
        element->parent = NULL;
        element->children = NULL;
        element->attributes = NULL;
        element->childCount = 0;
        element->attribCount = 0;
    }
    void Glayout::DeleteElement(G_UIelement* element)
    {
        element->mesh.Delete();
        element->supermesh.Delete();
        if (element->childCount > 0)
        {
            for (int i = 0; i < element->childCount; i++)
            {
                DeleteElement(element->children[i]);
            }
        }
        if (element->attribCount > 0)
        {
            for (int i = 0; i < element->attribCount; i++)
            {
                free(element->attributes[i]);
            }
        }
        if (element->parent)
        {
            for (int i = 0; i < element->childCount; i++)
            {
                if (element->children[i]->uniqueID == element->uniqueID)
                {
                    element->children[i] = NULL;
                }
            }
            SortChildren(element->parent);
        }
    }
    void Glayout::AddAttribute(G_UIelement* element, G_UIelementAttribute attribute)
    {
        element->attributes = (G_UIelementAttribute**)realloc(element->attributes, sizeof(G_UIelementAttribute*) * (element->attribCount + 1));
        element->attributes[element->attribCount] = (G_UIelementAttribute*)malloc(sizeof(G_UIelementAttribute));
        *element->attributes[element->attribCount] = attribute;
        element->attribCount++;
    }
    void Glayout::AddChild(G_UIelement* parent, G_UIelement* child)
    {
        parent->children = (G_UIelement**)realloc(parent->children, sizeof(G_UIelement*) * (parent->childCount + 1));
        parent->children[parent->childCount] = child;
        child->parent = parent;
        parent->childCount++;
    }
    G_UIelementAttribute* Glayout::GetAttributeByType(G_UIelement* element, G_UIattribType type)
    {
        for (int i = 0; i < element->attribCount; i++)
        {
            if (element->attributes[i]->type == type)
            {
                return element->attributes[i];
            }
        }
        return NULL;
    }
    int8_t Glayout::HasAttribute(G_UIelement* element, G_UIattribType type)
    {
        for (int i = 0; i < element->attribCount; i++)
        {
            if (element->attributes[i]->type == type)
            {
                return 1;
            }
        }
        return 0;
    }
    G_UIelement* Glayout::GetChildByType(G_UIelement* element, G_UIelementType type)
    {
        for (int16_t i = 0; i < element->childCount; i++)
        {
            if (element->children[i]->type == type)
            {
                return element->children[i];
            }
        }
        return NULL;
    }
    void Glayout::CalculateSupermesh(G_UIelement* element, char all) // all: 1 = calculate supermeshes for all childen, 0 = lazy calculation to take into account existing supermeshes
    {
        if (all)
        {
            for (int i = 0; i < element->childCount; i++)
            {
                CalculateSupermesh(element->children[i], 1);
            }
            if (element->childCount == 0)
            {
                Mesh mesh;
                MeshGenerator::CopyMesh(&mesh, &element->mesh);
                MeshGenerator::TransformMesh(&mesh, mesh.GetTransform());
                mesh.SetTransform();
                element->supermesh = mesh;
            } else {
                Mesh mesh;
                MeshGenerator::CopyMesh(&mesh, &element->mesh);
                MeshGenerator::TransformMesh(&mesh, mesh.GetTransform());
                mesh.SetTransform();
                for (int i = 0; i < element->childCount; i++)
                {
                    if (!Mesh::Empty().Equals(element->children[i]->supermesh) && element->children[i]->visible)
                    {
                        Mesh childmesh;
                        MeshGenerator::CopyMesh(&childmesh, &element->children[i]->supermesh);
                        MeshGenerator::TransformMesh(&childmesh, element->children[i]->transform);
                        MeshGenerator::AddMesh(&mesh, &childmesh);
                        childmesh.Delete();
                    }
                }
                element->supermesh = mesh;
            }
        } else {
            if (element->childCount == 0)
            {
                Mesh mesh;
                MeshGenerator::CopyMesh(&mesh, &element->mesh);
                MeshGenerator::TransformMesh(&mesh, mesh.GetTransform());
                mesh.SetTransform();
                element->supermesh = mesh;
            } else {
                Mesh mesh;
                MeshGenerator::CopyMesh(&mesh, &element->mesh);
                MeshGenerator::TransformMesh(&mesh, mesh.GetTransform());
                mesh.SetTransform();
                for (int i = 0; i < element->childCount; i++)
                {
                    if (!Mesh::Empty().Equals(element->children[i]->supermesh) && element->children[i]->visible)
                    {
                        Mesh childmesh;
                        MeshGenerator::CopyMesh(&childmesh, &element->children[i]->supermesh);
                        MeshGenerator::TransformMesh(&childmesh, element->children[i]->transform);
                        MeshGenerator::AddMesh(&mesh, &childmesh);
                        childmesh.Delete();
                    }
                }
                element->supermesh = mesh;
            }
        }
    }
    void Glayout::RecalculateSupermesh(G_UIelement* element)
    {
        CalculateSupermesh(element, 0);
        G_UIelement* parent = element->parent;
        while (parent)
        {
            CalculateSupermesh(parent, 0);
            parent = parent->parent;
        }
    }
    AABox Glayout::CalculateRelativeBounds(G_UIelement* element, uint16_t depth)
    {
        Mesh mesh;
        /*Transform stackedTransform;
        stackedTransform.position = glm::vec3(0.0, 0.0, 0.0);
        stackedTransform.rotation = glm::vec3(0.0, 0.0, 0.0);
        stackedTransform.scale = glm::vec3(1.0, 1.0, 1.0);*/
        if (Mesh::Empty().Equals(element->supermesh))
        {
            MeshGenerator::CopyMesh(&mesh, &element->mesh);
            MeshGenerator::TransformMesh(&mesh, element->mesh.transform);
            //stackedTransform = CombineTransforms(stackedTransform, element->mesh.transform);
        } else {
            MeshGenerator::CopyMesh(&mesh, &element->supermesh);
        }
        mesh.SetTransform();
        G_UIelement* transformElement = element;

        if (depth < 0)
        {
            while (transformElement->parent)
            {
                MeshGenerator::TransformMesh(&mesh, transformElement->transform);
                //stackedTransform = CombineTransforms(stackedTransform, transformElement->transform);
                transformElement = transformElement->parent;
            }
        } else
        {
            for (uint16_t i = 0; i < depth; i++)
            {
                if (transformElement->parent)
                {
                    MeshGenerator::TransformMesh(&mesh, transformElement->transform);
                    //stackedTransform = CombineTransforms(stackedTransform, transformElement->transform);
                    transformElement = transformElement->parent;
                } else { break; }
            }
        }
        //MeshGenerator::TransformMesh(&mesh, stackedTransform);
        MeshGenerator::CalculateBounds(&mesh);
        AABox box = mesh.GetBoundingBox();
        mesh.Delete();
        return box;
    }
    
    // Assembled functions
    // ----------------------------------------------------------------

    void Glayout::CreateButton(G_UIelement* element)
    {
        CreateElement(element, G_BUTTON);
        G_UIelementAttribute attribute;
        G_UIattribButton button;
        for (int i = 0; i < _MAX_MOUSE_BUTTON_COUNT; i++) { button.pressedWith[i] = 0; }
        button.type = G_BUTTON_ATTRIB;
        button.bounds = { 0.0, 0.0, 0.0, 0.0 };
        attribute.button = button;
        AddAttribute(element, attribute);
    }
    int Glayout::AddButtonCallbacks(G_UIelement* element, void (*onStateChange)(void*, G_UIattribButton), void (*onHoverIn)(void*), void (*onHoverOut)(void*), void (*onPress)(void*), void (*onRelease)(void*))
    {
        G_UIelementAttribute* attribute = GetAttributeByType(element, G_BUTTON_ATTRIB);
        if (!attribute) { return -1; }
        attribute->button.onStateChange = onStateChange;
        attribute->button.onHoverIn = onHoverIn;
        attribute->button.onHoverOut = onHoverOut;
        attribute->button.onPress = onPress;
        attribute->button.onRelease = onRelease;
        return 0;
    }
    int Glayout::AddButtonBounds(G_UIelement* element, AABox bounds)
    {
        G_UIelementAttribute* attribute = GetAttributeByType(element, G_BUTTON_ATTRIB);
        if (!attribute) { return -1; }
        attribute->button.bounds = bounds;
        return 0;
    }
    void Glayout::CreateUIWindow(G_UIelement* element, G_UIelementAttribute** buttonAttribute, glm::vec2 position, glm::vec2 size, float borderWidth, float buttonHeight)
    {
        G_UIelement* buttonElement = (G_UIelement*)malloc(sizeof(G_UIelement));
        G_UIelement* contentElement = (G_UIelement*)malloc(sizeof(G_UIelement));

        Mesh windowMesh;
        MeshGenerator::RegularShape(&windowMesh, G_RECTANGLE);
        windowMesh.transform = NewTransform();
        windowMesh.transform.scale = glm::vec3(size.x, size.y, 1.0);
        windowMesh.SetColour(glm::vec4(0.2, 0.2, 0.2, 1.0));


        CreateElement(element, G_PARENT);
        element->mesh = windowMesh;
        element->transform = NewTransform();
        element->transform.position = glm::vec3(position.x, position.y, 0.0);

        Mesh buttonMesh;
        MeshGenerator::RegularShape(&buttonMesh, G_RECTANGLE);
        buttonMesh.transform = NewTransform();
        buttonMesh.transform.scale = glm::vec3(size.x - 2.0 * borderWidth, buttonHeight, 1.0);
        buttonMesh.SetColour(glm::vec4(0.4, 0.4, 0.4, 1.0));

        CreateButton(buttonElement);
        AddButtonCallbacks(buttonElement, DefaultButtonStateChange, DefaultButtonHoverIn, DefaultButtonHoverOut, DefaultButtonPress, DefaultButtonRelease);
        buttonElement->mesh = buttonMesh;
        buttonElement->transform = NewTransform();
        buttonElement->transform.position = glm::vec3(0.0, (size.y - buttonHeight) / 2 - borderWidth, 0.0);
        *buttonAttribute = GetAttributeByType(buttonElement, G_BUTTON_ATTRIB);
        AddChild(element, buttonElement);
        AABox buttonBounds = CalculateRelativeBounds(buttonElement, -1);
        AddButtonBounds(buttonElement, buttonBounds);


        Mesh contentMesh;
        MeshGenerator::RegularShape(&contentMesh, G_RECTANGLE);
        contentMesh.transform = NewTransform();
        contentMesh.transform.scale = glm::vec3(size.x - 2.0 * borderWidth, size.y - buttonHeight - 3.0 * borderWidth, 1.0);
        contentMesh.SetColour(glm::vec4(0.9, 0.9, 0.9, 1.0));

        CreateElement(contentElement, G_MESH);
        contentElement->mesh = contentMesh;
        contentElement->transform = NewTransform();
        contentElement->transform.position = glm::vec3(0.0, -(buttonHeight + 3.0 * borderWidth) / 2.0 + borderWidth, 0.0);
        AddChild(element, contentElement);
    }
    int Glayout::ResizeUIWindow(G_UIelement* element, glm::vec2 size, float borderWidth, float buttonHeight)
    {
        return 0;
    }
    int Glayout::AddUIWindowColours(G_UIelement* element, glm::vec4 colour, glm::vec4 borderColour, glm::vec4 buttonColour)
    {

        G_UIelement* contents = GetChildByType(element, G_MESH);
        if (!contents) { return -1; }
        contents->mesh.SetColour(colour);

        element->mesh.SetColour(borderColour);

        G_UIelement* button = GetChildByType(element, G_BUTTON);
        if (!button) { return -1; }
        button->mesh.SetColour(buttonColour);

        return 0;
    }
    void Glayout::SortChildren(G_UIelement* parent)
    {
        int16_t offset = 0;
        for (int i = 0; i < parent->childCount; i++)
        {
            if (parent->children[i] == NULL)
            {
                offset++;
            }
            else
            {
                parent->children[i - offset] = parent->children[i];
            }
        }
        parent->childCount -= offset;
    }

    // UI system functions
    // ----------------------------------------------------------------

    void Glayout::SetUIshader(Shader shader)
    {
        UIshader = shader;
    }
    void Glayout::SetUIviewMatrix(glm::mat4 viewMatrix)
    {
        UIviewMatrix = viewMatrix;
    }
    void Glayout::SetUIprojectionMatrix(glm::mat4 projectionMatrix)
    {
        UIprojectionMatrix = projectionMatrix;
    }
    void Glayout::SetInput(HWInputs* Input)
    {
        this->Input = Input;
    }
    void Glayout::SetWindow(Window* Gwindow)
    {
        this->Gwindow = Gwindow;
    }
    void Glayout::AddElement(G_UIelement* element)
    {
        UI_elementList = (G_UIelement**)realloc(UI_elementList, sizeof(G_UIelement*) * (UI_elementCount + 1));
        UI_elementList[UI_elementCount] = element;
        UI_elementCount++;
        
        recursiveAddAttribute(element, G_BUTTON_ATTRIB);
        recursiveAddAttribute(element, G_SLIDER_ATTRIB);
    }
    void Glayout::RemoveElement(G_UIelement* element)
    {
        for (int i = 0; i < UI_elementCount; i++)
        {
            if (UI_elementList[i] == element)
            {
                for (int j = 0; j < (int)UI_attributeMap.size(); j++)
                {
                    recursiveRemoveAttribute(UI_elementList[i], (G_UIattribType)j);
                }
                UI_elementList[i] = NULL;
                break;
            }
        }

        uint16_t offset = 0;
        for (int i = 0; i < UI_elementCount; i++)
        {
            if (UI_elementList[i] == NULL)
            {
                offset++;
            }
            else
            {
                UI_elementList[i - offset] = UI_elementList[i];
            }
        }
        UI_elementCount -= offset;

        UI_elementList = (G_UIelement**)realloc(UI_elementList, sizeof(G_UIelement*) * UI_elementCount);
    }
    void Glayout::DrawElements()
    {
        for (uint16_t i = 0; i < UI_elementCount; i++)
        {
            if (UI_elementList[i]->visible)
            {
                Mesh mesh;
                if (Mesh::Empty().Equals(UI_elementList[i]->supermesh))
                {
                    mesh = UI_elementList[i]->mesh;
                    MeshGenerator::TransformMesh(&mesh, UI_elementList[i]->mesh.GetTransform());
                } else {
                    mesh = UI_elementList[i]->supermesh;
                }
                mesh.transform = UI_elementList[i]->transform;
                //printf("Element texture count: %d\n", mesh.TextureCount);
                Render->DrawMesh(mesh, 0, UIshader, UIviewMatrix, UIprojectionMatrix);
                //Render->DrawBoundingBox(mesh, UIshader, UIviewMatrix, UIprojectionMatrix);
            }
        }
    }
    G_UIelement* Glayout::GetElementByUniqueID(intptr_t uniqueID)
    {
        for (uint16_t i = 0; i < UI_elementCount; i++)
        {
            if (UI_elementList[i]->uniqueID == uniqueID)
            {
                return UI_elementList[i];
            }
        }
        return NULL;
    }
    int Glayout::Compile()
    {
        //printf("UI element count: %d\n", UI_elementCount);
        for (uint16_t i = 0; i < UI_elementCount; i++)
        {
            CalculateSupermesh(UI_elementList[i], -1); // Calculate supermeshes
            UI_elementList[i]->supermesh.SetBoundingBox(CalculateRelativeBounds(UI_elementList[i], -1)); // Calculate relative bounds
        }
        //printf("Button count: %d\n", (int)UI_attributeMap[G_BUTTON_ATTRIB].size());
        for (int i = 0; i < (int)UI_attributeMap[G_BUTTON_ATTRIB].size(); i++)
        {
            G_UIelement* element = UI_attributeMap[G_BUTTON_ATTRIB][i];
            if (!element) { printf("Null element!\n"); continue; }
            G_UIelementAttribute* attrib = GetAttributeByType(element, G_BUTTON_ATTRIB);
            if (!attrib) { printf("Button without an attribute!\n"); continue; }
            G_UIattribButton* button = &attrib->button;
            if (!button) { printf("Null button!\n"); continue; }
            GUI_button* buttonClass = (GUI_button*)button->button;
            if (!buttonClass) { printf("Null button class!\n"); continue; }

            buttonClass->Precalculate();
        }
        //printf("Slider count: %d\n", (int)UI_attributeMap[G_SLIDER_ATTRIB].size());
        for (int i = 0; i < (int)UI_attributeMap[G_SLIDER_ATTRIB].size(); i++)
        {
            G_UIelement* element = UI_attributeMap[G_SLIDER_ATTRIB][i];
            if (!element) { printf("Null element!\n"); continue; }
            G_UIelementAttribute* attrib = GetAttributeByType(element, G_SLIDER_ATTRIB);
            if (!attrib) { printf("Slider without an attribute!\n"); continue; }
            G_UIattribSlider* slider = &attrib->slider;
            if (!slider) { printf("Null slider!\n"); continue; }
            GUI_slider* sliderClass = (GUI_slider*)slider->slider;
            if (!sliderClass) { printf("Null slider class!\n"); continue; }

            sliderClass->Precalculate();
        }
        return 0;
    }
    void Glayout::Update()
    {
        for (int i = 0; i < (int)UI_attributeMap[G_BUTTON_ATTRIB].size(); i++)
        {
            G_UIelement* element = UI_attributeMap[G_BUTTON_ATTRIB][i];
            if (!element) { printf("Null element!\n"); continue; }
            G_UIelementAttribute* attrib = GetAttributeByType(element, G_BUTTON_ATTRIB);
            if (!attrib) { printf("Button without an attribute!\n"); continue; }

            G_UIattribButton* button = &attrib->button;
            if (!button) { printf("Null button!\n"); continue; }
            // last state
            G_UIattribButton laststate = *button;

            if (!button->isActive) { printf("Inactive!\n"); continue; }

            // Hover logic
            glm::vec2 mousePos = Input->ConvertPixelToWorldSpace(Input->Mouse.MousePosition, glm::vec2(Gwindow->Width, Gwindow->Height), UIviewMatrix, UIprojectionMatrix);
            glm::mat4 transform = glm::mat4(1.0f);
            G_UIelement* parent = element;
            while (parent)
            {
                transform = TransformToMatrix(parent->transform);
                parent = parent->parent;
            }

            mousePos = glm::inverse(transform) * glm::vec4(mousePos.x, mousePos.y, 0.0f, 1.0f);
            char isInBounds = PointInBounds(glm::vec2(mousePos.x, mousePos.y), button->bounds);
            char changed = 0;
            
            if (isInBounds)
            {
                if (!button->isHovered)
                {
                    if (button->onHoverIn) { button->onHoverIn(button->button); }
                    changed = 1;
                }
                button->isHovered = 1;
            }
            else
            {
                if (button->isHovered)
                {
                    if (button->onHoverOut) { button->onHoverOut(button->button); }
                    changed = 1;
                }
                button->isHovered = 0;
            }

            // Press/release logic for each mouse button
            for (int mb = 0; mb < _MAX_MOUSE_BUTTON_COUNT; mb++)
            {
                // Press any mouse button if hovered
                if (Input->Mouse.MouseButtonDown[mb] && button->isHovered)
                {
                    if (!button->pressedWith[mb])
                    {
                        // If no other button was pressed, call onPress
                        char noOtherPressed = 1;
                        for (int mb2 = 0; mb2 < _MAX_MOUSE_BUTTON_COUNT; mb2++)
                        {
                            if (mb2 != mb && button->pressedWith[mb2])
                            {
                                noOtherPressed = 0;
                                break;
                            }
                        }
                        button->pressedWith[mb] = 1;
                        changed = 1;
                        if (noOtherPressed)
                        {
                            button->isPressed = 1;
                            if (button->onPress) { button->onPress(button->button); }
                        }
                    }
                }

                if (Input->Mouse.MouseButtonUp[mb])
                {
                    if (button->isPressed)
                    {
                        button->isPressed = 0;
                        if (button->onRelease) { button->onRelease(button->button); }
                    }
                    if (button->pressedWith[mb])
                    {
                        button->pressedWith[mb] = 0;
                        changed = 1;
                    }
                }
            }

            if (changed)
            {
                if (button->onStateChange) { button->onStateChange(button->button, laststate); }
            }
        }
        for (int i = 0; i < (int)UI_attributeMap[G_SLIDER_ATTRIB].size(); i++)
        {
            G_UIelement* element = UI_attributeMap[G_SLIDER_ATTRIB][i];
            if (!element) { printf("Null element!\n"); continue; }
            G_UIelementAttribute* attrib = GetAttributeByType(element, G_SLIDER_ATTRIB);
            if (!attrib) { printf("Slider without an attribute!\n"); continue; }

            G_UIattribSlider* slider = &attrib->slider;
            if (!slider) { printf("Null slider!\n"); continue; }
            // last state
            G_UIattribSlider laststate = *slider;

            if (!slider->isActive) { printf("Inactive!\n"); continue; }

            // Hover logic
            glm::vec2 mousePos = Input->ConvertPixelToWorldSpace(Input->Mouse.MousePosition, glm::vec2(Gwindow->Width, Gwindow->Height), UIviewMatrix, UIprojectionMatrix);
            glm::mat4 transform = glm::mat4(1.0f);
            G_UIelement* parent = element;
            while (parent)
            {
                transform = TransformToMatrix(parent->transform);
                parent = parent->parent;
            }

            mousePos = glm::inverse(transform) * glm::vec4(mousePos.x, mousePos.y, 0.0f, 1.0f);
            char isInBounds = PointInBounds(glm::vec2(mousePos.x, mousePos.y), slider->bounds);
            char changed = 0;
            
            if (isInBounds)
            {
                if (!slider->isHovered)
                {
                    if (slider->onHoverIn) { slider->onHoverIn(slider->slider); }
                    changed = 1;
                }
                slider->isHovered = 1;
            }
            else
            {
                if (slider->isHovered)
                {
                    if (slider->onHoverOut) { slider->onHoverOut(slider->slider); }
                    changed = 1;
                }
                slider->isHovered = 0;
            }

            // Press/release logic for each mouse button
            for (int mb = 0; mb < _MAX_MOUSE_BUTTON_COUNT; mb++)
            {
                // Press any mouse button if hovered
                if (Input->Mouse.MouseButtonDown[mb] && slider->isHovered)
                {
                    if (!slider->pressedWith[mb])
                    {
                        // If no other button was pressed, call onPress
                        char noOtherPressed = 1;
                        for (int mb2 = 0; mb2 < _MAX_MOUSE_BUTTON_COUNT; mb2++)
                        {
                            if (mb2 != mb && slider->pressedWith[mb2])
                            {
                                noOtherPressed = 0;
                                break;
                            }
                        }
                        slider->pressedWith[mb] = 1;
                        changed = 1;
                        if (noOtherPressed)
                        {
                            slider->isPressed = 1;
                            if (slider->onPress) { slider->onPress(slider->slider); }
                        }
                    }
                }

                if (Input->Mouse.MouseButtonUp[mb])
                {
                    if (slider->isPressed)
                    {
                        slider->isPressed = 0;
                        if (slider->onRelease) { slider->onRelease(slider->slider); }
                    }
                    if (slider->pressedWith[mb])
                    {
                        slider->pressedWith[mb] = 0;
                        changed = 1;
                    }
                }
            }
            
            if (slider->isPressed)
            {
                if (slider->onPositionUpdate) { slider->onPositionUpdate(slider->slider); }
            }

            if (changed)
            {
                if (slider->onStateChange) { slider->onStateChange(slider->slider, laststate); }
            }
        }
    }

    // Static functions
    // ----------------------------------------------------------------

    void Glayout::DefaultButtonHoverIn(void* element)
    {
        G_UIelement* elementPtr = (G_UIelement*)element;
        elementPtr->mesh.colour *= 9.0 / 10.0;
    }
    void Glayout::DefaultButtonHoverOut(void* element)
    {
        G_UIelement* elementPtr = (G_UIelement*)element;
        elementPtr->mesh.colour *= 10.0 / 9.0;
        //                              
    }
    void Glayout::DefaultButtonPress(void* element)
    {
        G_UIelement* elementPtr = (G_UIelement*)element;
        elementPtr->mesh.colour *= 8.0 / 9.0;
    }
    void Glayout::DefaultButtonRelease(void* element)
    {
        G_UIelement* elementPtr = (G_UIelement*)element;
        elementPtr->mesh.colour *= 9.0 / 8.0;
    }
    void Glayout::DefaultButtonStateChange(void* element, G_UIattribButton laststate)
    {
        G_UIelement* elementPtr = (G_UIelement*)element;
        //G_UIattribButton button = GetAttributeByType(elementPtr, G_BUTTON_ATTRIB)->button;
        //printf("Button state change: %llx\n", elementPtr->uniqueID);

        Glayout::RecalculateSupermesh(elementPtr);
    }
}
