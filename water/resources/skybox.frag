uniform samplerCube u_Skybox;

in vec3 vs_TexCoord;

void main() {
  gl_FragColor = textureCube(u_Skybox, vs_TexCoord);
}
