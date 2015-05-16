/*	Copyright (C) 2011 by Eddy Luten, 2013—2014 by Aleksander Denisiuk

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

#include "Utils.h"
#include "Model.h"
#include "Program.h"
#include "Texture.h"
#include "Mist.h"

#define WINDOW_TITLE_PREFIX "Blender"

int CurrentWidth = 800,
	CurrentHeight = 600,
	WindowHandle = 0;
	
Matrix
	ProjectionMatrix,
	ViewMatrix;

clock_t LastTime = 0;

Mist Fogg={
        {0.7f, 0.8f, 0.7f, 1.0f}, //color
        3.0f, //start
        60.0f, //end
        0.05f //density
};

GLint FoggType=1;

float TorusR=1.5,
      Torusr=0.5;
int   TorusM=40,
      TorusN=30;
int	  rotate = 0,
	  PlaneM=20,
      PlaneN=10;

int xOrigin = -1;
int yOrigin = -1;


Material IceMaterial={
  {0.2f, 0.2f, 0.2f, 1.0f}, //Ambient
  {0.85f, 0.85f, 1.05f, 1.0f}, //Diffuse
  {0.95f, 0.95f, 0.95f, 1.0f}, //Specular
  {0.0f, 0.0f, 0.0f, 1.0f}, //Emission
  60.0f	//Shininess
};

Material RedMaterial1={
  {0.0f, 0.0f, 0.0f, 0.05f}, //Ambient
  {0.0f, 0.0f, 0.0f, 0.05f}, //Diffuse
  {0.0f, 0.0f, 0.0f, 0.05f}, //Specular
  {0.0f, 0.0f, 0.0f, 0.05f}, //Emission
  20.0f	//Shininess
};

Material RedMaterial={
  {0.2f, 0.2f, 0.2f, 1.0f}, //Ambient
  {0.9f, 0.1f, 0.1f, 1.0f}, //Diffuse
  {0.5f, 0.5f, 0.5f, 1.0f}, //Specular
  {0.0f, 0.0f, 0.0f, 1.0f}, //Emission
  20.0f	//Shininess
};

Material YellowMaterial={
  {0.4f, 0.4f, 0.4f, 1.0f}, //Ambient
  {0.9f, 0.9f, 0.0f, 1.0f}, //Diffuse
  {0.1f, 0.1f, 0.1f, 1.0f}, //Specular
  {0.0f, 0.0f, 0.0f, 1.0f}, //Emission
  90.0f	//Shininess
};


PointLight  Light0={
      {3.0f, 5.0f, 10.0f, 1.0f}, //position
      {0.1f, 0.1f, 0.1f, 1.0f}, //ambient
      {1.0f, 1.0f, 1.0f, 1.0f}, //diffuse
      {1.0f, 1.0f, 1.0f, 1.0f}, //specular
      {0.009f, 0.0f, 0.009f}	//attenuation
    };
PointLight  LightSecond={
      {-5.0f, 3.0f, -15.0f, 1.0f}, //position
      {0.1f, 0.1f, 0.1f, 1.0f}, //ambient
      {1.0f, 1.0f, 1.0f, 1.0f}, //diffuse
      {1.0f, 1.0f, 1.0f, 1.0f}, //specular
      {0.08f, 0.0f, 0.04f}	//attenuation
    };
    
DirectionalLight  Light1={
      {0.0f, 6.0f, 3.0f, 0.0f}, //position
      {0.1f, 0.1f, 0.1f, 1.0f}, //ambient
      {1.0f, 1.0f, 1.0f, 1.0f}, //diffuse
      {1.0f, 1.0f, 1.0f, 1.0f} //specular
    };
    
SpotLight  Light2={
      {0.0f, 6.0f, 3.0f, 1.0f}, //position
      {0.1f, 0.1f, 0.1f, 1.0f}, //ambient
      {1.0f, 1.0f, 1.0f, 1.0f}, //diffuse
      {1.0f, 1.0f, 1.0f, 1.0f}, //specular
      {0.05f, 0.0f, 0.02f},	//attenuation
      {-1.0f, -6.0f, -2.0f},	//direction
      25,			//cutoff, degrees
      4			//exponent
};


void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);
void KeyboardFunction(unsigned char, int, int);
void SpecialKeyboardFunction(int, int, int);

void MouseMove(int , int );
void MouseButton(int , int , int , int );

#define N_OF_PROGRAMS 3
Program	program[N_OF_PROGRAMS];
Program skyProgram;
Program coneProgram;
Program haloProgram;
Program bumpProgram;

unsigned int CurrentProgram=0;
void SetupCamera(Matrix *);
void SetupMistType(GLuint);
void SetupMist(Mist *);


Model cube;
Model lad;
Model morze;
Model sky;
Model cone;
Model halo;

Texture cubeTexture;
Texture ladTexture;
Texture morzeTexture;
Texture morzeBumpTexture;
Texture skyTexture;
Texture haloTexture;

void DestroyAll(void);

int main(int argc, char* argv[])
{
	Initialize(argc, argv);

	glutMainLoop();
	
	exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[])
{
	GLenum GlewInitResult;

	InitWindow(argc, argv);
	
	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(GlewInitResult)
		);
		exit(EXIT_FAILURE);
	}
	
	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);

	glGetError();
	glClearColor(0.5f, 0.7f, 0.8f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	ExitOnGLError("ERROR: Could not set OpenGL depth testing options");

	LoadModel(&cube, "kostka");
	LoadModel(&lad , "lad");
	LoadModel(&morze, "morze");
	LoadModel(&cone, "stozek");
	CreateSky(&sky);
	CreateHalo(&halo, 1);

	CreateSkyProgram(&skyProgram, "SkyShader.vertex.glsl","SkyShader.fragment.glsl", "MistShader.glsl");
	
	CreateProgram(&(program[0]), "PointLight.vertex.glsl","PointLight.fragment.glsl", "MistShader.glsl");
	CreateProgram(&(program[1]), "DirectionalLight.vertex.glsl","DirectionalLight.fragment.glsl", "MistShader.glsl");
	CreateProgram(&(program[2]), "SpotLight.vertex.glsl","SpotLight.fragment.glsl", "MistShader.glsl");
	printf("\nProgram2\n");
	CreateProgram2(&bumpProgram,"PointLightBump.vertex.glsl","PointLightBump.fragment.glsl");
	CreateProgram2(&coneProgram, "LightShader.vertex.glsl", "LightShader.fragment.glsl");

	CreateSpriteProgram(&haloProgram, "Sprite.vertex.glsl","Sprite.fragment.glsl");

	SetupPointLight(&(program[0]), &Light0);
	SetupPointLight2(&(program[0]), &LightSecond);
	SetupDirectionalLight(&(program[1]), &Light1);
	SetupSpotLight(&(program[2]), &Light2);

	CreateTextureFromTGA(&cubeTexture,"kostka.tga");

	// parametry interpolacji tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// parametry ekstrapolacji teksturt
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	CreateTextureFromTGA(&morzeBumpTexture,"morzeBump.tga");
	// parametry interpolacji tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// parametry ekstrapolacji teksturt
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	CreateSkyTextureFromTGA(&skyTexture,"a/");

	// parametry interpolacji tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// parametry ekstrapolacji teksturt
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	CreateTextureFromTGA(&ladTexture,"lad.tga");
	// parametry interpolacji tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// parametry ekstrapolacji teksturt
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	
	CreateTextureFromTGA(&morzeTexture,"morze.tga");
	// parametry interpolacji tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	

	CreateTextureFromTGA(&haloTexture,"star1.tga");	
	// parametry interpolacji tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	

	SetModelTexture(&cube, &cubeTexture);
	SetModelTexture(&lad, &ladTexture);
	SetModelTexture(&morze, &morzeTexture);
	SetModelBumpTexture(&morze, &morzeBumpTexture);
	SetModelTexture(&sky, &skyTexture);
	

	SetModelTexture(&halo, &haloTexture);

	SetModelMaterial(&cone, &RedMaterial);
	ViewMatrix = IDENTITY_MATRIX;
	TranslateMatrix(&ViewMatrix, 0, -10, 20);
	RotateAboutY(&ViewMatrix, 180);

	

	ProjectionMatrix =
		CreateProjectionMatrix(
			60,
			(float)CurrentWidth / CurrentHeight,
			0.1f,
			100.0f
		);

	
	glUseProgram(program[0].MainProgram);
	SetProjectionMatrix(&program[0], &ProjectionMatrix);
	SetViewMatrix(&program[0], &ViewMatrix);
	SetTextureUnit(&program[0], 0);
	glUseProgram(program[1].MainProgram);
	SetProjectionMatrix(&program[1], &ProjectionMatrix);
	SetViewMatrix(&program[1], &ViewMatrix);
	SetTextureUnit(&program[1], 0);
	glUseProgram(program[2].MainProgram);
	SetProjectionMatrix(&program[2], &ProjectionMatrix);
	SetViewMatrix(&program[2], &ViewMatrix);
	SetTextureUnit(&program[2], 0);	
	glUseProgram(skyProgram.MainProgram);
	SetProjectionMatrix(&skyProgram, &ProjectionMatrix);
	SetViewMatrix(&skyProgram, &ViewMatrix);
	SetTextureUnit(&skyProgram, 1);
	glUseProgram(coneProgram.MainProgram);
	printf("laduje cone \n");
        SetProjectionMatrix(&coneProgram, &ProjectionMatrix);
	SetViewMatrix(&coneProgram, &ViewMatrix);
	SetTextureUnit(&coneProgram, 1);
        printf("laduje halo \n");
	glUseProgram(haloProgram.MainProgram);
	SetProjectionMatrix(&haloProgram, &ProjectionMatrix);
	SetViewMatrix(&haloProgram, &ViewMatrix);
	SetTextureUnit(&haloProgram, 3);
	printf("\nzaladowalem wszystkie\n");
	glUseProgram(bumpProgram.MainProgram);
	SetProjectionMatrix(&bumpProgram, &ProjectionMatrix);
	SetViewMatrix(&bumpProgram, &ViewMatrix);
	SetTextureUnit(&bumpProgram, 0);
	SetBumpTextureUnit(&bumpProgram, 4);
	glUseProgram(0);

	SetupMist(& Fogg);
	SetupMistType(FoggType);

}

void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
	);
	
	glutInitWindowSize(CurrentWidth, CurrentHeight);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

	if(WindowHandle < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
		);
		exit(EXIT_FAILURE);
	}
	
	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(KeyboardFunction);
	glutSpecialFunc(SpecialKeyboardFunction);
	glutCloseFunc(DestroyAll);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMove);
}

void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
	ProjectionMatrix =
		CreateProjectionMatrix(
			60,
			(float)CurrentWidth / CurrentHeight,
			0.1f,
			100.0f
		);
	glUseProgram(program[0].MainProgram);
	SetProjectionMatrix(&(program[0]), &ProjectionMatrix);
	glUseProgram(program[1].MainProgram);
	SetProjectionMatrix(&(program[1]), &ProjectionMatrix);
	glUseProgram(program[2].MainProgram);
	SetProjectionMatrix(&(program[2]), &ProjectionMatrix);
	glUseProgram(skyProgram.MainProgram);
	SetProjectionMatrix(&skyProgram, &ProjectionMatrix);
	glUseProgram(coneProgram.MainProgram);
	SetProjectionMatrix(&coneProgram, &ProjectionMatrix);
	glUseProgram(haloProgram.MainProgram);
	SetProjectionMatrix(&haloProgram, &ProjectionMatrix);
	glUseProgram(bumpProgram.MainProgram);
	SetProjectionMatrix(&bumpProgram, &ProjectionMatrix);
	glUseProgram(0);

}

void RenderFunction(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawModel(&cube, &(program[CurrentProgram]));
	DrawModel(&lad, &(program[CurrentProgram]));
	DrawModel(&morze, &(program[CurrentProgram]));
        DrawModel(&morze, &bumpProgram);
        
	clock_t Now = clock();

	if (LastTime == 0) 
	  LastTime = Now;
	if(rotate>360) rotate -=360;
	rotate += ceil(((float)(Now - LastTime) / CLOCKS_PER_SEC));
	LastTime = Now;
	
	DrawSky(&sky, &skyProgram);
	DrawModelCone(&cone,  &coneProgram, rotate);
	DrawHalo(&halo, &haloProgram);
	glutSwapBuffers();
	glutPostRedisplay();
}


void DestroyAll(void)
{
  DestroyProgram(&(program[0]));
  DestroyProgram(&(program[1]));
  DestroyProgram(&(program[2]));
  DestroyProgram(&skyProgram);
  DestroyModel(&cube);
  DestroyModel(&lad);
  DestroyModel(&morze);
  DestroyTexture(&cubeTexture);
  DestroyProgram2(&coneProgram);
  DestroyProgram2(&haloProgram);

  DestroyTexture(&morzeBumpTexture);
  DestroyTexture(&skyTexture);
  DestroyTexture(&morzeTexture);
  DestroyTexture(&ladTexture);

  DestroyModel(&cone);
  DestroyModel(&halo);
}


void KeyboardFunction(unsigned char Key, int X, int Y)
{
  switch (Key){
    case 27:
      glutLeaveMainLoop();
      break;
    default:
      break;
  }
}

void SpecialKeyboardFunction(int Key, int X, int Y){
  switch (Key){
    case GLUT_KEY_F1:
      CurrentProgram=0;
    break;
    case GLUT_KEY_F2:
      CurrentProgram=1;
    break;
    case GLUT_KEY_F3:
      CurrentProgram=2;
    break;
case GLUT_KEY_F4:
      SetupMistType(3);
    break;
    case GLUT_KEY_F5:
      SetupMistType(0);
    break;
    case GLUT_KEY_F6:
      SetupMistType(1);
    break;
    case GLUT_KEY_F7:
      SetupMistType(2);
    break;
    default:
    break;
  }
  glutPostRedisplay();
}


void MouseMove(int x, int y){	// this will only be true when the left button is down
    float deltaXAngle=0;
    float deltaYAngle=0;
    if (xOrigin >= 0&&yOrigin>=0) {
		// update deltaAngle
        deltaXAngle = (x - xOrigin) * 0.1f;
        deltaYAngle = (y - yOrigin) * 0.1f;
	xOrigin=x;
	yOrigin=y;

		// update camera's direction
    RotateAboutY(&ViewMatrix, -deltaXAngle);
	RotateAboutX(&ViewMatrix, deltaYAngle);
	SetupCamera(&ViewMatrix);
        glutPostRedisplay();
   }
}


#if !defined(GLUT_WHEEL_UP)
# define GLUT_WHEEL_UP 3
# define GLUT_WHEEL_DOWN 4
#endif

void MouseButton(int button, int state, int x, int y) {
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {
	      // when the button is released
	      if (state == GLUT_UP) {
		      xOrigin = -1;
		      yOrigin = -1;
	      }
	      else  {// state = GLUT_DOWN
		      xOrigin = x;
		      yOrigin = y;
	      }
	}
    else if ( button == GLUT_WHEEL_UP ){
        TranslateMatrix(&ViewMatrix, 0,0,-0.1f);
	SetupCamera(&ViewMatrix);
        glutPostRedisplay();
    }
    else if( button == GLUT_WHEEL_DOWN ){
        TranslateMatrix(&ViewMatrix, 0,0,0.1f);
	SetupCamera(&ViewMatrix);
        glutPostRedisplay();
    }
}


void SetupCamera(Matrix * ViewMatrix){
  int i;
  for (i=0; i<N_OF_PROGRAMS; i++){
    glUseProgram(program[i].MainProgram);
    SetViewMatrix(&program[i], ViewMatrix);
    glUseProgram(skyProgram.MainProgram);
	SetViewMatrix(&skyProgram, ViewMatrix);

  }
  glUseProgram(bumpProgram.MainProgram);
  SetViewMatrix(&bumpProgram, ViewMatrix);
  glUseProgram(coneProgram.MainProgram);
  SetViewMatrix(&coneProgram, ViewMatrix);
  glUseProgram(haloProgram.MainProgram);
  SetViewMatrix(&haloProgram, ViewMatrix);
  glUseProgram(0);
}

void SetupMist(Mist * Fogg){
  int i;
  for (i=0; i<N_OF_PROGRAMS; i++){
     SetupProgramMist(&program[i], Fogg);
  }
  SetupProgramMist(&skyProgram, Fogg);
  glUseProgram(0);
}

void SetupMistType(GLuint t){
  int i;
  for (i=0; i<N_OF_PROGRAMS; i++){
     SetupProgramMistType(&program[i], t);
  }
  SetupProgramMistType(&skyProgram, t);
  glUseProgram(0);
}


