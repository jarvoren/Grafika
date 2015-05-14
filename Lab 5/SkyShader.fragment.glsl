#version 330 core
#define NONE 3


in vec3 texcoord;

out vec4 out_Color;

uniform  samplerCube skyTexture;

uniform struct Mist{
   vec4 color; // Mist color
   float start; // This is only for linear fog
   float end; // This is only for linear fog
   float density; // exp and exp2 equation
   
   int type; // 0 = linear, 1 = exp, 2 = exp2 3 = no mist at all
} mist;

float getMistFactor(Mist params, float coord);


void main(void){
   out_Color = texture(skyTexture, texcoord);
  if(mist.type!=NONE) {
    float mistCoord=40;
    float mistFactor=getMistFactor(mist, mistCoord);
    out_Color = mix(out_Color, mist.color, mistFactor) ;
  }
}

