#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 drawColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
    drawColor = aColor;
    TexCoord = aTexCoord;
}