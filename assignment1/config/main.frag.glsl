#version 410 core

in vec3 fragColor;
uniform vec3 myColor;
uniform int displayMode;
out vec4 color;

void main()
{
    if (displayMode == 1 || displayMode == 2){
      color = vec4(myColor,1.0f);
    } else
    color = vec4(fragColor,1.0f);
}