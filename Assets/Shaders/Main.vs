#ifdef SHADER_USE_COLOR
flat out vec4 color;
#else
out vec2 tex;
#endif
out vec3 normal;
out vec3 pos;

void main() {
#ifdef SHADER_USE_COLOR
  color = inColor;
#else
  tex = inTexCoord;
#endif
#ifndef SHADER_2D
  normal = inNormal;
  vec3 position = inPosition;
#endif
#ifdef SHADER_SKELETAL
  vec4 quat = vec4(0,0,0,1);
  vec3 trans = vec3(0,0,0);
  for (uint n = 0u; n < 4u; ++n) {
    uint index;
    float weight;
    index = inBoneIndexes0[n];
    weight = inBoneWeights0[n];
    quat.w = weight * uniBones[index].rot.w + (1-weight);
    quat.xyz = weight * uniBones[index].rot.xyz;
    quat = normalize(quat);
    position = quatRot(quat, position);
    trans += uniBones[index].trans*weight;
    normal = quatRot(quat, normal);
  }
  position += trans;
#endif
#ifdef SHADER_2D
  vec4 t = (uniModel*uniViewProj)[0];
  gl_Position = uniModel*uniViewProj*vec4(inPosition2d,0,1);
  gl_PointSize = 100;
#else
  normal = normalize(transpose(inverse(mat3(uniModel))) * normal);
  gl_Position = uniViewProj*uniModel*vec4(position,1);
  gl_PointSize = gl_Position.z < 1 ? 25 : 25/gl_Position.z;
#endif
  pos = gl_Position.xyz;
}