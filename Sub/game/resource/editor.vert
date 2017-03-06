#version 100

uniform vec2 u_Offset;
uniform vec2 u_Scale;
uniform mat2 u_Rotation;

attribute vec2 a_Position;

varying highp vec3 vs_WorldSpacePosition;
varying highp vec3 vs_Normal;
varying highp vec2 vs_TexCoords;

void main()
{
    gl_Position = vec4(u_Offset + u_Scale * (u_Rotation * a_Position), 0.0, 1.0);
}
