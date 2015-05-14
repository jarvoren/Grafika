/*	Copyright (C) 2015 by Aleksander Denisiuk

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

#include <string.h>

#include "Model.h"
#include "Program.h"

typedef struct TexVertex
{
  float Position[4];
  float Texture[2];
  float Normal[3];
  	float Tangent[3];
    float Bitangent[3];
} TexVertex;

typedef struct Vertex
{
	float Position[4];
	float Texture[2];
	float Normal[3];

} Vertex;

typedef struct TransVertex
{
	float Position[4];
	float Normal[3];
} TransVertex;

typedef struct StarVertex
{
  float Position[4];
  float Color[4];
} StarVertex;

typedef struct SkyVertex
{
  float Position[4];
  float Texture[3];
} SkyVertex;


typedef float Vec2[2];
typedef float Vec3[3];


// int LoadGroups(NamedMaterial * Materials,char * filename, VertexGroup * *Groups);
// int LoadMaterials(char* filename, NamedMaterial * *Materials);
int LoadGroups(Model * mod, char * filename);
int LoadMaterials(Model * mod, char* filename);
void cpf(float* From, float* To, int N);



void LoadModel(Model * mod, char * filename){
    char MtlFile[256];
    char ObjFile[256];
    strcpy (MtlFile,filename);
    strcat (MtlFile,".mtl");
    strcpy (ObjFile,filename);
    strcat (ObjFile,".obj");
    mod->NofMaterials=LoadMaterials(mod, MtlFile);
    mod->NofGroups=LoadGroups(mod, ObjFile);

  
}


int FindMatNumber(Model * mod, const char* MatName){
    int i;
    for (i=0;i<mod->NofMaterials;i++){
        if(strcmp(MatName,(const char* )mod->Materials[i].Name)==0){
            return i;
        }
    }
    return -1;
}

int LoadGroups(Model * mod, char * filename){
  int Nof=-1; // Groups
  int Max=2;
  int Delta=2;
  int MatNumber=-1;
  int NofVert=-1; //Vertices
  int MaxVert=2;
  int DVert=2;
  int NofVertInGr=-1; // Vertices in group
  int MaxVinG=6;
  int DVinG=6; // >=3
  int NofTex=-1; // Texture
  int MaxTex=2;
  int DTex=2;
  int NofNorm=-1; // Normals
  int MaxNorm=2;
  int DNorm=2;
  FILE* file;
  char Mode[10];
  char MatName[30];
  char g;
  int i;
  VertexGroup * NewGroups;
  Vec3 * Vertices;
  TexVertex * VerticesInGr;
  Vec2 * TexCoords;
  Vec3 * Normals;
  Vec3 * NewVertices;
  TexVertex * NewVerticesInGr;
  Vec2 * NewTexCoords;
  Vec3 * NewNormals;
  
  int v,t,n;//vertex data
  

  if (NULL != (file = fopen(filename, "r"))){
    mod->Groups=malloc(Max*sizeof(VertexGroup));
    Vertices=malloc(MaxVert*sizeof(Vec3));
    VerticesInGr=malloc(MaxVinG*sizeof(TexVertex));
    TexCoords=malloc(MaxTex*sizeof(Vec2));
    Normals=malloc(MaxNorm*sizeof(Vec3));
    
    while(!feof(file) && fscanf(file,"%9s",Mode)){
    if(strcmp("v",(const char*)Mode)==0){
	//Vertex
	NofVert++;
	if(NofVert>=MaxVert){
	  MaxVert+=DVert;
	  NewVertices=realloc(Vertices,MaxVert*sizeof(Vec3));
	  if(NULL!=NewVertices) Vertices=NewVertices;
	  else {
	    fprintf(stderr, "ERROR: Could not allocate memory for vertices\n");
	    exit(EXIT_FAILURE);
	  }
	}
	fscanf(file, "%f %f %f", &Vertices[NofVert][0], &Vertices[NofVert][1], &Vertices[NofVert][2]);
      }
      else if(strcmp("vn",(const char*)Mode)==0){
	//Normal
	NofNorm++;
	if(NofNorm>=MaxNorm){
	  MaxNorm+=DNorm;
	  NewNormals=realloc(Normals,MaxNorm*sizeof(Vec3));
	  if(NULL!=NewNormals) Normals=NewNormals;
	  else {
	    fprintf(stderr, "ERROR: Could not allocate memory for normals\n");
	    exit(EXIT_FAILURE);
	  }
	}
    fscanf(file, "%f %f %f", &Normals[NofNorm][0], &Normals[NofNorm][1], &Normals[NofNorm][2]);
      }
      else if(strcmp("vt",(const char*)Mode)==0){
	//Texture
	NofTex++;
	if(NofTex>=MaxTex){
	  MaxTex+=DTex;
	  NewTexCoords=realloc(TexCoords,MaxTex*sizeof(Vec2));
	  if(NULL!=NewTexCoords) TexCoords=NewTexCoords;
	  else {
	    fprintf(stderr, "ERROR: Could not allocate memory for texture coordinates\n");
	    exit(EXIT_FAILURE);
	  }
	}
	fscanf(file, "%f %f", &TexCoords[NofTex][0], &TexCoords[NofTex][1]);
      }
      else if(strcmp("f",(const char*)Mode)==0){
	//Face
	if(NofVertInGr+3>=MaxVinG){
	  MaxVinG+=DVinG;
	  NewVerticesInGr=realloc(VerticesInGr,MaxVinG*sizeof(TexVertex));
      if(NULL!=NewVerticesInGr) VerticesInGr=NewVerticesInGr;
	  else {
	    fprintf(stderr, "ERROR: Could not allocate memory for new vertices\n");
	    exit(EXIT_FAILURE);
	  }
	}
	for(i=0;i<3;i++){
	  // three vertices
	  fscanf(file, "%i%*c%i%*c%i", &v, &t, &n);
      NofVertInGr++;
      cpf(Vertices[v-1],VerticesInGr[NofVertInGr].Position,4);
      cpf(TexCoords[t-1],VerticesInGr[NofVertInGr].Texture,2);
      cpf(Normals[n-1],VerticesInGr[NofVertInGr].Normal,3);
      VerticesInGr[NofVertInGr].Tangent[0] = 0;
      VerticesInGr[NofVertInGr].Tangent[1] = 0;
      VerticesInGr[NofVertInGr].Tangent[2] = 1;
      VerticesInGr[NofVertInGr].Bitangent[0] = 1;
      VerticesInGr[NofVertInGr].Bitangent[1] = 0;
      VerticesInGr[NofVertInGr].Bitangent[2] = 0;

    }
      }
      else if(strcmp("usemtl",(const char*)Mode)==0){
	//Nowa grupa
	if(Nof>=0){

	  //send current group to GPU
      glGenVertexArrays(1, &mod->Groups[Nof].VAO);
      glBindVertexArray(mod->Groups[Nof].VAO);

	  glEnableVertexAttribArray(0);
	  glEnableVertexAttribArray(1);
	  glEnableVertexAttribArray(2);
	  
      glGenBuffers(1, &mod->Groups[Nof].VBO);
      glBindBuffer(GL_ARRAY_BUFFER, mod->Groups[Nof].VBO);
	  glBufferData(GL_ARRAY_BUFFER, (NofVertInGr+1)*sizeof(TexVertex), VerticesInGr, GL_STATIC_DRAW);
	  ExitOnGLError("ERROR: Could not bind the VBO to the VAO");
	  
	  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VerticesInGr[0]), (GLvoid*)0);
	  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VerticesInGr[0]), (GLvoid*)sizeof(VerticesInGr[0].Position));
	  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VerticesInGr[0]), (GLvoid*)(sizeof(VerticesInGr[0].Position)+ sizeof(VerticesInGr[0].Texture)));
glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VerticesInGr[0]), (GLvoid*)(sizeof(VerticesInGr[0].Position)+sizeof(VerticesInGr[0].Texture)+sizeof(VerticesInGr[0].Normal)));
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VerticesInGr[0]), (GLvoid*)(sizeof(VerticesInGr[0].Position)+sizeof(VerticesInGr[0].Texture)+sizeof(VerticesInGr[0].Normal)+sizeof(VerticesInGr[0].Tangent)));
	  ExitOnGLError("ERROR: Could not set VAO attributes");
	
	  glBindVertexArray(0);
	  
	  
	  mod->Groups[Nof].NofVertices=NofVertInGr+1;

	}
	Nof++;

	if(Nof>=Max){
	  Max+=Delta;
      NewGroups=realloc(mod->Groups,Max*sizeof(VertexGroup));
      if(NULL!=NewGroups) mod->Groups=NewGroups;
	  else {
	    fprintf(stderr, "ERROR: Could not allocate memory for new groups\n");
	    exit(EXIT_FAILURE);
	  }
	}
    fscanf(file, "%29s", MatName);

    MatNumber=FindMatNumber(mod, MatName);
    if(MatNumber<0){
	  fprintf(stderr, "ERROR: Could not find material\n");
	  exit(EXIT_FAILURE);
	}
    mod->Groups[Nof].MatNumber=MatNumber;
	NofVertInGr=-1;
      }
      else while ((g = fgetc(file)) != EOF && g != '\n'){} ;

    }// While
    fclose(file);
    if(Nof>=0){
        NofVertInGr-=3;
      //send current group to GPU
      glGenVertexArrays(1, &mod->Groups[Nof].VAO);
      glBindVertexArray(mod->Groups[Nof].VAO);
      
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glEnableVertexAttribArray(2);
      
      glGenBuffers(1, &mod->Groups[Nof].VBO);
      glBindBuffer(GL_ARRAY_BUFFER, mod->Groups[Nof].VBO);
      glBufferData(GL_ARRAY_BUFFER, (NofVertInGr+1)*sizeof(TexVertex), VerticesInGr, GL_STATIC_DRAW);
      ExitOnGLError("ERROR: Could not bind the VBO to the VAO");
      
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VerticesInGr[0]), (GLvoid*)0);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VerticesInGr[0]), (GLvoid*)sizeof(VerticesInGr[0].Position));
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VerticesInGr[0]), (GLvoid*)(sizeof(VerticesInGr[0].Position)+ sizeof(VerticesInGr[0].Texture)));
glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VerticesInGr[0]), (GLvoid*)(sizeof(VerticesInGr[0].Position)+sizeof(VerticesInGr[0].Texture)+sizeof(VerticesInGr[0].Normal)));
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VerticesInGr[0]), (GLvoid*)(sizeof(VerticesInGr[0].Position)+sizeof(VerticesInGr[0].Texture)+sizeof(VerticesInGr[0].Normal)+sizeof(VerticesInGr[0].Tangent)));
      ExitOnGLError("ERROR: Could not set VAO attributes");
      
      glBindVertexArray(0);
      
      
      mod->Groups[Nof].NofVertices=NofVertInGr+1;


    }
    
    free(Vertices);
    free(VerticesInGr);
    free(TexCoords);
    free(Normals);
  }// If fopen 
  else{ //
    fprintf(stderr, "ERROR: Could not open file %s\n", filename);
  }
  return Nof+1;
}

  
  
int LoadMaterials(Model * mod, char* filename){
  int Nof=-1;
  int MaxMat=2;
  int DMat=2;
  FILE* file;
  char Mode[10];
  char g;
  NamedMaterial * NewMaterials;
  
  if (NULL != (file = fopen(filename, "r"))){
    mod->Materials=(NamedMaterial*) malloc(MaxMat*sizeof(NamedMaterial));
      
    while(!feof(file) && fscanf(file,"%9s",Mode)){
      if(strcmp("newmtl",(const char*)Mode)==0){
        //Material Name
        Nof++;
        if(Nof>=MaxMat){
          MaxMat+=DMat;
          NewMaterials=(NamedMaterial*)realloc(mod->Materials,MaxMat*sizeof(NamedMaterial));
          if(NULL!=NewMaterials) mod->Materials=NewMaterials;
          else {
            fprintf(stderr, "ERROR: Could not allocate memory for materials\n");
            fclose(file);
            exit(EXIT_FAILURE);
          }
        }
        fscanf(file, "%29s", mod->Materials[Nof].Name);
        mod->Materials[Nof].m.Emission[0]=0.0f;
        mod->Materials[Nof].m.Emission[1]=0.0f;
        mod->Materials[Nof].m.Emission[2]=0.0f;
        mod->Materials[Nof].m.Emission[3]=0.1f;
      }
      else if(strcmp("Ns",(const char*)Mode)==0){
	// Shininess
    fscanf(file, "%f", &mod->Materials[Nof].m.Shininess);
      }
      else if(strcmp("Ka",(const char*)Mode)==0){
	// Ambient
    fscanf(file, "%f %f %f", &mod->Materials[Nof].m.Ambient[0],  &mod->Materials[Nof].m.Ambient[1],  &mod->Materials[Nof].m.Ambient[2]);
     mod->Materials[Nof].m.Ambient[3]=1.0f;
      }
      else if(strcmp("Kd",(const char*)Mode)==0){
	// Diffuse
    fscanf(file, "%f %f %f",  &mod->Materials[Nof].m.Diffuse[0],  &mod->Materials[Nof].m.Diffuse[1],  &mod->Materials[Nof].m.Diffuse[2]);
     mod->Materials[Nof].m.Diffuse[3]=1.0f;
      }
      else if(strcmp("Ks",(const char*)Mode)==0){
	// Specular
    fscanf(file, "%f %f %f",  &mod->Materials[Nof].m.Specular[0],  &mod->Materials[Nof].m.Specular[1],  &mod->Materials[Nof].m.Specular[2]);
     mod->Materials[Nof].m.Specular[3]=1.0f;
      }
      else //fscanf(file,"%*[\n]s");
       while (!feof(file) && (g = fgetc(file)) &&g != '\n');
    }
    fclose(file);
  }
  else
    fprintf(stderr, "ERROR: Could not open file %s\n", filename);
  return Nof+1;
}


void DestroyGroup(VertexGroup Gr){
    glDeleteBuffers(1, &Gr.VBO);
    glDeleteVertexArrays(1,&Gr.VAO);
    ExitOnGLError("ERROR: Could not destroy the buffer objects");

}

void DestroyGroups(Model * mod)
{
  int i;
  for (i=0;i< mod->NofGroups; i++){
    DestroyGroup(mod->Groups[i]);
  }
  free(mod->Groups);
  mod->NofGroups=0;
}

void DestroyMaterials(Model * mod)
{
  free(mod->Materials);
  mod->NofMaterials=0;
}



void DestroyModel(Model * mod)
{
  DestroyMaterials(mod);
  DestroyGroups(mod);
}

void DestroyModelSky(Model * mod)
{
  DestroyMaterials(mod);
  DestroyGroups(mod);
}


void SetModelTexture(Model * mod, Texture * Tex){
	mod->Texture=Tex->Texture;
}


void cpf(float* From, float* To, int N){
    if(N==4){
        To[3]=1.0f;
        N--;
    }
    while(N-->0){
        To[N]=From[N];
    }
    return;
}



void DrawModel(Model * mod, Program * pr)
{
GLuint ModelMatrixUniformLocation, NormMatrixUniformLocation;
  int i;
  

  mod->ModelMatrix = IDENTITY_MATRIX;

  mod->NormalMatrix = IDENTITY_MATRIX;

  
  glUseProgram(pr->MainProgram);
  ExitOnGLError("ERROR: Could not use the shader program");

  
  ModelMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "ModelMatrix");
  NormMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "NormMatrix");
  ExitOnGLError("ERROR: Could not get shader uniform locations");
  
  
  glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, mod->ModelMatrix.m);
  glUniformMatrix4fv(NormMatrixUniformLocation, 1, GL_TRUE, mod->NormalMatrix.m);
  ExitOnGLError("ERROR: Could not set the shader uniforms");

 // glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);
//glFrontFace(GL_CCW);
 // ExitOnGLError("ERROR: Could not set OpenGL culling options");
  
  glActiveTexture(GL_TEXTURE0);
  
  glBindTexture(GL_TEXTURE_2D, mod->Texture);
  
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, mod->BumpTexture);

  
  for (i=0; i<mod->NofGroups; i++){
      glBindVertexArray(mod->Groups[i].VAO);
      ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");
      
      SetMaterial(pr, &mod->Materials[mod->Groups[i].MatNumber].m);
      ExitOnGLError("ERROR: Could not setup the Material");
      
      glDrawArrays(GL_TRIANGLES, 0, mod->Groups[i].NofVertices);
      
  }



  ExitOnGLError("ERROR: Could not draw the model");
  glDisable(GL_CULL_FACE);

  glBindVertexArray(0);
  glUseProgram(0);
}

void SetModelMaterial(Model * mod, Material * mat){
  mod->Mat = * mat;
}

void DrawModelCone(Model * mod, Program * pr, int rotate)
{
  int i;

  mod->ModelMatrix = IDENTITY_MATRIX;
  RotateAboutX(&mod->ModelMatrix,0);
  RotateAboutY(&mod->ModelMatrix, rotate);
  TranslateMatrix(&mod->ModelMatrix, -11.4, 8.3, 10.5);

  TranslateMatrix(&mod->ModelMatrix, 0, 0, 0);
  mod->NormalMatrix = IDENTITY_MATRIX;
  RotateAboutY(&mod->NormalMatrix,-rotate);
  RotateAboutX(&mod->NormalMatrix,0);
  
  glUseProgram(pr->MainProgram);
  ExitOnGLError("ERROR: Could not use the shader program");

  SetModelMatrix(pr, &mod->ModelMatrix);
  SetNormalMatrix(pr, &mod->NormalMatrix);
  SetMaterial(pr, &mod->Mat);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  ExitOnGLError("ERROR: Could not set OpenGL culling options");  
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(0);

  for (i=0; i<mod->NofGroups; i++){
      glBindVertexArray(mod->Groups[i].VAO);
      ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");
      ExitOnGLError("ERROR: Could not setup the Material");
      
      glDrawArrays(GL_TRIANGLES, 0, mod->Groups[i].NofVertices);
      
  }

  glDisable(GL_BLEND);
  glDepthMask(1);

  ExitOnGLError("ERROR: Could not draw the model");
  glDisable(GL_CULL_FACE);

  glBindVertexArray(0);
  glUseProgram(0);
}

void CreateSky(Model * mod){
      const SkyVertex VERTICES[8] =
    {
      //front
      {{-50.0f, 50.0f, 50.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}}, //0
      {{ 50.0f, 50.0f, 50.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},  //1
      {{ 50.0f,-50.0f, 50.0f, 1.0f}, {1.0f, -1.0f, 1.0f}}, //2
      {{-50.0f,-50.0f, 50.0f, 1.0f}, {-1.0f, -1.0f, 1.0f}},//3
      // back
      {{ 50.0f, 50.0f,-50.0f, 1.0f}, {1.0f, 1.0f, -1.0f}}, //4
      {{-50.0f, 50.0f,-50.0f, 1.0f}, {-1.0f, 1.0f, -1.0f}}, //5
      {{-50.0f,-50.0f,-50.0f, 1.0f}, {-1.0f, -1.0f, -1.0f}}, //6
      {{ 50.0f,-50.0f,-50.0f, 1.0f}, {1.0f, -1.0f, -1.0f}} //7
    };

    const GLuint INDICES[36] =
    {
      0, 1, 3,  1, 2, 3, // front
      4, 5, 7,  5, 6, 7, // back
      5, 4,0,  4,1,0, // top
      7,6,2, 6,3,2, // bottom
      5,0,6, 0,3,6, // left
      1,4,2, 4,7,2  // right
    };

  glGenVertexArrays(1, &(mod->VAO));
  ExitOnGLError("ERROR: Could not generate the VAO for sky");
  glBindVertexArray(mod->VAO);
  ExitOnGLError("ERROR: Could not bind the VAO for sky");
  
  glGenBuffers(2, &(mod->Buffers[0]));
  ExitOnGLError("ERROR: Could not generate the buffer objects for sky");

    glBindBuffer(GL_ARRAY_BUFFER, mod->Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    ExitOnGLError("ERROR: Could not send vertex data for sky");

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    ExitOnGLError("ERROR: Could not enable vertex array attributes for sky");


    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)sizeof(VERTICES[0].Position));
    ExitOnGLError("ERROR: Could not set vertex array attribute pointers for sky");

    glGenBuffers(1, &mod->Buffers[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod->Buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
    ExitOnGLError("ERROR: Could not send indeces for sky");

    glBindVertexArray(0);

}


void DrawSky(Model * mod , Program * pr){
      glUseProgram(pr->MainProgram);
	ExitOnGLError("ERROR: Could not use the sky shader program");

	glBindVertexArray(mod->VAO);
	ExitOnGLError("ERROR: Could not bind the sky VAO for drawing purposes");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	ExitOnGLError("ERROR: Could not set OpenGL culling options for sky");
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mod->Texture);
	ExitOnGLError("ERROR: Could not bind the sky texture");

      glDepthMask(0);
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLvoid*)0);
	ExitOnGLError("ERROR: Could not drwa the sky");
      glDepthMask(1);

      glDisable(GL_CULL_FACE);

      glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

      glUseProgram(0);
      glBindVertexArray(0);
}

void CreateHalo(Model * mod, int n){
    mod->N = n;
    glGenVertexArrays(1, &(mod->VAO));
    glBindVertexArray(mod->VAO);

    
    glGenBuffers(1, &(mod->Buffers[0]));
    mod->Buffers[1]=0;

    glBindBuffer(GL_ARRAY_BUFFER, mod->Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, mod->N * sizeof(StarVertex), NULL, GL_STATIC_DRAW);

    StarVertex * Star = (StarVertex*) glMapBufferRange(GL_ARRAY_BUFFER, 0, mod->N  * sizeof(StarVertex), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    int i;

    for (i = 0; i < mod->N; i++)
    {
      Star[i].Position[0] = -11.4f;
      Star[i].Position[1] = 8.3f;
      Star[i].Position[2] = 10.5f;
      Star[i].Position[3] = 1.0f;
      Star[i].Color[0] = 0.75f;
      Star[i].Color[1] = 0.75f;
      Star[i].Color[2] = 0.75f;
      Star[i].Color[3] = 1.0f;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(StarVertex), NULL);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(StarVertex), (void *) sizeof(Star[0].Position));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void SetModelBumpTexture(Model * mod, Texture * Tex){
    mod->BumpTexture=Tex->Texture;
}
void DrawHalo(Model * mod, Program * pr)
{
  glUseProgram(pr->MainProgram);
  ExitOnGLError("ERROR: Could not use the shader program");

  glBindVertexArray(mod->VAO);
  mod->ModelMatrix = IDENTITY_MATRIX;
  //TranslateMatrix(&mod->ModelMatrix, -0.4, 5.3, -2.5);

  mod->NormalMatrix = IDENTITY_MATRIX;
  ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, mod->Texture);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  
  glEnable(GL_PROGRAM_POINT_SIZE);
   
  glDrawArrays(GL_POINTS, 0, mod->N);

  glDisable(GL_BLEND);
  glDisable(GL_PROGRAM_POINT_SIZE); 
  glBindVertexArray(0);
  glUseProgram(0);
}
