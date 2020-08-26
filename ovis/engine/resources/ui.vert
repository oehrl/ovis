#define M_PI 3.1415926535897932384626433832795

uniform vec2 u_HalfScreenSize;

in vec2 a_Position;
in vec2 a_TextureCoordinates;
in vec4 a_Color;

out vec4 vs_Color;
out vec2 vs_TextureCoordinates;

void main() {
  gl_Position = vec4((a_Position - u_HalfScreenSize)/u_HalfScreenSize, 0.0, 1.0);
  gl_Position.y = -gl_Position.y;
  vs_Color = a_Color;
  vs_TextureCoordinates = a_TextureCoordinates;
}