#include <iostream>
#include <cmath>

// GLAD
#include <glad/glad.h>
#include "glad.c"

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "shader.h"
// #include "camera.h"
#include "mesh.h"



// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
bool loadOBJ(
    const char * path,
    std::vector < glm::vec3 > & out_vertices,
    std::vector < glm::vec2 > & out_uvs,
    std::vector < glm::vec3 > & out_normals
);


// Window dimensions
GLuint WIDTH = 800, HEIGHT = 600;

// Camera
// Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// Mesh Rotate
MeshRotate meshRotate;

// plot mode
int displayMode=4;
// color changing with keyboard

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Deltatime
GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f;      // Time of last frame

// The MAIN function, from here we start the application and run the game loop
int main() {
  // rand seed
  srand((unsigned)time(nullptr));
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

  glEnable(GL_DEPTH_TEST);


  // Build and compile our shader program
  Shader ourShader("main.vert.glsl", "main.frag.glsl");


  // Read our obj file
  std::vector< glm::vec3 > vertices;
  std::vector< glm::vec2 > uvs;
  std::vector< glm::vec3 > normals; // Won't be used at the moment.
  bool res = loadOBJ("eight.uniform.obj", vertices, uvs, normals);
  if (!res){
    std::cout<<"Failed to load obj file"<<std::endl;
    return -1;
  }

  // generate random colors
  glm::vec3 tempColor;
  std::vector< glm::vec3 > vertexColors;
  for (int i=0;i<vertices.size();i=i+3){
    tempColor.x = (GLfloat)random()/RAND_MAX;
    tempColor.y = (GLfloat)random()/RAND_MAX;
    tempColor.z = (GLfloat)random()/RAND_MAX;
    for (int j=0;j<3;j++) {
      vertexColors.push_back(tempColor);
    }
  }


  // Set up vertex data (and buffer(s)) and attribute pointers

  GLuint VBO, VAO, VAO1;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  // Color information load, you should gen & bind Buffers again first, then read the new buffers
  // The two Array should be bond in the same VAO
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexColors.size() * sizeof(glm::vec3), &vertexColors[0], GL_STATIC_DRAW);

  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//  glEnableVertexAttribArray(1);
  glBindVertexArray(0); // Unbind VAO



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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activate shader
    ourShader.Use();

    // model
    glm::mat4 model(1);
    // Mesh rotation & transformation
    model = glm::rotate(model, glm::radians(meshRotate.Roll), glm::vec3(1.0f,0.0f,0.0f));
    model = glm::rotate(model, glm::radians(meshRotate.Pitch), glm::vec3(0.0f,1.0f,0.0f));
    model = glm::rotate(model, glm::radians(meshRotate.Yaw), glm::vec3(0.0f,0.0f,1.0f));
    model = glm::translate(model, meshRotate.Position);
    // Camera/View transformation
    glm::mat4 view(1);
    // view = camera.GetViewMatrix();
    view = glm::lookAt(glm::vec3(0.0f,0.0f,-10.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
    // Projection
    glm::mat4 projection(1);
    projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, -2.0f, 2.0f);

    // Get the uniform locations
    GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
    GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
    GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
    GLint displayModeLoc = glGetUniformLocation(ourShader.Program, "displayMode");
    // Pass the matrices to the shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1i(displayModeLoc, displayMode);

    if (displayMode == 1 || displayMode == 2){
      glBindVertexArray(VAO);
      glDisableVertexAttribArray(1);
      GLint triangleColorLocation = glGetUniformLocation(ourShader.Program, "myColor");
//      glUniform3fv(triangleColorLocation, vertexColors.size(), vertexColors[0]);
      glUniform3f(triangleColorLocation, meshRotate.DisplayColor.x,meshRotate.DisplayColor.y,meshRotate.DisplayColor.z);
      glBindVertexArray(0);
    } else {
      glBindVertexArray(VAO);
      glEnableVertexAttribArray(1);
      glBindVertexArray(0);
    }

    switch (displayMode){
      case 1:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
      case 2:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
      case 3:
      case 4:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
      default:
        break;
    }

    // enable depth test
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    if (displayMode == 3){
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }
    glBindVertexArray(0);

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
    meshRotate.ProcessKeyboard(FORWARD, deltaTime);
  if (keys[GLFW_KEY_W])
    meshRotate.ProcessKeyboard(BACKWARD, deltaTime);
  if (keys[GLFW_KEY_A])
    meshRotate.ProcessKeyboard(LEFT, deltaTime);
  if (keys[GLFW_KEY_S])
    meshRotate.ProcessKeyboard(RIGHT, deltaTime);
  if (keys[GLFW_KEY_Z])
    meshRotate.ProcessKeyboard(LIFT, deltaTime);
  if (keys[GLFW_KEY_X])
    meshRotate.ProcessKeyboard(SINK, deltaTime);
  if (keys[GLFW_KEY_E])
    meshRotate.ProcessKeyboard(UPWARD, deltaTime);
  if (keys[GLFW_KEY_R])
    meshRotate.ProcessKeyboard(DOWNWARD, deltaTime);
  if (keys[GLFW_KEY_D])
    meshRotate.ProcessKeyboard(CLOCKWISE, deltaTime);
  if (keys[GLFW_KEY_F])
    meshRotate.ProcessKeyboard(ANTICLOCKWISE, deltaTime);
  if (keys[GLFW_KEY_C])
    meshRotate.ProcessKeyboard(LEFTWARD, deltaTime);
  if (keys[GLFW_KEY_V])
    meshRotate.ProcessKeyboard(RIGHTWARD, deltaTime);
  if (keys[GLFW_KEY_1])
    displayMode = 1;
  if (keys[GLFW_KEY_2])
    displayMode = 2;
  if (keys[GLFW_KEY_3])
    displayMode = 3;
  if (keys[GLFW_KEY_4])
    displayMode = 4;
  if (keys[GLFW_KEY_P] && displayMode<3)
    meshRotate.ProcessKeyboard(R_CHANGE, deltaTime);
  if (keys[GLFW_KEY_L] && displayMode<3)
    meshRotate.ProcessKeyboard(G_CHANGE, deltaTime);
  if (keys[GLFW_KEY_M] && displayMode<3)
    meshRotate.ProcessKeyboard(B_CHANGE, deltaTime);
}


// Refer to the GL tutorial  "http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/"
// Modified by Qian
bool loadOBJ(
    const char * path,
    std::vector < glm::vec3 > & out_vertices,
    std::vector < glm::vec2 > & out_uvs,
    std::vector < glm::vec3 > & out_normals
) {
  // some auxiliary vector
  std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  std::vector< glm::vec3 > temp_vertices;
  std::vector< glm::vec2 > temp_uvs;
  std::vector< glm::vec3 > temp_normals;

  // open a file first
  FILE * file = std::fopen(path, "r");
  if( file == NULL ){
    printf("Impossible to open the file !\n");
    return false;
  }
  // read this file until the end
  while( 1 ) {
    char lineHeader[128];
    /* notice that we assume that the first word of a line won’t be longer than 128,
     * which is a very silly assumption.
     * But for a toy parser, it’s all right*/
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF)
      break; // EOF = End Of File. Quit the loop.

    // else : parse lineHeader
    // deal with the vertices first
    if (strcmp(lineHeader, "v") == 0) { // i.e. If the first word of the line is “v”, then the rest has to be 3 floats, so create a glm::vec3 out of them, and add it to the vector.
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      temp_vertices.push_back(vertex);
    } else if (strcmp(lineHeader, "vt") == 0) { // i.e. if it’s not a “v” but a “vt”, then the rest has to be 2 floats, so create a glm::vec2 and add it to the vector.
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      temp_uvs.push_back(uv);
    } else if (strcmp(lineHeader, "vn") == 0) { // same thing for the normals
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      temp_normals.push_back(normal);
    } else if (strcmp(lineHeader, "f") == 0) { // “f”, which is more difficult, modified by Qian
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3];
      int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
      if (matches != 3) {
        printf("File can't be read by our simple parser : ( Try exporting with other options\n");
        return false;
      }
      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
    }
  }

  // process data
  // For each vertex of each triangle
  for (unsigned int i=0; i<vertexIndices.size(); i++ ){
    // the index to the vertex position is vertexIndices[i]
    unsigned int vertexIndex = vertexIndices[i];
    // so the position is temp_vertices[ vertexIndex-1 ] (there is a -1 because C++ indexing starts at 0 and OBJ indexing starts at 1)
    glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
    // And this makes the position of our new vertex
    out_vertices.push_back(vertex);
  }

  // for uv
  for (unsigned int i=0; i<uvIndices.size(); i++){
    unsigned int uvIndex = uvIndices[i];
    glm::vec2 uv = temp_uvs[uvIndex-1];
    out_uvs.push_back(uv);
  }

  for (unsigned int i=0; i<normalIndices.size(); i++){
    unsigned int normalIndex = vertexIndices[i];
    glm::vec3 normal = temp_normals[normalIndex-1];
    out_normals.push_back(normal);
  }

  return true;
}
