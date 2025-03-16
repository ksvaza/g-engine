#include "../include/texture.hpp"
#include <stdio.h>
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Gengine
{
    int Texture::LoadData(const char* filepath)
    {
        data = stbi_load(filepath, &width, &height, &nrChannels, 0);
        if (!data)
        {
            printf("Failed to load texture %s\n", filepath);
            return -1;
        }
        return 0;
    }
    void Texture::FreeData()
    {
        if (data)
        {
            stbi_image_free(data);
        }
    }
    int Texture::Load(const char* filepath)
    {
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
        unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);
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
        stbi_image_free(data);
        return 0;
    }
    Texture Texture::Empty()
    {
        Texture texture;
        texture.textureID = 0U;
        texture.data = NULL;
        texture.width = 0;
        texture.height = 0;
        texture.nrChannels = 0;
        texture.atlas = NULL;
        texture.atlasIndex = -1;
        return texture;
    }
}