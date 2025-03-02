#include "../include/textureatlas.hpp"
#include <stdio.h>
#include <string.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mesh.hpp>

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
        textures[textureCount]->atlasIndex = textureCount;
        textureCount++;
        return 0;
    }
    int TextureAtlas::Bake(const char* filepath)
    {
        if (textureCount < 1)
        {
            //printf("No textures to bake\n");
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
        //printf("Total width: %d, Max height: %d, textureCount: %d\n", totalWidth, maxHeight, textureCount);

        free(textureBounds);
        textureBounds = malloc(sizeof(AABox) * textureCount);

        uint16_t completion = 0;

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
                    if (y == textures[i]->height)
                    {
                        AABox bounds;
                        bounds.x = xOffset;
                        bounds.y = y - textures[i]->height;
                        bounds.z = 0;
                        bounds.width = textures[i]->width;
                        bounds.height = textures[i]->height;
                        bounds.depth = 0;
                        ((AABox*)textureBounds)[i] = bounds;
                        completion++;
                    }
                    
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
        width = totalWidth + (textureCount * 2);
        height = maxHeight + 2;
        nrChannels = 4;


        //printf("Success: %d/%d\n", completion, textureCount);

        glGenTextures(1, &textureID);
        if (!textureID)
        {
            printf("Failed to generate texture\n");
            return -1;
        }
        glBindTexture(GL_TEXTURE_2D, textureID);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        if (data)
        {
            if (nrChannels == 3)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else if (nrChannels == 4)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            printf("Failed to load texture\n");
        }
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
        atlas.elementReference = NULL;
        atlas.textureCount = 0;
        return atlas;
    }
}