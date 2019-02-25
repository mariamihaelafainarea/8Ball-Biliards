#version 330

// TODO: get values from fragment shader
in vec3 frag_normal;
//ce primeste de la vertex shader
in vec3 frag_color;

//pt fragment shader facem si o parte de output care e culoarea
layout(location = 0) out vec4 color;


void main()
{
	// TODO: write pixel out color
	color = vec4(frag_color,1);


}