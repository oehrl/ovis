uniform mat4 u_WorldViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_NormalTransformMatrix;

uniform float u_Dimensions;
uniform ivec2 u_TextureTiling;
uniform vec2 u_TextureOffset;

attribute vec2 a_Coordinates;
attribute float a_Height;
attribute vec3 a_Normal;

varying vec2 vs_TextureCoordinates;
varying float vs_Depth;
varying vec3 vs_Normal;

void main()
{
    vec2 coordinates = a_Coordinates * u_Dimensions - 0.5 * u_Dimensions;
    vec4 view_space_position = u_WorldViewMatrix * vec4(coordinates.x, a_Height, coordinates.y, 1.0);
    
    gl_Position = u_ProjectionMatrix * view_space_position;
    vs_Depth = view_space_position.z;
    vs_TextureCoordinates = u_TextureOffset + a_Coordinates * vec2(u_TextureTiling);
    vs_Normal = a_Normal;
}
