#version 330

// TODO: get vertex attributes from each location

//inputul e de 2 tipuri 
//1 - cel pt Vertex

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_text_coord;
layout(location = 3) in vec3 v_color;

//var globale

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float ElapsedTime;

//se declara atributul de iesire catre FragmentShader :)
out vec3 frag_color;

// TODO: output values to fragment shader

void main()
{
	// TODO: send output to fragment shader
	//transmitem o culoare de la vertex sh la frg shdr
	frag_color = v_color;

	// TODO: compute gl_Position
	//vertex shader tb sa dea o poz in spatiul canonic
	//ca dintr-o var ob sa o ducem in sp.canonic tb sa aplicam toate cele 3 transf din sp ob -> sp.cubului canonic
	gl_Position = Projection * View * ( Model * (vec4(v_position ,1) + vec4(ElapsedTime,0,0,0)));

	//a aparut in vec4 1 -> am un vec4 !
}
