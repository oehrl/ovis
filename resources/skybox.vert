#define M_PI 3.1415926535897932384626433832795

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

in vec3 a_Position;

out vec3 vs_TexCoord;

void main() {
  vs_TexCoord = a_Position;
  gl_Position = u_ViewMatrix * vec4(a_Position, 0.0);
  gl_Position = u_ProjectionMatrix * vec4(gl_Position.xyz, 1.0);
}
