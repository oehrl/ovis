#define M_PI 3.1415926535897932384626433832795

uniform mat4 u_ViewProjectionMatrix;

in vec3 a_Position;

out vec3 vs_PositionWS;

void main() {
  vs_PositionWS = a_Position;
  gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}
