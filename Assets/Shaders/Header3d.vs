#version 150 core
#extension GL_ARB_explicit_uniform_location : enable

in vec3 inPosition;
in vec4 inColor;
in vec2 inTexCoord;
in vec3 inNormal;

uniform layout(location = 1) mat4 uniModel;
uniform layout(location = 2) mat4 uniView;
uniform layout(location = 3) mat4 uniProj;
