#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   
    glfwSetKeyCallback(window, key_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
  	Shader ourShader("resources/shaders/vertex_shader.vs", "resources/shaders/fragment_shader.fs");

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,1.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //position attribute
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,6*sizeof(float),(void*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();
        
        //cube drawing
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


}
