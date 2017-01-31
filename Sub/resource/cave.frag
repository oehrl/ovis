precision mediump float;

#define M_PI 3.1415926535897932384626433832795

uniform sampler2D u_AlbedoSampler;

uniform vec3 u_LightPosition;
uniform vec3 u_LightDirection;

varying vec3 vs_WorldSpacePosition;
varying vec3 vs_Normal;
varying vec2 vs_TexCoords;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


void main()
{
    const float LIGHT_INNER_ANGLE = radians(45.0);
    const float LIGHT_OUTER_ANGLE = radians(60.0);
    const float LIGHT_HALF_INNER_ANGLE = 0.5 * LIGHT_INNER_ANGLE;
    const float LIGHT_HALF_OUTER_ANGLE = 0.5 * LIGHT_OUTER_ANGLE;
    const float INTENSITY_LEVELS = 4.0;
    
    vec3 normal = normalize(vs_Normal);
    
    vec3 light_ray = vs_WorldSpacePosition - u_LightPosition;
    vec3 light_ray_dir = normalize(light_ray);
    float light_ray_angle = acos(dot(light_ray_dir, u_LightDirection));
    float light_intensity =
        (2.0 / INTENSITY_LEVELS + (1.0 - smoothstep(LIGHT_HALF_INNER_ANGLE, LIGHT_HALF_OUTER_ANGLE, light_ray_angle))) *
        max(0.0, -dot(normal, light_ray_dir));
    float final_intensity = 0.25 / INTENSITY_LEVELS + light_intensity;
    
    
    gl_FragColor =
        final_intensity * //floor(final_intensity * INTENSITY_LEVELS + 0.5) / INTENSITY_LEVELS *
        texture2D(u_AlbedoSampler, vs_TexCoords);
}
