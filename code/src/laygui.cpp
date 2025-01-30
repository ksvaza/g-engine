#include "../include/laygui.hpp"
#include <mesh.hpp>
#include <glm/glm.hpp>
#include <functional>

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
        element->transform = (Transform){position: glm::vec3(0.0f, 0.0f, 0.0f), scale: glm::vec3(1.0f, 1.0f, 1.0f)};
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
        free((void*)element->uniqueID);
        if (element->attribCount > 0)
        {
            for (int i = 0; i < element->attribCount; i++)
            {
                free(element->attributes[i]);
            }
        }
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
        attribute.button = (G_UIattribButton){type: G_BUTTON_ATTRIB};
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

    void Glayout::AddElement(G_UIelement element)
    {
        elementList.push_back(element);
    }
    void Glayout::RemoveElement(intptr_t uniqueID)
    {
        for (int i = 0; i < elementList.size(); i++)
        {
            if (elementList[i].uniqueID == uniqueID)
            {
                for (int j = 0; j < elementList[i].childCount; j++)
                {
                    DeleteElement(elementList[i].children[j]);
                }
                elementList.erase(elementList.begin() + i);
                break;
            }
        }
    }
    Mesh Glayout::PackupMeshes(intptr_t uniqueID)
    {
        // Find requested root element
        G_UIelement* root = nullptr;
        for (int i = 0; i < elementList.size(); i++)
        {
            if (elementList[i].uniqueID == uniqueID)
            {
                root = &elementList[i];
                break;
            }
        }
        if (!root)
            return Mesh::Empty();

        // Merges src's vertices/indices into dest, offsetting the indices appropriately
        auto mergeMeshes = [&](Mesh& dest, Mesh& src)
        {
            int oldVertexCount = dest.VertexCount;
            int newVertexCount = oldVertexCount + src.VertexCount;
            int oldIndexCount  = dest.IndexCount;
            int newIndexCount  = oldIndexCount + src.IndexCount;

            // Backup existing data
            std::vector<Vertex> oldVertices(oldVertexCount);
            for(int i=0; i<oldVertexCount; i++)
                oldVertices[i] = dest.GetVertices()[i];
            std::vector<Index> oldIndices(oldIndexCount);
            for(int i=0; i<oldIndexCount; i++)
                oldIndices[i] = dest.GetIndices()[i];

            // Recreate dest with bigger arrays
            dest.Create(newVertexCount, newIndexCount);

            // Restore old data
            for(int i=0; i<oldVertexCount; i++)
                dest.GetVertices()[i] = oldVertices[i];
            for(int i=0; i<oldIndexCount; i++)
                dest.GetIndices()[i] = oldIndices[i];

            // Append new vertices
            for (int i = 0; i < src.VertexCount; i++)
                dest.GetVertices()[oldVertexCount + i] = src.GetVertices()[i];

            // Append new indices (each offset by oldVertexCount)
            for (int i = 0; i < src.IndexCount; i++)
            {
                Index idx = src.GetIndices()[i];
                // If Index is an integer type, just add oldVertexCount:
                idx.I[0] += oldVertexCount;
                idx.I[1] += oldVertexCount;
                idx.I[2] += oldVertexCount; 
                dest.GetIndices()[oldIndexCount + i] = idx;
            }
        };

        // Return a copy of the mesh transformed by t
        auto transformMesh = [&](Mesh& inMesh, const Transform& t) -> Mesh
        {
            Mesh outMesh;
            outMesh.Create(inMesh.VertexCount, inMesh.IndexCount);

            // Copy indices as-is
            for (int i = 0; i < inMesh.IndexCount; i++)
                outMesh.GetIndices()[i] = inMesh.GetIndices()[i];

            // Apply matrix transform to each vertex
            glm::mat4 mat = TransformToMatrix(t);
            for (int i = 0; i < inMesh.VertexCount; i++)
            {
                Vertex v = inMesh.GetVertices()[i];
                glm::vec4 pos = mat * glm::vec4(v.x, v.y, v.z, 1.0f);
                v.x = pos.x; 
                v.y = pos.y; 
                v.z = pos.z;
                outMesh.GetVertices()[i] = v;
            }
            return outMesh;
        };

        // Recursive function to gather child meshes
        std::function<Mesh(G_UIelement*, const Transform&)> gather =
        [&](G_UIelement* elem, const Transform& parentT) -> Mesh
        {
            // Combine transforms, build mesh
            Transform combined = CombineTransforms(parentT, elem->transform);
            Mesh current = transformMesh(elem->mesh, combined);

            // Gather children
            for(int i = 0; i < elem->childCount; i++)
            {
                Mesh childMesh = gather(elem->children[i], combined);
                mergeMeshes(current, childMesh);
            }
            return current;
        };

        // Start recursion from root, using an identity transform
        Transform rootT{};
        rootT.position = glm::vec3(0.f);
        rootT.rotation = glm::vec3(0.f);
        rootT.scale    = glm::vec3(1.f);

        return gather(root, rootT);
    }
}