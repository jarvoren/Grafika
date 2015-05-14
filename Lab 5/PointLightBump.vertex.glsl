#version 430 core

layout(location=0) in vec4 in_Position;
layout(location=1) in vec2 in_Texture;
layout(location=2) in vec3 in_Normal;
layout(location=3) in vec3 in_Tangent;
layout(location=4) in vec3 in_Bitangent;


uniform mat4 ModelMatrix;
uniform mat4 NormMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform struct PointLight{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 attenuation;
} light;


out struct Vertex {
    vec2  texcoord;
    vec3  lightDir;
    vec3  viewDir;
    float dist;
} Vert;


void main(void){
    vec4 vertex   = ModelMatrix * in_Position;
    Vert.lightDir = (light.position.xyz - vertex.xyz);


    vec3 E_Normal   = normalize(vec3(NormMatrix * vec4(in_Normal,0.0)));
    vec3 E_Tangent  = normalize(vec3(NormMatrix * vec4(in_Tangent,0.0)));
    vec3 E_BiTangent = normalize(vec3(NormMatrix * vec4(in_Bitangent,0.0)));

    // Eye -> TBN matrix
    // no need to inverse, transpose will do as we wont do non-uniform scaling
    mat3 E_TBN = transpose( mat3( E_Tangent, E_BiTangent, E_Normal ));

    vec4 camera = -ViewMatrix*vec4(0.0, 0.0, 0.0, 1);
    
    Vert.viewDir = normalize(E_TBN*(camera.xyz-vertex.xyz));
    Vert.lightDir = normalize(E_TBN*Vert.lightDir);


    Vert.dist = distance(light.position, vertex);

    gl_Position = (ProjectionMatrix * ViewMatrix) * vertex;
    
    Vert.texcoord = in_Texture;

}

