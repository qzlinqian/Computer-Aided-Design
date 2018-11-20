#include <iostream>
#include <cmath>
#include <string>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// SOIL
#include <SOIL.h>

// Freetype
#include <ft2build.h>
#include FT_FREETYPE_H

// Other includes
#include "shader.h"
#include "camera.h"
#include "rotate.h"

#define PI 3.14159265

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void generateSphere(std::vector<GLfloat>& vertices, GLfloat x=0, GLfloat y=0, GLfloat z=0, GLfloat radius=0.5);


// Window dimensions
GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// plot mode
//int displayMode=4;
// color changing with keyboard

bool keys[1024];
//GLfloat lastX = 400, lastY = 300;
//bool firstMouse = true;

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
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MeshDisplay", NULL, NULL);
  glfwMakeContextCurrent(window);

  // Set the required callback functions
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  // Initialize GLAD to setup the OpenGL Function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

//  glEnable(GL_DEPTH_TEST);


  // Build and compile our shader program
  Shader ourShader("main.vert.glsl", "main.frag.glsl");

  // get sphere vertices
  std::vector<GLfloat> vertices;
  generateSphere(vertices);
  // position and scales of different planets
  Rotation rotation;


  // Set up vertex data (and buffer(s)) and attribute pointers
  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
//  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  // Texture
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0); // Unbind VAO

  // batch load img
  int width[10], height[10];
  unsigned char* image[10];
  char* textureName[10] = {const_cast<char *>("texture/sun.jpg"), const_cast<char *>("texture/mercury.jpg"),
                           const_cast<char *>("texture/venus.jpg"), const_cast<char *>("texture/earth.jpg"),
                           const_cast<char *>("texture/moon.jpg"), const_cast<char *>("texture/mars.jpg"),
                           const_cast<char *>("texture/jupiter.jpg"), const_cast<char *>("texture/saturn.jpg"),
                           const_cast<char *>("texture/uranus.jpg"), const_cast<char *>("texture/neptune.jpg")};
  GLuint texture[10];

  for (int i=0;i<10;i++){
    image[i] = SOIL_load_image(textureName[i], &width[i], &height[i], 0, SOIL_LOAD_RGB);
    glGenTextures(1, &texture[i]);
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, image[i]);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image[i]);
    glBindTexture(GL_TEXTURE_2D, 0);
  }



  // Game loop
  while (!glfwWindowShouldClose(window))
  {
    // Calculate deltatime of current frame
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();
    do_movement();

    // Render
    // Clear the colorbuffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activate shader
    ourShader.Use();
    glEnable(GL_DEPTH_TEST);

//    glBindVertexArray(VAO);
    // model
    glm::mat4 model(1);
    // Camera/View transformation
    glm::mat4 view(1);
    view = camera.GetViewMatrix();
//    view = glm::lookAt(glm::vec3(0.0f,0.0f,-10.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
    // Projection
    glm::mat4 projection(1);
    projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

    // Get the uniform locations
    GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
    GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
    // Pass the matrices to the shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    /*// Get the uniform locations
    GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
    // Pass the matrices to the shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));*/

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    rotation.PositionUpdate(deltaTime);


    for (int i=0;i<10;i++){
      glActiveTexture(texture[i]);//
      glBindTexture(GL_TEXTURE_2D, texture[i]);
      // model should be reset every time
      model = glm::mat4(1);
      if (i == 4){ // Moon
        model = glm::rotate(model, glm::radians(rotation.orbitAngle[3]), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, rotation.Positions[3]);
      }
      model = glm::rotate(model, glm::radians(rotation.orbitAngle[i]), glm::vec3(0.0f, 1.0f, 0.0f));
      model = glm::translate(model, rotation.Positions[i]);


      GLfloat scaleTime = rotation.Radius[i];
      model = glm::scale(model, glm::vec3(scaleTime, scaleTime, scaleTime));
      // Get the uniform locations
      GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
      // Pass the matrices to the shader
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      glPolygonMode(GL_FRONT, GL_FILL);

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size()/5 * 3);
      glBindVertexArray(0);
    }

    /*glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 36);
    glBindVertexArray(0);*/

    // Swap the screen buffers
    glfwSwapBuffers(window);
  }
  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  WIDTH = width;
  HEIGHT = height;
  glViewport(0, 0, width, height);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key >= 0 && key < 1024)
  {
    if (action == GLFW_PRESS)
      keys[key] = true;
    else if (action == GLFW_RELEASE)
      keys[key] = false;
  }
}

void do_movement()
{
  // Camera controls
  GLfloat cameraSpeed = 5.0f * deltaTime;
  if (keys[GLFW_KEY_Q])
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (keys[GLFW_KEY_W])
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (keys[GLFW_KEY_A])
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (keys[GLFW_KEY_S])
    camera.ProcessKeyboard(RIGHT, deltaTime);
  if (keys[GLFW_KEY_Z])
    camera.ProcessKeyboard(LIFT, deltaTime);
  if (keys[GLFW_KEY_X])
    camera.ProcessKeyboard(SINK, deltaTime);
  if (keys[GLFW_KEY_LEFT])
    camera.ProcessKeyboard(LEFTWARD, deltaTime);
  if (keys[GLFW_KEY_RIGHT])
    camera.ProcessKeyboard(RIGHTWARD, deltaTime);
  if (keys[GLFW_KEY_UP])
    camera.ProcessKeyboard(UPWARD, deltaTime);
  if (keys[GLFW_KEY_DOWN])
    camera.ProcessKeyboard(DOWNWARD, deltaTime);
}

void generateSphere(std::vector<GLfloat >& vertices, GLfloat x, GLfloat y, GLfloat z, GLfloat radius){
  GLfloat xStep = 1.0f/360, yStep = 1.0f/180; // the step for the texture
  // GLfloat theta = 0, phi = -90;
  // GLfloat thetaStep = 1, phiStep = 1;
  for (GLint phi = -90; phi<90; phi ++){
    for (GLint theta = 0; theta <= 360; theta ++){
      // GLfloat r = radius * std::cos(phi * PI / 180); // the radius of this small circle !!! this is erroneous, it would only generate 179 cylindrical surfaces
      GLfloat x_this = radius * std::cos(theta * PI / 180), y_this = radius * std::sin(theta * PI / 180); // fixed theta -> fixed x,y; varied z
      vertices.emplace_back(std::cos(phi * PI / 180) * x_this +x);
      vertices.emplace_back(std::cos(phi * PI / 180) * y_this +y);
      vertices.emplace_back(radius * std::sin(phi * PI / 180) +z); // first point on this line
      vertices.emplace_back(theta * xStep); // texture,  from left bottom
      vertices.emplace_back((phi+90) * yStep);

      vertices.emplace_back(std::cos((phi+1) * PI / 180) * x_this+x);
      vertices.emplace_back(std::cos((phi+1) * PI / 180) * y_this+y);
      vertices.emplace_back(radius * std::sin((phi+1) * PI / 180) +z); // second point on this line
      vertices.emplace_back(theta * xStep); // texture, from left bottom, a little higher: y+1
      vertices.emplace_back((phi+91) * yStep);
    }
  }
}