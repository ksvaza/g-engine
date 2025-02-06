#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <mesh.hpp>
#include <render.hpp>
#include <shader.hpp>
#include <transform.hpp>
#include <vector>
#include <map>

namespace Gengine
{
    // Enumerators
    // ----------------------------------------------------------------

    enum G_UIelementType {
        G_EMPTY = 0x00,
        G_MESH = 0x01,
        G_BUTTON = 0x02,
        G_PARENT = 0x03,
    };
    enum G_UIattribType {
        G_EMPTY_ATTRIB = 0x00,
        G_BUTTON_ATTRIB = 0x01,
    };
    enum G_UIbuttonState {
        G_IDLE = 0x00,
        G_HOVER = 0x01,
        G_PRESS = 0x02,
        G_RELEASE = 0x03,
        G_DISABLED = 0x04,
    };

    // Structs and Unions
    // ----------------------------------------------------------------

    struct G_UIattribButton {
        G_UIattribType type = G_EMPTY_ATTRIB;
        AABox bounds = { 0.0, 0.0, 0.0, 0.0 };
        void* onHoverIn = NULL;
        void* onHoverOut = NULL;
        void* onPress = NULL;
        void* onRelease = NULL;
        G_UIbuttonState state = G_IDLE;
    };
    union G_UIelementAttribute {
        G_UIattribType type = G_EMPTY_ATTRIB;
        G_UIattribButton button;
        G_UIelementAttribute() {}
        ~G_UIelementAttribute() {}
    };
    struct G_UIelement{
        Mesh mesh;
        Mesh supermesh;
        int8_t visible = 1;
        intptr_t uniqueID = 0;
        G_UIelementType type = G_EMPTY;
        Transform transform;
        G_UIelement* parent;
        G_UIelement** children = NULL;
        G_UIelementAttribute** attributes = NULL;
        int16_t childCount;
        int16_t attribCount;
    };

    class Glayout
    {
    private:
        std::vector<G_UIelement> elementList;
        //std::vector<G_UIelement*> UI_buttonList;
        std::map<G_UIattribType, std::vector<G_UIelement*>> UI_attributeMap;
        Mesh recursiveMeshAdder(G_UIelement* element);
        //void recursiveAddButtons(G_UIelement* element, std::vector<G_UIelement*>* buttonList);
        //void recursiveRemoveButtons(G_UIelement* element, std::vector<G_UIelement*>* buttonList);
        void recursiveAddAttribute(G_UIelement* element, G_UIattribType type);
        void recursiveRemoveAttribute(G_UIelement* element, G_UIattribType type);
        static Transform recursiveTransformCombiner(G_UIelement* element);
    public:
        // G_UIelement basic functions
        static void CreateElement(G_UIelement* element, G_UIelementType type);
        static void DeleteElement(G_UIelement* element);
        static void AddAttribute(G_UIelement* element, G_UIelementAttribute attribute);
        static void AddChild(G_UIelement* parent, G_UIelement* child);
        static G_UIelementAttribute* GetAttributeByType(G_UIelement* element, G_UIattribType type);
        static int8_t HasAttribute(G_UIelement* element, G_UIattribType type);
        static void CalculateSupermesh(G_UIelement* element, char all);
        static AABox CalculateRelativeBounds(G_UIelement* element, uint16_t depth);
        
        // Assembled functions
        static void CreateButton(G_UIelement* element);
        static void AddButtonCallbacks(G_UIelement* element, void* onHoverIn, void* onHoverOut, void* onPress, void* onRelease);
        static void SortChildren(G_UIelement* parent);
        static AABox CalculateFinalizedBounds(G_UIelement* element);

        // UI system functions
        void AddElement(G_UIelement element);
        void RemoveElement(intptr_t uniqueID);
        Mesh PackupMeshes(intptr_t uniqueID);
        void UpdateAttributeMap();
        void DrawElements(Renderer render, Shader shader);
        G_UIelement* GetElementByUniqueID(intptr_t uniqueID);
        void Update();
        int Compile();
    };
}