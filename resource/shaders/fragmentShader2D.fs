#version 330 core
flat in vec3 ourColor;
out vec4 FragColor;
void main()
{
    FragColor = vec4(ourColor.xyz, 1.0f);
}
