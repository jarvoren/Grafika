/*	Copyright (C) 2014 by Aleksander Denisiuk

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Texture.h"

GLboolean LoadTGAFile(GLenum target, const char  * filename);
  



typedef struct TGAHeader
{
  uint8_t  idlength;
  uint8_t  colormap;
  uint8_t  datatype;
  uint8_t  colormapinfo[5];
  uint16_t xorigin;
  uint16_t yorigin;
  uint16_t width;
  uint16_t height;
  uint8_t  bitperpel;
  uint8_t  description;
} TGAHeader;


void CreateSkyTextureFromTGA(Texture * tex, const char *filename){
    GLenum target;
    char filename1[255];
    char i; // parametr pętli
    char n; // numer w nazwie pliku
    int len; // długość nazwy pliku

    // nowy indeks tekstury
    glGenTextures(1, &tex->Texture);

//    glActiveTexture(GL_TEXTURE1);
    // aktywizacja
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex->Texture);

    // parametry interpolacji tekstury
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // parametry
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);



    strcpy(filename1,filename);
    len=strlen(filename);
    n='1';
    filename1[len+1]='.';
    filename1[len+2]='t';
    filename1[len+3]='g';
    filename1[len+4]='a';
    filename1[len+5]='\0';

    for(i=0;i<6;i++){
        filename1[len]=n++;
      switch(i){
        case 0:
//            right;
            target=GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        break;
        case 1:
//             back;
          target=GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        break;
        case 2:
//            top;
          target=GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        break;
        case 3:
//            left;
          target=GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        break;
        case 4:
//            front;
          target=GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        break;
        case 5:
//            bottom;
          target=GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        break;
      }
      LoadTGAFile(target, filename1);
//      qDebug()<<filename1;
      ExitOnGLError("ERROR: Could not create sky texture");
    }
}

void CreateTextureFromTGA(Texture * tex, const char * filename){
  

    // nowy indeks tekstury
    glGenTextures(1, &(tex->Texture));
    
        
    // aktywizacja
    glBindTexture(GL_TEXTURE_2D, tex->Texture);
    
    LoadTGAFile(GL_TEXTURE_2D, filename);

    glBindTexture(GL_TEXTURE_2D, tex->Texture);
    // 
    ExitOnGLError("ERROR: Could not load texture");
}




GLboolean LoadTGAFile(GLenum target, const char *filename)
{
  
  TGAHeader*  header;
  GLchar*     buffer;
  size_t      file_size;
  GLuint format;
  GLuint internalFormat;
  
  FILE* file;
  long size = -1;
  
  if (NULL != (file = fopen(filename, "rb")) &&
    0 == fseek(file, 0, SEEK_END) &&
    -1 != (size = ftell(file)))
  {
    rewind(file);
    
    if (NULL != (buffer = (GLchar*)malloc(size))){
      if (size != (long)fread(buffer, sizeof(GLchar), size, file)){
	fprintf(stderr, "ERROR: Could not read file %s\n", filename);
	free(buffer);
	fclose(file);
	return 0;
      }
    }
    else {
      fprintf(stderr, "ERROR: Could not allocate %i bytes.\n", size);
      fclose(file);
      return 0;
    }
    fclose(file);
  }
  else
    fprintf(stderr, "ERROR: Could not open file %s\n", filename);
  
  
  // Rozmiar jest mniejszy od nagłówka
    if (size <= sizeof(TGAHeader))
    {
      fprintf(stderr, "Too small file '%s'\n", filename);
      free(buffer);
      return 0;
    }
    
    header = (TGAHeader*)buffer;
    
    // sprawa się plik TGA: nieskompresowany RGB lub RGBA obraz
    if (header->datatype != 2 || (header->bitperpel != 24 && header->bitperpel != 32))
    {
      fprintf(stderr, "Wrong TGA format '%s'\n", filename);
      free(buffer);
      return 0;
    }
    
   
    
    // format tekstury
    format = (header->bitperpel == 24 ? GL_BGR : GL_BGRA);
    internalFormat = (format == GL_BGR ? GL_RGB8 : GL_RGBA8);

      // wysyłanie tekstury na GPU
      glTexImage2D(target, 0, internalFormat, header->width, header->height, 0, format,
           GL_UNSIGNED_BYTE, (const GLvoid*)(buffer + sizeof(TGAHeader) + header->idlength));

      free(buffer);
      
    return 1;
}
    


void DestroyTexture(Texture * tex){
  glDeleteTextures(1, &(tex->Texture));
  ExitOnGLError("ERROR: Could not destroy the texture");
}	
