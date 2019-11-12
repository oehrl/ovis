in vec2 a_Position;

out vec2 vs_TexCoord;

void main() {
  vs_TexCoord = a_Position + 0.5;
  gl_Position = vec4(a_Position * 2.0, 0.0, 1.0);
}
