uniform sampler2D u_Texture;

in vec4 vs_Color;
in vec2 vs_TextureCoordinates;

void main() {
  gl_FragColor = vs_Color * texture2D(u_Texture, vs_TextureCoordinates);
}