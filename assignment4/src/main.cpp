#include <iostream>
#include <cmath>
#include <string>
#include <map>

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
void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);


// Window dimensions
GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f;      // Time of last frame

// Holds all state information relevant to a character as loaded using FreeType
struct Character {
  GLuint TextureID;   // ID handle of the glyph texture
  glm::ivec2 Size;    // Size of glyph
  glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
  GLuint Advance;    // Horizontal offset to advance to next glyph
};

// text display
std::map<GLchar, Character> Characters;
GLuint VAOF, VBOF;


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
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Rotation", NULL, NULL);
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
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  // Build and compile our shader program
  Shader ourShader("main.vert.glsl", "main.frag.glsl");
  Shader textShader("text.vert.glsl", "text.frag.glsl");
  glm::mat4 projectionText =  glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
  textShader.Use();
  glUniformMatrix4fv(glGetUniformLocation(textShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionText));

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
  }
  glBindTexture(GL_TEXTURE_2D, 0);

  // Planet names
  std::string planetName[10] = {"Sun", "Mercury", "Venus", "Earth", "Moon", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune"};


  // FreeType
  FT_Library ft;
  // All functions return a value different than 0 whenever an error occurred
  if (FT_Init_FreeType(&ft))
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

  // Load font as face
  FT_Face face;
  if (FT_New_Face(ft, "arial.ttf", 0, &face))
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

  // Set size to load glyphs as
  FT_Set_Pixel_Sizes(face, 0, 48);

  // Disable byte-alignment restriction
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Load first 128 characters of ASCII set
  for (GLubyte c = 0; c < 128; c++)
  {
    // Load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }
    // Generate texture
    GLuint textureF;
    glGenTextures(1, &textureF);
    glBindTexture(GL_TEXTURE_2D, textureF);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Now store character for later use
    Character character = {
        textureF,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        GLuint(face->glyph->advance.x)
    };
    Characters.insert(std::pair<GLchar, Character>(c, character));
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  // Destroy FreeType once we're finished
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  // Configure VAO/VBO for texture quads
  glGenVertexArrays(1, &VAOF);
  glGenBuffers(1, &VBOF);
  glBindVertexArray(VAOF);
  glBindBuffer(GL_ARRAY_BUFFER, VBOF);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);


  // Game loop
  while (!glfwWindowShouldClose(window)) {
    /* auto move window to solve the Mojave + Xcode10 problem of open with a black window
        (the content would only show after the window being resized or moved)*/
    glfwSetWindowPos(window, 100, 100);
    
    // Calculate deltatime of current frame
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();
    do_movement();

    // Render
    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Activate shader
    ourShader.Use();
    glEnable(GL_DEPTH_TEST);

    // model
    glm::mat4 model(1);
    // Camera/View transformation
    glm::mat4 view(1);
    view = camera.GetViewMatrix();
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


    rotation.PositionUpdate(deltaTime);  // update planets' position

    // pass parameter to text shader
    textShader.Use();
    GLint viewLocText = glGetUniformLocation(textShader.Program, "view");
    GLint projLocText = glGetUniformLocation(textShader.Program, "projection");
    glUniformMatrix4fv(viewLocText, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLocText, 1, GL_FALSE, glm::value_ptr(projection));

    // active main shader again
    ourShader.Use();


    for (int i=0;i<10;i++){
      // Calculate the x, y position of the star
      GLfloat xNow = 0, zNow = 0;
      // model should be reset every time
      model = glm::mat4(1);
      // put the matrix by reversed order
      if (i == 4){ // Moon
        model = glm::rotate(model, glm::radians(rotation.orbitAngle[3]), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, rotation.Positions[3]);
        xNow = rotation.Positions[3].x * std::cos(rotation.orbitAngle[3] * PI / 180);  // text position
        zNow = -rotation.Positions[3].x * std::sin(rotation.orbitAngle[3] * PI / 180);
      }
      model = glm::rotate(model, glm::radians(rotation.orbitAngle[i]), glm::vec3(0.0f, 1.0f, 0.0f));
      model = glm::translate(model, rotation.Positions[i]);
      xNow += rotation.Positions[i].x * std::cos(rotation.orbitAngle[i] * PI / 180);
      zNow -= rotation.Positions[i].x * std::sin(rotation.orbitAngle[i] * PI / 180);

      textShader.Use();
      // text.x = planet.x, text.z = planet.z, text.y = planet.radius + offset
      // if (planet != moon) planet.x = range * cos(orbitRotateAngle); range = Position[i].x
      GLint zLocText = glGetUniformLocation(textShader.Program, "z");
      glUniform1f(zLocText, zNow);
      RenderText(textShader, planetName[i], xNow, rotation.Radius[i] * 0.5 + 0.005f, 0.0003f, glm::vec3(1.0f, 1.0f, 0.0f));
      // Shader, VAO, Texture for planet should bind again. Cause the RenderText(...) actives shader, VAO, Texture for text
      ourShader.Use();
      glBindVertexArray(VAO);

      GLfloat scaleTime = rotation.Radius[i];
      model = glm::scale(model, glm::vec3(scaleTime, scaleTime, scaleTime));
      // Get the uniform locations
      GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
      // Pass the matrices to the shader
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      glActiveTexture(texture[i]);  // that might not be default, so should active first
      glBindTexture(GL_TEXTURE_2D, texture[i]);  // texture for this planet
      glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size()/5 * 3);  // every vertex used for 3 times; every vertex has 5 GLfloat
      glBindVertexArray(0);
    }

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
      // GLfloat r = radius * std::cos(phi * PI / 180); // the radius of this small circle !!!
      // former: "fixed theta -> fixed x,y; varied z" this is erroneous, it would only generate 180 cylindrical surface
      GLfloat x_this = radius * std::cos(theta * PI / 180), y_this = radius * std::sin(theta * PI / 180);
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


// for words display
void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
  // Activate corresponding render state
  shader.Use();
  glUniform3f(glGetUniformLocation(shader.Program, "textColor"), color.x, color.y, color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAOF);

  // Iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    Character ch = Characters[*c];

    GLfloat xpos = x + ch.Bearing.x * scale;
    GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    GLfloat w = ch.Size.x * scale;
    GLfloat h = ch.Size.y * scale;
    // Update VBO for each character
    GLfloat verticesFt[6][4] = {
        { xpos,     ypos + h,   0.0, 1.0 },
        { xpos,     ypos,       0.0, 0.0 },
        { xpos + w, ypos,       1.0, 0.0 },

        { xpos,     ypos + h,   0.0, 1.0 },
        { xpos + w, ypos,       1.0, 0.0 },
        { xpos + w, ypos + h,   1.0, 1.0 }
    };
    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBOF);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesFt), verticesFt); // Be sure to use glBufferSubData and not glBufferData

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}