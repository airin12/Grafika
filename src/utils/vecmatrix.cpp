#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>

#include <vecmatrix.h>

using namespace std;

matrix::matrix(int s): size(s)
{
	mat = allocMat(size);
	zeroMat(mat, size);
}

matrix::~matrix(void)
{
	deleteMat(mat, size);
}

float **matrix::allocMat(int s)
{
	float **m = new float* [s];

	for (int i = 0; i < s; i++)
		m[i] = new float [s];
	return m;
}

void matrix::zeroMat(float **m, int s)
{
	for (int i = 0; i < s; i++)
		for (int j = 0; j < s; j++)
			m[i][j] = 0;
}

void matrix::deleteMat(float **m, int s)
{
	for (int i = 0; i < s; i++)
		delete [] m[i];
	delete [] m;
}

void matrix::setIdentity(void)
{
	zeroMat(mat, size);
	for (int i = 0; i < size; i++)
		mat[i][i] = 1.0f;
}

float *matrix::get(void)
{
	float *mat1d = new float [size * size];

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			mat1d[i * size + j] = mat[i][j];

	return mat1d;
}

void matrix::writeMatrix(void)
{
	for (int i = 0; i < size; i++) {
		cout << endl;
		for (int j = 0; j < size; j++)
			cout << mat[i][j] << '\t';
	}
	cout << "=============" << endl;
}

void matrix::setTranslate(float x, float y, float z)
{
	mat[0][3] = x;
	mat[1][3] = y;
	mat[2][3] = z;
}

void matrix::translate(float x, float y, float z)
{
	matrix matTranslate(4);
	matTranslate.setIdentity();
	matTranslate.setTranslate(x, y, z);
	multMatrix(matTranslate.get2d());
//	cout << " === translate === " << endl;
//	writeMatrix();
}

void matrix::setRotate(float a, float x, float y, float z)
{
	mat[0][0] = x * x * (1 - cos(a)) + cos(a);
	mat[0][1] = x * y * (1 - cos(a)) - z * sin(a);
	mat[0][2] = x * z * (1 - cos(a)) + y * sin(a);
	mat[1][0] = x * y * (1 - cos(a)) + z * sin(a);
	mat[1][1] = y * y * (1 - cos(a)) + cos(a);
	mat[1][2] = y * z * (1 - cos(a)) - x * sin(a);
	mat[2][0] = x * z * (1 - cos(a)) - y * sin(a);
	mat[2][1] = y * z * (1 - cos(a)) + x * sin(a);
	mat[2][2] = z * z * (1 - cos(a)) + cos(a);
}

void matrix::rotate(float a, float x, float y, float z)
{
	matrix matRotate(4);
	matRotate.setIdentity();
	matRotate.setRotate(a, x, y, z);
	multMatrix(matRotate.get2d());
//	cout << " === rotate === " << endl;
//	writeMatrix();
}

void matrix::setScale(float sx, float sy, float sz)
{
	mat[0][0] = sx;
	mat[1][1] = sy;
	mat[2][2] = sz;
}

void matrix::scale(float sx, float sy, float sz)
{
	matrix matScale(4);
	matScale.setIdentity();
	matScale.setScale(sx, sy, sz);
	multMatrix(matScale.get2d());
//	cout << " === scale === " << endl;
//	writeMatrix();	
}

void matrix::setPerspective(float s, float n, float f)
{
	mat[0][0] = s;
	mat[1][1] = s;
	mat[2][2] = (n + f) / (n - f);
	mat[2][3] = 2 * n * f / (n - f);
	mat[3][2] = -1.0;
}

void matrix::perspective(float s, float n, float f)
{
	matrix matPerspective(4);
	matPerspective.setIdentity();
	matPerspective.setPerspective(s, n, f);
	multMatrix(matPerspective.get2d());
//	cout << " === perspective === " << endl;
//	writeMatrix();
}

void matrix::multMatrix(float **m)
{
	float **result = allocMat(size);

	zeroMat(result, size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			for (int k = 0; k < size; k++)
				result[i][j] += mat[i][k] * m[k][j];

	deleteMat(mat, size);
	mat = result;
}

void matrix::transform(float *v)
{
	float *result = new float [size];
	for (int i = 0; i < size; i++)
		result[i] = 0.0;

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			result[i] += mat[i][j] * v[j];
	for (int i = 0; i < size; i++)
		v[i] = result[i];

	delete [] result;
}

void matrix::loadMatrix(float **m)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			mat[i][j] = m[i][j];
}



void matrixStack::put(matrix *m)
{
	matrix *newTop = new matrix(4);

	newTop->loadMatrix(m->get2d());
	prev = top;
	top = newTop;
//	cout << " === matrixStack === " << endl;
//	top->writeMatrix();
}

void matrixStack::pop(matrix *m)
{
	if (!top) return;

	m->loadMatrix(top->get2d());
	delete top;
	top = prev;
}