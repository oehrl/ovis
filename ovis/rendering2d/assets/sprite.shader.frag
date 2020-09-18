uniform sampler2D u_Texture;
uniform vec4 u_Color;

in vec2 vs_TextureCoordinates;

void main() {
  // gl_FragColor = u_Color * texture2D(u_Texture, vs_TextureCoordinates);
  gl_FragColor = u_Color;
}