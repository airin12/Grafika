#include <cmath>

class matrix
{
private:
	int size;
	float **mat;
	float **allocMat(int);
	void deleteMat(float **, int);
	void zeroMat(float **, int);
	void multMatrix(float **);
	void setTranslate(float, float, float);
	void setRotate(float, float, float, float);
	void setScale(float, float, float);
	void setPerspective(float, float, float);

public:
	matrix(void) { mat = NULL; };
	matrix(int);
	~matrix(void);
	void setIdentity(void);
	float *get(void);
	float **get2d(void) { return mat; };
	void translate(float, float, float);
	void rotate(float, float, float, float);
	void scale(float, float, float);
	void perspective(float, float, float);
	void transform(float *);
	void loadMatrix(float **);

	void writeMatrix(void);
};

class matrixStack
{
private:
	matrix *top;
	matrix *prev;

public:
	matrixStack(void) { top = NULL; prev = NULL;};
	void put (matrix *);
	void pop (matrix *);
};