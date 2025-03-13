#include "../include/uitext.hpp"
#include <uicreator.hpp>
#include <stdio.h>
#include <string.h>
#include <textfont.hpp>

namespace Gengine
{
    void GUI_text::updateTextAttribute()
    {
        if (!textAttribute)
        {
            printf("Attribute not found\n");
            return;
        }
        textAttribute->text.textContent = textContent;
        textLength = strlen(textContent);
        textAttribute->text.textLength = textLength;
        textAttribute->text.font = &font;
        textAttribute->text.fontSize = fontSize;
        textAttribute->text.textColour = textColour;
    }
    void GUI_text::getPositionofChar(char symbol, glm::vec2* index)
    {
        // Hardcoded for SUS-8 font

        // get SUS-8 char code
        uint8_t code = 0;
        if ((symbol >= '0') && (symbol < '9' + 1)) { code = symbol - 48; }
        else if ((symbol >= 'a') && (symbol < 'z' + 1) && (symbol != 'v')) { code = symbol - 87; }
        else if ((symbol >= 'A') && (symbol < 'Z' + 1)) { code = symbol - 55; }
        else
        {
            switch (symbol)
            {
            case ' ':
                code = 36;
                break;
            case '#':
                code = 37;
                break;
            case '=':
                code = 38;
                break;
            case '+':
                code = 39;
                break;
            case '-':
                code = 40;
                break;
            case '*':
                code = 41;
                break;
            case '/':
                code = 42;
                break;
            case '_':
                code = 44;
                break;
            case '<':
                code = 45;
                break;
            case '>':
                code = 46;
                break;
            case '?':
                code = 49;
                break;
            case '!':
                code = 50;
                break;
            case '\'':
                code = 51;
                break;
            case '^':
                code = 52;
                break;
            case 'v':
                code = 53;
                break;
            case '.':
                code = 54;
                break;
            case ',':
                code = 55;
                break;
            case ':':
                code = 56;
                break;
            case ';':
                code = 57;
                break;
            case '(':
                code = 58;
                break;
            case ')':
                code = 59;
                break;
            case '[':
                code = 60;
                break;
            case ']':
                code = 61;
                break;
            case '{':
                code = 62;
                break;
            case '}':
                code = 63;
                break;
            default:
                code = 64;
                break;
            }
        }

        // now calculate the index positions
        int row = code / 8;
        int column = code % 8;
        index->x = column;
        index->y = row;
        //printf("Symbol: %c, Code: %d, Row: %d, Column: %d\n", symbol, code, row, column);
    }
    void GUI_text::calculateTextureCoords(Vertex* vertices, char symbol)
    {
        glm::vec2 index;
        getPositionofChar(symbol, &index);
        float advance = 1.0 / 8.0;
        float pixel = advance * 1.0 / 6.0;
        float x = index.x * advance;
        float y = index.y * advance;
        float width = advance - pixel;
        float height = advance - pixel;

        vertices[0].u = x;
        vertices[0].v = y + height;
        vertices[1].u = x + width;
        vertices[1].v = y + height;
        vertices[2].u = x + width;
        vertices[2].v = y;
        vertices[3].u = x;
        vertices[3].v = y;
    }
    void GUI_text::calculateTextMesh()
    {
        if (!textElement)
        {
            printf("Element not found\n");
            return;
        }
        Mesh textMesh = Mesh::Empty();

        //Texture font;
        //font.Load(fontName); // SUS-8 font

        // Hardcoded for SUS-8 font
        const float defaultFontSize = 5.0;
        const float defaultAdvance = 6.0;

        float advance = defaultAdvance * fontSize / defaultFontSize;
        glm::vec2 offset = glm::vec2(advance / 2.0, 0.0);


        /*MeshGenerator::RegularShape(&textMesh, G_RECTANGLE);
        textMesh.transform = NewTransform();
        textMesh.transform.position = glm::vec3(offset, fontSize / 2.0, 0.0);
        textMesh.transform.scale = glm::vec3(fontSize * 3.0 / 4.0, fontSize, 1.0);*/
        uint16_t iOffset = 0;
        for (int i = 0; i < textLength; i++)
        {
            if (textContent[i] == '\n')
            {
                offset.x = advance / 2.0;
                offset.y -= advance;
                iOffset++;
                continue;
            }

            Mesh charMesh;
            MeshGenerator::RegularShape(&charMesh, G_RECTANGLE);
            charMesh.transform = NewTransform();

            calculateTextureCoords(charMesh.GetVertices(), textContent[i]);

            Transform charTransform;
            charTransform.position = glm::vec3(offset.x, offset.y - fontSize / 2.0, 0.0);
            charTransform.rotation = glm::vec3(0.0, 0.0, 0.0);
            charTransform.scale = glm::vec3(fontSize, fontSize, 1.0);
            MeshGenerator::TransformMesh(&charMesh, charTransform);

            MeshGenerator::AddMesh(&textMesh, &charMesh);
            offset.x += advance;
            charMesh.Delete();
        }
        textMesh.FillColour(1.0, 1.0, 1.0, 1.0);
        //textMesh.AddTexture(font);
        textMesh.FillTextureID(16);
        textMesh.SetColour(textColour);

        textElement->mesh.Delete();
        textElement->mesh = textMesh;
    }

    int GUI_text::Create(const char* textContent, glm::vec2 size, glm::vec3 position, glm::vec4 textColour, TextFont font, uint16_t fontSize)
    {
        textElement = (G_UIelement*)malloc(sizeof(G_UIelement));
        this->size = size;
        this->textContent = (char*)textContent;
        textLength = strlen(textContent);
        this->font = font;
        this->fontSize = fontSize;
        this->textColour = textColour;

        Glayout::CreateElement(textElement, G_TEXT);
        G_UIelementAttribute attribute;
        G_UIattribText text;
        text.type = G_TEXT_ATTRIB;
        text.textContent = this->textContent;
        text.textLength = this->textLength;
        text.font = &this->font;
        text.fontSize = this->fontSize;
        attribute.text = text;
        attribute.text.text = this;
        Glayout::AddAttribute(textElement, attribute);
        textAttribute = Glayout::GetAttributeByType(textElement, G_TEXT_ATTRIB);
        textElement->transform = NewTransform();
        textElement->transform.position = position;

        if (this->font.charsCount > 0)
        {
            this->font.CalculateTextMesh(&textElement->mesh, textContent, fontSize, textLength, textColour);
            textElement->mesh.AddTexture(this->font.fontTexture);
        }

        return 0;
    }
    int GUI_text::UpdateText()
    {
        updateTextAttribute();
        if (this->font.charsCount > 0)
        {
            this->font.CalculateTextMesh(&textElement->mesh, textContent, fontSize, textLength, textColour);
            textElement->mesh.AddTexture(this->font.fontTexture);
        }
        return 0;
    }
    void GUI_text::SetReferenceLayout(Glayout* layout)
    {
        referenceLayout = layout;
    }
    void GUI_text::AddToLayout()
    {
        if (referenceLayout)
        {
            referenceLayout->AddElement(textElement);
        }
    }
    void GUI_text::AddAsChild(G_UIelement* parent)
    {
        Glayout::AddChild(parent, textElement);
    }
    G_UIelement* GUI_text::TextElement()
    {
        return textElement;
    }
    void GUI_text::RemoveFromLayout()
    {
        if (referenceLayout)
        {
            referenceLayout->RemoveElement(textElement);
        }
    }
    void GUI_text::Delete()
    {
        free(textContent);
        font.Free();
        Glayout::DeleteElement(textElement);
    }
}