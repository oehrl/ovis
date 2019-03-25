uniform vec3 u_CameraPosition;
uniform samplerCube u_Skybox;
uniform sampler2D u_OceanNormals1;
uniform sampler2D u_OceanNormals2;
uniform float u_Time;

in vec3 vs_PositionWS;

vec3 SampleNormalMap(sampler2D normal_map, vec2 texcoords) {
  return texture2D(u_OceanNormals1, texcoords).xzy * 2.0 - 1.0;
}

void main() {
  vec3 normal1 = SampleNormalMap(
      u_OceanNormals1, u_Time * vec2(0.010, 0) + vs_PositionWS.xz * 0.01);
  vec3 normal2 = SampleNormalMap(
      u_OceanNormals2, u_Time * vec2(-0.001, 0) + vs_PositionWS.xz * 0.01);
  vec3 normal = normalize(normal1 + normal2);

  vec3 light_direction = normalize(vec3(0.0, 1.0, -0.7));
  vec3 view_direction = normalize(vs_PositionWS - u_CameraPosition);
  vec3 reflection_direction = reflect(view_direction, normal);
  float n_dot_l = dot(light_direction, normal);
  vec3 half_vector = normalize(light_direction + view_direction);
  float n_dot_h = dot(normal, half_vector);
  float specular_intensity = pow(n_dot_h, 5);

  vec4 base_color = n_dot_l * (vec4(0.0, 0.4, .8, 1.0) +
                               specular_intensity * vec4(1.0, 1.0, 1.0, 1.0));
  vec4 reflect_color = textureCube(u_Skybox, reflection_direction);

  gl_FragColor = base_color + 0.5 * reflect_color;
}
