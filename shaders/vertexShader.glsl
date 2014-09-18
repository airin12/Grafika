// OpenGL 3.1
#version 140

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

uniform vec3 inLightDir;
uniform vec4 inColor;

in vec3 inVertex;
in vec3 inNormal;

uniform vec4 inLightPositionOC;
uniform vec3 inSpotDirectionOC;

out vec4 lightPositionOC;
out vec3 spotDirectionOC;

out vec3 normal;
out vec3 lightDir;
out vec4 color;

out vec4 gl_Position;

out vec4 vertexO;

out mat4 ModelViewMatrix;

out mat4 ProjectionMatrix;

out vec3 eyeVec;

void main(void)
{
	vec4 vertex = vec4(inVertex,1.0);
	normal = normalize((modelViewMatrix * vec4(inNormal, 0.0)).xyz);

	lightDir = inLightDir;
	color = inColor;
	gl_Position = projectionMatrix * modelViewMatrix * vertex;
	ModelViewMatrix = modelViewMatrix;
	ProjectionMatrix = projectionMatrix;
	vertexO = vertex;
	lightPositionOC = inLightPositionOC;
	spotDirectionOC = inSpotDirectionOC;
	eyeVec = vec3(gl_Position);
}
