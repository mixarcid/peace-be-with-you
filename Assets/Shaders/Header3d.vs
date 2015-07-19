#version 150 core

const uint MAX_BONES = 50u;

/*vec4 quatConjugate(vec4 rot) {
  return vec4(-rot.x, -rot.y, -rot.z, rot.w);
  }*/

vec4 quatMult(vec4 q1, vec4 q2)
{ 
  vec4 ret;
  ret.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
  ret.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
  ret.z = (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
  ret.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
  return ret;
}

vec3 quatRot( vec4 q, vec3 v ) {
  vec3 qv = vec3(-q.z, q.x, q.y);
  //vec3 qv = vec3(-q.x,-q.y,-q.z);
  return v + 2.0*cross(cross(v, qv) + q.w*v, qv);
}

struct Transform {
  vec3 trans;
  vec4 rot;
};

in vec3 inPosition;
in vec4 inColor;
in vec2 inTexCoord;
in vec3 inNormal;
in uint inNumBones;
in uvec4 inBoneIndexes0;
in vec4 inBoneWeights0;
in uvec4 inBoneIndexes1;
in vec4 inBoneWeights1;

uniform mat4 uniModel;
uniform mat4 uniView;
uniform mat4 uniProj;
layout(std140) uniform uniBoneData {
  Transform uniBones[MAX_BONES];
};
