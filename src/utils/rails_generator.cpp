#include <rail_generator.h>;
#include <math.h>;
#include <iostream>;

using namespace std;

const GLfloat pi = 3.14159265358979323846;

rail::rail(){
}

rail::rail(GLfloat radius, GLfloat step, GLfloat width, GLfloat height){
	GLfloat actualX=radius;
	GLfloat actualY=0.0f;
	GLfloat circumference = 2.0f*pi*radius;

	GLfloat secondActualX=radius+width;
	GLfloat secondActualY=0.0f;
	GLfloat secondCircumference = 2.0f*pi*(radius+width);

	int numberOfPoints = ceil(circumference/step)*4;

	nVertices = numberOfPoints+4;
	nNormals = numberOfPoints+4;
	nFaces = numberOfPoints*2+8;

	vertices = new vector3<GLfloat>[numberOfPoints+4];
	normals = new vector3<GLfloat>[numberOfPoints+4];
	faces = new vector3<GLint>[numberOfPoints*2+8];

	vertices[numberOfPoints] = vector3<GLfloat>(actualX,actualY,0.0f);
	vertices[numberOfPoints+1] = vector3<GLfloat>(actualX,actualY,height);
	vertices[numberOfPoints+2] = vector3<GLfloat>(secondActualX,secondActualY,0.0f);
	vertices[numberOfPoints+3] = vector3<GLfloat>(secondActualX,secondActualY,height);
	normals[numberOfPoints] = vector3<GLfloat>(actualX,actualY,-1.0f);
	normals[numberOfPoints+1] = vector3<GLfloat>(actualX,actualY,height);
	normals[numberOfPoints+2] = vector3<GLfloat>(secondActualX,secondActualY,1.0f);
	normals[numberOfPoints+3] = vector3<GLfloat>(secondActualX,secondActualY,height);


	for(int i=0,j=0;i<numberOfPoints;i+=4,j+=8){

		GLfloat angle = (step*360.0f)/(circumference);
		angle=angle*(pi/180.0f);

		GLfloat newX = actualX*cos(angle)-actualY*sin(angle);
		GLfloat newY = actualX*sin(angle)+actualY*cos(angle);
		actualX=newX;
		actualY=newY;
		//actualX=actualX;
		//actualY=actualY+step;
		vertices[i] = vector3<GLfloat>(actualX,actualY,0.0f);
		vertices[i+1] = vector3<GLfloat>(actualX,actualY,height);
		normals[i] = vector3<GLfloat>(actualX,actualY,-1.0f);
		normals[i+1] = vector3<GLfloat>(actualX,actualY,1.0f);

		GLfloat secondNewX = secondActualX*cos(angle)-secondActualY*sin(angle);
		GLfloat secondNewY = secondActualX*sin(angle)+secondActualY*cos(angle);
		secondActualX=secondNewX;
		secondActualY=secondNewY;
		//secondActualX=secondActualX;
		//secondActualY=secondActualY+step;
		vertices[i+2] = vector3<GLfloat>(secondActualX,secondActualY,0.0f);
		vertices[i+3] = vector3<GLfloat>(secondActualX,secondActualY,height);
		normals[i+2] = vector3<GLfloat>(secondActualX,secondActualY,-1.0f);
		normals[i+3] = vector3<GLfloat>(secondActualX,secondActualY,1.0f);


		

		
	}

	for(int i=0,j=0;i<numberOfPoints+4;i+=4,j+=8){
		int divBy = (numberOfPoints+4)/4;
		if(i<numberOfPoints){
			faces[j] = vector3<GLint>(i,i+1,i+4);
			faces[j+1] = vector3<GLint>(i+4,i+1,i+5);
			faces[j+2] = vector3<GLint>(i+1,i+3,i+5);
			faces[j+3] = vector3<GLint>(i+5,i+3,i+7);
			faces[j+4] = vector3<GLint>(i+3,i+2,i+7);
			faces[j+5] = vector3<GLint>(i+7,i+2,i+6);
			faces[j+6] = vector3<GLint>(i+2,i,i+6);
			faces[j+7] = vector3<GLint>(i+6,i,i+4);

		} else {
			faces[j] = vector3<GLint>(i,i+1,(i+4)%divBy);
			faces[j+1] = vector3<GLint>((i+4)%divBy,i+1,(i+5)%divBy);
			faces[j+2] = vector3<GLint>(i+1,i+3,(i+5)%divBy);
			faces[j+3] = vector3<GLint>((i+5)%divBy,i+3,(i+7)%divBy);
			faces[j+4] = vector3<GLint>(i+3,i+2,(i+7)%divBy);
			faces[j+5] = vector3<GLint>((i+7)%divBy,i+2,(i+6)%divBy);
			faces[j+6] = vector3<GLint>(i+2,i,(i+6)%divBy);
			faces[j+7] = vector3<GLint>((i+6)%divBy,i,(i+4)%divBy);
		}
	}

}

bar::bar(void){
}

bar::bar(GLfloat width, GLfloat height, GLfloat length, GLfloat initialX){
	vertices = new vector3<GLfloat>[8];
	normals = new vector3<GLfloat>[8];
	faces = new vector3<GLint>[2*6];

	vertices[0]=vector3<GLfloat>(initialX, 0.0f,  -width/2);
	vertices[1]=vector3<GLfloat>(initialX, 0.0f,  width/2);
	vertices[2]=vector3<GLfloat>(initialX,  height,  -width/2);
	vertices[3]=vector3<GLfloat>(initialX,  height,  width/2);
	vertices[4]=vector3<GLfloat>(initialX+length, 0.0f,  -width/2);
	vertices[5]=vector3<GLfloat>(initialX+length, 0.0f,  width/2);
	vertices[6]=vector3<GLfloat>(initialX+length,  height,  -width/2);
	vertices[7]=vector3<GLfloat>(initialX+length,  height,  width/2);

	normals[0]=vector3<GLfloat>(-1.0f, -1.0f,  1.0f);
	normals[1]=vector3<GLfloat>(1.0f, -1.0f,  1.0f);
	normals[2]=vector3<GLfloat>(-1.0f,  1.0f,  1.0f);
	normals[3]=vector3<GLfloat>(1.0f,  1.0f,  1.0f);
	normals[4]=vector3<GLfloat>(-1.0f, -1.0f, -1.0f);
	normals[5]=vector3<GLfloat>(1.0f, -1.0f, -1.0f);
	normals[6]=vector3<GLfloat>(-1.0f,  1.0f, -1.0f);
	normals[7]=vector3<GLfloat>(1.0f,  1.0f, -1.0f);

	faces[0]=vector3<GLint>(0, 1, 2);
	faces[1]=vector3<GLint>(2, 1, 3);
	faces[2]=vector3<GLint>(1, 5, 3);
	faces[3]=vector3<GLint>(3, 5, 7);
	faces[4]=vector3<GLint>(5, 4, 7);
	faces[5]=vector3<GLint>(7, 4, 6);
	faces[6]=vector3<GLint>(4, 0, 6);
	faces[7]=vector3<GLint>(6, 0, 2);
	faces[8]=vector3<GLint>(1, 0, 5);
	faces[9]=vector3<GLint>(5, 0, 4);
	faces[10]=vector3<GLint>(3, 7, 2);
	faces[11]=vector3<GLint>(2, 7, 6);

	nVertices=8;
	nNormals=8;
	nFaces=2*6;
}


void generateRails(rail &r1, rail &r2,GLfloat radius, GLfloat step, GLfloat width, GLfloat height, GLfloat widthBetween, bar &b){
	r1= rail(radius,step,width,height);
	r2= rail(radius+widthBetween,step,width,height);
	b = bar(width,height,widthBetween+4*width,radius-2*width);
}