#include "../include/textfont.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mesh.hpp>

namespace Gengine
{
    int TextFont::calculateTextureCoords(Vertex* vertices, uint8_t symbol)
    {
        glm::vec2 symbolPos;
        glm::vec2 symbolSize;
        symbolPos.x = chars[symbol].x;
        symbolPos.y = chars[symbol].y;
        symbolSize.x = chars[symbol].width;
        symbolSize.y = chars[symbol].height;

        vertices[0].u = symbolPos.x / (float)fontTexture.width;
        vertices[0].v = (symbolPos.y + symbolSize.y) / (float)fontTexture.height;
        vertices[1].u = (symbolPos.x + symbolSize.x) / (float)fontTexture.width;
        vertices[1].v = (symbolPos.y + symbolSize.y) / (float)fontTexture.height;
        vertices[2].u = (symbolPos.x + symbolSize.x) / (float)fontTexture.width;
        vertices[2].v = symbolPos.y / (float)fontTexture.height;
        vertices[3].u = symbolPos.x / (float)fontTexture.width;
        vertices[3].v = symbolPos.y / (float)fontTexture.height;

        return 0;
    }

    int TextFont::LoadImage(const char* path)
    {
        fontTexture.LoadData(path);
        return 0;
    }
    int TextFont::LoadFont(const char* path)
    {
        FILE* file = fopen(path, "r");
        if (!file)
        {
            printf("Failed to open font file\n");
            return -1;
        }

        info_face = (char*)malloc(128 * sizeof(char));
        info_charset = (char*)malloc(128 * sizeof(char));
        page_file = (char*)malloc(128 * sizeof(char)); 

        if (!info_face || !info_charset || !page_file) {
            fprintf(stderr, "Failed to allocate memory for face or charset or pagefile\n");
            Free();
            fclose(file);
            return -1;
        }
        
        // Read the font info
        int result = fscanf(file, 
            "info face=\"%127[^\"]\" size=%hhu bold=%hhd italic=%hhd charset=\"%127[^\"]\" unicode=%hhd stretchH=%hu smooth=%hhd aa=%hhd padding=%hhd,%hhd,%hhd,%hhd spacing=%hhd,%hhd\n", 
            info_face, &info_size, &info_bold, &info_italic, info_charset, &info_unicode, &info_stretchH, &info_smooth, &info_aa, &info_padding[0], &info_padding[1], &info_padding[2], &info_padding[3], &info_spacing[0], &info_spacing[1]
        );
        if (result != 15) {
            fprintf(stderr, "Error reading font info, expected 16 items, got %d\n", result);
            Free();
            fclose(file);
            return -1;
        }

        // Read the common info
        result = fscanf(file, 
            "common lineHeight=%hu base=%hu scaleW=%hu scaleH=%hu pages=%hu packed=%hhd\n", 
            &common_lineHeight, &common_base, &common_scaleW, &common_scaleH, &common_pages, &common_packed
        );
        if (result != 6) {
            fprintf(stderr, "Error reading common info, expected 6 items, got %d\n", result);
            Free();
            fclose(file);
            return -1;
        }

        result = fscanf(file, "page id=%hd file=\"%127[^\"]\"\n", &page_id, page_file);
        if (result != 2) {
            fprintf(stderr, "Error reading page info, expected 2 items, got %d\n", result);
            Free();
            fclose(file);
            return -1;
        }

        // Read the chars count
        result = fscanf(file, "chars count=%hhu\n", &charsCount);
        if (result != 1) {
            fprintf(stderr, "Error reading chars count, expected 1 item, got %d\n", result);
            Free();
            fclose(file);
            return -1;
        }

        chars = (CharEntry*)malloc(sizeof(CharEntry) * 256); // ordered by ASCII
        memset(chars, 0, sizeof(CharEntry) * 256);

        // Read the chars
        for (int i = 0; i < charsCount; i++)
        {
            CharEntry newEntry = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            result = fscanf(file, 
                "char id=%hhu x=%hu y=%hu width=%hu height=%hu xoffset=%hd yoffset=%hd xadvance=%hd page=%hhu chnl=%hhu\n", 
                &newEntry.id, &newEntry.x, &newEntry.y, &newEntry.width, &newEntry.height, &newEntry.xOffset, &newEntry.yOffset, &newEntry.xAdvance, &newEntry.page, &newEntry.chnl
            );
            if (result != 10) {
                fprintf(stderr, "Error reading char %d, expected 10 items, got %d\n", i, result);
                Free();
                fclose(file);
                return -1;
            }
            chars[newEntry.id] = newEntry;
        }

        fclose(file);
        return 0;
    }
    int TextFont::Load(const char* path)
    {
        LoadFont(path);
        if (!info_face || !info_charset || !page_file) { return -1; }

        // get the folder path of the font file
        char* folderPath = (char*)malloc(128);
        int i = 0;
        for (i = strlen(path) - 1; i >= 0; i--)
        {
            if (path[i] == '/' || path[i] == '\\')
            {
                break;
            }
        }
        if (i < 0)
        {
            folderPath[0] = '\0';
        }
        else
        {
            strncpy(folderPath, path, i + 1);
            folderPath[i + 1] = '\0';
        }

        // construct the full path of the image file using that page_file is next to the font file
        char* imagePath = (char*)malloc(128);
        snprintf(imagePath, 128, "%s%s", folderPath, page_file);
        LoadImage(imagePath);
        
        return 0;
    }
    void TextFont::Free()
    {
        free(info_face);
        free(info_charset);
        free(page_file);
        free(chars);
        free(fontTexture.data);
    }
    int TextFont::CalculateTextMesh(Mesh* mesh, const char* text, float fontSize, uint16_t textLength, glm::vec4 textColour)
    {
        if (mesh->atlas)
        {
            TextureAtlas temp = *(TextureAtlas*)mesh->atlas;
            printf("Temp texture atlas: %d\n", temp.textureCount);
            mesh->Delete();
            mesh->atlas = (TextureAtlas*)malloc(sizeof(TextureAtlas));
            *(TextureAtlas*)mesh->atlas = temp;
        }
        else
        {
            mesh->Delete();
        }
        
        

        if (textLength == 0) { return 0; }

        glm::vec2 offset = glm::vec2(0.0, 0.0);
        for (int i = 0; i < textLength; i++)
        {
            CharEntry current = chars[(int)text[i]];
            if (current.id == 0) { continue; }
            if (text[i] == '\n')
            {
                if (i + 1 == textLength) { break; }
                offset.x = 0.0;
                offset.y -= fontSize;
                continue;
            }
            else if (text[i] == '\0') { break; }

            Mesh charMesh = Mesh::Empty();
            MeshGenerator::Rectangle(&charMesh, glm::vec2(current.width, current.height));
            charMesh.transform = NewTransform();
            charMesh.transform.position = glm::vec3(offset.x + ((float)current.xOffset + (float)current.width / 2.0) * fontSize / (float)info_size, offset.y - ((float)current.yOffset + (float)current.height / 2.0) * fontSize / (float)info_size, 0.0);
            charMesh.transform.scale = glm::vec3(fontSize / (float)info_size, fontSize / (float)info_size, 1.0);

            calculateTextureCoords(charMesh.GetVertices(), text[i]);

            MeshGenerator::AddMesh(mesh, &charMesh);
            offset.x += (float)current.xAdvance * fontSize / (float)info_size;
            charMesh.Delete();
        }
        mesh->SetColour(textColour);

        return 0;
    }
}
