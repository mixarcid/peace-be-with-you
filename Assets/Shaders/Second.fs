uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D depth;

in vec2 texCoord;
out vec4 outColor;

//code taken from https://open.gl/framebuffers
vec4 sobel(sampler2D tex, vec2 coord, vec2 d) {
  vec4 top = texture(tex, vec2(coord.x, coord.y + d.y));
  vec4 bottom = texture(tex, vec2(coord.x, coord.y - d.y));
  vec4 left = texture(tex, vec2(coord.x - d.x, coord.y));
  vec4 right = texture(tex, vec2(coord.x + d.x, coord.y));
  vec4 topLeft = texture(tex, vec2(coord.x - d.x, coord.y + d.y));
  vec4 topRight = texture(tex, vec2(coord.x + d.x, coord.y + d.y));
  vec4 bottomLeft = texture(tex, vec2(coord.x - d.x, coord.y - d.y));
  vec4 bottomRight = texture(tex, vec2(coord.x + d.x, coord.y - d.y));
  vec4 sx = -topLeft - 2 * left - bottomLeft + topRight   + 2 * right  + bottomRight;
  vec4 sy = -topLeft - 2 * top  - topRight   + bottomLeft + 2 * bottom + bottomRight;
  return sqrt(sx * sx + sy * sy);
}

vec4 convolve(sampler2D tex, vec2 coord, vec2 d, mat3 mat) {
  vec4 reg = texture(tex, vec2(coord.x, coord.y));
  vec4 top = texture(tex, vec2(coord.x, coord.y + d.y));
  vec4 bottom = texture(tex, vec2(coord.x, coord.y - d.y));
  vec4 left = texture(tex, vec2(coord.x - d.x, coord.y));
  vec4 right = texture(tex, vec2(coord.x + d.x, coord.y));
  vec4 topLeft = texture(tex, vec2(coord.x - d.x, coord.y + d.y));
  vec4 topRight = texture(tex, vec2(coord.x + d.x, coord.y + d.y));
  vec4 bottomLeft = texture(tex, vec2(coord.x - d.x, coord.y - d.y));
  vec4 bottomRight = texture(tex, vec2(coord.x + d.x, coord.y - d.y));
  return ((topLeft*mat[0][0] + top*mat[1][0] + topRight*mat[2][0] +
	   left*mat[0][1] + reg*mat[1][1] + right*mat[2][1] +
	   bottomLeft*mat[0][2] + bottom*mat[1][2] + bottomRight*mat[2][2])
	  / float(9));
	  }

float rand(float num) {
  return fract(sin(num) * 4374358.5453) - 0.5;
}

float interp(float a, float b, float x) {
  float f = (1-cos(x*3.1415927))*0.5;
  return  a*(1-f) + b*f;
}

float noise(float n) {

  float num = n;
  float a = floor(num);
  float x = num - a;
  float n1 = interp(rand(a), rand(a+1), x);

  num = n*2;
  a = floor(num);
  x = num - a;
  float n2 = interp(rand(a), rand(a+1), x)/2;

  num = n*3;
  a = floor(num);
  x = num - a;
  float n3 = interp(rand(a), rand(a+1), x)/3;

  return n1 + n2 + n3;
}

void main() {

  vec2 res = textureSize(normal,0);
  vec2 d = vec2(1/res.x, 1/res.y);
  vec3 ret;
  float cur_depth = texture(depth, texCoord).r;
  ret = texture(diffuse, texCoord).rgb;
  float sn = length(sobel(normal, texCoord, d).rgb);
  float sd = sobel(depth, texCoord, d).r;
  
  if (sn*0.9 + sd*0.1 > cur_depth*1.8) {
    ret = vec3(0);
  }
  vec3 gamma = vec3(1.0/2.2);
  //ret = texture(normal, texCoord).xyz;
  outColor = vec4(pow(ret, gamma), 1);
}
