#include "../include/laygui.hpp"
#include <stdio.h>
#include <render.hpp>
#include <shader.hpp>
#include <mesh.hpp>
#include <glm/glm.hpp>

using namespace std;

namespace Gengine
{
    // Private functions
    // ----------------------------------------------------------------

    // Mesh Glayout::recursiveMeshAdder(G_UIelement* element)
    // {
    //     Mesh mesh = element->mesh;
    //     for (int i = 0; i < element->childCount; i++)
    //     {
    //         Mesh childMesh = recursiveMeshAdder(element->children[i]);
    //         MeshGenerator::AddMesh(&mesh, &childMesh);
    //     }
    //     mesh.SetTransform(CombineTransforms(element->transform, mesh.GetTransform()));
    //     return mesh;
    // }
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
    // Transform Glayout::recursiveTransformCombiner(G_UIelement* element)
    // {
    //     Transform transform = element->transform;
    //     if (element->parent)
    //     {
    //         transform = CombineTransforms(recursiveTransformCombiner(element->parent), transform);
    //     }
    //     return transform;
    // }

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
                Mesh mesh = element->mesh;
                MeshGenerator::TransformMesh(&mesh, mesh.GetTransform());
                element->mesh.SetTransform();
                element->supermesh = mesh;
            } else {
                Mesh mesh = element->mesh;
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
                Mesh mesh = element->mesh;
                MeshGenerator::TransformMesh(&mesh, mesh.GetTransform());
                element->mesh.SetTransform();
                element->supermesh = mesh;
            } else {
                Mesh mesh = element->mesh;
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
        Mesh mesh = element->supermesh;
        MeshGenerator::TransformMesh(&mesh, element->transform);
        G_UIelement* transformElement = element;

        if (depth < 0) // no limit
        {
            while (transformElement->parent)
            {
                transformElement = transformElement->parent;
                MeshGenerator::TransformMesh(&mesh, transformElement->transform);
            }
        } else
        {
            for (int i = 0; i < depth; i++)
            {
                if (transformElement->parent)
                {
                    transformElement = transformElement->parent;
                    MeshGenerator::TransformMesh(&mesh, transformElement->transform);
                } else {
                    break;
                }
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
    void Glayout::AddButtonCallbacks(G_UIelement* element, void* onHoverIn, void* onHoverOut, void* onPress, void* onRelease)
    {
        G_UIelementAttribute* attribute = GetAttributeByType(element, G_BUTTON_ATTRIB);
        attribute->button.onHoverIn = onHoverIn;
        attribute->button.onHoverOut = onHoverOut;
        attribute->button.onPress = onPress;
        attribute->button.onRelease = onRelease;
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
    // AABox Glayout::CalculateFinalizedBounds(G_UIelement* element)
    // {
    //     Transform transform = recursiveTransformCombiner(element);
    //     Mesh mesh = element->mesh;
    //     mesh.SetTransform(CombineTransforms(transform, mesh.GetTransform()));
    //     MeshGenerator::CalculateBounds(&mesh);
    //     AABox box = mesh.GetBoundingBox();
    //     mesh.Delete();
    //     return box;
    // }

    // UI system functions
    // ----------------------------------------------------------------

    void Glayout::AddElement(G_UIelement element)
    {
        elementList.push_back(element);
        for (int i = 0; i < element.attribCount; i++)
        {
            recursiveAddAttribute(&element, element.attributes[i]->type);
        }
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
                for (int j = 0; j < (int)UI_attributeMap.size(); j++)
                {
                    recursiveRemoveAttribute(&elementList[i], (G_UIattribType)j);
                }
                break;
            }
        }
    }
    // Mesh Glayout::PackupMeshes(intptr_t uniqueID)
    // {
    //     G_UIelement* root = NULL;
    //     for (int i = 0; i < (int)elementList.size(); i++)
    //     {
    //         if (elementList[i].uniqueID == uniqueID)
    //         {
    //             root = &elementList[i];
    //             break;
    //         }
    //     }
    //     if (!root) { return Mesh::Empty(); }
    //     return recursiveMeshAdder(root);
    // }
    void Glayout::DrawElements(Renderer render, Shader shader)
    {
        for (int i = 0; i < (int)elementList.size(); i++)
        {
            if (elementList[i].visible)
            {
                Mesh mesh;
                if (Mesh::Empty().Equals(elementList[i].supermesh))
                {
                    mesh = elementList[i].mesh;
                    MeshGenerator::TransformMesh(&mesh, elementList[i].mesh.GetTransform());
                } else {
                    mesh = elementList[i].supermesh;
                }
                mesh.transform = elementList[i].transform;
                render.DrawMesh(mesh, 0, 1, shader);
            }
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
    int Glayout::Compile()
    {
        for (int i = 0; i < (int)elementList.size(); i++)
        {
            CalculateSupermesh(&elementList[i], 1); // Calculate supermeshes
            printf("\nOriginal mesh:\n");
            elementList[i].supermesh.Print();
            Mesh temp = elementList[i].supermesh;
            elementList[i].supermesh.SetBoundingBox(CalculateRelativeBounds(&elementList[i], 0)); // Calculate relative bounds
            printf("\nRelative bounds:\n");
            elementList[i].supermesh.Print();
            printf("Are meshes equal after bound calculation: %d\n", temp.Equals(elementList[i].supermesh));
            printf("Element %d supermesh vertex count: %d\n", i, elementList[i].supermesh.VertexCount);
        }
        return 0;
    }
}
