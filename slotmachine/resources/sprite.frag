uniform sampler2D u_SpriteTexture;

in vec2 vs_TexCoord;

void main() {
  gl_FragColor = texture2D(u_SpriteTexture, vs_TexCoord);
}
