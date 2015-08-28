const uint MAX_BONES = 50u;
struct Bone {
  vec3 trans;
  vec4 rot;
};

vec4 quatMult(vec4 q1, vec4 q2) { 
  vec4 ret;
  ret.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
  ret.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
  ret.z = (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
  ret.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
  return ret;
}

vec3 quatRot(vec4 q, vec3 v) {
  vec3 qv = vec3(-q.x, -q.y, -q.z);
  return v + 2.0*cross(cross(v, qv) + q.w*v, qv);
}

in vec3 inPosition;
in vec3 inNormal;

#ifdef SHADER_USE_COLOR
in vec4 inColor;
#else
in vec2 inTexCoord;
#endif
#ifdef SHADER_SKELETAL
in uvec4 inBoneIndexes0;
in vec4 inBoneWeights0;
#endif

layout(std140) uniform _uniModel {
  mat4 uniModel;
};

layout(std140) uniform _uniViewProj {
  mat4 uniViewProj;
};

layout(std140) uniform _uniBones {
  Bone uniBones[MAX_BONES];
};
