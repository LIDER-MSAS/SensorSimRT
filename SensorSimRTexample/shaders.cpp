#define STRINGIFY(A) #A

const char *vertexSource = 
"#version 330\n"
STRINGIFY(
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 viewportMatrix;


in vec3 position;

out vec3 FColor;

uniform vec3 translate;

uniform vec4 lightPos;
uniform mat4 lightmodel;
uniform float yawLight;

uniform float yaw;
uniform float pitch;

uniform float scale;

void main()
{	
	vec3 pos=position;

	gl_Position = projectionMatrix * modelViewMatrix * /*frame */ vec4(pos.x,pos.y,pos.z,1);
}
);
/////////////////////////////////
const char *fragmentSource = 

"#version 330\n"
STRINGIFY(	

uniform vec3 color3f;

uniform vec3 FColor;
/*in vec3 FColor;*/
           
out vec4 frag_color;

void main()
{
	frag_color =  vec4(FColor,1.0);
}
);
