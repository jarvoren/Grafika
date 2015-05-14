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

#ifndef MODELS_H
#define MODELS_H

#include "Utils.h"
#include "Program.h"
#include "Texture.h"
#include "Material.h"

GLuint VAO;
 GLuint Buffers[2];


typedef struct NamedMaterial{
  char Name[30];
  Material m;
} NamedMaterial;


typedef struct VertexGroup{
  GLuint VAO;
  GLuint VBO;
  int MatNumber;
  int NofVertices;
} VertexGroup;


typedef struct{
  GLuint VAO;
  GLuint Buffers[2];
  GLuint Texture;
  GLuint BumpTexture;
  Matrix ModelMatrix;
  Matrix ModelMatrixPrim;
  Matrix NormalMatrix;
  Matrix NormalMatrixPrim;
  Material Mat;
  Material MatPrim;
  float RotationAngle;
  float RotationAnglePrim;
  float Velocity;
  float VelocityPrim;
  int N;
  int M;
  int NofGroups;
  int NofMaterials;
  NamedMaterial * Materials;
  VertexGroup * Groups;
} Model;




void LoadModel(Model * m, char* filename);

void CreatePoints(Model *, int);
void DestroyModel(Model *);

void DrawPoints(Model *, Program *);
void SetModelBumpTexture(Model *, Texture *);
void SetModelTexture(Model *, Texture *);

void DrawModel(Model *, Program *);

void MovePoints(Model *, GLfloat);

#endif
