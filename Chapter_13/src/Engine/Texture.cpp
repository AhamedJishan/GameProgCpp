#include "Texture.h"

#include <GLEW/GL/glew.h>
#include <stbi/stb_image.h>
#include <cstdio>
#include <algorithm>

namespace jLab
{
    Texture::Texture()
    {
        mId = 0;
        mWidth = 0;
        mHeight = 0;
        mType = Type::Diffuse;
    }

    Texture::~Texture()
    {
        Unload();
    }
    
    bool Texture::Load(const std::string& filename, bool flipVertically, Type type)
    {
        Unload();

        int nrChannels = 0;

        mType = type;
        mName = filename;
        stbi_set_flip_vertically_on_load(flipVertically);
        unsigned char* data = stbi_load(mName.c_str(), &mWidth, &mHeight, &nrChannels, 0);

        if (!data)
        {
            printf("ERROR: Failed to load texture '%s'", mName.c_str());
            return false;
        }

        GLenum format;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;
        else
        {
            printf("ERROR: Texture '%s' has unsupported number of channels : %d", mName.c_str(), nrChannels);
            return false;
        }

        glGenTextures(1, &mId);
        glBindTexture(GL_TEXTURE_2D, mId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        return true;
    }

    bool Texture::Load(const std::string& filename, const aiTexture* textureData, bool flipVertically, Type type)
    {
        Unload();

        mType = type;
        mName = filename;

        unsigned char* data = nullptr;
        int nrChannels = 0;

        if (mHeight == 0)
        {
            // Compressed
            int sizeInBytes = static_cast<int>(textureData->mWidth);
            unsigned char* blob = reinterpret_cast<unsigned char*>(textureData->pcData);

            stbi_set_flip_vertically_on_load(flipVertically);
            data = stbi_load_from_memory(blob, sizeInBytes, &mWidth, &mHeight, &nrChannels, STBI_rgb_alpha);

            if (!data)
            {
                printf("ERROR: Failed to load texture '%s'", mName.c_str());
                return false;
            }
        }
        else
        {
            // Raw data
            mWidth = textureData->mWidth;
            mHeight = textureData->mHeight;
            nrChannels = 4;

            size_t datasize = static_cast<size_t>(mWidth) * static_cast<size_t>(mHeight) * 4u;
            data = new unsigned char[datasize];
            memcpy(data, textureData->pcData, datasize);
        }

        GLenum format;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;
        else
        {
            printf("ERROR: Texture '%s' has unsupported number of channels : %d", mName.c_str(), nrChannels);
            return false;
        }

        glGenTextures(1, &mId);
        glBindTexture(GL_TEXTURE_2D, mId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (textureData->mHeight == 0)
            stbi_image_free(data);
        else
            delete[] data;

        return true;
    }
    
    void Texture::CreateFromTexture(SDL_Surface* surface)
    {
        Unload();

        mWidth = surface->w;
        mHeight = surface->h;

        glGenTextures(1, &mId);
        glBindTexture(GL_TEXTURE_2D, mId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::CreateForRendering(int width, int height, unsigned int format)
    {
        Unload();

        mWidth = width;
        mHeight = height;
        glGenTextures(1, &mId);
        glBindTexture(GL_TEXTURE_2D, mId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    
    void Texture::Unload()
    {
        if (mId != 0)
            glDeleteTextures(1, &mId);

        mId = 0;
        mWidth = 0;
        mHeight = 0;
        mType = Type::Diffuse;
    }

    void Texture::SetActive(int slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, mId);
    }
}