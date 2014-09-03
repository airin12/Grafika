// OpenGL 3.1
#version 140

// zmienne wejściowe - zwrócone przez shader wierzhołków
in vec3 normal;
in vec3 lightDir;
in vec4 color;

// zmienna typu out z tego shadera jest ustalonym kolorem fragmentu
out vec4 fragColor;

void main(void)
{
	// normalizacja wektora kierunku światła
	vec3 light = normalize(lightDir);

	// ustalenie koloru "ambient"
	vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

	// Obliczenie współczynnika intensywności oświetlenia, zależnego
	// jedynie od orientacji fragmentu względem kierunku światła.
	// Iloczyn skalarny pomiędzy wektorem normalnym, a wektorem kierunku światła
	float diffuseIntensity = max(dot(normal, light), 0.0);
	// ustalenie koloru fragmentu
	vec4 diffuse = vec4((color * diffuseIntensity).xyz, 1.0);
	// złożenie koloru fragmentu z obydwu składowych
	fragColor = ambient + diffuse;
}

