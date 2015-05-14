#version 330 core

layout(location=0) in vec4 in_Position;
layout(location=1) in vec3 in_Texture;


uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 texcoord;


void main(void){
    texcoord = in_Texture;

    gl_Position = (ProjectionMatrix * ViewMatrix) * in_Position;
}

