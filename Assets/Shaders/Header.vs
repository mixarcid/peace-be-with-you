const uint MAX_BONES = 50u;
const uint MAX_MODEL_MATS = 1000u;

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


#ifdef SHADER_2D
in vec2 inPosition2d;
#endif

#ifdef SHADER_3D
in vec3 inPosition;
#endif

#ifdef SHADER_USE_NORMAL
in vec3 inNormal;
#endif

#ifdef SHADER_USE_COLOR
in vec4 inColor;
#endif

#if defined(SHADER_USE_TEXTURE) && !defined(SHADER_TERRAIN)
in vec2 inTexCoord;
#endif

#ifdef SHADER_SKELETAL
in uvec4 inBoneIndexes0;
in vec4 inBoneWeights0;
#endif

#ifdef SHADER_TERRAIN
in float inHeight;
in vec2 inPositionTerrain;
in vec4 inBiomeData;
#endif


layout(std140) uniform _uniModel {
  mat4 uniModel[MAX_MODEL_MATS];
};

layout(std140) uniform _uniViewProj {
  mat4 uniViewProj;
};

layout(std140) uniform _uniBones {
  Bone uniBones[MAX_BONES];
};

layout(std140) uniform _uniBillBoardCenter {
  vec3 uniBillboardCenter;
};
