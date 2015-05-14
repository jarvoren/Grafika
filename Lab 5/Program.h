/*	Copyright 2013—2015 by Aleksander Denisiuk

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
#ifndef PROGRAM_H
#define PROGRAM_H

#include "Utils.h"
#include "Material.h"
#include "Mist.h"
#include "Lights.h"

typedef struct{ 
  GLuint MainProgram;
  GLuint Shaders[3];
  GLint ProjectionMatrixUniformLocation;
  GLint ViewMatrixUniformLocation;
  GLint ModelMatrixUniformLocation;
  GLint NormalMatrixUniformLocation;
  GLint TextureUnitUniformLocation;
  GLint BumpTextureUnitUniformLocation;
  GLint MaterialAmbientUniformLocation;
  GLint MaterialEmissionUniformLocation;
  GLint MaterialSpecularUniformLocation;
  GLint MaterialDiffuseUniformLocation;
  GLint MaterialShininessUniformLocation;
  GLint MistTypeUniformLocation;
  GLint MistColorUniformLocation;
  GLint MistStartUniformLocation;
  GLint MistEndUniformLocation;
  GLint MistDensityUniformLocation;

  GLint TimeUniformLocation;
} Program;

//	We wszystkich Set*
//	glUseProgram(...) należy 
//	zrobić w programie zewnętrznym;

void SetModelMatrix(Program *, Matrix *);
void SetVewMatrix(Program *, Matrix*);
void SetProjectionMatrix(Program *, Matrix*);
void SetNormalMatrix(Program *, Matrix*);

void SetMaterial(Program *, Material*);

void SetTextureUnit(Program*, GLuint);
void SetBumpTextureUnit(Program*, GLuint);

//	We wszystkich Setup*
//	glUseProgram(...) nie trzeba
//	robić w programie zewnętrznym;
void SetupPointLight(Program *, PointLight*);
void SetupDirectionalLight(Program *, DirectionalLight*);
void SetupSpotLight(Program *, SpotLight*);

void SetupProgramMist(Program *, Mist *);
void SetupProgramMistType(Program *, GLuint);


void CreateProgram(Program *, const char *, const char*, const char *);
void CreateSkyProgram(Program *, const char *, const char *, const char *);
void DestroyProgram(Program *);

#endif
