#include "Structs.glsl"

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

uniform Transform transform;

layout (location = 0) out vec3 out_position;
layout (location = 1) out vec3 out_normal;

void main()
{
    gl_Position = transform.proj * transform.view * transform.model * vec4(in_position, 1.0);

    vec4 pos = transform.model * vec4(in_position, 1.0f);
    pos /= pos.w;
    out_position = vec3(pos);

    out_normal = mat3(transform.worldNormal) * in_normal;
}
