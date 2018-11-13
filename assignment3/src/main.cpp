#include <iostream>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>

// Other includes
#include "shader.h"
#include "snowing.h"


// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
GLuint WIDTH = 600, HEIGHT = 600;

Snowing falling;

// Deltatime
GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f;      // Time of last frame

// The MAIN function, from here we start the application and run the game loop
int main() {
  // Init GLFW
  glfwInit();
  // Set all the required options for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create a GLFWwindow object that we can use for GLFW's functions
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Snowing", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  // Set the required callback functions
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  // Initialize GLAD to setup the OpenGL Function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Build and compile our shader program
  Shader ourShader("main.vert.glsl", "main.frag.glsl");

  // Setup some OpenGL options
  // glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  // Set up vertex data (and buffer(s)) and attribute pointers for the snow
  GLfloat vertices[] = {
      // Positions          // Texture Coords
      0.04f,  0.04f, 0.0f,   1.0f, 1.0f, // Top Right
      0.04f, -0.04f, 0.0f,   1.0f, 0.0f, // Bottom Right
     -0.04f, -0.04f, 0.0f,   0.0f, 0.0f, // Bottom Left
     -0.04f,  0.04f, 0.0f,   0.0f, 1.0f  // Top Left
  };
  GLuint indices[] = {  // Note that we start from 0!
      0, 1, 3, // First Triangle
      1, 2, 3  // Second Triangle
  };
  GLuint snowVBO, snowVAO, snowEBO;
  glGenVertexArrays(1, &snowVAO);
  glGenBuffers(1, &snowVBO);
  glGenBuffers(1, &snowEBO);

  glBindVertexArray(snowVAO);

  glBindBuffer(GL_ARRAY_BUFFER, snowVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, snowEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
  // TexCoord attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

  glBindVertexArray(0); // Unbind snowVAO


  // Load and create a texture
  GLuint snowTexture;
  glGenTextures(1, &snowTexture);
  glBindTexture(GL_TEXTURE_2D, snowTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
  // Set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // Load image, create texture and generate mipmaps
  int width, height;
  unsigned char* image = SOIL_load_image("snow.png", &width, &height, 0, SOIL_LOAD_RGBA);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.


  GLfloat castleVertices[] = {
      // Positions                // Texture Coords
       1.0f,  1.0f,  0.0f,        1.0f, 1.0f, // Top Right
       1.0f, -1.0f,  0.0f,        1.0f, 0.0f, // Bottom Right
      -1.0f, -1.0f,  0.0f,        0.0f, 0.0f, // Bottom Left
      -1.0f,  1.0f,  0.0f,        0.0f, 1.0f  // Top Left
  };
  // castle indices = indices, no need to write again
  // actually, the castleVertices can also use vertices -- just re-scale in the loop
  GLuint castleVBO, castleVAO, castleEBO;
  glGenVertexArrays(1, &castleVAO);
  glGenBuffers(1, &castleVBO);
  glGenBuffers(1, &castleEBO);

  glBindVertexArray(castleVAO);

  glBindBuffer(GL_ARRAY_BUFFER, castleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(castleVertices), castleVertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, castleEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
  // TexCoord attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

  glBindVertexArray(0); // unbind castleVAO

  GLuint castleTexture;
  glGenTextures(1, &castleTexture);
  glBindTexture(GL_TEXTURE_2D, castleTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
  // Set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // Load image, create texture and generate mipmaps
  int width2, height2;
  unsigned char* imageCastle = SOIL_load_image("castle.jpg", &width2, &height2, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, imageCastle);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(imageCastle);
  glBindTexture(GL_TEXTURE_2D, 0); // Unbind castleTexture when done


  // Game loop
  while (!glfwWindowShouldClose(window)) {
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, -0.1f, 100.0f);

    // Calculate deltatime of current frame
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glm::mat4 model(1);

    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();

    // Render
    // Clear the colorbuffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* auto move window to solve the Mojave + Xcode10 problem of open with a black window
        (the content would only show after the window being resized or moved)*/
    glfwSetWindowPos(window, 100, 100);

    // Activate shader
    ourShader.Use();

    // Draw castle
    glBindVertexArray(castleVAO);
    // Bind Texture
    glBindTexture(GL_TEXTURE_2D, castleTexture);
    model = glm::mat4(1);
    glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Draw snow
    glBindVertexArray(snowVAO);
    // Bind Texture
    glBindTexture(GL_TEXTURE_2D, snowTexture);
    for (int i=0;i<falling.getFlakeSize();i++){ // draw snow flake according to the positions and scales
      model = glm::mat4(1);
      model = glm::translate(model, falling.Positions[i]);
      GLfloat tempSize = falling.Scales[i];
      model = glm::scale(model, glm::vec3(tempSize, tempSize, tempSize));
      glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    falling.positionUpdate(deltaTime);

    glBindVertexArray(0);
    // Swap the screen buffers
    glfwSwapBuffers(window);
  }
  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &snowVAO);
  glDeleteBuffers(1, &snowVBO);
  glDeleteBuffers(1, &snowEBO);
  glDeleteVertexArrays(1, &castleVAO);
  glDeleteBuffers(1, &castleVBO);
  glDeleteBuffers(1, &castleEBO);
  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  WIDTH = width;
  HEIGHT = height;
  glViewport(0, 0, width, height);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}
