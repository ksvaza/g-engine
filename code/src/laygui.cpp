#include "../include/laygui.hpp"
#include <render.hpp>
#include <shader.hpp>
#include <mesh.hpp>
#include <glm/glm.hpp>

using namespace std;

namespace Gengine
{
    // G_UIelement basic functions
    // ----------------------------------------------------------------

    void Glayout::CreateElement(G_UIelement* element, G_UIelementType type)
    {
        element->mesh = Mesh::Empty();
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
    void Glayout::AddAttribute(G_UIelement* element, G_UIelementAttribute attribute)
    {
        element->attributes = (G_UIelementAttribute**)realloc(element->attributes, sizeof(G_UIelementAttribute*) * (element->attribCount + 1));
        element->attributes[element->attribCount] = (G_UIelementAttribute*)malloc(sizeof(G_UIelementAttribute));
        *element->attributes[element->attribCount] = attribute;
        element->attribCount++;
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

    // Assembled functions
    // ----------------------------------------------------------------

    void Glayout::CreateButton(G_UIelement* element)
    {
        CreateElement(element, G_BUTTON);
        G_UIelementAttribute attribute;
        G_UIattribButton button;
        button.type = G_BUTTON_ATTRIB;
        attribute.button = button;
        AddAttribute(element, attribute);
    }
    void Glayout::AddButtonCallbacks(G_UIelement* element, void* onHoverIn, void* onHoverOut, void* onPress, void* onRelease)
    {
        G_UIelementAttribute* attribute = GetAttributeByType(element, G_BUTTON_ATTRIB);
        attribute->button.onHoverIn = onHoverIn;
        attribute->button.onHoverOut = onHoverOut;
        attribute->button.onPress = onPress;
        attribute->button.onRelease = onRelease;
    }
    void Glayout::AddChild(G_UIelement* parent, G_UIelement* child)
    {
        parent->children = (G_UIelement**)realloc(parent->children, sizeof(G_UIelement*) * (parent->childCount + 1));
        parent->children[parent->childCount] = child;
        parent->childCount++;
    }

    // UI system functions
    // ----------------------------------------------------------------

    void Glayout::recursiveAddButtons(G_UIelement* element, std::vector<G_UIelement*>* buttonList)
    {
        if (element->type == G_BUTTON)
        {
            buttonList->push_back(element);
        }
        for (int i = 0; i < element->childCount; i++)
        {
            recursiveAddButtons(element->children[i], buttonList);
        }
    }
    void Glayout::recursiveRemoveButtons(G_UIelement* element, std::vector<G_UIelement*>* buttonList)
    {
        if (element->type == G_BUTTON)
        {
            for (int i = 0; i < (int)buttonList->size(); i++)
            {
                if ((*buttonList)[i]->uniqueID == element->uniqueID)
                {
                    buttonList->erase(buttonList->begin() + i);
                    break;
                }
            }
        }
        for (int i = 0; i < element->childCount; i++)
        {
            recursiveRemoveButtons(element->children[i], buttonList);
        }
    }
    void Glayout::AddElement(G_UIelement element)
    {
        elementList.push_back(element);
        recursiveAddButtons(&element, &UI_buttonList);
    }
    void Glayout::RemoveElement(intptr_t uniqueID)
    {
        for (int i = 0; i < (int)elementList.size(); i++)
        {
            if (elementList[i].uniqueID == uniqueID)
            {
                for (int j = 0; j < elementList[i].childCount; j++)
                {
                    DeleteElement(elementList[i].children[j]);
                }
                elementList.erase(elementList.begin() + i);
                recursiveRemoveButtons(&elementList[i], &UI_buttonList);
                break;
            }
        }
    }
    Mesh Glayout::recursiveMeshAdder(G_UIelement* element)
    {
        Mesh mesh = element->mesh;
        for (int i = 0; i < element->childCount; i++)
        {
            Mesh childMesh = recursiveMeshAdder(element->children[i]);
            MeshGenerator::AddMesh(&mesh, &childMesh);
        }
        mesh.SetTransform(CombineTransforms(element->transform, mesh.GetTransform()));
        return mesh;
    }
    Mesh Glayout::PackupMeshes(intptr_t uniqueID)
    {
        G_UIelement* root = NULL;
        for (int i = 0; i < (int)elementList.size(); i++)
        {
            if (elementList[i].uniqueID == uniqueID)
            {
                root = &elementList[i];
                break;
            }
        }
        if (!root) { return Mesh::Empty(); }

        return recursiveMeshAdder(root);
    }
    void Glayout::DrawElements(Renderer render, Shader shader)
    {
        for (int i = 0; i < (int)elementList.size(); i++)
        {
            Mesh mesh = PackupMeshes(elementList[i].uniqueID);
            render.DrawMesh(mesh, 0, 0, shader);
        }
    }
    G_UIelement* Glayout::GetElementByUniqueID(intptr_t uniqueID)
    {
        for (int i = 0; i < (int)elementList.size(); i++)
        {
            if (elementList[i].uniqueID == uniqueID)
            {
                return &elementList[i];
            }
        }
        return NULL;
    }
}
