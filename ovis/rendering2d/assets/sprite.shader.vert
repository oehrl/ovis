uniform mat4 u_WorldViewProjection;

in vec2 a_Position;
in vec2 a_TextureCoordinates;

out vec2 vs_TextureCoordinates;

void main() {
  gl_Position = u_WorldViewProjection * vec4(a_Position, 0.0, 1.0);
  vs_TextureCoordinates = a_TextureCoordinates;
}