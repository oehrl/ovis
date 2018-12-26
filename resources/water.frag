uniform vec3 u_CameraPosition;
uniform samplerCube u_Skybox;
uniform sampler2D u_OceanNormals1;

in vec3 vs_PositionWS;

void main() {
  vec3 normal =
      texture2D(u_OceanNormals1, vs_PositionWS.xz * 0.01).xzy * 2.0 - 1.0;

  vec3 view_direction = vs_PositionWS - u_CameraPosition;
  vec3 reflection_direction = reflect(view_direction, normal);

  vec4 base_color = vec4(0.0, 0.0, 1.0, 1.0);
  vec4 reflect_color = textureCube(u_Skybox, reflection_direction);

  gl_FragColor = mix(base_color, reflect_color, 0.5);
}
