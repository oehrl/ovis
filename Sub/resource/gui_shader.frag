#version 100

precision highp float;

uniform sampler2D u_TextureSampler;

varying vec2 vs_TexCoords;
varying vec2 vs_ClipRange;
varying vec4 vs_Color;

void main()
{
    if (any(lessThan   (vs_ClipRange, vec2(0.0, 0.0))) ||
        any(greaterThan(vs_ClipRange, vec2(1.0, 1.0))) )
    {
        discard;
    }
    
    gl_FragColor = vs_Color * texture2D(u_TextureSampler, vs_TexCoords);
}
