#version 330

// TODO: get vertex attributes from each location

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

//prin pipe-urile astea se face legarea intre argumente si atribute :)
layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec3 v_text_coord;
layout(location = 1) in vec3 v_color;

uniform vec3 object_color;



// TODO: output values to fragment shader

//se seteaza atributul de iesire catre fragment shader
out vec3 frag_color;


void main()
{
	// TODO: send output to fragment shader
	frag_color = object_color;
	// TODO: compute gl_Position
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);

}
