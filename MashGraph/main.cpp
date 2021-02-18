#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "object.h"
#include "framebuffer.h"
#include "global.h"

#include <vector>
#include <string>

extern Global Context;

void CalculateTnBvectors(std::vector<float>& verts, int vert_size)
{
    for (int i = 0; i < verts.size(); i += vert_size * 3) {
        glm::vec3 pos1(verts[i], verts[i + 1], verts[i + 2]);
        glm::vec3 pos2(verts[vert_size + i], verts[vert_size + i + 1], verts[vert_size + i + 2]);
        glm::vec3 pos3(verts[2 * vert_size + i], verts[2 * vert_size + i + 1], verts[2 * vert_size + i + 2]);

        glm::vec2 uv1(verts[i + 6], verts[i + 7]);
        glm::vec2 uv2(verts[vert_size + i + 6], verts[vert_size + i + 7]);
        glm::vec2 uv3(verts[2 * vert_size + i + 6], verts[2 * vert_size + i + 7]);

        glm::vec3 nm(verts[i + 3], verts[i + 4], verts[i + 5]);

        glm::vec3 vec1 = pos2 - pos1;
        glm::vec3 vec2 = pos3 - pos1;
        glm::vec2 dVU1 = uv2 - uv1;
        glm::vec2 dVU2 = uv3 - uv1;

        float A = (dVU1.x * dVU2.y - dVU2.x * dVU1.y);

        glm::vec3 tangent, bitangent;
        tangent.x = (dVU2.y * vec1.x - dVU1.y * vec2.x) / A;
        tangent.y = (dVU2.y * vec1.y - dVU1.y * vec2.y) / A;
        tangent.z = (dVU2.y * vec1.z - dVU1.y * vec2.z) / A;
        bitangent.x = (dVU1.x * vec2.x - dVU2.x * vec1.x) / A;
        bitangent.y = (dVU1.x * vec2.y - dVU2.x * vec1.y) / A;
        bitangent.z = (dVU1.x * vec2.z - dVU2.x * vec1.z) / A;

        for (int j = 0; j < 3 * vert_size; j += vert_size) {
            verts[i + j + 8] = tangent.x;
            verts[i + j + 9] = tangent.y;
            verts[i + j + 10] = tangent.z;
            verts[i + j + 11] = bitangent.x;
            verts[i + j + 12] = bitangent.y;
            verts[i + j + 13] = bitangent.z;
        }

    }
}

int main()
{
    glm::vec3 cube_positions[] = {
        glm::vec3(1.5f, -1.5f,  1.5f),
        glm::vec3(-2.0f, 2.0f, -2.0f),
        glm::vec3(1.0f,  2.0f,  2.0f),
        glm::vec3(-2.0f,-2.0f, -2.0f),
        glm::vec3(-1.5f, 0.0f,  1.5f),
        glm::vec3(2.0f,  0.0f, -1.0f),
    };
    glm::mat4 cube_rotation_mats[] = {
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat4(1.0f),
    };
    glm::vec3 light_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 light_specular = glm::vec3(0.7f, 0.7f, 0.0f);
    glm::vec3 light_diffuse = glm::vec3(0.5f, 0.1f, 0.1f);
    glm::vec3 light_ambient = glm::vec3(0.1f, 0.1f, 0.1f);

    glm::vec3 plane_position = glm::vec3(0.0f, -5.0f, 0.0f);

    std::vector<float> vertices = {
        // координаты         // нормали            // текстуры   //T                 //B
       -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,

        0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,

       -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
    };
    std::vector<int> offsets = { 0, 3, 6, 8, 11, 14 };
    std::vector<std::string> paths_to_textures = {
        "textures/cube_companion.jpg",
        "textures/cube_companion_metallic.jpg",
        "textures/cube_companion_normal.jpg",
        "textures/cube_companion_height.jpg",
        "textures/cube_companion_roughness.jpg"
    };

    CalculateTnBvectors(vertices, offsets[offsets.size() - 1]);

    Object Cube = Object(vertices, offsets, "shaders/vertex_shader.vsh", "shaders/fragment_shader.fsh", paths_to_textures, GL_TEXTURE_2D);
    Cube.shader.Use();
    Cube.shader.setInt("material.diffuse", 0);
    Cube.shader.setInt("material.specular", 1);
    Cube.shader.setInt("material.normal", 2);
    Cube.shader.setInt("material.height", 3);
    Cube.shader.setInt("material.roughness", 4);

    std::vector<float> skybox_vertices = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
    };
    std::vector<int> skybox_offsets = { 0, 3 };
    std::vector<std::string> skybox_paths_to_textures = {
        "textures/right.jpg",
        "textures/left.jpg",
        "textures/top.jpg",
        "textures/bottom.jpg",
        "textures/front.jpg",
        "textures/back.jpg"
    };
    Object Skybox = Object(skybox_vertices, skybox_offsets, "shaders/skybox.vsh", "shaders/skybox.fsh", skybox_paths_to_textures, GL_TEXTURE_CUBE_MAP);
    Skybox.shader.Use();
    Skybox.shader.setInt("skybox", 0);

    std::vector<float> light_vertices = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,

       -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,

       -0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,

       -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f, -0.5f,
    };
    std::vector<int> light_offsets = { 0, 3 };
    std::vector<std::string> light_paths_to_textures = {};
    Object Light = Object(light_vertices, light_offsets, "shaders/light.vsh", "shaders/light.fsh", light_paths_to_textures, 0);

    std::vector<float> plane_vertices = {
         1.0f, 0.0f,  1.0f,
        -1.0f, 0.0f,  1.0f,
        -1.0f, 0.0f, -1.0f,

         1.0f, 0.0f,  1.0f,
        -1.0f, 0.0f, -1.0f,
         1.0f, 0.0f, -1.0f,
    };
    std::vector<int> plane_offsets = { 0, 3 };
    std::vector<std::string> plane_paths_to_textures = { "textures/noise.jpg" };
    Object Plane = Object(plane_vertices, plane_offsets, "shaders/plane.vsh", "shaders/plane.fsh", plane_paths_to_textures, GL_TEXTURE_2D);
    Plane.shader.Use();
    Plane.shader.setInt("iTex", 0);


    Framebuffer Frame = Framebuffer("shaders/framebuffer.vsh", "shaders/framebuffer.fsh", Context.width, Context.height);
    Frame.screen.shader.Use();
    Frame.screen.shader.setInt("screen", 0);

    Context.objects.insert(std::make_pair("cube", &Cube));
    Context.objects.insert(std::make_pair("skybox", &Skybox));
    Context.objects.insert(std::make_pair("light", &Light));
    Context.objects.insert(std::make_pair("plane", &Plane));
    Context.buffers.insert(std::make_pair("frame", &Frame));

    // Цикл рендера
    while (!glfwWindowShouldClose(Context.window))
    {
        Context.UpdateCurrentTime();

        Context.ProcessInput();

        if (Context.state.post_effect) {
            Context.buffers["frame"]->PrepareRender();
        }
        Context.PrepareRender();
        glm::mat4 projection = Context.GetProjectionMatrix();
        glm::mat4 view = Context.camera.GetViewMatrix();

        Context.objects["light"]->PrepareRender();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, light_pos);
        model = glm::scale(model, glm::vec3(0.2f));
        Context.objects["light"]->shader.setMat4("model", model);
        Context.objects["light"]->shader.setMat4("view", view);
        Context.objects["light"]->shader.setMat4("projection", projection);
        Context.objects["light"]->Draw();
        glBindVertexArray(0);

        Context.objects["cube"]->PrepareRender();

        Context.objects["cube"]->shader.setMat4("view", view);
        Context.objects["cube"]->shader.setMat4("projection", projection);
        Context.objects["cube"]->shader.setFloat("material.shininess", Context.state.shininess_coeff);
        Context.objects["cube"]->shader.setVec3("light.ambient", light_ambient);
        Context.objects["cube"]->shader.setVec3("light.diffuse", light_diffuse);
        Context.objects["cube"]->shader.setVec3("light.specular", light_specular);
        Context.objects["cube"]->shader.setVec3("lightPos", light_pos);
        Context.objects["cube"]->shader.setVec3("viewPos", Context.camera.position);
        Context.objects["cube"]->shader.setFloat("height_coeff", Context.state.height_coeff);
        Context.objects["cube"]->shader.setBool("advanced_light_mode", Context.state.advanced_light);
        for (int i = 0; i < 6; ++i) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);
            if (Context.state.rotation) {
                cube_rotation_mats[i] = glm::rotate(cube_rotation_mats[i], glm::radians(20 * Context.delta_time), glm::normalize(glm::vec3(0.0, 1.0, 1.0)));
                cube_positions[i] = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(10 * Context.delta_time), glm::normalize(glm::vec3(-1.0, 1.0, 1.0)))) * cube_positions[i];
            }
            model = model * cube_rotation_mats[i];
            Context.objects["cube"]->shader.setMat4("model", model);
            Context.objects["cube"]->Draw();
        }
        glBindVertexArray(0);

        Context.objects["plane"]->PrepareRender();

        Context.objects["plane"]->shader.setMat4("view", view);
        Context.objects["plane"]->shader.setMat4("projection", projection);
        model = glm::mat4(1.0f);
        model = glm::translate(model, plane_position);
        model = glm::scale(model, glm::vec3(50.0f));
        Context.objects["plane"]->shader.setMat4("model", model);
        Context.objects["plane"]->shader.setFloat("iTime", glfwGetTime());
        Context.objects["plane"]->Draw();

        glBindVertexArray(0);

        glDepthFunc(GL_LEQUAL);
        Context.objects["skybox"]->PrepareRender();

        view = glm::mat4(glm::mat3(Context.camera.GetViewMatrix()));
        Context.objects["skybox"]->shader.setMat4("view", view);
        Context.objects["skybox"]->shader.setMat4("projection", projection);

        Context.objects["skybox"]->Draw();
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        if (Context.state.post_effect) {
            Context.buffers["frame"]->Draw();
        }
        Context.SwapBuffersNPollEvents();
    }
    Context.Terminate();
    return 0;
}