#include "framebuffer.h"

Framebuffer::Framebuffer(std::string path_to_vertex_shader, std::string path_to_fragment_shader, unsigned int width, unsigned int height)
{
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    std::vector<float> screen_verts = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    std::vector<int> screen_offsets = { 0, 2, 4 };
    std::vector<std::string> paths = { std::string{} };
    screen = Object(screen_verts, screen_offsets, path_to_vertex_shader, path_to_fragment_shader, paths, GL_TEXTURE_2D, GL_RGB, width, height);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, screen.textures[0].type, screen.textures[0].id, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::PrepareRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Framebuffer::Draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    screen.PrepareRender();
    screen.Draw();
}

void Framebuffer::Terminate()
{
    screen.Terminate();
    glDeleteFramebuffers(1, &FBO);
}

void Framebuffer::UpdateScreenSize(int w, int h)
{
    screen.textures[0].width = w;
    screen.textures[0].height = h;
    glBindTexture(GL_TEXTURE_2D, screen.textures[0].id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

