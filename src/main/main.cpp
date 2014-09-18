#include <shaders.h>
#include <vecmatrix.h>
#include <vectors.h>
#include <obj.h>
#include <rail_generator.h>

// Załadowanie pliku nagłówkowego biblioteki GLEW - znajdują się w nim wszystkie
// rozszerzenia biblioteki OpenGL również dla wersji wyższych niż 3.0
// Nie ma więc potrzeby wykorzysytwania pliku nagłówkowego gl3.h
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>


const GLfloat pi = 3.14159265358979323846;
using namespace std;

GLfloat prScale = 1.0, prNear = 0.1f, prFar = 45.0f;

GLuint shader, ambientShader;
GLuint cubeVertexArray, locomotiveVertexArray, treeVertexArray, trainVertexArray, floorVertexArray, railsVertexArray1, railsVertexArray2, barVertexArray;

matrix modelView(4);
matrix projection(4);

GLfloat translWorld[3] = {0.0f, 0.0f, -2.0f};
GLfloat angX = 0.0f, angY = 0.0f, angZ = 0.0f;
GLfloat angWorldX = 0.0f, angWorldY = 0.0f, angWorldZ = 0.0f;

matrixStack modelViewStack;

// obiekt reprezentujący bryłę wczytaną jako .obj
objShape locomotive, tree, train;



rail r1, r2;
bar b;

GLfloat radius;
GLfloat railHeight;
GLfloat widthBetween;
GLfloat railWidth;

float mouseControlX=0.0f;
float mouseControlY=0.0f;

float deltaAngle = 0.0f;
float angle;
float lx,lz;
int xOrigin = -1;

int centralX=300;
int centralY=300;

int clickX;
int clickY;

float lastx = 0.0;
float lasty = 0.0;

float actualSpeed = 0.0f;
float actualAcceleration = 0.0f;

float minAcceleration = 0.0f;
float maxAcceleration = 0.3f;

bool ctrOn=false;
bool accelerate=false;

float mouseSpeed=0.01;

void createVertexArray(GLuint vertexArray,
	GLfloat *vertices, size_t verticesSize,
	GLfloat *normals, size_t normalsSize,
	GLuint *indices, size_t indicesSize)
{
	GLuint dataBuffer;

	glBindVertexArray(vertexArray);

	//wspolrzedne
	glGenBuffers(1, &dataBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, (GLfloat*)vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// normalne:
	glGenBuffers(1, &dataBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, normalsSize, (GLfloat*)normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// indeksy:
	glGenBuffers(1, &dataBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dataBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, (GLuint*)indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // wierzchołki
	glEnableVertexAttribArray(1); // normalne
}

int init()
{
	radius=2.0f;
	railHeight=0.01f;
	railWidth=0.01f;
	widthBetween=0.15f;
	generateRails(r1,r2,radius,0.01f,railWidth,railHeight,widthBetween,b);

GLfloat cubeVertices[3*8] = {
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f
};
// normalne
GLfloat cubeNormals[3*8] = {
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f
};
// indeksy
GLuint cubeIndices[3*6*2] = {
	0, 1, 2,
	2, 1, 3,
	1, 5, 3,
	3, 5, 7,
	5, 4, 7,
	7, 4, 6,
	4, 0, 6,
	6, 0, 2,
	1, 0, 5,
	5, 0, 4,
	3, 7, 2,
	2, 7, 6
};

GLfloat floorVertices[3*4] = {
	-10.0f, -10.0f,  0.0f,
	 10.0f, -10.0f,  0.0f,
	-10.0f,  10.0f,  0.0f,
	 10.0f,  10.0f,  0.0f
};

GLfloat floorNormals[3*4] = {
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
};

GLuint floorIndices[3*2] = {
	0, 1, 2,
	2, 1, 3
};


	if ( locomotive.readFromFile("../obj/loc4.obj") || tree.readFromFile("../obj/tree_oak.obj") || train.readFromFile("../obj/train2.obj"))
		return 1;
	tree.scale();
	locomotive.scale();
	train.scale();

	if (!locomotive.nNormals || !tree.nNormals || !train.nNormals){
		train.genSmoothNormals();
		tree.genSmoothNormals();
		locomotive.genSmoothNormals();
	}else{
		if (!train.normIndGood || ! tree.normIndGood || locomotive.normIndGood){
			tree.rebuildAttribTable('n');
			locomotive.rebuildAttribTable('n');
			train.rebuildAttribTable('n');
		}
	}
	
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

	AttachVertexShader(shader, "../shaders/vertexShader.glsl");
	AttachFragmentShader(shader, "../shaders/fragmentShader.glsl");
	LinkProgram(shader);

	glGenVertexArrays(1, &cubeVertexArray);
	createVertexArray(cubeVertexArray,
		cubeVertices, sizeof(cubeVertices),
		cubeNormals, sizeof(cubeNormals),
		cubeIndices, sizeof(cubeIndices));

	glGenVertexArrays(1, &locomotiveVertexArray);
	createVertexArray(locomotiveVertexArray,
		(GLfloat*)locomotive.vertices, 3*locomotive.nAttribs*sizeof(GLfloat),
		(GLfloat*)locomotive.normals, 3*locomotive.nAttribs*sizeof(GLfloat),
		(GLuint*)locomotive.faces, 3*locomotive.nFaces*sizeof(GLuint));

	glGenVertexArrays(1, &treeVertexArray);
	createVertexArray(treeVertexArray,
		(GLfloat*)tree.vertices, 3*tree.nAttribs*sizeof(GLfloat),
		(GLfloat*)tree.normals, 3*tree.nAttribs*sizeof(GLfloat),
		(GLuint*)tree.faces, 3*tree.nFaces*sizeof(GLuint));

	glGenVertexArrays(1, &trainVertexArray);
	createVertexArray(trainVertexArray,
		(GLfloat*)train.vertices, 3*train.nAttribs*sizeof(GLfloat),
		(GLfloat*)train.normals, 3*train.nAttribs*sizeof(GLfloat),
		(GLuint*)train.faces, 3*train.nFaces*sizeof(GLuint));



	glGenVertexArrays(1, &floorVertexArray);
	createVertexArray(floorVertexArray,
		floorVertices, sizeof(floorVertices),
		floorNormals, sizeof(floorNormals),
		floorIndices, sizeof(floorIndices));


	glGenVertexArrays(1, &railsVertexArray1);
		createVertexArray(railsVertexArray1,
		(GLfloat*)r1.vertices, 3*r1.nVertices*sizeof(GLfloat),
		(GLfloat*)r1.normals, 3*r1.nNormals*sizeof(GLfloat),
		(GLuint*)r1.faces, 3*r1.nFaces*sizeof(GLuint));

		glGenVertexArrays(1, &railsVertexArray2);
		createVertexArray(railsVertexArray2,
		(GLfloat*)r2.vertices, 3*r2.nVertices*sizeof(GLfloat),
		(GLfloat*)r2.normals, 3*r2.nNormals*sizeof(GLfloat),
		(GLuint*)r2.faces, 3*r2.nFaces*sizeof(GLuint));

		glGenVertexArrays(1, &barVertexArray);
		createVertexArray(barVertexArray,
		(GLfloat*)b.vertices, 3*b.nVertices*sizeof(GLfloat),
		(GLfloat*)b.normals, 3*b.nNormals*sizeof(GLfloat),
		(GLuint*)b.faces, 3*b.nFaces*sizeof(GLuint));

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glEnable(GL_CULL_FACE);

	return 0;
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	projection.setIdentity();
	projection.perspective(prScale, prNear, prFar);
}


void drawRails(GLfloat radius, GLfloat step, matrix &modelView){
	GLfloat brownColor[4] = {0.55f, 0.27f, 0.0f, 0.08f};
	GLfloat silverColor[4] = {0.75f, 0.75f, 0.75f, 1.0f};

	glUniform4fv(glGetUniformLocation(shader, "inColor"), 1, silverColor);

	glBindVertexArray(railsVertexArray1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 3*r1.nFaces, GL_UNSIGNED_INT, 0);

	glBindVertexArray(railsVertexArray2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 3*r2.nFaces, GL_UNSIGNED_INT, 0);


	int numberOfBars = floor(radius/step);
	GLfloat radians = (360.0f/numberOfBars)*(pi/180.0f);

	glUniform4fv(glGetUniformLocation(shader, "inColor"), 1, brownColor);
	for(int i=0;i<numberOfBars;i++){
		glBindVertexArray(barVertexArray);
		glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewMatrix"), 1, GL_TRUE, modelView.get());
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 3*b.nFaces, GL_UNSIGNED_INT, 0);
		modelView.rotate(radians, 0.0f, 0.0f, 1.0f);
	}
	
}

void display(void)
{
	GLfloat greenColor[4] = {0.0f, 0.7f, 0.0f, 1.0f};
	GLfloat blueColor[4] = {0.0f, 0.0f, 0.8f, 1.0f};
	GLfloat brownColor[4] = {0.55f, 0.27f, 0.0f, 0.08f};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	modelView.setIdentity();
	glUseProgram(shader);


	GLfloat lightDir[4] = {0.0f, 0.0f, 10.0f, 1.0f};

	glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMatrix"), 1, GL_TRUE, projection.get());

	modelView.translate(translWorld[0], translWorld[1], translWorld[2]);
	
	modelView.rotate(angWorldX, 1.0f, 0.0f, 0.0f);
	modelView.rotate(angWorldY, 0.0f, 1.0f, 0.0f);
	modelView.rotate(angWorldZ, 0.0f, 0.0f, 1.0f);
	

	glBindVertexArray(cubeVertexArray);
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewMatrix"), 1, GL_TRUE, modelView.get());
	glUniform4fv(glGetUniformLocation(shader, "inColor"), 1, greenColor);




	glBindVertexArray(floorVertexArray);

	// narysowanie danych zawartych w tablicy
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_INT, 0);



	modelViewStack.put(&modelView);
	drawRails(radius,0.05f,modelView);
	modelViewStack.pop(&modelView);


	actualSpeed+=actualAcceleration;
	if(!accelerate){
		if(actualAcceleration>0.0f)
			actualAcceleration-=0.0005f;
	}
	accelerate=false;


	modelViewStack.put(&modelView);
	glBindVertexArray(treeVertexArray);

	modelView.rotate(1.55f, 1.0f, 0.0f, 0.0f);

	modelView.translate(0.0f, 0.4f+railHeight, 0.0f);

	glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewMatrix"), 1, GL_TRUE, modelView.get());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 3*tree.nFaces, GL_UNSIGNED_INT, 0);


	modelView.translate(3.0f, 0.0f, 0.0f);

	glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewMatrix"), 1, GL_TRUE, modelView.get());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 3*tree.nFaces, GL_UNSIGNED_INT, 0);


	modelView.translate(-1.0f, 0.0f, -3.0f);
	modelView.rotate(1.0f, 0.0f, 1.0f, 0.0f);	

	glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewMatrix"), 1, GL_TRUE, modelView.get());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 3*tree.nFaces, GL_UNSIGNED_INT, 0);


	modelView.translate(-2.0f, 0.0f, -2.2f);
	modelView.rotate(1.0f, 0.0f, 1.0f, 0.0f);	

	glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewMatrix"), 1, GL_TRUE, modelView.get());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 3*tree.nFaces, GL_UNSIGNED_INT, 0);


	modelView.translate(-2.0f, 0.0f, 3.0f);
	modelView.rotate(2.0f, 0.0f, 1.0f, 0.0f);	

	glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewMatrix"), 1, GL_TRUE, modelView.get());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 3*tree.nFaces, GL_UNSIGNED_INT, 0);


	modelViewStack.pop(&modelView);


	glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewMatrix"), 1, GL_TRUE, modelView.get());

	modelView.rotate(actualSpeed, 0.0f, 0.0f, 1.0f);
	modelView.rotate(1.55f, 1.0f, 0.0f, 0.0f);


	modelView.translate(0.0f, 0.1f+railHeight, radius+widthBetween/2 - 0.02f);

	modelView.rotate(1.55f, 0.0f, 1.0f, 0.0f);


	glBindVertexArray(locomotiveVertexArray);
	
	glUniform4fv(glGetUniformLocation(shader, "inColor"), 1, blueColor);
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewMatrix"), 1, GL_TRUE, modelView.get());


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 3*locomotive.nFaces, GL_UNSIGNED_INT, 0);



	glBindVertexArray(trainVertexArray);
	modelView.translate(0.22f, 0.0f/*1.0f + 0.1f+railHeight*/, -0.95f /* radius+widthBetween/2 - 0.02f*/);
	modelView.rotate(1.10f, 0.0f, 1.0f, 0.0f);

	glUniform4fv(glGetUniformLocation(shader, "inColor"), 1, blueColor);
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewMatrix"), 1, GL_TRUE, modelView.get());


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 3*train.nFaces, GL_UNSIGNED_INT, 0);


	glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewMatrix"), 1, GL_TRUE, modelView.get());

	glUniform3fv(glGetUniformLocation(shader, "inLightDir"), 1, lightDir);
	static	GLfloat lightPosition[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	static GLfloat spotDirection[3] = {-2.0f, 0.0f, 0.0f};
	static GLfloat vec[3] = {modelView.get()[0] + projection.get()[0], modelView.get()[1] +  projection.get()[1], modelView.get()[2] + projection.get()[2]};

	glUniform3fv(glGetUniformLocation(shader, "inSpotDirectionOC"), 1, spotDirection);
	glUniform3fv(glGetUniformLocation(shader, "inLightPositionOC"), 1, lightPosition);


	glBindVertexArray(0);

	glUseProgram(0);

	glFlush();

	glutSwapBuffers();
}

void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state != GLUT_UP) {
			clickX=x;
			clickY=y;
		}
	}
}

void mouseMove(int x, int y) {

	if(clickX!=0){
		lastx=clickX;
		clickX=0;
	}
	if(clickY!=0){
		lasty=clickY;
		clickY=0;
	}
	lastx = (float)x - lastx;
    lasty = (float)y - lasty;

	if(ctrOn){
		translWorld[0] += lastx*mouseSpeed;
		translWorld[1] -= lasty*mouseSpeed;
	} else {
		if((float)x > lastx)
			angWorldZ+=(-lastx)*mouseSpeed;
		else
			angWorldZ+=(lastx*mouseSpeed);
		if((float)y > lasty)
			angWorldX+=(lasty*mouseSpeed);
		else 
			angWorldX+=(-lasty*mouseSpeed);
	}
    lastx = (float)x;
    lasty = (float)y;
}

void specialKbd (int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_UP: 
			accelerate=true;
			if(actualAcceleration<maxAcceleration)
				actualAcceleration+=0.001f;
			break;
		case GLUT_KEY_DOWN: 
			if(actualAcceleration>minAcceleration)
				actualAcceleration-=0.001f;
			break;
		case GLUT_KEY_CTRL_L:
			ctrOn=(!ctrOn);
			break;
	}
	glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y)
{
    if (dir > 0)
    {
       translWorld[2] += 0.1f;
    }
    else
    {
        translWorld[2] -= 0.1f;
    }

    return;
}

int main(int argc, char** argv)
{
	glutInitContextVersion(3, 1);
	glutInitContextFlags(GLUT_DEBUG);
	// profil CORE - pełna zgodność z v3.2
	glutInitContextProfile(GLUT_CORE_PROFILE);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Projekt \"ciuchcia\" - Marcin Bogusz, Karol Wojcik");

	// inicjalizacja biblioteki GLEW
	glewExperimental = GL_TRUE;
	GLenum glewErr = glewInit();
	// sprawdzenie poprawności inicjalizacji GLEWa
	if (GLEW_OK != glewErr) {
		cout << "Blad glewInit: " << glewGetErrorString(glewErr) << endl;
		return 2;
	}
	
	if ( init() ){
		return 3;
	}

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	
	glutSpecialFunc(specialKbd);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutMouseWheelFunc(mouseWheel);
	glutIdleFunc(display);

	glutMainLoop();

	return 0;
}
