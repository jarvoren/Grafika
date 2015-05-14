#version 330 core

in Vertex {
    vec3  normal;
    vec3  lightDir;
    vec3  viewDir;
} Vert;

out vec4 out_Color;

uniform struct PointLight{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
} light;


uniform struct Material
{
  vec4  ambient;
  vec4  diffuse;
  vec4  specular;
  vec4  emission;
  float shininess;
} material;




void main(void){

  vec3 normal   = normalize(Vert.normal);
  vec3 lightDir = normalize(Vert.lightDir);
  vec3 viewDir  = normalize(Vert.viewDir);


  out_Color = material.emission;
  out_Color += material.ambient * light.ambient;
  float NdotL = max(dot(normal, lightDir), 0.0);
  out_Color += material.diffuse * light.diffuse * NdotL;
  float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shininess), 0.0);
  out_Color += material.specular * light.specular * RdotVpow;

}

