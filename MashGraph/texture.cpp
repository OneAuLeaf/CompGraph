#include "texture.h"

Texture::Texture(std::string path, unsigned int type, unsigned int color_scheme, unsigned int w, unsigned int h) : type{type}, width{w}, height{h}
{
    glGenTextures(1, &id);
    glBindTexture(type, id);

    if (path.empty()) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }
    else {
        LoadNInit(path, color_scheme);
    }

    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(type, 0);

}

Texture::Texture(std::vector<std::string> paths, unsigned int color_scheme, unsigned int w, unsigned int h) : type{GL_TEXTURE_CUBE_MAP}, width{w}, height{h}
{
    glGenTextures(1, &id);
    glBindTexture(type, id);

    LoadNInit(paths, color_scheme);

    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::LoadNInit(std::vector<std::string> paths, unsigned color_scheme)
{
    int w, h, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < paths.size(); i++)
    {
        unsigned char* data = stbi_load(paths[i].c_str(), &w, &h, &nrChannels, 0);
        if (data)
        {
            if (width != 0) w = width;
            else width = w;
            if (height != 0) h = height;
            else height = h;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, color_scheme, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "With path: " << paths[i] << "Failed to load cubemap texture " <<  std::endl;
            
        }
        stbi_image_free(data);
    }
}

void Texture::LoadNInit(std::string path, unsigned color_scheme)
{
    int w, h, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &nrChannels, 0);
    if (data) {
        if (width != 0) w = width;
        else width = w;
        if (height != 0) h = height;
        else height = h;
        glTexImage2D(type, 0, GL_RGB, w, h, 0, color_scheme, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(type);
    }
    else {
        std::cout << "With path: " << path << " Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}


void Texture::Attach(unsigned int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(type, id);
}

