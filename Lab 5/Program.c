/*	Copyright (C) 2013—2014 by Aleksander Denisiuk

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

#include "Program.h"


GLuint LoadShader(const char* filename, GLenum shader_type)
{
	GLuint shader_id = 0;
	FILE* file;
	long file_size = -1;
	GLchar* glsl_source;

	if (NULL != (file = fopen(filename, "rb")) &&
		0 == fseek(file, 0, SEEK_END) &&
		-1 != (file_size = ftell(file)))
	{
		rewind(file);
		
		if (NULL != (glsl_source = (GLchar*)malloc(file_size + 1)))
		{
		  if (file_size == (long)fread(glsl_source, sizeof(GLchar), file_size, file))
			{
				glsl_source[file_size] = '\0';

				if (0 != (shader_id = glCreateShader(shader_type)))
				{
					glShaderSource(shader_id, 1,(const GLchar**) &glsl_source, NULL);
					glCompileShader(shader_id);
					ExitOnGLError("Could not compile a shader");
				}
				else
					fprintf(stderr, "ERROR: Could not create a shader.\n");
			}
			else
				fprintf(stderr, "ERROR: Could not read file %s\n", filename);
			free(glsl_source);
		}
		else
			fprintf(stderr, "ERROR: Could not allocate %li bytes.\n", file_size);

		fclose(file);
	}
	else
		fprintf(stderr, "ERROR: Could not open file %s\n", filename);

	return shader_id;
}
	
void CreateProgram(Program * pr, const char* VertexShaderFile, const char * FragmentShaderFile, const char * MistShaderFile)
{
  printf("\nCreating %s", VertexShaderFile);
  pr->MainProgram = glCreateProgram();
  ExitOnGLError("ERROR: Could not create the shader program");
  {
    pr->Shaders[0] = LoadShader(FragmentShaderFile, GL_FRAGMENT_SHADER);
    pr->Shaders[1] = LoadShader(VertexShaderFile, GL_VERTEX_SHADER);
    pr->Shaders[2] = LoadShader(MistShaderFile, GL_FRAGMENT_SHADER);
    glAttachShader(pr->MainProgram, pr->Shaders[0]);
    glAttachShader(pr->MainProgram, pr->Shaders[1]);
    glAttachShader(pr->MainProgram, pr->Shaders[2]);
  }
  glLinkProgram(pr->MainProgram);
  ExitOnGLError("ERROR: Could not link the shader program");
  
  glUseProgram(pr->MainProgram);
  
  pr->ModelMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "ModelMatrix");
  if(pr->ModelMatrixUniformLocation<0){
	fprintf(stderr,"ERROR: model matrix uniform location not found\n");
	exit(EXIT_FAILURE);
  }

  pr->ViewMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "ViewMatrix");
  if(pr->ViewMatrixUniformLocation<0){
	fprintf(stderr,"ERROR: view matrix uniform location not found\n");
	exit(EXIT_FAILURE);
  }

  pr->ProjectionMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "ProjectionMatrix");
  if(pr->ProjectionMatrixUniformLocation<0){
	fprintf(stderr,"ERROR: projection matrix uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  
  pr->NormalMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "NormMatrix");
  if(pr->NormalMatrixUniformLocation<0){
	fprintf(stderr,"ERROR: normal matrix uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  
  pr->TextureUnitUniformLocation = glGetUniformLocation(pr->MainProgram, "material.colorTexture");
  if(pr->TextureUnitUniformLocation<0){
  // fprintf(stderr,"ERROR: texture unit uniform location not found\n");
  // exit(EXIT_FAILURE);
  }

  pr->BumpTextureUnitUniformLocation = glGetUniformLocation(pr->MainProgram,    "material.bumpTexture");
  if(pr->BumpTextureUnitUniformLocation<0){
  fprintf(stderr,"ERROR: bump texture unit uniform location not found\n");
  //exit(EXIT_FAILURE);
  }

  pr->MaterialAmbientUniformLocation = glGetUniformLocation(pr->MainProgram, "material.ambient");
  if(pr->MaterialAmbientUniformLocation<0){
	fprintf(stderr,"ERROR: material.ambient uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  
  pr->MaterialDiffuseUniformLocation = glGetUniformLocation(pr->MainProgram, "material.diffuse");
  if(pr->MaterialDiffuseUniformLocation<0){
	fprintf(stderr,"ERROR: material.diffuse uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  
  pr->MaterialEmissionUniformLocation = glGetUniformLocation(pr->MainProgram, "material.emission");
  if(pr->MaterialEmissionUniformLocation<0){
	fprintf(stderr,"ERROR: material.emission uniform location not found\n");
	exit(EXIT_FAILURE);
  }

  pr->MaterialSpecularUniformLocation = glGetUniformLocation(pr->MainProgram, "material.specular");
  if(pr->MaterialSpecularUniformLocation<0){
	fprintf(stderr,"ERROR: material.specular uniform location not found\n");
	exit(EXIT_FAILURE);
  }

  pr->MaterialShininessUniformLocation = glGetUniformLocation(pr->MainProgram, "material.shininess");
  if(pr->MaterialShininessUniformLocation<0){
	fprintf(stderr,"ERROR: material.shininess uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  
  pr->MistTypeUniformLocation = glGetUniformLocation(pr->MainProgram, "mist.type");
  if(pr->MistTypeUniformLocation<0){
	fprintf(stderr,"ERROR: mist.type uniform location not found\n");
	exit(EXIT_FAILURE);
  }

  pr->MistDensityUniformLocation = glGetUniformLocation(pr->MainProgram, "mist.density");
  if(pr->MistDensityUniformLocation<0){
	fprintf(stderr,"ERROR: mist.density uniform location not found\n");
	exit(EXIT_FAILURE);
  }

  pr->MistColorUniformLocation = glGetUniformLocation(pr->MainProgram, "mist.color");
  if(pr->MistColorUniformLocation<0){
	fprintf(stderr,"ERROR: mist.color uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  
  pr->MistStartUniformLocation = glGetUniformLocation(pr->MainProgram, "mist.start");
  if(pr->MistStartUniformLocation<0){
	fprintf(stderr,"ERROR: mist.start uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  
  pr->MistEndUniformLocation = glGetUniformLocation(pr->MainProgram, "mist.end");
  if(pr->MistEndUniformLocation<0){
	fprintf(stderr,"ERROR: mist.end uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  ExitOnGLError("ERROR: Could find uniform locations");
  printf(" OK");
  glUseProgram(0);
  
}
void CreateProgram2(Program * pr, const char* VertexShaderFile, const char * FragmentShaderFile)
{

	pr->MainProgram = glCreateProgram();
	ExitOnGLError("ERROR: Could not create the shader program");
	{
		pr->Shaders[0] = LoadShader(FragmentShaderFile, GL_FRAGMENT_SHADER);
		pr->Shaders[1] = LoadShader(VertexShaderFile, GL_VERTEX_SHADER);
		glAttachShader(pr->MainProgram, pr->Shaders[0]);
		glAttachShader(pr->MainProgram, pr->Shaders[1]);
	}
	glLinkProgram(pr->MainProgram);
	ExitOnGLError("ERROR: Could not link the shader program");

	glUseProgram(pr->MainProgram);

	pr->ModelMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "ModelMatrix");
	if (pr->ModelMatrixUniformLocation<0){
		fprintf(stderr, "ERROR: model matrix uniform location not found\n");
		exit(EXIT_FAILURE);
	}

	pr->ViewMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "ViewMatrix");
	if (pr->ViewMatrixUniformLocation<0){
		fprintf(stderr, "ERROR: view matrix uniform location not found\n");
		exit(EXIT_FAILURE);
	}

	pr->ProjectionMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "ProjectionMatrix");
	if (pr->ProjectionMatrixUniformLocation<0){
		fprintf(stderr, "ERROR: projection matrix uniform location not found\n");
		exit(EXIT_FAILURE);
	}

	pr->NormalMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "NormMatrix");
	if (pr->NormalMatrixUniformLocation<0){
		fprintf(stderr, "ERROR: normal matrix uniform location not found\n");
		exit(EXIT_FAILURE);
	}

	pr->TextureUnitUniformLocation = glGetUniformLocation(pr->MainProgram, "material.colorTexture");
	if (pr->TextureUnitUniformLocation<0){
		//  fprintf(stderr,"ERROR: texture unit uniform location not found\n");
		//  exit(EXIT_FAILURE);
	}
 pr->BumpTextureUnitUniformLocation = glGetUniformLocation(pr->MainProgram,    "material.bumpTexture");
  if(pr->BumpTextureUnitUniformLocation<0){
  fprintf(stderr,"ERROR: bump texture unit uniform location not found\n");
  //exit(EXIT_FAILURE);
  }


	pr->MaterialAmbientUniformLocation = glGetUniformLocation(pr->MainProgram, "material.ambient");
	if (pr->MaterialAmbientUniformLocation<0){
		fprintf(stderr, "ERROR: material.ambient uniform location not found\n");
		exit(EXIT_FAILURE);
	}

	pr->MaterialDiffuseUniformLocation = glGetUniformLocation(pr->MainProgram, "material.diffuse");
	if (pr->MaterialDiffuseUniformLocation<0){
		fprintf(stderr, "ERROR: material.diffuse uniform location not found\n");
		exit(EXIT_FAILURE);
	}

	pr->MaterialEmissionUniformLocation = glGetUniformLocation(pr->MainProgram, "material.emission");
	if (pr->MaterialEmissionUniformLocation<0){
		fprintf(stderr, "ERROR: material.emission uniform location not found\n");
		exit(EXIT_FAILURE);
	}

	pr->MaterialSpecularUniformLocation = glGetUniformLocation(pr->MainProgram, "material.specular");
	if (pr->MaterialSpecularUniformLocation<0){
		fprintf(stderr, "ERROR: material.specular uniform location not found\n");
		exit(EXIT_FAILURE);
	}

	pr->MaterialShininessUniformLocation = glGetUniformLocation(pr->MainProgram, "material.shininess");
	if (pr->MaterialShininessUniformLocation<0){
		fprintf(stderr, "ERROR: material.shininess uniform location not found\n");
		exit(EXIT_FAILURE);
	}

	ExitOnGLError("ERROR: Could find uniform locations");
	glUseProgram(0);

}

void DestroyProgram(Program * pr)
{
  glUseProgram(pr->MainProgram);
  glDetachShader(pr->MainProgram, pr->Shaders[0]);
  glDetachShader(pr->MainProgram, pr->Shaders[1]);
  glDetachShader(pr->MainProgram, pr->Shaders[2]);
  glDeleteShader(pr->Shaders[0]);
  glDeleteShader(pr->Shaders[1]);
  glDeleteShader(pr->Shaders[2]);
  glDeleteProgram(pr->MainProgram);
  ExitOnGLError("ERROR: Could not destroy the shaders");
}
void DestroyProgram2(Program * pr)
{
  glUseProgram(pr->MainProgram);
  glDetachShader(pr->MainProgram, pr->Shaders[0]);
  glDetachShader(pr->MainProgram, pr->Shaders[1]);
  glDeleteShader(pr->Shaders[0]);
  glDeleteShader(pr->Shaders[1]);
  glDeleteProgram(pr->MainProgram);
  ExitOnGLError("ERROR: Could not destroy the shaders");
}


//	We wszystkich Set*
//	glUseProgram(...) należy 
//	zrobić w programie zewnętrznym;

void SetModelMatrix(Program * pr, Matrix * mat){
	glUniformMatrix4fv(pr->ModelMatrixUniformLocation, 1, GL_FALSE, mat->m);
	ExitOnGLError("ERROR: Could set the Model Matrix");
}

void SetViewMatrix(Program * pr, Matrix * mat){
	glUniformMatrix4fv(pr->ViewMatrixUniformLocation, 1, GL_FALSE, mat->m);
	ExitOnGLError("ERROR: Could set the View Matrix");
}

void SetProjectionMatrix(Program * pr, Matrix * mat){
	glUniformMatrix4fv(pr->ProjectionMatrixUniformLocation, 1, GL_FALSE, mat->m);
	ExitOnGLError("ERROR: Could set the Projection Matrix");
}

void SetNormalMatrix(Program * pr, Matrix * mat){
	glUniformMatrix4fv(pr->NormalMatrixUniformLocation, 1, GL_TRUE, mat->m);
	ExitOnGLError("ERROR: Could set the Normal Matrix");
}


void SetTextureUnit(Program * pr, GLuint u){
	  printf ("%i\n",pr->TextureUnitUniformLocation);
	glUniform1i(pr->TextureUnitUniformLocation, u);
	ExitOnGLError("ERROR: Could set the Texture Unit");
}
void SetBumpTextureUnit(Program * pr, GLuint u){
    glUniform1i(pr->BumpTextureUnitUniformLocation, u);
printf("Bump , %i\n",pr->BumpTextureUnitUniformLocation);
    ExitOnGLError("ERROR: Could set the Bump Texture Unit");
}

void SetMaterial(Program * pr, Material * mat){
  glUniform4fv(pr->MaterialAmbientUniformLocation, 1, mat->Ambient);
  glUniform4fv(pr->MaterialDiffuseUniformLocation, 1, mat->Diffuse);
  glUniform4fv(pr->MaterialEmissionUniformLocation, 1, mat->Emission);
  glUniform4fv(pr->MaterialSpecularUniformLocation, 1, mat->Specular);
  glUniform1f(pr->MaterialShininessUniformLocation, mat->Shininess);
	ExitOnGLError("ERROR: Could set the Material");
}

//	We wszystkich Setup*
//	glUseProgram(...) nie trzeba
//	robić w programie zewnętrznym;
void SetupProgramMist(Program * pr, Mist * mis){
    glUseProgram(pr->MainProgram);
    glUniform4fv(pr->MistColorUniformLocation, 1, mis->Color);
    glUniform1f(pr->MistStartUniformLocation, mis->Start);
    glUniform1f(pr->MistEndUniformLocation, mis->End);
    glUniform1f(pr->MistDensityUniformLocation, mis->Density);
}

void SetupProgramMistType(Program * pr, GLuint t){
    glUseProgram(pr->MainProgram);
    glUniform1i(pr->MistTypeUniformLocation, t);
}
void SetupPointLight2(Program * pr, PointLight* light){
  GLint location;
  glUseProgram(pr->MainProgram);
  location = glGetUniformLocation(pr->MainProgram, "light2.position");
  if(location <0){
	  	fprintf(stderr,"ERROR: pointlight.position uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Position);
  location = glGetUniformLocation(pr->MainProgram, "light2.diffuse");
  if(location <0){
	  	fprintf(stderr,"ERROR: pointlight.diffuse uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Diffuse);
  location = glGetUniformLocation(pr->MainProgram, "light2.ambient");
  if(location <0){
	  	fprintf(stderr,"ERROR: pointlight.ambient uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Ambient);
  location = glGetUniformLocation(pr->MainProgram, "light2.specular");
  if(location <0){
	  	fprintf(stderr,"ERROR: pointlight.specular uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Specular);
  location = glGetUniformLocation(pr->MainProgram, "light2.attenuation");
  if(location <0){
	  	fprintf(stderr,"ERROR: pointlight.attenuation uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform3fv(location, 1, light->Attenuation);
  ExitOnGLError("ERROR: Could not setup the point light");
  glUseProgram(0);
}

void SetupPointLight(Program * pr, PointLight* light){
  GLint location;
  glUseProgram(pr->MainProgram);
  location = glGetUniformLocation(pr->MainProgram, "light.position");
  if(location <0){
	  	fprintf(stderr,"ERROR: pointlight.position uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Position);
  location = glGetUniformLocation(pr->MainProgram, "light.diffuse");
  if(location <0){
	  	fprintf(stderr,"ERROR: pointlight.diffuse uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Diffuse);
  location = glGetUniformLocation(pr->MainProgram, "light.ambient");
  if(location <0){
	  	fprintf(stderr,"ERROR: pointlight.ambient uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Ambient);
  location = glGetUniformLocation(pr->MainProgram, "light.specular");
  if(location <0){
	  	fprintf(stderr,"ERROR: pointlight.specular uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Specular);
  location = glGetUniformLocation(pr->MainProgram, "light.attenuation");
  if(location <0){
	  	fprintf(stderr,"ERROR: pointlight.attenuation uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform3fv(location, 1, light->Attenuation);
  ExitOnGLError("ERROR: Could not setup the point light");
  glUseProgram(0);
}


void SetupDirectionalLight(Program * pr, DirectionalLight* light){
  GLint location;
  glUseProgram(pr->MainProgram);
  location = glGetUniformLocation(pr->MainProgram, "light.diffuse");
  if(location <0){
	  	fprintf(stderr,"ERROR: directionallight.diffuse uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Diffuse);
  location = glGetUniformLocation(pr->MainProgram, "light.ambient");
  if(location <0){
	  	fprintf(stderr,"ERROR: directionallight.ambient uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Ambient);
  location = glGetUniformLocation(pr->MainProgram, "light.specular");
  if(location <0){
	  	fprintf(stderr,"ERROR: directionallight.specular uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Specular);
  location = glGetUniformLocation(pr->MainProgram, "light.position");
  if(location <0){
	  	fprintf(stderr,"ERROR: directionallight.position uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Position);
  ExitOnGLError("ERROR: Could not setup the directional light");
  glUseProgram(0);
}

void SetupSpotLight(Program * pr, SpotLight* light){
  GLint location;
  float Cutoff=cos(DegreesToRadians(light->Cutoff));
  
  glUseProgram(pr->MainProgram);
  location = glGetUniformLocation(pr->MainProgram, "light.position");
  if(location <0){
	  	fprintf(stderr,"ERROR: spotlight.position uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Position);
  location = glGetUniformLocation(pr->MainProgram, "light.diffuse");
  if(location <0){
	  	fprintf(stderr,"ERROR: spotlight.diffuse uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Diffuse);
  location = glGetUniformLocation(pr->MainProgram, "light.ambient");
  if(location <0){
	  	fprintf(stderr,"ERROR: spotlight.ambient uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Ambient);
  location = glGetUniformLocation(pr->MainProgram, "light.specular");
  if(location <0){
	  	fprintf(stderr,"ERROR: spotlight.specular uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform4fv(location, 1, light->Specular);
  location = glGetUniformLocation(pr->MainProgram, "light.attenuation");
  if(location <0){
	  	fprintf(stderr,"ERROR: spotlight.attenuation uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform3fv(location, 1, light->Attenuation);
  location = glGetUniformLocation(pr->MainProgram, "light.direction");
  if(location <0){
	  	fprintf(stderr,"ERROR: spotlight.direction uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform3fv(location, 1, light->Direction);
  location = glGetUniformLocation(pr->MainProgram, "light.cutoff");
  if(location <0){
	  	fprintf(stderr,"ERROR: spotlight.cutoff uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform1f(location, Cutoff);
  location = glGetUniformLocation(pr->MainProgram, "light.exponent");
  if(location <0){
	  	fprintf(stderr,"ERROR: spotlight.exponent uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  glUniform1f(location, light->Exponent);
  ExitOnGLError("ERROR: Could not setup the spot light");
  glUseProgram(0);
}

void CreateSkyProgram(Program * pr, const char* VertexShaderFile, const char * FragmentShaderFile, const char * MistShaderFile)
{//Brak materiałów, śiwatel, macierzy ModelMatrix, NormalMatrix;

  pr->MainProgram = glCreateProgram();
  ExitOnGLError("ERROR: Could not create the sky shader program");
  {
    pr->Shaders[0] = LoadShader(FragmentShaderFile, GL_FRAGMENT_SHADER);
    pr->Shaders[1] = LoadShader(VertexShaderFile, GL_VERTEX_SHADER);
    pr->Shaders[2] = LoadShader(MistShaderFile, GL_FRAGMENT_SHADER);
    glAttachShader(pr->MainProgram, pr->Shaders[0]);
    glAttachShader(pr->MainProgram, pr->Shaders[1]);
   glAttachShader(pr->MainProgram, pr->Shaders[2]);
  }
  glLinkProgram(pr->MainProgram);
  ExitOnGLError("ERROR: Could not link the sky shader program");
  
  glUseProgram(pr->MainProgram);
  
  pr->ViewMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "ViewMatrix");
  if(pr->ViewMatrixUniformLocation<0){
	fprintf(stderr,"ERROR: sky view matrix uniform location not found\n");
	exit(EXIT_FAILURE);
  }

  pr->ProjectionMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "ProjectionMatrix");
  if(pr->ProjectionMatrixUniformLocation<0){
	fprintf(stderr,"ERROR: sky projection matrix uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  
  pr->TextureUnitUniformLocation = glGetUniformLocation(pr->MainProgram, "skyTexture");

  if(pr->TextureUnitUniformLocation<0){
	fprintf(stderr,"ERROR: sky texture unit uniform location not found\n");
	exit(EXIT_FAILURE);
  }

pr->MistTypeUniformLocation = glGetUniformLocation(pr->MainProgram, "mist.type");
  if(pr->MistTypeUniformLocation<0){
	fprintf(stderr,"ERROR: mist.type uniform location not found\n");
	exit(EXIT_FAILURE);
  }

  pr->MistDensityUniformLocation = glGetUniformLocation(pr->MainProgram, "mist.density");
  if(pr->MistDensityUniformLocation<0){
	fprintf(stderr,"ERROR: mist.density uniform location not found\n");
	exit(EXIT_FAILURE);
  }

  pr->MistColorUniformLocation = glGetUniformLocation(pr->MainProgram, "mist.color");
  if(pr->MistColorUniformLocation<0){
	fprintf(stderr,"ERROR: mist.color uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  
  pr->MistStartUniformLocation = glGetUniformLocation(pr->MainProgram, "mist.start");
  if(pr->MistStartUniformLocation<0){
	fprintf(stderr,"ERROR: mist.start uniform location not found\n");
	exit(EXIT_FAILURE);
  }
  
  pr->MistEndUniformLocation = glGetUniformLocation(pr->MainProgram, "mist.end");
  if(pr->MistEndUniformLocation<0){
	fprintf(stderr,"ERROR: mist.end uniform location not found\n");
	exit(EXIT_FAILURE);
  }


  ExitOnGLError("ERROR: Could find uniform locations");
  glUseProgram(0);
  
}

void CreateSpriteProgram(Program * pr, const char* VertexShaderFile, const char * FragmentShaderFile)
{

  pr->MainProgram = glCreateProgram();
  ExitOnGLError("ERROR: Could not create the shader program");
  {
    pr->Shaders[0] = LoadShader(FragmentShaderFile, GL_FRAGMENT_SHADER);
    pr->Shaders[1] = LoadShader(VertexShaderFile, GL_VERTEX_SHADER);
    glAttachShader(pr->MainProgram, pr->Shaders[0]);
    glAttachShader(pr->MainProgram, pr->Shaders[1]);
  }
  glLinkProgram(pr->MainProgram);
  ExitOnGLError("ERROR: Could not link the shader program");
  
  glUseProgram(pr->MainProgram);
  
  pr->ViewMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "ViewMatrix");
  if(pr->ViewMatrixUniformLocation<0){
  fprintf(stderr,"ERROR: view matrix uniform location not found\n");
  //exit(EXIT_FAILURE);
  }

  pr->ProjectionMatrixUniformLocation = glGetUniformLocation(pr->MainProgram, "ProjectionMatrix");
  if(pr->ProjectionMatrixUniformLocation<0){
  fprintf(stderr,"ERROR: projection matrix uniform location not found\n");
  //exit(EXIT_FAILURE);
  }
  
  
  pr->TextureUnitUniformLocation = glGetUniformLocation(pr->MainProgram, "tex_star");
  if(pr->TextureUnitUniformLocation<0){
  fprintf(stderr,"ERROR: texture unit uniform location not found\n");
  exit(EXIT_FAILURE);
  }
  
  ExitOnGLError("ERROR: Could find uniform locations");
  glUseProgram(0);
 
}
