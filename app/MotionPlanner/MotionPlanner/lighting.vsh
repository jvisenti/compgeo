#version 330 core

uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;
uniform mat3 u_NormalMatrix;
uniform int u_MatIndex;

in vec4 a_position;
in vec3 a_normal;

out vec3 v_normal;
out vec4 v_position;

void main()
{
    v_normal = u_NormalMatrix * a_normal;
    v_position = u_MVMatrix * a_position;
    
    gl_Position = u_MVPMatrix * a_position;
}