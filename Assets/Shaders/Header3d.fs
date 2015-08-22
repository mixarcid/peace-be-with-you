#version 150 core

#ifdef SHADER_USE_COLOR
#else
uniform sampler2D uniTexture;
#endif
out vec4 outColor;
