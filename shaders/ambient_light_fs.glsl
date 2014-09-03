// OpenGL 3.1
#version 140

// zmienne wejściowe - zwrócone przez shader wierzhołków
in vec4 color;

// zmienna typu out z tego shadera jest ustalonym kolorem fragmentu
out vec4 fragColor;

void main(void)
{
	//ustawienie koloru fragmentu
	fragColor = color;
}

