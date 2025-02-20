#include "../include/texgen.hpp"
#include <stdio.h>
#include <string.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Gengine
{
    int TextureAtlas::AddTexture(Texture* texture)
    {
        if (!texture)
        {
            printf("Null texture\n");
            return -1;
        }
        
        textures = (Texture**)realloc(textures, sizeof(Texture*) * (textureCount + 1));
        texture->atlas = this;
        textures[textureCount] = texture;
        textureCount++;
        return 0;
    }
    int TextureAtlas::Bake(const char* filepath)
    {
        if (textureCount < 1)
        {
            printf("No textures to bake\n");
            return -1;
        }

        int16_t maxHeight = 0;
        int32_t totalWidth = 0;
        for (int i = 0; i < textureCount; i++)
        {
            if (textures[i]->height > maxHeight)
            {
                maxHeight = textures[i]->height;
            }
            totalWidth += textures[i]->width;
        }
        data = (unsigned char*)malloc((maxHeight + 2) * (totalWidth + (textureCount * 2)) * 4);
        printf("Total width: %d, Max height: %d, textureCount: %d\n", totalWidth, maxHeight, textureCount);

        memset(data, 0, (totalWidth + (textureCount * 2)) * 4);
        for (int y = 1; y < maxHeight + 2; y++)
        {
            int32_t xOffset = 0;
            for (uint16_t i = 0; i < textureCount; i++)
            {
                memset(data + 4 * (xOffset + y * (totalWidth + (textureCount * 2))), 0, 4);
                xOffset++;
                if (y >= textures[i]->height)
                {
                    memset(data + 4 * (xOffset + y * (totalWidth + (textureCount * 2))), 0, textures[i]->width * 4);
                    xOffset += textures[i]->width;
                }
                else
                {
                    memcpy(data + 4 * (xOffset + y * (totalWidth + (textureCount * 2))), textures[i]->data + y * textures[i]->width * 4, textures[i]->width * 4);
                    xOffset += textures[i]->width;
                }
                memset(data + 4 * (xOffset + y * (totalWidth + (textureCount * 2))), 0, 4);
                xOffset++;
            }
        }
        stbi_write_bmp(filepath, totalWidth + (textureCount * 2), maxHeight + 2, 4, data);
        free(data);


        /*int64_t totalDataSize = 0;
        double totalArea = 0.0;
        for (int i = 0; i < textureCount; i++)
        {
            totalDataSize += textures[i].width * textures[i].height * textures[i].nrChannels;
            totalArea += textures[i].width * textures[i].height;
        }
        data = (unsigned char*)malloc(totalDataSize);
        intptr_t dataOffset = 0;
        for (int i = 0; i < textureCount; i++)
        {
            memcpy(data + dataOffset, textures[i].data, textures[i].width * textures[i].height * textures[i].nrChannels);
            dataOffset += textures[i].width * textures[i].height * textures[i].nrChannels;
        }
        stbi_write_bmp(filepath, textures[0].width, 2.0 * textures[0].height, 4, data);*/
        return 0;
    }

    TextureAtlas TextureAtlas::Empty()
    {
        TextureAtlas atlas;
        atlas.textureID = 0U;
        atlas.data = NULL;
        atlas.width = 0;
        atlas.height = 0;
        atlas.nrChannels = 0;
        atlas.textures = NULL;
        atlas.textureBounds = NULL;
        atlas.textureCount = 0;
        return atlas;
    }
}