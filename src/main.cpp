#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "scene.h"
#include "shader.h"

const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 1080;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
bool first_mouse = true;

float delta_time = 0.0f;
float last_frame = 0.0f;

float fps = 0.0f;
int frame_count = 0;
float fps_update_timer = 0.0f;
const float FPS_UPDATE_INTERVAL = 0.5f;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.processKeyboard(UP, delta_time);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.processKeyboard(DOWN, delta_time);
}

void mouseCallback(GLFWwindow* window, double x_pos_in, double y_pos_in) {
    float x_pos = static_cast<float>(x_pos_in);
    float y_pos = static_cast<float>(y_pos_in);

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return;
    }

    if (first_mouse) {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    float x_offset = x_pos - last_x;
    float y_offset =
        last_y - y_pos; // reversed since y-coordinates go from bottom to top

    last_x = x_pos;
    last_y = y_pos;

    camera.processMouseMovement(x_offset, y_offset);
}

void scrollCallback(GLFWwindow* window, double x_offset, double y_offset) {
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return;
    }
    camera.processMouseScroll(static_cast<float>(y_offset));
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

std::string errorName(int err) {
    switch (err) {
        #define PER_GL_ERROR(x) case GL_##x: return #x;
        PER_GL_ERROR(NO_ERROR)
            PER_GL_ERROR(INVALID_ENUM)
            PER_GL_ERROR(INVALID_VALUE)
            PER_GL_ERROR(INVALID_OPERATION)
            PER_GL_ERROR(STACK_OVERFLOW)
            PER_GL_ERROR(STACK_UNDERFLOW)
            PER_GL_ERROR(OUT_OF_MEMORY)
    }
    return "unknown error: " + std::to_string(err);
}

void updateFPS() {
    frame_count++;
    fps_update_timer += delta_time;

    if (fps_update_timer >= FPS_UPDATE_INTERVAL) {
        fps = frame_count / fps_update_timer;
        frame_count = 0;
        fps_update_timer = 0.0f;
    }
}

void renderImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Control Panel");

    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Frame Time: %.3f ms", delta_time * 1000.0f);

    ImGui::Separator();

    glm::vec3 cam_pos = camera.Position;
    ImGui::Text("Camera Position:");
    ImGui::Text("X: %.2f, Y: %.2f, Z: %.2f", cam_pos.x, cam_pos.y, cam_pos.z);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main() {
    /*  init  */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ZShader", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    //capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glEnable(GL_DEPTH_TEST);

    /* prepare data  */
    Scene scene("../assets/common/cube.scn");

    /*  render  */
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        delta_time = currentFrame - last_frame;
        last_frame = currentFrame;
        
        updateFPS();
        
        processInput(window);

        scene.drawSceneDeferred(camera);

        renderImGui();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}