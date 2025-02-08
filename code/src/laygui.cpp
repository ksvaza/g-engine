#include "../include/laygui.hpp"
#include <stdio.h>
#include <render.hpp>
#include <shader.hpp>
#include <mesh.hpp>
#include <hwinputs.hpp>
#include <glm/glm.hpp>

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
                element->mesh.SetTransform();
                element->supermesh = mesh;
            } else {
                Mesh mesh;
                MeshGenerator::CopyMesh(&mesh, &element->mesh);
                MeshGenerator::TransformMesh(&mesh, mesh.GetTransform());
                element->mesh.SetTransform();
                Mesh submesh = Mesh::Empty();
                submesh.SetTransform();
                for (int i = 0; i < element->childCount; i++)
                {
                    if (!Mesh::Empty().Equals(element->children[i]->supermesh) && element->children[i]->visible)
                    {
                        Mesh childmesh = element->children[i]->supermesh;
                        MeshGenerator::TransformMesh(&childmesh, element->children[i]->transform);
                        MeshGenerator::AddMesh(&submesh, &element->children[i]->supermesh);
                    }
                }
                MeshGenerator::AddMesh(&mesh, &submesh);
                element->supermesh = submesh;
            }
        } else {
            if (element->childCount == 0)
            {
                Mesh mesh;
                MeshGenerator::CopyMesh(&mesh, &element->mesh);
                MeshGenerator::TransformMesh(&mesh, mesh.GetTransform());
                element->mesh.SetTransform();
                element->supermesh = mesh;
            } else {
                Mesh mesh;
                MeshGenerator::CopyMesh(&mesh, &element->mesh);
                MeshGenerator::TransformMesh(&mesh, mesh.GetTransform());
                element->mesh.SetTransform();
                Mesh submesh = Mesh::Empty();
                submesh.SetTransform();
                for (int i = 0; i < element->childCount; i++)
                {
                    if (!Mesh::Empty().Equals(element->children[i]->supermesh) && element->children[i]->visible)
                    {
                        Mesh childmesh = element->children[i]->supermesh;
                        MeshGenerator::TransformMesh(&childmesh, element->children[i]->transform);
                        MeshGenerator::AddMesh(&submesh, &element->children[i]->supermesh);
                    }
                }
                MeshGenerator::AddMesh(&mesh, &submesh);
                element->supermesh = submesh;
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
        if (Mesh::Empty().Equals(element->supermesh))
        {
            MeshGenerator::CopyMesh(&mesh, &element->mesh);
            MeshGenerator::TransformMesh(&mesh, element->mesh.transform);
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
                transformElement = transformElement->parent;
            }
        } else
        {
            for (uint16_t i = 0; i < depth; i++)
            {
                if (transformElement->parent)
                {
                    MeshGenerator::TransformMesh(&mesh, transformElement->transform);
                    transformElement = transformElement->parent;
                } else { break; }
            }
        }
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
        
        for (int16_t i = 0; i < element->attribCount; i++)
        {
            recursiveAddAttribute(element, element->attributes[i]->type);
        }
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
                Render->DrawMesh(mesh, 0, 1, UIshader);
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
        for (uint16_t i = 0; i < UI_elementCount; i++)
        {
            CalculateSupermesh(UI_elementList[i], 1); // Calculate supermeshes
            UI_elementList[i]->supermesh.SetBoundingBox(CalculateRelativeBounds(UI_elementList[i], -1)); // Calculate relative bounds
        }
        return 0;
    }
    void Glayout::Update()
    {
        // Starting with all the buttons
        for (int i = 0; i < (int)UI_attributeMap[G_BUTTON_ATTRIB].size(); i++)
        {
            G_UIelement* element = UI_attributeMap[G_BUTTON_ATTRIB][i];
            G_UIelementAttribute* button = GetAttributeByType(element, G_BUTTON_ATTRIB);
            if (!button) { printf("Button without a button attribute! AttribCount: %d\n", element->attribCount); continue; }

            G_UIattribButton lastState = button->button;
            if (!lastState.isActive) { continue; }

            char stateChanged = 0;
            char hoverIn = 0;
            char hoverOut = 0;
            char press = 0;
            char release = 0;
            glm::vec2 mousePos = Input->ScreenToWorldSpace(Input->Mouse.MousePosition, glm::vec2(Gwindow->Width, Gwindow->Height));

            glm::mat4 transform = glm::mat4(1.0f);
            G_UIelement* parent = element;
            do {
                transform = TransformToMatrix(parent->transform);
                parent = parent->parent;
            } while (parent);

            AABox Tbounds = button->button.bounds;
            glm::vec4 bounds[4] = {
                glm::vec4(Tbounds.x, Tbounds.y, 0.0f, 1.0f),
                glm::vec4(Tbounds.x + Tbounds.width, Tbounds.y, 0.0f, 1.0f),
                glm::vec4(Tbounds.x + Tbounds.width, Tbounds.y + Tbounds.height, 0.0f, 1.0f),
                glm::vec4(Tbounds.x, Tbounds.y + Tbounds.height, 0.0f, 1.0f)
            };
            for (int j = 0; j < 4; j++)
            {
                bounds[j] = transform * bounds[j];
            }
            Tbounds.x = bounds[0].x;
            Tbounds.y = bounds[0].y;
            Tbounds.width = bounds[1].x - bounds[0].x;
            Tbounds.height = bounds[2].y - bounds[1].y;

            glm::vec4 mousePosT = glm::vec4(mousePos.x, mousePos.y, 0.0f, 1.0f);

            char isInBounds = PointInBounds(glm::vec2(mousePosT.x, -mousePosT.y), Tbounds);
            char isPressed = Input->Mouse.MouseButton[GLFW_MOUSE_BUTTON_LEFT] && isInBounds;

            if (isInBounds)
            {
                button->button.isHovered = 1;
                if (!lastState.isHovered)
                {
                    stateChanged = 1;
                    hoverIn = 1;
                }
            } else
            {
                button->button.isHovered = 0;
                if (lastState.isHovered)
                {
                    stateChanged = 1;
                    hoverOut = 1;
                }
            }

            if (isPressed)
            {
                button->button.isPressed = 1;
                if (!lastState.isPressed)
                {
                    stateChanged = 1;
                    press = 1;
                }
            } else
            {
                button->button.isPressed = 0;
                if (lastState.isPressed)
                {
                    stateChanged = 1;
                    release = 1;
                }
            }

            if (stateChanged)
            {
                if (button->button.onStateChange) { button->button.onStateChange(element, lastState); }
                if (hoverIn && button->button.onHoverIn) { button->button.onHoverIn(element); }
                if (hoverOut && button->button.onHoverOut) { button->button.onHoverOut(element); }
                if (press && button->button.onPress) { button->button.onPress(element); }
                if (release && button->button.onRelease) { button->button.onRelease(element); }
            }
        }
    }

    // Static functions
    // ----------------------------------------------------------------

    void Glayout::DefaultButtonStateChange(void* element, G_UIattribButton laststate)
    {
        G_UIelement* elementPtr = (G_UIelement*)element;
        G_UIattribButton button = GetAttributeByType(elementPtr, G_BUTTON_ATTRIB)->button;
        printf("Button state change: %llx\n", elementPtr->uniqueID);
    }
    void Glayout::DefaultButtonHoverIn(void* element)
    {
        G_UIelement* elementPtr = (G_UIelement*)element;
        elementPtr->supermesh.colour *= 2.0 / 3.0;
    }
    void Glayout::DefaultButtonHoverOut(void* element)
    {
        G_UIelement* elementPtr = (G_UIelement*)element;
        elementPtr->supermesh.colour *= 3.0 / 2.0;
    }
    void Glayout::DefaultButtonPress(void* element)
    {
        G_UIelement* elementPtr = (G_UIelement*)element;
        elementPtr->supermesh.colour *= 0.5;
    }
    void Glayout::DefaultButtonRelease(void* element)
    {
        G_UIelement* elementPtr = (G_UIelement*)element;
        elementPtr->supermesh.colour *= 2.0;
    }
}
