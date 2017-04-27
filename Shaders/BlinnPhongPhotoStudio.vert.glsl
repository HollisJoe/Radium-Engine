#include "Structs.glsl"

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec3 in_bitangent;
layout (location = 4) in vec3 in_texcoord;
// TODO(Charly): Add other inputs

uniform Transform transform;
uniform Material material;

uniform mat4 uLightSpace;

layout (location = 0) out vec3 out_position;
layout (location = 1) out vec3 out_normal;
layout (location = 2) out vec3 out_texcoord;
layout (location = 3) out vec3 out_eye;
layout (location = 4) out vec3 out_tangent;
layout (location = 5) out mat4 out_mvp;

void main()
{
/*
    // scale + translate
    mat4 rot = mat4( 4,           0,            0,  0,
                     0,           4,            0,  0,
                     0,           0,            4,  0,
                     0,          -1,            0,  1);
*/
/*
    // try rotate
    mat4 rot = mat4( 4,            0,            0,  0,
                     0,            4,            0,  0,
                     0,            0,            4,  0,
                     0,            -0.5,            -1,  1);
*/

    // mat4 mvp = transform.proj * transform.view * transform.model;

    mat4 newModel = transform.model;

    newModel[0].xyz *= -transform.view[3].z;
    newModel[1].xyz *= -transform.view[3].z;
    newModel[2].xyz *= -transform.view[3].z;
    newModel[3].xyz = vec3( transform.view[3].z, transform.view[3].z, transform.view[3].z );

    float scale = newModel[3].z / transform.view[3].z;

    newModel[0][0] *= 5 + scale;
    newModel[1][1] *= 5 + scale;
    newModel[2][2] *= 5 + scale;

    newModel[3][2] *= 2 * scale;

    mat4 newMvp = transform.proj * newModel;

    gl_Position = newMvp * vec4(in_position, 1.0);

    out_mvp = newMvp;

    vec4 pos = transform.model * vec4(in_position, 1.0);
    pos /= pos.w;
    vec3 normal = mat3(transform.worldNormal) * in_normal;

    vec3 eye = -transform.view[3].xyz * mat3(transform.view);

    out_position = vec3(pos);
    out_normal   = normal;
    out_eye      = vec3(eye);
    out_tangent  = in_tangent;

    out_texcoord = in_texcoord;
}
