#include "texture.h"
#include <src/stbi_image.h>

texture::texture():
    imageData(nullptr)
{
}

texture::~texture()
{
}

bool texture::load(std::string filePath)
{
    textureSource = filePath;

    // check library if image already loaded
    for (int i = 0; i < textureLib.size(); i++)
    {
        auto libPair = textureLib[i];
        if (textureSource == libPair.first)
        {
            this->ID = libPair.second;
            std::cout << "loaded texture from library w ID:" << this->ID << std::endl;
            return true;
        }
    }

    // loading of image using stb
    imageData = stbi_load((texturePath + filePath).c_str(), &width, &height, &numColorChannels, 0);

    if (imageData)
    {
        // generate texture object
        glGenTextures(1, &ID);
        bind();

        // filtering mode
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // sampling mode
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // generate texture based on loaded data - attaches tex image to bound texture
        // when texture image data is in RGB format
        if (numColorChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
                0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
            std::cout << "RGB texture created" << std::endl;
        }
        // when texture image data is in RGBA format
        else if (numColorChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
            std::cout << "RGBA texture created" << std::endl;
        }
        else
        {
            std::cout << "Unknown number of channels on Texture" << std::endl;
            stbi_image_free(imageData);
            return false;
        }

        // add to texture library
        auto libP = std::make_pair(textureSource, ID);
        textureLib.push_back(libP);
    }
    else
    {
        std::cout << "failed to load texture w / filepath:" << filePath << std::endl;
        return false;
    }

    // free image memory
    stbi_image_free(imageData);

    return true;
}

void texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, ID);
}


void texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
