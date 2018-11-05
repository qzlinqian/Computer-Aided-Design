#version 410 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 color;
layout (location = 1) in vec2 texCoord;

uniform mat4 model;
//uniform vec3 color;
//out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = model * vec4(position, 1.0f);
//    ourColor = color;
    TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
}
