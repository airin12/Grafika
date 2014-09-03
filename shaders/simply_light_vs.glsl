// OpenGL 3.1
#version 140

// zmienne typu "uniform" - te same dla wszystkich instancji shadera
// iloczyn maciezy Widoku Modelu oraz Projekcji
uniform mat4 projectionMatrix;
// macierz Widoku Modelu
uniform mat4 modelViewMatrix;

// kierunek źródła światła
uniform vec3 inLightDir;
// kolor obiektu
uniform vec4 inColor;

// zmienne typu "in" - wejściowe, różne dla każdego wierzchołka
// współrzędne wierzchołka
in vec3 inVertex;
// współrzędne wektora normalnego w wierzchołku
in vec3 inNormal;

// zmienne typu "out" - wyjściowe zmienne przekazywane do shadera fragmentów
// wektor normalny we współrzędnych obserwatora
out vec3 normal;
// wektork kierunku światła we współrzędnych obserwatora
out vec3 lightDir;
// kolor obiektu - wyjściowy
out vec4 color;

void main(void)
{
	// przekształcenie trójelementowego wektora współrzędnych do czteroelemntowego -
	// jest to potrzebne do przemnożenia przez niego macierzy 4x4
	vec4 vertex = vec4(inVertex,1.0);

	// Analogiczne przekształcenie trójelementowego wektora normalnej,
	// do czteroelementowego wektora i przemnożenie przez niego macierzy Modelu Widoku
	// Jest to przekształcenie wektora normalnego do układu obserwatora.
	// Innymi słowy poddanie go wszystkim przekształceniom geometrycznym
	normal = normalize((modelViewMatrix * vec4(inNormal, 0.0)).xyz);

	//Przepisanie zmiennej wejściowej na wyjściową, aby przekazać ją do kolejnego shadera.
	lightDir = inLightDir;
	color = inColor;

	// Obliczenie współrzędnych wierzchołka - przemnożenie go przez iloczyn obu macierzy
	// oraz ustawienie wartości specjalnej zmiennej o nazwie "gl_Position"
	// Czyli przejście do układu obserwatora (przemnożenie przez macierz Widoku Modelu),
	// a następnie poddanie go przekształceniom zawartym w macierzy Projekcji - u nas perspektywie.
	gl_Position = projectionMatrix * modelViewMatrix * vertex;
}
