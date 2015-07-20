smooth out vec4 color;
out vec2 tex;
smooth out vec3 normal;

void main() {
  //color = inColor;
  tex = inTexCoord;
  normal = inNormal;

  vec3 position = inPosition;

  color = vec4(1,1,1,1);

  vec4 quat = vec4(0,0,0,1);
  for (uint n = 0u; n < inNumBones; ++n) {
    uint index;
    float weight;
    if (n < 4u) {
      index = inBoneIndexes0[n];
      weight = inBoneWeights0[n];
    } else {
      uint tmp = n - 4u;
      index = inBoneIndexes1[tmp];
      weight = inBoneWeights1[tmp];
    }
    quat.w = weight * uniBones[index].rot.w + (1-weight);
    quat.xyz = weight * uniBones[index].rot.xyz;
    //quat = uniBones[index].rot;
    quat = normalize(quat);
    position = quatRot(quat, position);
    normal = quatRot(quat, normal);
    }
  //color.r = uniBones[3].rot.y;
  //position = quatRot(vec4(0,0,0,1), position);

  normal = normalize(transpose(inverse(mat3(uniModel))) * normal);

  gl_Position = uniProj * uniView *
    uniModel * vec4(position, 1.0);
}
