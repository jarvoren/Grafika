#version 330 core
#define NONE 3

in struct Vertex {
    vec2  texcoord;
    vec3  normal;
    vec3  lightDir;
    vec3  viewDir;
    vec4  viewCoord;
    float dist;
} Vert;

out vec4 out_Color;

uniform struct SpotLight{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 attenuation;
    vec3 direction;
    float cutoff;
    float exponent;
} light;


uniform struct Material
{
  sampler2D colorTexture;
  vec4  ambient;
  vec4  diffuse;
  vec4  specular;
  vec4  emission;
  float shininess;
} material;


uniform struct Mist{
   vec4 color; // Mist color
   float start; // This is only for linear fog
   float end; // This is only for linear fog
   float density; // exp and exp2 equation
   
   int type; // 0 = linear, 1 = exp, 2 = exp2 3 = no mist at all
} mist;


float getMistFactor(Mist params, float coord);


void main(void){

  vec3 normal   = normalize(Vert.normal);
  vec3 lightDir = normalize(Vert.lightDir);
  vec3 viewDir  = normalize(Vert.viewDir);


  float spotEffect = dot(normalize(light.direction), -lightDir);
  float spot       = float(spotEffect > light.cutoff);
  spotEffect = max(pow(spotEffect, light.exponent), 0.0);


 float attenuation = spot * spotEffect / (light.attenuation[0] +
                light.attenuation[1] * Vert.dist +
                light.attenuation[2] * Vert.dist * Vert.dist);



  out_Color = material.emission;
  out_Color += material.ambient * light.ambient;
  float NdotL = max(dot(normal, lightDir), 0.0);
  out_Color += material.diffuse * light.diffuse * NdotL;
  float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shininess), 0.0);
  out_Color += material.specular * light.specular * RdotVpow;
  
  out_Color *= texture(material.colorTexture, Vert.texcoord)*attenuation;

  if(mist.type!=NONE) {
    float mistCoord=abs(Vert.viewCoord.z/Vert.viewCoord.w);
    float mistFactor=getMistFactor(mist, mistCoord);
    out_Color = mix(out_Color, mist.color, mistFactor) ;
  }


}

