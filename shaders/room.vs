#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPos_MS;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec3 vNorm_MS;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 pos_WS;
out vec3 norm_CS;
out vec3 eyeDir_CS;
out vec3 lightDir_CS;

// Values that stay constant for the whole mesh.
uniform mat4 camView;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightPos_WS;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  camView * vec4(vPos_MS,1);

	// Position of the vertex, in worldspace : M * position
	pos_WS = (M * vec4(vPos_MS,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vPos_CS = ( V * M * vec4(vPos_MS,1)).xyz;
	eyeDir_CS = vec3(0,0,0) - vPos_CS;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 lightPos_CS = ( V * vec4(lightPos_WS,1)).xyz;
	lightDir_CS = lightPos_CS + eyeDir_CS;
	
	// Normal of the the vertex, in camera space
	norm_CS = ( V * M * vec4(vNorm_MS,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.	

	// UV of the vertex. No special space for this one.
	UV = vUV;
}

