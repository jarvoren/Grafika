#version 430 core

in struct Vertex {
    vec2  texcoord;
    vec3  lightDir;
    vec3  viewDir;
    float dist;
} Vert;

out vec4 out_Color;

uniform struct PointLight{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 attenuation;
} light;


uniform struct Material
{
  sampler2D colorTexture;
  sampler2D bumpTexture;
  vec4  ambient;
  vec4  diffuse;
  vec4  specular;
  vec4  emission;
  float shininess;
} material;


void main(void){

  float attenuation = 1.0 /(light.attenuation[0] +
     light.attenuation[1] * Vert.dist +
     light.attenuation[2] * Vert.dist * Vert.dist);

  vec3 lightDir = normalize(Vert.lightDir);
  vec3 viewDir  = normalize(Vert.viewDir);

 vec3 normal = normalize(texture(material.bumpTexture ,  Vert.texcoord ).rgb*2.0-1.0);

  out_Color = material.emission;
  out_Color += material.ambient * light.ambient;
  float NdotL = max(dot(normal, lightDir), 0.0);
  out_Color += material.diffuse * light.diffuse * NdotL;
  float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shininess), 0.0);
  out_Color += material.specular * light.specular * RdotVpow;
  
  out_Color *= texture(material.colorTexture, Vert.texcoord)* attenuation;

}

