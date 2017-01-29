precision mediump float;

uniform lowp sampler2D u_SandTextureSampler;

varying mediump vec2 vs_TextureCoordinates;
varying highp float vs_Depth;
varying mediump vec3 vs_Normal;

void main()
{
    const vec4 FOG_COLOR = vec4(0.0, 52.0 / 255.0, 138.0 / 255.0, 1.0);
    vec4 texture_color = texture2D(u_SandTextureSampler, vs_TextureCoordinates);
    
    
    gl_FragColor =
        max(0.1, dot(vs_Normal, vec3(0.0, 1.0, 0.0))) *
        mix(texture_color, FOG_COLOR, vs_Depth / 510000.0);
}
