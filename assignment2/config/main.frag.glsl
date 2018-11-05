#version 410 core

uniform vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main(){
//    color = texture(ourTexture, TexCoord);
    color = texture(ourTexture, TexCoord) * vec4(ourColor, 0.0f);
}
