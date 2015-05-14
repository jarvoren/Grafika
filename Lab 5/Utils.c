/*	Copyright (C) 2011 by Eddy Luten, 2013—2014 by Aleksander Denisiuk
 * 
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

float Cotangent(float angle)
{
	return (float)(1.0 / tan(angle));
}

float DegreesToRadians(float degrees)
{
	return degrees * (float)(M_PI / 180);
}

float RadiansToDegrees(float radians)
{
	return radians * (float)(180 / M_PI);
}

Matrix MultiplyMatrices(const Matrix* m1, const Matrix* m2)
{
	Matrix out = IDENTITY_MATRIX;
	unsigned int row, column, row_offset;

	for (row = 0, row_offset = row * 4; row < 4; ++row, row_offset = row * 4)
		for (column = 0; column < 4; ++column)
			out.m[row_offset + column] =
				(m1->m[row_offset + 0] * m2->m[column + 0]) +
				(m1->m[row_offset + 1] * m2->m[column + 4]) +
				(m1->m[row_offset + 2] * m2->m[column + 8]) +
				(m1->m[row_offset + 3] * m2->m[column + 12]);

	return out;
}

void ScaleMatrix(Matrix* m, float x, float y, float z)
{
	Matrix scale = IDENTITY_MATRIX;

	scale.m[0] = x;
	scale.m[5] = y;
	scale.m[10] = z;
	
	Matrix output=MultiplyMatrices(m, &scale);
	
	memcpy(m->m, output.m, sizeof(m->m));
}

void TranslateMatrix(Matrix* m, float x, float y, float z)
{
	Matrix translation = IDENTITY_MATRIX;
	
	translation.m[12] = x;
	translation.m[13] = y;
	translation.m[14] = z;

	Matrix output=MultiplyMatrices(m, &translation);
	
	memcpy(m->m, output.m, sizeof(m->m));
}

void RotateAboutX(Matrix* m, float dangle)
{
	float angle=dangle*M_PI/180.0f;
	Matrix rotation = IDENTITY_MATRIX;
	float sine = (float)sin(angle);
	float cosine = (float)cos(angle);
	
	rotation.m[5] = cosine;
	rotation.m[6] = -sine;
	rotation.m[9] = sine;
	rotation.m[10] = cosine;
	
	Matrix output=MultiplyMatrices(m, &rotation);
	
	memcpy(m->m, output.m, sizeof(m->m));
}

void RotateAboutY(Matrix* m, float dangle)
{
	float angle=dangle*M_PI/180.0f;
	Matrix rotation = IDENTITY_MATRIX;
	float sine = (float)sin(angle);
	float cosine = (float)cos(angle);
	
	rotation.m[0] = cosine;
	rotation.m[8] = sine;
	rotation.m[2] = -sine;
	rotation.m[10] = cosine;

	Matrix output=MultiplyMatrices(m, &rotation);
	
	memcpy(m->m, output.m, sizeof(m->m));
}

void RotateAboutZ(Matrix* m, float dangle)
{
	float angle=dangle*M_PI/180.0f;
	Matrix rotation = IDENTITY_MATRIX;
	float sine = (float)sin(angle);
	float cosine = (float)cos(angle);
	
	rotation.m[0] = cosine;
	rotation.m[1] = -sine;
	rotation.m[4] = sine;
	rotation.m[5] = cosine;

	Matrix output=MultiplyMatrices(m, &rotation);
	
	memcpy(m->m, output.m, sizeof(m->m));
}

Matrix CreateProjectionMatrix(
	float fovy,
	float aspect_ratio,
	float near_plane,
	float far_plane
)
{
	Matrix out = { { 0 } };

	const float
		y_scale = Cotangent((fovy*M_PI/180) / 2),
		x_scale = y_scale / aspect_ratio,
		frustum_length = far_plane - near_plane;

	out.m[0] = x_scale;
	out.m[5] = y_scale;
	out.m[10] = -((far_plane + near_plane) / frustum_length);
	out.m[11] = -1;
	out.m[14] = -((2 * near_plane * far_plane) / frustum_length);
	
	return out;
}

void ExitOnGLError(const char* error_message)
{
	const GLenum ErrorValue = glGetError();

	if (ErrorValue != GL_NO_ERROR)
	{
		const char* APPEND_DETAIL_STRING = ": %s\n";
		const size_t APPEND_LENGTH = strlen(APPEND_DETAIL_STRING) + 1;
		const size_t message_length = strlen(error_message);
		char* display_message = (char*)malloc(message_length + APPEND_LENGTH);

		memcpy(display_message, error_message, message_length);
		memcpy(&display_message[message_length], APPEND_DETAIL_STRING, APPEND_LENGTH);

		fprintf(stderr, display_message, gluErrorString(ErrorValue));

		free(display_message);
		exit(EXIT_FAILURE);
	}
}
