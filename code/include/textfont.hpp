#pragma once
#include <texture.hpp>
#include <mesh.hpp>

namespace Gengine
{
    struct CharEntry
    {
        uint8_t id;
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;
        int16_t xOffset;
        int16_t yOffset;
        int16_t xAdvance;
        uint8_t page;
        uint8_t chnl;
    };

    class TextFont
    {
    private:
        // info
        char* info_face = NULL;
        uint8_t info_size = 0;
        int8_t info_bold = 0;
        int8_t info_italic = 0;
        char* info_charset = NULL;
        int8_t info_unicode = 0; // not supported
        uint16_t info_stretchH = 0;
        int8_t info_smooth = 0;
        int8_t info_aa = 0;
        int8_t info_padding[4] = { 0, 0, 0, 0 };
        int8_t info_spacing[2] = { 0, 0 };

        // common
        uint16_t common_lineHeight = 0;
        uint16_t common_base = 0;
        uint16_t common_scaleW = 0;
        uint16_t common_scaleH = 0;
        uint16_t common_pages = 0;
        int8_t common_packed = 0;

        // page
        uint16_t page_id = 0;
        char* page_file = NULL;

        CharEntry* chars = NULL;

        int calculateTextureCoords(Vertex* vertices, uint8_t symbol);
    public:
        uint8_t charsCount = 0;
        Texture fontTexture;

        int LoadImage(const char* path);
        int LoadFont(const char* path);
        int Load(const char* path);
        void Free();
        int CalculateTextMesh(Mesh* mesh, const char* text, float fontSize, uint16_t textLength, glm::vec4 textColour);
    };
}