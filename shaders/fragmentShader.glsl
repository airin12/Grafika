// OpenGL 3.1
#version 140

in vec3 normal;
in vec3 lightDir;
in vec4 color;
in vec4 gl_Position;
in vec4 vertexO; 

in mat4 ModelViewMatrix;

in mat4 ProjectionMatrix;

out vec4 fragColor;


in vec4 lightPositionOC ;
in vec3 spotDirectionOC ;
uniform float spotCutoff = 20.0f;


void main(void)
{
   vec3 lightPosition;
   vec3 spotDirection;
   vec3 lightDirection;
   float angle;
   //vec4 vertex = vertexO;

   //vec4 lightPositionOC = vec4(0.0f, 0.0f, 1.0f, 1.0f); 
   //vec3 spotDirectionOC = vec3(0.0f, 0.0f, 0.0f);


    //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	//gl_Position = ProjectionMatrix * vertex;

	
	lightPosition  =  (ProjectionMatrix *  ModelViewMatrix * vec4( -0.5f, -1.9f, -0.51f, 1.0f)).xyz;
    spotDirection  = normalize(spotDirectionOC * gl_NormalMatrix);

    vec4 vertex =  ProjectionMatrix * ModelViewMatrix *  vertexO;
    lightDirection = /*vec3(0.0f,0.5f,0.1f);*/       normalize(vertex.xyz - lightPosition.xyz);

    angle = dot( normalize(spotDirection),
                -normalize(lightDirection));
    angle = max(angle,0);   

  if(acos(angle) < radians(spotCutoff)){

    	//float diffuseIntensity = max(dot(color, light), 0.0);
		//vec4 diffuse = vec4((color * diffuseIntensity).xyz, 1.0);
	
	   fragColor = vec4(1,1,0,1)  * color * ( ( radians(spotCutoff)) / acos(angle) );
	}else{

		vec3 light = ( ProjectionMatrix * ModelViewMatrix * vec4( normalize(lightDir), 1.0) ).xyz;
		vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
	
		float diffuseIntensity = max(dot(normal, light), 0.0);
		vec4 diffuse = vec4((color * diffuseIntensity).xyz, 1.0);
		fragColor = ambient + diffuse;
		
		//fragColor = color * 0.5f ;// vec4(0,0,0,1);    
	}
}

