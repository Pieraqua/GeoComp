#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

flat out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
    gl_PointSize = 5;
    ourColor = aColor;
}
