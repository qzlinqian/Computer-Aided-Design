#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;
//uniform mat4 model;
uniform mat4 view;
uniform float z;

void main()
{
    gl_Position = projection * view * vec4(vertex.xy, z, 1.0f);
//  gl_Position = projection* vec4(vertex.xy, 0.0f, 1.0f);
    TexCoords = vec2(vertex.z, 1.0f - vertex.w);
}
