uniform vec3 u_CameraPosition;
uniform samplerCube u_Skybox;

in vec3 vs_PositionWS;

void main() {
  vec3 view_direction = vs_PositionWS - u_CameraPosition;
  vec3 reflection_direction = reflect(view_direction, vec3(0.0, 1.0, 0.0));

  vec4 base_color = vec4(1.0, 0.0, 0.0, 1.0);
  vec4 reflect_color = textureCube(u_Skybox, reflection_direction);

  gl_FragColor = mix(base_color, reflect_color, 0.5);
}
