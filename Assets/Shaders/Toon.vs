flat out vec4 color;
out vec2 tex;
smooth out vec3 normal;

void main() {
  color = inColor;
  tex = inTexCoord;
  normal = inNormal;

  vec3 position = inPosition;
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

  normal = normalize(transpose(inverse(mat3(uniModel))) * normal);

  gl_Position = uniProj * uniView *
    uniModel * vec4(position, 1.0);
}
