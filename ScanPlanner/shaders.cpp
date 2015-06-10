#define STRINGIFY(A) #A

const char *vertexSource = 
"#version 330\n"
STRINGIFY(
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;


layout(location = 0) in vec3 position;
layout(location = 1) in float vertexColor;
out vec3 fragmentColor;


void main()
{	
	vec3 pos=position;

	gl_Position = projectionMatrix * modelViewMatrix * /*frame */ vec4(pos.x,pos.y,pos.z,1);
	
	fragmentColor = vec3(vertexColor,vertexColor,vertexColor);
}
);
/////////////////////////////////
const char *fragmentSource = 
"#version 330\n"
STRINGIFY(	

in vec3 fragmentColor;
           
out vec4 frag_color;

void main()
{
	frag_color =  vec4(fragmentColor,1.0);
	//frag_color =  vec4(FColor,1.0);
}
);
