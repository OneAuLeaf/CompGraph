#include "object.h"

Object::Object(std::vector<float>& vert, std::vector<int>& offsets,
    std::string path_to_vertex_shader, std::string path_to_fragment_shader,
    std::vector<std::string>& paths_to_textures, unsigned int textures_type, unsigned int color_sheme, unsigned int w, unsigned int h) : vertices{vert}, vertice_offsets{offsets}
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    int num_of_coords_type = vertice_offsets.size() - 1;
    for (int i = 0; i < num_of_coords_type; ++i)
    {
        glVertexAttribPointer(i, vertice_offsets[i + 1] - vertice_offsets[i],
            GL_FLOAT, GL_FALSE, vertice_offsets[num_of_coords_type] * sizeof(float), (void*)(vertice_offsets[i] *sizeof(float)));
        glEnableVertexAttribArray(i);
    }

    shader = ShaderProgram(path_to_vertex_shader, path_to_fragment_shader);

    textures = std::vector<Texture>();
    if (textures_type == GL_TEXTURE_CUBE_MAP) {
        textures.push_back(Texture(paths_to_textures, color_sheme, w, h));
    }
    else {
        for (std::string path : paths_to_textures) {
            textures.push_back(Texture(path, textures_type, color_sheme, w, h));
        }
    }
}

void Object::PrepareRender()
{
    glBindVertexArray(VAO);
    for (int i = 0; i < textures.size(); ++i) {
        textures[i].Attach(i);
    }
    shader.Use();
}

void Object::Draw()
{
    int num_of_coords_type = vertice_offsets.size() - 1;
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / vertice_offsets[num_of_coords_type]);
}

void Object::Terminate()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

