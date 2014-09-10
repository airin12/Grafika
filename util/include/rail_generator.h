#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <vectors.h>

class bar{
public:
	vector3<GLfloat> *vertices;
	vector3<GLfloat> *normals;
	vector3<GLint> *faces;

	GLint nVertices;
	GLint nNormals;
	GLint nFaces;

	bar(void);
	bar(GLfloat width, GLfloat height, GLfloat length, GLfloat initialX);
};

class rail{
public:
	vector3<GLfloat> *vertices;
	vector3<GLfloat> *normals;
	vector3<GLint> *faces;

	GLint nVertices;
	GLint nNormals;
	GLint nFaces;

	rail(void);
	rail(GLfloat radius, GLfloat step, GLfloat width, GLfloat height);
};

void generateRails(rail &r1, rail &r2,GLfloat radius, GLfloat step, GLfloat width, GLfloat height, GLfloat widthBetween,bar &b);