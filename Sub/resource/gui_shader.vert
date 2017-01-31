#version 100

uniform vec2 u_ViewportSize;
uniform vec4 u_ClipRect;

attribute vec2 a_Position;
attribute vec2 a_TexCoords;
attribute vec4 a_Color;

varying vec2 vs_TexCoords;
varying vec2 vs_ClipRange;
varying vec4 vs_Color;

void main()
{
    vec2 half_viewport_size = 0.5 * u_ViewportSize;
    vec2 position = vec2(
        a_Position.x,
        u_ViewportSize.y - a_Position.y
    );

    gl_Position  = vec4(position / half_viewport_size - 1.0, 0.0, 1.0);
    vs_TexCoords = a_TexCoords;
    vs_ClipRange = (a_Position - u_ClipRect.xy) / u_ClipRect.zw;
    vs_Color     = a_Color;
}
