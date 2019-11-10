#define M_PI 3.1415926535897932384626433832795

uniform mat4 u_Transform;

in vec2 a_Position;

out vec2 vs_TexCoord;

void main() {
  vs_TexCoord = a_Position * vec2(0.5, -0.5) + 0.5;
  // gl_Position = u_WorldViewProjection * vec4(a_Position, 0.0, 0.0);
  gl_Position = u_Transform * vec4(a_Position, 0.0, 1.0);
}
