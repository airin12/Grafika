// dołączenie plików nagłówkowych
#include <shaders.h>
#include <vecmatrix.h>
#include <vectors.h>
#include <obj.h>

// Załadowanie pliku nagłówkowego biblioteki GLEW - znajdują się w nim wszystkie
// rozszerzenia biblioteki OpenGL również dla wersji wyższych niż 3.0
// Nie ma więc potrzeby wykorzysytwania pliku nagłówkowego gl3.h
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>

using namespace std;

// ustawienia parametrów perspektywy
GLfloat prScale = 1.0, prNear = 0.1f, prFar = 45.0f;

// identyfikator obiektu programu
GLuint simplyShader, ambientShader;

// identyfikatory obiektów tablic wierchołków dla .obj i sześcianu
GLuint cubeVertexArray, objVertexArray, floorVertexArray;

// obiekty reprezentujące odpowiednio macierz widoku modelu i projekcji (rzutowania)
matrix modelView(4);
matrix projection(4);

// początkowe wartości wektora przesunięcia
GLfloat translWorld[3] = {0.0f, 0.0f, -2.0f};
// początkowe wartości kątów obrotów sześcianu wokół odpowiednich osi
GLfloat angX = 0.0f, angY = 0.0f, angZ = 0.0f;
// początkowe wartości kątów obrotów świata (sceny) wokół odpowiednich osi
GLfloat angWorldX = 0.0f, angWorldY = 0.0f, angWorldZ = 0.0f;

matrixStack modelViewStack;

// obiekt reprezentujący bryłę wczytaną jako .obj
objShape obj;

//=============================================================================
// utworzenie i wypełnienie danymi tablicy wierzchołków
//=============================================================================
void createVertexArray(GLuint vertexArray,
	GLfloat *vertices, size_t verticesSize,
	GLfloat *normals, size_t normalsSize,
	GLuint *indices, size_t indicesSize)
{
	// identyfikator bufora danych
	GLuint dataBuffer;

	// ustawienie jako aktywnej macierzy wierzchołków, dla której wywołano funkcję
	glBindVertexArray(vertexArray);

	// utworzenie obiektu bufora wierzchołków (VBO) i załadowanie danych
	//współrzędne
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

	// włączenie obiektów buforów wierchołków
	glEnableVertexAttribArray(0); // wierzchołki
	glEnableVertexAttribArray(1); // normalne
}

//=============================================================================
// inicjalizacja stałych elementów maszyny stanu OpenGL
//=============================================================================
int init(char *objFileName)
{
//   definicja sześchar *objFileNamecianu:
// wierzchołki
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
	-1.5f, -1.5f,  0.0f,
	 1.5f, -1.5f,  0.0f,
	-1.5f,  1.5f,  0.0f,
	 1.5f,  1.5f,  0.0f
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

	int objError;

	// wczytanie obiektu z pliku .obj i przygotowanie go
	if ( (objError = obj.readFromFile(objFileName)) )
		return objError;
	obj.writeProps();
	// przeskalowanie wczytanego obj, tak aby był wpisany w jednostkowy sześcian
	// o środku w początku układu współrzędnych
	obj.scale();

	// sprawdzenie czy zostały poprawnie zdefiniowane normalne
	if (!obj.nNormals)
		// wygenerowanie uśrednionych normalnych
		obj.genSmoothNormals();
		// wygenerowanie normalnych dla ścianek
		//obj.genFacesNormals();
	else
		if (!obj.normIndGood)
			// gdy indeksy normalnych nie zgadzają się z indeksami wierzhołków
			// należy przebudować obie tablice, aby były tak samo indeksowane
			// przbudowanie indeksów normalnych i jeśli trzeba indeksów wierchołków
			obj.rebuildAttribTable('n');

	cout << "===================================================" << endl;
	// wypisanie właściwości wczytanego obiektu
	obj.writeProps();
//	obj.writeTables();


	// pobranie danych ze sterownika
	cout << "===================================================" << endl;
	cout << "Producent: " << glGetString(GL_VENDOR) << endl;
	cout << "Karta graficzna: " << glGetString(GL_RENDERER) << endl;
	cout << "Wersja OpenGL: " << glGetString(GL_VERSION) << endl;
	cout << "Wersja GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	cout << "===================================================" << endl;

	// ustawienie koloru tłatranslation.cpp
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// wczytanie shaderów i przygotowanie obsługi programu
	AttachVertexShader(simplyShader, "../shaders/simply_light_vs.glsl");
	AttachFragmentShader(simplyShader, "../shaders/simply_light_fs.glsl");
	// wykonanie powiązania pomiędzy zmienną a indeksem ogólnych atrybutów wierzchołka
	// operacja ta nie jest konieczna i będzie wykonana automatycznie w czasie
	// konsolidacji shadera - przypisany zostanie "pierwszy wolny" indeks
	LinkProgram(simplyShader);

	// wczytanie shaderów i przygotowanie obsługi programu
	AttachVertexShader(ambientShader, "../shaders/ambient_light_vs.glsl");
	AttachFragmentShader(ambientShader, "../shaders/ambient_light_fs.glsl");
	// wykonanie powiązania pomiędzy zmienną a indeksem ogólnych atrybutów wierzchołka
	// operacja ta nie jest konieczna i będzie wykonana automatycznie w czasie
	// konsolidacji shadera - przypisany zostanie "pierwszy wolny" indeks
	LinkProgram(ambientShader);

	// wygenerowanie i włączenie tablicy wierzchołków sześcianu
	glGenVertexArrays(1, &cubeVertexArray);
	createVertexArray(cubeVertexArray,
		cubeVertices, sizeof(cubeVertices),
		cubeNormals, sizeof(cubeNormals),
		cubeIndices, sizeof(cubeIndices));

	// wygenerowanie i włączenie tablicy wierzchołków .obj
	glGenVertexArrays(1, &objVertexArray);
	createVertexArray(objVertexArray,
		(GLfloat*)obj.vertices, 3*obj.nAttribs*sizeof(GLfloat),
		(GLfloat*)obj.normals, 3*obj.nAttribs*sizeof(GLfloat),
		(GLuint*)obj.faces, 3*obj.nFaces*sizeof(GLuint));

	// wygenerowanie i włączenie tablicy wierzchołków podlogi
	glGenVertexArrays(1, &floorVertexArray);
	createVertexArray(floorVertexArray,
		floorVertices, sizeof(floorVertices),
		floorNormals, sizeof(floorNormals),
		floorIndices, sizeof(floorIndices));

	// włączenie wykorzystania bufora głębokości
	glEnable(GL_DEPTH_TEST);

	// ustawienie sposobu rysowania odpowiednich stron ścian
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	// właczenie pominięcia renderowania niewidocznych ścian
	glEnable(GL_CULL_FACE);

	return 0;
}

//=============================================================================
// zmiana rozmiaru okna
//=============================================================================
void reshape(int width, int height)
{
	// ustawienie obszaru renderingu - całe okno
	glViewport(0, 0, width, height);

	// załadowanie macierzy jednostkowej do macierzy projekcji
	projection.setIdentity();
	// ustawienie perspektywy (przemnożenie przez macierz rzutowania)
	projection.perspective(prScale, prNear, prFar);
}

//=============================================================================
// wyświetlenie sceny
//=============================================================================
void display(void)
{
	GLfloat redColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat greenColor[4] = {0.0f, 1.0f, 0.0f, 1.0f};
	GLfloat blueColor[4] = {0.0f, 0.0f, 1.0f, 1.0f};
	GLfloat yellowColor[4] = {1.0f, 1.0f, 0.0f, 1.0f};
	GLfloat cyanColor[4] = {0.0f, 1.0f, 1.0f, 1.0f};
	GLfloat magentaColor[4] = {1.0f, 0.0f, 1.0f, 1.0f};

	// czyszczenie bufora koloru
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// załadowanie macierzy jednostkowej do macierzy widoku modelu
	modelView.setIdentity();

	// użycie obiektu shadera
	glUseProgram(simplyShader);


	// kierunek źródła światła
	GLfloat lightDir[4] = {4.0f, 6.0f, 3.0f, 1.0f};

	// załadowanie do shadera wektora położenia światła (w aktualnym układzie współrzędnych)
	glUniform3fv(glGetUniformLocation(simplyShader, "inLightDir"), 1, lightDir);

	
	// załadownanie do shadera bieżącego stanu macierzy projekcji - wystarczy raz, bo jest tu niezmienna
	glUniformMatrix4fv(glGetUniformLocation(simplyShader, "projectionMatrix"), 1, GL_TRUE, projection.get());
	// UWAGA: w języku GLSL macierze zapisywane są kolumnami, a nie wierszami; dlatego też
	// wpierw macierz musi zostać transponowana lub zostawiamy tę operację shaderowi
	// wymuszając ją poprzez ustawienie trzeciego argumentu na GL_TRUE

	// wykonanie przekształceń geometrycznych - przemnażanie bieżącej macierzy
	// widoku modelu przez odpowiednią macierz przekształcenia
	// Wpierw wykonywane są przekształcenia całego świata (obserwatora)
	modelView.rotate(angWorldX, 1.0f, 0.0f, 0.0f);
	modelView.rotate(angWorldY, 0.0f, 1.0f, 0.0f);
	modelView.rotate(angWorldZ, 0.0f, 0.0f, 1.0f);
	modelView.translate(translWorld[0], translWorld[1], translWorld[2]);

	modelViewStack.put(&modelView);
	modelView.rotate(angX, 1.0f, 0.0f, 0.0f);
	modelView.rotate(angY, 0.0f, 1.0f, 0.0f);
	modelView.rotate(angZ, 0.0f, 0.0f, 1.0f);

		// włączenie tablicy wierzchołków sześcianu
	glBindVertexArray(cubeVertexArray);
	// załadownanie do shadera bieżącego stanu macierzy widoku modelu
	glUniformMatrix4fv(glGetUniformLocation(simplyShader, "modelViewMatrix"), 1, GL_TRUE, modelView.get());
	// załadowanie do shadera wektora koloru obiektu
	glUniform4fv(glGetUniformLocation(simplyShader, "inColor"), 1, blueColor);




	glBindVertexArray(floorVertexArray);

	// narysowanie danych zawartych w tablicy
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_INT, 0);

	modelView.translate(0.0f, 0.0f, 1.0f);
	modelViewStack.pop(&modelView);
	modelViewStack.put(&modelView);
	modelView.rotate(angX, 1.0f, 0.0f, 0.0f);
	modelView.rotate(angY, 0.0f, 1.0f, 0.0f);
	modelView.rotate(angZ, 0.0f, 0.0f, 1.0f);
		// załadownanie do shadera bieżącego stanu macierzy widoku modelu
	glUniformMatrix4fv(glGetUniformLocation(simplyShader, "modelViewMatrix"), 1, GL_TRUE, modelView.get());
	// załadowanie do shadera wektora koloru obiektu
	glUniform4fv(glGetUniformLocation(simplyShader, "inColor"), 1, yellowColor);

	// włączenie tablicy wierzchołków .obj
	glBindVertexArray(objVertexArray);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// narysowanie danych zawartych w tablicy wierzchołków .obj
	glDrawElements(GL_TRIANGLES, 3*obj.nFaces, GL_UNSIGNED_INT, 0);

	
	// pobranie macierzy widoku modelu ze stosu
	modelViewStack.pop(&modelView);

	// wyłączenie tablic wierzhołków
	glBindVertexArray(0);

	// wyłączenie shadera
	glUseProgram(0);

	// wyrenderowanie sceny
	glFlush();

	glutSwapBuffers();
}

//=============================================================================
// obsługa klawiatury - klawisze standardowe
//=============================================================================
void standardKbd(unsigned char key, int x, int y)
{
	GLint viewport[4]; // aktualne parametry okna

	// pobranie własności bieżącego okna - współrzędne x,y okna oraz jego
	// szerokość i wysokość    
	glGetIntegerv(GL_VIEWPORT,viewport);

	// obsługa standardowych klawiszy
	switch (key) {
		// zdefiniowanie współczynnika skalowania dla perspectywy
		case 'a': prScale += 0.1f;
			reshape(viewport[2], viewport[3]);
			break;
		case 'A': prScale -= 0.1f;
			reshape(viewport[2], viewport[3]);
			break;
		// zdefiniowanie obrotów całej sceny (świata) wokół osi x, y, z
		case 'j': angWorldX += 0.1f;
			break;
		case 'J': angWorldX -= 0.1f;
			break;
		case 'k': angWorldY += 0.1f;
			break;
		case 'K': angWorldY -= 0.1f;
			break;
		case 'l': angWorldZ += 0.1f;
			break;
		case 'L': angWorldZ -= 0.1f;
			break;
		// zdefiniowanie obrotów sześcianu wokół osi x, y, z
		case 'x': angX += 0.1f;
			break;
		case 'X': angX -= 0.1f;
			break;
		case 'y': angY += 0.1f;
			break;
		case 'Y': angY -= 0.1f;
			break;
		case 'z': angZ += 0.1f;
			break;
		case 'Z': angZ -= 0.1f;
			break;
		case 27: exit(0);
	}
	// wymuszenie odrysowania okna
	// (wywołanie zarejestrowanej funcji do obsługi tego zdarzenia)
	glutPostRedisplay();
}

//=============================================================================
// obsługa klawiatury - klawisze specjalne
//=============================================================================
void specialKbd (int key, int x, int y)
{
	// obsługa klawiszy funkcyjnych - analogicznie jak podstawowych
	switch (key) {
		case GLUT_KEY_RIGHT: translWorld[0] += 0.1f;
			break;
		case GLUT_KEY_LEFT: translWorld[0] -= 0.1f;
			break;
		case GLUT_KEY_UP: translWorld[1] += 0.1f;
			break;
		case GLUT_KEY_DOWN: translWorld[1] -= 0.1f;
			break;
		case GLUT_KEY_PAGE_UP: translWorld[2] += 0.1f;
			break;
		case GLUT_KEY_PAGE_DOWN: translWorld[2] -= 0.1f;
			break;
	}
	glutPostRedisplay();
}

//=============================================================================
// główna funkcja programu
//=============================================================================
int main(int argc, char** argv)
{
	if (argc != 2) {
		cout << "usage:" << endl;
		cout << "   " << argv[0] << " <obj file>" << endl;
		return 1;
	}

	// ustalenie odpowiedniego kontekstu renderowania
	glutInitContextVersion(3, 1);
	glutInitContextFlags(GLUT_DEBUG);
	// określenie wykorzystywanego profilu - profil CORE pełna zgodność z v3.2
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);
	// określenie trybu pracy biblioteki - kolor w formacie RGB
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	// rozmiar tworzonego okna (w pikselach)
	glutInitWindowSize(600, 600);
	// położenie okna na ekranie (względem lewego dolnego rogu)
	glutInitWindowPosition(100, 100);
	// stworzenie okna programu
	glutCreateWindow("03 - oświetlone kostki ruchomym światłem");

	// inicjalizacja biblioteki GLEW
	glewExperimental = GL_TRUE;
	GLenum glewErr = glewInit();
	// sprawdzenie poprawności inicjalizacji GLEWa
	if (GLEW_OK != glewErr) {
		// nieudana inicjalizacja biblioteki
		cout << "Blad glewInit: " << glewGetErrorString(glewErr) << endl;
		return 2;
	}
	cout << "Wersja biblioteki GLEW: " << glewGetString(GLEW_VERSION) << endl;

	// wykonanie czynności przygotowawczych programu
	if ( init(argv[1]) )
		return 3;

	// ======================   funkcje callback ==================================
	// funkcja obsługująca zdarzenie konieczności odrysowania okna
	glutDisplayFunc(display);
	// funkcja obsługująca zdarzenie związane ze zmianą rozmiaru okna
	glutReshapeFunc(reshape);
	// funkcja obsługująca naciśnięcie standardowego klawisza z klawiatury
	glutKeyboardFunc(standardKbd);
	// funkcja obsługująca naciśnięcie klawisza specjalnego z klawiatury
	glutSpecialFunc(specialKbd);
	glutIdleFunc(display);
	//=============================================================================
	// główna pętla programu
	glutMainLoop();

	return 0;
}
