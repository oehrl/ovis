#version 100

uniform mat4 u_WorldViewMatrix;
uniform mat4 u_ProjectionMatrix;

attribute vec4 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_TexCoords;

varying highp vec3 vs_WorldSpacePosition;
varying highp vec3 vs_Normal;
varying highp vec2 vs_TexCoords;

void main()
{
    vec4 view_space_position = u_WorldViewMatrix * a_Position;
    gl_Position = u_ProjectionMatrix * view_space_position;
    vs_WorldSpacePosition = a_Position.xyz;
    vs_Normal = a_Normal;
    vs_TexCoords = a_TexCoords;
}
