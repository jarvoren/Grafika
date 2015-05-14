#version 330 core
#define NONE 3

in struct Vertex {
    vec2  texcoord;
    vec3  normal;
    vec3  lightDir;
    vec3  lightDir2;
    vec3  viewDir;
    vec4  viewCoord;
    float dist;
    float dist2;
} Vert;

out vec4 out_Color; 
out vec4 color1;
out vec4 color2;

uniform struct PointLight{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 attenuation;
} light, light2;


uniform struct Material
{
  sampler2D colorTexture;
  vec4  ambient;
  vec4  diffuse;
  vec4  specular;
  vec4  emission;
  float shininess;
} material;


uniform struct Mist
{
   vec4 color; // Mist color
   float start; // This is only for linear fog
   float end; // This is only for linear fog
   float density; // For exp and exp2 equation

   int type; // 0 = linear, 1 = exp, 2 = exp2 3 = no mist at all
} mist;


float getMistFactor(Mist params, float coord);


void main(void){

   float attenuation = 1.0 /(light.attenuation[0] +
     light.attenuation[1] * Vert.dist +
     light.attenuation[2] * Vert.dist * Vert.dist);

   float attenuation2 = 1.0 /(light.attenuation[0] +
     light2.attenuation[1] * Vert.dist2 +
     light2.attenuation[2] * Vert.dist2 * Vert.dist2);

  vec3 normal   = normalize(Vert.normal);
  vec3 lightDir = normalize(Vert.lightDir);
  vec3 lightDir2 = normalize(Vert.lightDir2);
  vec3 viewDir  = normalize(Vert.viewDir);

  out_Color = material.emission;
  
  color1 = material.ambient * light.ambient;
  color2 = material.ambient * light2.ambient;

  float NdotL = max(dot(normal, lightDir), 0.0);
  float NdotL2 = max(dot(normal, lightDir2), 0.0);

  color1 += material.diffuse * light.diffuse * NdotL;
  color2 += material.diffuse * light2.diffuse * NdotL2;
  
  float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shininess), 0.0);
  float RdotVpow2 = max(pow(dot(reflect(-lightDir2, normal), viewDir), material.shininess), 0.0);
  
  color1 += material.specular * light.specular * RdotVpow;
  color2 += material.specular * light2.specular * RdotVpow2;

  color1 *= attenuation;
  color2 *= attenuation2;

  out_Color += color1+color2;
  out_Color *= texture(material.colorTexture, Vert.texcoord);

  if(mist.type!=NONE) {
    float mistCoord=abs(Vert.viewCoord.z/Vert.viewCoord.w);
    float mistFactor=getMistFactor(mist, mistCoord);
    out_Color = mix(out_Color, mist.color, mistFactor) ;
  }


}

