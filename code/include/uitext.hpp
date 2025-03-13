#pragma once
#include <stdint.h>
#include <laygui.hpp>
#include <uicreator.hpp>
#include <glm/glm.hpp>
#include <textfont.hpp>

namespace Gengine
{
    class GUI_text
    {
    private:
        Glayout* referenceLayout = NULL;
        G_UIelement* textElement = NULL;
        G_UIelementAttribute* textAttribute = NULL;
        void updateTextAttribute();
        void getPositionofChar(char symbol, glm::vec2* index);
        void calculateTextureCoords(Vertex* vertices, char symbol);
        void calculateTextMesh();
    public:
        glm::vec2 size = glm::vec2(0.0);
        char* textContent = NULL;
        uint16_t textLength = 0;
        TextFont font;
        uint16_t fontSize = 0;
        glm::vec4 textColour = glm::vec4(1.0);
        
        int Create(const char* textContent, glm::vec2 size, glm::vec3 position, glm::vec4 textColour, TextFont font, uint16_t fontSize);
        int Resize(glm::vec2 size);
        int Recolour(glm::vec4 textColour);
        int UpdateText();
        void SetReferenceLayout(Glayout* layout);
        void AddToLayout();
        void RemoveFromLayout();
        void AddAsChild(G_UIelement* parent);
        G_UIelement* TextElement();
        void Delete();
    };
}