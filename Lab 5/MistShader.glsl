#version 330 core

#define LINEAR 0
#define EXP1 1
#define EXP2 2
#define NONE 3

uniform struct Mist{
   vec4 color; // Mist color
   float start; // This is only for linear fog
   float end; // This is only for linear fog
   float density; // exp and exp2 equation
   
   int type; // 0 = linear, 1 = exp, 2 = exp2 3 = no mist at all
} mist;

float getMistFactor(Mist params, float coord){
   float res = 0.0;
   switch(params.type){
   case LINEAR:
      res = (params.end-coord)/(params.end-params.start);
      break;
   case EXP1:
       res = exp(-params.density*coord);
      break;
   case EXP2:
      res = exp(-pow(params.density*coord, 2.0));
      break;
   }
   return 1.0 - clamp(res, 0.0, 1.0);
}
