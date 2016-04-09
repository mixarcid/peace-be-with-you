uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D offset;
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

vec4 boxBlur(sampler2D tex, vec2 coord, vec2 d) {
  vec4 reg = texture(tex, vec2(coord.x, coord.y));
  vec4 top = texture(tex, vec2(coord.x, coord.y + d.y));
  vec4 bottom = texture(tex, vec2(coord.x, coord.y - d.y));
  vec4 left = texture(tex, vec2(coord.x - d.x, coord.y));
  vec4 right = texture(tex, vec2(coord.x + d.x, coord.y));
  vec4 topLeft = texture(tex, vec2(coord.x - d.x, coord.y + d.y));
  vec4 topRight = texture(tex, vec2(coord.x + d.x, coord.y + d.y));
  vec4 bottomLeft = texture(tex, vec2(coord.x - d.x, coord.y - d.y));
  vec4 bottomRight = texture(tex, vec2(coord.x + d.x, coord.y - d.y));
  return (reg+top+bottom+left+right+topLeft+topRight+bottomLeft+bottomRight)/9;
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
  mat2 m = mat2
    (0.2, 0.6,
     0.3, 0.5);
  float seed = dot(texCoord, vec2(12.9898,78.233))*0.5;
  vec2 o = boxBlur(offset, texCoord, d).xy - vec2(0.5);
  vec3 s = sobel(normal, texCoord + o*0.05, d).xyz;
  vec3 ret = texture(diffuse, texCoord).xyz;
  /*if (length(s) > 1.4) {
    float l = (s.x + s.y + s.z)*(2/3.0);
    ret = vec3(1) - vec3(l,l,l);
    }*/
  vec3 gamma = vec3(1.0/2.2);
  //ret = vec3(o, 0);
  outColor = vec4(pow(ret, gamma), 1);//vec4(texture(normal, texCoord));
}
