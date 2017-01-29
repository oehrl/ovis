precision highp float;

#define M_PI 3.1415926535897932384626433832795

uniform sampler2D u_AlbedoSampler;
uniform sampler2D u_NormalSampler;

uniform vec3 u_LightPosition;
uniform vec3 u_LightDirection;

varying vec3 vs_WorldSpacePosition;
varying vec3 vs_Normal;
varying vec3 vs_Tangent;
varying vec3 vs_Bitangent;
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
    
    mat3 tbn = mat3(
        normalize(vs_Tangent),
        normalize(vs_Bitangent),
        normalize(vs_Normal)
    );
    
    vec3 normal = normalize(vs_Normal);//normalize(tbn * (texture2D(u_NormalSampler, vs_TexCoords).xyz * 2.0 - 1.0));
    
    vec3 light_ray = vs_WorldSpacePosition - u_LightPosition;
    vec3 light_ray_dir = normalize(light_ray);
    float light_ray_angle = acos(dot(light_ray_dir, u_LightDirection));
    float light_intensity =
        (2.0 / INTENSITY_LEVELS + (1.0 - smoothstep(LIGHT_HALF_INNER_ANGLE, LIGHT_HALF_OUTER_ANGLE, light_ray_angle))) *
        max(0.0, -dot(normal, light_ray_dir));
    float final_intensity = 0.25 / INTENSITY_LEVELS + light_intensity;
    
    vec4 random = vec4(
        rand(vs_WorldSpacePosition.xy),
        rand(vs_WorldSpacePosition.yz),
        rand(vs_WorldSpacePosition.zx),
        0.5
    );
    vec4 dither = 2.0 * random - 1.0;
    
    
    gl_FragColor =
        final_intensity * //floor(final_intensity * INTENSITY_LEVELS + 0.5) / INTENSITY_LEVELS *
        texture2D(u_AlbedoSampler, vs_TexCoords) +
        //vec4(190.0, 180.0, 240.0, 205.0) / 255.0 +
        dither * 4.0 / pow(2.0, 8.0);
}
