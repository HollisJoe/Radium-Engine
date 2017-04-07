#include "Structs.glsl"

uniform Light light;
uniform Material material;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

out vec4 fragColor;

void main() {

    vec3 dirLight;

    switch (light.type) {
        case 0:
            dirLight = normalize(-light.directional.direction);
            break;
        case 1:
            dirLight = -normalize(in_position - light.point.position);
            break;
        case 2:
            dirLight = normalize(-light.spot.direction);
            break;
        default:
            dirLight = vec3(0);
            break;
    }

    float cosTheta = dot(dirLight, normalize(in_normal));

    float intensity = float(int(cosTheta * 5)) / 5;

    vec4 result = vec4(0);

    if (material.tex.hasKd == 1)
    {
        vec2 in_texcoord = vec2(intensity, 0.5);
        result += vec4(texture(material.tex.kd, in_texcoord.xy).rgb, 1.0f);
    }

    else if (material.kd != vec4(0))
    {
        result += intensity * material.kd;
    }

    if (material.tex.hasKs == 1)
    {
        vec2 in_texcoord = vec2(intensity, 0.5);
        result += vec4(texture(material.tex.ks, in_texcoord.xy).rgb, 1.0f);
    }

    else if (material.ks != vec4(0))
    {
        result += intensity * material.ks;
    }

    fragColor = result;

    /*vec3 kd = vec3(0.3, 0.1, 0.7);
    float ndotl = max(dot(normalize(in_normal), dirLight), 0.);
    float toonFactor = 0.5+0.5*step(0.5, ndotl);

    fragColor = vec4(toonFactor * kd, 1.0f);
    fragColor = vec4(texture(material.tex.kd, vec2(ndotl, 0.5)).rgb, 1.0f);*/
}
