#version 100

uniform mat4 u_WorldViewProjection;

attribute vec4 a_Position;

void main()
{
    gl_Position = u_WorldViewProjection * a_Position;
}
