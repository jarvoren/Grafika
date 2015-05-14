#version 330 core

layout(location=0) in vec4 in_Position;
layout(location=1) in vec3 in_Normal;


uniform mat4 ModelMatrix;
uniform mat4 NormMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform struct PointLight{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
} light;


out Vertex {
    vec3  normal;
    vec3  lightDir;
    vec3  viewDir;
} Vert;


void main(void){
    vec4 vertex   = ModelMatrix * in_Position;
    Vert.lightDir = (light.position.xyz - vertex.xyz);
   
    Vert.normal=vec3(NormMatrix * vec4(in_Normal,0.0));

    vec4 camera = -ViewMatrix*vec4(0.0, 0.0, 0.0, 1);
    
    Vert.viewDir = camera.xyz-vertex.xyz;

    gl_Position = (ProjectionMatrix * ViewMatrix) * vertex;
    
}

