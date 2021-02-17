#include "global.h"

const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 800;

Global Context = Global(WIDTH, HEIGHT);

EffectState::EffectState()
{
    advanced_light = false;
    post_effect = false;
    rotation = false;

    height_coeff = 0.1f;
    shininess_coeff = 64.0f;
}

Global::Global(unsigned int width, unsigned int height) : width{width}, height{height}
{
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    last_x = width / 2.0f;
    last_y = height / 2.0f;
    first_mouse = true;

    delta_time = 0.0f;
    last_frame = 0.0f;

    key_pressed = 0;

    Init();

    window = glfwCreateWindow(width, height, "MashGraph", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    Configure();
}

glm::mat4 Global::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(camera.fov), (float) width / (float) height, 0.1f, 100.0f);
}


void Global::UpdateCurrentTime()
{
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
}

void Global::ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, delta_time);
    if (!key_pressed) {

    }
    if (!key_pressed) {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            state.post_effect = !state.post_effect;
            key_pressed = GLFW_KEY_E;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            state.rotation = !state.rotation;
            key_pressed = GLFW_KEY_R;
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
            state.advanced_light = !state.advanced_light;
            key_pressed = GLFW_KEY_T;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            state.height_coeff += 0.02f;
            if (state.height_coeff > 1.0f) {
                state.height_coeff = 1.0f;
            }
            key_pressed = GLFW_KEY_UP;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            state.height_coeff -= 0.02f;
            if (state.height_coeff < 0.0f) {
                state.height_coeff = 0.0f;
            }
            key_pressed = GLFW_KEY_DOWN;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            state.shininess_coeff *= 2.0f;
            if (state.shininess_coeff > 512.0f) {
                state.shininess_coeff = 512.0f;
            }
            key_pressed = GLFW_KEY_RIGHT;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            state.shininess_coeff /= 2.0f;
            if (state.shininess_coeff < 2.0f) {
                state.shininess_coeff = 2.0f;
            }
            key_pressed = GLFW_KEY_LEFT;
        }

    } else {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE && key_pressed == GLFW_KEY_E) {
            key_pressed = false;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE && key_pressed == GLFW_KEY_R) {
            key_pressed = false;
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE && key_pressed == GLFW_KEY_T) {
            key_pressed = false;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && key_pressed == GLFW_KEY_UP) {
            key_pressed = false;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && key_pressed == GLFW_KEY_DOWN) {
            key_pressed = false;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE && key_pressed == GLFW_KEY_RIGHT) {
            key_pressed = false;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && key_pressed == GLFW_KEY_LEFT) {
            key_pressed = false;
        }
    }
}

void Global::PrepareRender()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Global::SwapBuffersNPollEvents()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Global::Terminate()
{
    for (auto object : objects) {
        object.second->Terminate();
    }
    for (auto buffer : buffers) {
        buffer.second->Terminate();
    }
    glfwTerminate();
}

void Global::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Global::Configure()
{
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, ProxyFramebufferSizeCallback);
    glfwSetCursorPosCallback(window, ProxyMouseMoveCallback);
    glfwSetScrollCallback(window, ProxyMouseScrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
}

void Global::FramebufferSizeCallback(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    for (auto buffer : buffers) {
        buffer.second->UpdateScreenSize(w, h);
    }
}

void Global::MouseMoveCallback(double xpos, double ypos)
{
    if (first_mouse)
    {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos; 

    last_x = xpos;
    last_y = ypos;

    camera.ProcessMouseMove(xoffset, yoffset);
}

void Global::MouseScrollCallback(double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void ProxyFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Context.FramebufferSizeCallback(width, height);
}
void ProxyMouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
    Context.MouseMoveCallback(xpos, ypos);
}
void ProxyMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Context.MouseScrollCallback(xoffset, yoffset);
}