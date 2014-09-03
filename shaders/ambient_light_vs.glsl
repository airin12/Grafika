// OpenGL 3.1
#version 140

// zmienne typu "uniform" - te same dla wszystkich instancji shadera
// iloczyn maciezy Widoku Modelu oraz Projekcji
uniform mat4 projectionMatrix;
// macierz Widoku Modelu
uniform mat4 modelViewMatrix;

// kolor obiektu
uniform vec4 inColor;

// zmienne typu "in" - wejściowe, różne dla każdego wierzchołka
// współrzędne wierzchołka
in vec3 inVertex;

// zmienne typu "out" - wyjściowe zmienne przekazywane do shadera fragmentów
// kolor obiektu - wyjściowy
out vec4 color;

void main(void)
{
	// przekształcenie trójelementowego wektora współrzędnych do czteroelemntowego -
	// jest to potrzebne do przemnożenia przez niego macierzy 4x4
	vec4 vertex = vec4(inVertex,1.0);

	//Przepisanie zmiennej wejściowej na wyjściową, aby przekazać ją do kolejnego shadera.
	color = inColor;

	// Obliczenie współrzędnych wierzchołka - przemnożenie go przez iloczyn obu macierzy
	// oraz ustawienie wartości specjalnej zmiennej o nazwie "gl_Position"
	// Czyli przejście do układu obserwatora (przemnożenie przez macierz Widoku Modelu),
	// a następnie poddanie go przekształceniom zawartym w macierzy Projekcji - u nas perspektywie.
	gl_Position = projectionMatrix * modelViewMatrix * vertex;
}
