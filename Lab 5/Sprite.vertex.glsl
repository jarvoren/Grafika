#version 330 core

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec4 in_Color;

uniform mat4 ModelMatrix;
uniform mat4 NormMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

flat out vec4 starColor;

void main(void)
{
    vec4 vertex = ModelMatrix * in_Position;

    float size = 250.0 -vertex.z*10;

    starColor = smoothstep(1.0, 7.0, size) * in_Color;

    vec4 camera = -ViewMatrix*vec4(0.0, 0.0, 0.0, 1);

    gl_Position = (ProjectionMatrix * ViewMatrix) * in_Position;
    gl_PointSize = size;
}
