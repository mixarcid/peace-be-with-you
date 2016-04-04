#ifdef SHADER_USE_COLOR
flat out vec4 color;
#endif

#ifdef SHADER_USE_TEXTURE
out vec2 tex;
#endif

#ifdef SHADER_USE_NORMAL
out vec3 normal;
#endif

#ifdef SHADER_TERRAIN
out vec4 biome_data;
#endif

out vec3 pos;

void main() {
  
#ifdef SHADER_USE_COLOR
  color = inColor;
#endif

#if defined(SHADER_USE_TEXTURE) && !defined(SHADER_TERRAIN)
  tex = inTexCoord;
#endif
  
#ifdef SHADER_USE_NORMAL
  normal = normalize(transpose(inverse(mat3(uniModel))) * inNormal);
#endif

#ifdef SHADER_3D
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

#ifdef SHADER_BILLBOARD
  vec4 center_world =
    uniModel*vec4(uniBillboardCenter,1);
  vec3 cam_right = vec3(uniViewProj[0][0],
			uniViewProj[1][0],
			uniViewProj[2][0]);
  vec3 cam_up = vec3(uniViewProj[0][1],
		     uniViewProj[1][1],
		     uniViewProj[2][1]);
  gl_Position = center_world +
    vec4((cam_right * inPosition.x) +
	 (cam_up * inPosition.y),1);
#endif

#ifdef SHADER_TERRAIN

  const float TERRAIN_CHUNK_SIZE = 300.0f;
  const uint TERRAIN_CHUNK_RES = 127;
  const float TERRAIN_CHUNK_STEP
    = (TERRAIN_CHUNK_SIZE / float(TERRAIN_CHUNK_RES));
  
  gl_Position = uniViewProj*uniModel*vec4(inPositionTerrain,inHeight,1);
  tex = 20*(inPositionTerrain/(TERRAIN_CHUNK_SIZE*2)+1);
  tex = 0.25*(tex - floor(tex));
  biome_data = inBiomeData;
#endif
  
#ifdef SHADER_2D
  vec4 t = (uniModel*uniViewProj)[0];
  gl_Position = uniModel*uniViewProj*vec4(inPosition2d,0,1);
  gl_PointSize = 100;
#else
  gl_PointSize = 10;//gl_Position.z < 1 ? 25 : 25/gl_Position.z;
#endif

#if defined(SHADER_3D) && !defined(SHADER_TERRAIN)
  gl_Position = uniViewProj*uniModel*vec4(position,1);
#endif
  
  pos = gl_Position.xyz;
}
