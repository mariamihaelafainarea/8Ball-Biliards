#include "Tema2.h"

#include "Ractangle.h"
#include "Border.h"
#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::statistici() {
	cout << "----------------Statistici------------------\n";
	if (players[turn].color == glm::vec3(1, 0, 0)) {
		cout << "Playerul " << turn << " a bagat " << players[turn].numberOfBalls << " de culoare rosie si a comis " <<
			players[turn].faults << " fault-uri\n";
	}
	else if (players[turn].color == glm::vec3(1, 1, 0)) {
		cout << "Playerul " << turn << " a bagat " << players[turn].numberOfBalls << " de culoare galbena si a comis " <<
			players[turn].faults << " fault-uri\n";
	}
	if (players[1-turn].color == glm::vec3(1, 0, 0)) {
		cout << "Playerul " << 1- turn << " a bagat " << players[1-turn].numberOfBalls << " de culoare rosie si a comis " <<
			players[1 - turn].faults << " fault-uri\n";
	}
	else if(players[1 - turn].color == glm::vec3(1, 1, 0)) {
		cout << "Playerul " << 1-turn << " a bagat " << players[1-turn].numberOfBalls << " de culoare galbena si a comis " <<
			players[ 1 - turn].faults << " fault-uri\n";
	}
	cout << "Este randul lui " << turn << ". ";
	if (players[turn].color == glm::vec3(1, 0, 0)) {
		cout << "El trebuie sa bage bile de culoare rosie\n";
	}
	else if (players[turn].color == glm::vec3(1, 1, 0)) {
		cout << "El trebuie sa bage bile de culoare galbena\n";
	}else {
	cout << "El poate baga orice culoare.\n Inca nu s-au impartit culorile\n";
	}
	cout << "\n";
}
ajutor Tema2::collisionWithBalls(Bile first,Bile second, float firstRadix, float seconRadix) {
	//Calculez vitezele dupa momentul ciocnirii
	vec3 n = vec3(first.x - second.x,0,first.z - second.z);
	n = normalize(n);
	float a1 = glm::dot(glm::vec3(first.vellocityX * first.speed , 0, first.vellocityZ  * first.speed ), n);
	float a2 = glm::dot(glm::vec3(second.vellocityX  * second.speed , 0, second.vellocityZ  * second.speed), n);

	float optimizedP = (a1 - a2);

	vec3 firstV = vec3(first.vellocityX  * first.speed , 0, first.vellocityZ*  first.speed );
	vec3 secondV = vec3(second.vellocityX* second.speed , 0, second.vellocityZ * second.speed );
	vec3 v1prim = firstV - optimizedP * n;

	vec3 v2prim = secondV + optimizedP * n;

	ajutor a;
	a.first = v1prim;
	a.second = v2prim;
	return a;
	
}

bool Tema2::collision(Bile* first, Bile* second, float firstRadix, float seconRadix) {
	//Verific daca cele doua bile au colizionat
	//Modificarile *2 si /10 se datoreaza scalarilor pe care le-am facut mai inainte
	//la randarea elementelor.
	//Velocitatile si distantele trebuie sa fie in aceleasi proportii cu dimensiunile bilei
	//Altfel, coliziunea nu e detectata cum trebuie. Iar diferentele sunt vizibile
	
	double dist =  glm::distance(glm::vec3(second->x*2 ,0,second->z*2 ),glm::vec3(first->x *2,0,first->z*2)) ;

	double sumRadii = firstRadix + seconRadix;

	dist -= sumRadii;	

	glm::vec3 moveVec = glm::vec3(first->vellocityX/10 * first->speed  - second->vellocityX /10 * second->speed ,
									0,
								first->vellocityZ /10 * first->speed -second->vellocityZ /10 * second->speed);

	double moveVecMagnitude = sqrt(first->vellocityX /10 * first->speed * first->vellocityX/10 * first->speed +
		first->vellocityZ/10 * first->speed * first->vellocityZ/10 * first->speed ) ;


	if (moveVecMagnitude <  dist  ) {
		return false;
	}

	glm::vec3 n = moveVec;
	n = normalize(n);
	glm::vec3 c;
	c = glm::vec3(glm::vec3(second->x* 2,0, second->z * 2) - glm::vec3(first->x* 2, 0 , first->z*2));

	double d = glm::dot(n, c);
	if (d <= 0) {

		return false;
	}

	double lengthC = sqrt(c.x * c.x + c.z * c.z);
	double f = (lengthC * lengthC) - (d*d);

	double sumRadiiSquared = sumRadii * sumRadii;

	if (f >= sumRadiiSquared) {

		return false;
	}

	double T = sumRadiiSquared - f;

	if (T < 0) {

		return false;
	} 

	double distance = d - sqrt(T);

	if (moveVecMagnitude < distance) {

		return false;
	}
	return true;

}


bool Tema2::collisionWithHoles(glm::vec3 first, glm::vec3 second, float firstRadix, float seconRadix) {
	//Coliziunea cu gaurile se face ca intre doua cercuri obisnuite
	float dist = sqrt((first.x - second.x) * (first.x - second.x) +
		(first.z - second.z) * (first.z - second.z)
	);
	//desi am pus firstRadix + secondRadix , in utilizare
	//dau jumatate din raza bilei
	if (dist <= firstRadix + seconRadix ) {
		return true;
	}

	return false;
}
void Tema2::Init()
{
	BilaAlba.visible = true;
	BilaAlba.vellocityX = 1;
	BilaAlba.vellocityZ = 0;
	BilaAlba.speed = 0;

	//Pozitia initiala a bilei albe
	BilaAlba.x = -1.3;
	BilaAlba.y = 0.3;
	BilaAlba.z = 0;

	deltaXMouseMove = 0;
	deltaZMouseMove = 0;


	ballIsMoving = false;
	decideThePlayer = true;

	wasAnyBallInThePocket = false;
	
	releaseAngle = 0; //ma ajuta ca tacul sa fie drept la urm lovitura :)

	//caracteristicile fiecarui jucator
	for (int b = 0; b < 2; b++) {
	players[b].color = glm::vec3(0, 0, 0);
	//cate bile a bagat
	players[b].numberOfBalls = 0;
	//nr de fault-uri :cate bile albe a bagat in gaura, cate bile
	//de alta culoare a bagat (fata de culoarea sa), cate bile a atins
	//care nu sunt de culoarea sa.
	players[b].faults = 0;
	//variabila de mai jos ma ajuta sa aflu
	//daca in runda curenta player-ul a lovit vreo bila
	//sau nu
	//Aceasta va fi utila cand vom verifica daca 
	//prima bila e de culoarea player-ului sau nu,
	//in combinatie cu primaBilaOk.
	players[b].primaBilaLovita = true;
	//Aceaasta variabila ma ajuta sa aflu daca e ok
	//bila lovita prima data sau nu.
	players[b].primaBilaOk = true;
	
	}

	//La inceput vederea top-down e dezactivata
	topDownView = false;

	camera = new Laborator::Camera();
	//VEDERE DINTR-O PARTE LA INCEPUT
	//Vederea la inceput nu e nici Top-Down nici ca cea din space. 
	//Seamana cu cea din space dar este putin mai de sus
	//Target-ul este bila alba
	camera->Set(glm::vec3(-2.2, 0.6, 0), glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z), glm::vec3(0, 1, 0));

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	// Se incarca shaderele
	{
		shader = new Shader("ShaderLab6");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		shader = new Shader("ShaderTac");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShaderTac.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShaderTac.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	//Planseul mesei dar si marginile sunt create in clasele Ractangle si Border din
	//vertecsi si indecsi
	Mesh* ractangle = Ractangle::CreateRactangle("ractangle", glm::vec3(0, 0, 0), 4, 0.4, 2, glm::vec3(0, float(128) / 255, 0), glm::vec3(float(139) / 255, (float)69 / 255, (float)19 / 255), true);
	AddMeshToList(ractangle);

	Mesh* firstBorder = Border::CreateBorder("firstFoot", glm::vec3(0, 0, 0), 4, 0.15, 0.15, glm::vec3(float(135) / 255, (float)68 / 255, (float)18 / 255),
		glm::vec3(float(0) / 255, (float)136 / 255, (float)0 / 255), true);
	AddMeshToList(firstBorder);

	Mesh* secondBorder = Border::CreateBorder("secondFoot", glm::vec3(0, 0, 0), 4, 0.15, 0.15, glm::vec3(float(135) / 255, (float)68 / 255, (float)18 / 255),
		glm::vec3(float(0) / 255, (float)136 / 255, (float)0 / 255), true);
	AddMeshToList(secondBorder);

	Mesh* thirdBorder = Border::CreateBorder("thirdFoot", glm::vec3(0, 0, 0), 2, 0.15, 0.15, glm::vec3(float(135) / 255, (float)68 / 255, (float)18 / 255),
		glm::vec3(float(0) / 255, (float)136 / 255, (float)0 / 255), true);
	AddMeshToList(thirdBorder);

	Mesh* fourthBorder = Border::CreateBorder("fourthFoot", glm::vec3(0, 0, 0), 2, 0.15, 0.15, glm::vec3(float(135) / 255, (float)68 / 255, (float)18 / 255),
		glm::vec3(float(0) / 255, (float)136 / 255, (float)0 / 255), true);
	AddMeshToList(fourthBorder);
	//Gaurile sunt facute in blender si importate aici
	{
		Mesh* mesh = new Mesh("hole1");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "mySphere3.obj");
		meshes[mesh->GetMeshID()] = mesh;
		holes[0].mesh = mesh;
		AddMeshToList(mesh);
	}

	{
		Mesh* mesh = new Mesh("hole2");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "mySphere3.obj");
		meshes[mesh->GetMeshID()] = mesh;
		holes[1].mesh = mesh;

		AddMeshToList(mesh);
	}

	{
		Mesh* mesh = new Mesh("hole3");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "mySphere3.obj");
		meshes[mesh->GetMeshID()] = mesh;
		holes[2].mesh = mesh;

		AddMeshToList(mesh);
	}

	{
		Mesh* mesh = new Mesh("hole4");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "mySphere3.obj");
		meshes[mesh->GetMeshID()] = mesh;
		holes[3].mesh = mesh;

		AddMeshToList(mesh);
	}

	{
		Mesh* mesh = new Mesh("hole5");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "mySphere3.obj");
		meshes[mesh->GetMeshID()] = mesh;
		holes[4].mesh = mesh;

		AddMeshToList(mesh);
	}

	{
		Mesh* mesh = new Mesh("hole6");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "mySphere3.obj");
		meshes[mesh->GetMeshID()] = mesh;
		holes[5].mesh = mesh;

		AddMeshToList(mesh);
	}
		//simbol deasupra care seamana cu identificatorul din sims
		//ma ajuta sa vad al cui e randul
		//va aparea in partea din spate a mesei o sfera de o culoare anume
		//sau neagra daca inca nu s-au ales culorile
		//la schimbarea player-ului se schimba culoarea sferei
	{
		Mesh* mesh = new Mesh("sims");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "mySphere3.obj");
		meshes[mesh->GetMeshID()] = mesh;
		holes[5].mesh = mesh;

		AddMeshToList(mesh);
	}
	
	//Pentru bile am folosit meshele date in framework
	for (int i = 0; i < 7; i++) {
		string nrBila = std::to_string(i);
		string name = "bilaRosie";
		name = name + nrBila;
		ballsOnTable[i].mesh = new Mesh(name);
		ballsOnTable[i].mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[ballsOnTable[i].mesh->GetMeshID()] = ballsOnTable[i].mesh;
		AddMeshToList(ballsOnTable[i].mesh);
	}

	for (int i = 7; i < 14; i++) {
		string nrBila = std::to_string(i-7);
		string name = "bilaGalbena";
		name = name + nrBila;
		ballsOnTable[i].mesh = new Mesh(name);
		ballsOnTable[i].mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[ballsOnTable[i].mesh->GetMeshID()] = ballsOnTable[i].mesh;
		AddMeshToList(ballsOnTable[i].mesh);
		}

	Mesh* bilaNeagra = new Mesh("bilaNeagra");
	ballsOnTable[14].mesh = bilaNeagra;
	ballsOnTable[14].mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[ballsOnTable[14].mesh->GetMeshID()] = ballsOnTable[14].mesh;
	AddMeshToList(ballsOnTable[14].mesh);
	
	Mesh* bilaAlba = new Mesh("bilaAlba");
	bilaAlba->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[bilaAlba->GetMeshID()] = bilaAlba;
	AddMeshToList(bilaAlba);


	BilaAlba.mesh = bilaAlba;
	
	//tac
	FOVAngle = 45.0f;
	projectionMatrixForTac = glm::perspective(RADIANS(FOVAngle), window->props.aspectRatio, 0.01f, 200.0f);;
	Mesh* tac = Ractangle::CreateRactangle("tac", glm::vec3(0, 0, 0), 1, 0.01, 0.01, glm::vec3(float(92) / 255, float(64) / 255, float(51) / 255), glm::vec3(float(92) / 255, (float)64 / 255, (float)51 / 255), true);
	AddMeshToList(tac);

	//Pozitiile gaurilor
	holes[0].x = 0;
	holes[0].y = 0.2;
	holes[0].z = 0.85;

	holes[1].x = 1.85;
	holes[1].y = 0.2;
	holes[1].z = 0.85;

	holes[2].x = -1.85;
	holes[2].y = 0.2;
	holes[2].z = 0.85;

	holes[3].x = 0;
	holes[3].y = 0.2;
	holes[3].z = -0.85;

	holes[4].x = 1.85;
	holes[4].y = 0.2;
	holes[4].z = -0.85;


	holes[5].x = -1.85;
	holes[5].y = 0.2;
	holes[5].z = -0.85;

	//se initializeaza proprietatile bilelor
	for (int i = 0; i < 15; i++) {
		ballsOnTable[i].vellocityX = 0;
		ballsOnTable[i].vellocityZ = 0;
		ballsOnTable[i].isMoving = false;
		ballsOnTable[i].speed = 0;
		ballsOnTable[i].visible = true;
	
	}

	//stabilesc locatiile bilelor
	ballsOnTable[14].x = 1.1;
	ballsOnTable[14].y = 0.3;
	ballsOnTable[14].z = 0;

	ballsOnTable[0].x = 0.9;
	ballsOnTable[0].y = 0.3;
	ballsOnTable[0].z = 0;

	ballsOnTable[1].x = 1;
	ballsOnTable[1].y = 0.3;
	ballsOnTable[1].z = 0.05;


	ballsOnTable[2].x = 1.1;
	ballsOnTable[2].y = 0.3;
	ballsOnTable[2].z = -0.1;

	ballsOnTable[3].x = 1.2;
	ballsOnTable[3].y = 0.3;
	ballsOnTable[3].z = -0.05;


	ballsOnTable[4].x = 1.2;
	ballsOnTable[4].y = 0.3;
	ballsOnTable[4].z = 0.05;

	ballsOnTable[5].x = 1.3;
	ballsOnTable[5].y = 0.3;
	ballsOnTable[5].z = -0.2;

	ballsOnTable[6].x = 1.3;
	ballsOnTable[6].y = 0.3;
	ballsOnTable[6].z = 0.1;

	ballsOnTable[7].x = 1;
	ballsOnTable[7].y = 0.3;
	ballsOnTable[7].z = -0.05;

	ballsOnTable[8].x = 1.1;
	ballsOnTable[8].y = 0.3;
	ballsOnTable[8].z = 0.1;

	ballsOnTable[9].x = 1.2;
	ballsOnTable[9].y = 0.3;
	ballsOnTable[9].z = -0.15;

	ballsOnTable[10].x = 1.2;
	ballsOnTable[10].y = 0.3;
	ballsOnTable[10].z = 0.15;

	ballsOnTable[11].x = 1.3;
	ballsOnTable[11].y = 0.3;
	ballsOnTable[11].z = -0.1;

	ballsOnTable[12].x = 1.3;
	ballsOnTable[12].y = 0.3;
	ballsOnTable[12].z = 0;


	ballsOnTable[13].x = 1.3;
	ballsOnTable[13].y = 0.3;
	ballsOnTable[13].z = 0.2;
}

Mesh* Tema2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	//seteaza pipe-ul cu numarul 0 ca fiind utilizat pentru a lega obiectele de shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	//seteaza pipe-ul cu numarul 1 ca fiind utilizat pentru a lega obiectele de shader

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();


	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];

}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

}

void Tema2::reinitializari() {

	//de fiecare data cand intra bila alba in gaura
	//sau cand se produce un fault, 
	//bila alba tb sa ajunga in treimea inferioara a mesei
	//si sa se dea tacul celuilalt jucator

	BilaAlba.visible = true;
	BilaAlba.vellocityX = 1;
	BilaAlba.vellocityZ = 0;
	BilaAlba.speed = 0;

	BilaAlba.x = -1.3;
	BilaAlba.y = 0.3;
	BilaAlba.z = 0;
	//reintializez ca sa pot avea unghiurile corecte data viitoare
	//altfel ramane cu garbage de la pozitia trecuta
	deltaXMouseMove = 0;
	deltaZMouseMove = 0;

	for (int i = 0; i < 2; i++) {
		//Prima bila Lovita = true inseamna ca inca nu a lovit nicio bila
		//va fi folosita mai jos
		//Prima bila ok == true (in caz ca primaBilaLovita == false, va fi folosita aceasta 
		//variabila pt a vedea daca prima bila lovita e cea corecta,corespunzatoare culorii
		//player-ului.)
		// Altfel avem FAULT.
		players[i].primaBilaLovita = true;
		players[i].primaBilaOk = true;
	}
	ballIsMoving = false;
	releaseAngle = 0; //ma ajuta ca tacul sa fie drept la urm lovitura :)
	topDownView = false;

	camera->Set(glm::vec3(-2.2, 0.6, 0), glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z), glm::vec3(0, 1, 0));

}
void Tema2::Update(float deltaTimeSeconds)
{
		if (topDownView) {
			//folositor atunci cand bilele se misca
			camera->Set(glm::vec3(0.0f, 4, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
			camera->RotateThirdPerson_OY(RADIANS(90));
		}

		//Coliziunile bilei albe cu celelelate bile
		for (int i = 0; i < 15; i++) {

			if (ballsOnTable[i].visible == true && BilaAlba.speed != 0 && 
				collision(&BilaAlba, &ballsOnTable[i], 0.1, 0.1)) {

				//i < 7 == bilele rosii
				//i >= 7 && i < 14 => bilele galbene

				if (i < 7 && players[turn].color == glm::vec3(1, 0, 0) && 
					players[turn].primaBilaLovita == true) {

					//daca inca nu a lovit nicio bila
					//si rosu e culoarea lui
					players[turn].primaBilaLovita = false;
					players[turn].primaBilaOk = true;

				}if (i < 7 && players[turn].color == glm::vec3(1, 1, 0) &&
					players[turn].primaBilaLovita == true) {

					//daca inca nu a lovit nicio bila
					//si bila rosie nu e a lui
					players[turn].faults++;
					players[turn].primaBilaLovita = false;
					players[turn].primaBilaOk = false;

				}
				else if (i >= 7 && i < 14 && players[turn].color == glm::vec3(1, 1, 0) && 
					players[turn].primaBilaLovita == true) {

					//daca inca nu a lovit nicio bila
					//si bila galbena e a lui
					players[turn].primaBilaLovita = false;
					players[turn].primaBilaOk = true;

				}
				else if (i >= 7 && i < 14 && players[turn].color == glm::vec3(1, 0, 0) &&
					players[turn].primaBilaLovita == true) {

					//daca inca nu a lovit nicio bila
					//si bila galbena nu e a lui
					players[turn].faults++;
					players[turn].primaBilaLovita = false;
					players[turn].primaBilaOk = false;


				}
				//aceasta structura ma ajuta sa intorc ambele viteze o data

				ajutor a = collisionWithBalls(BilaAlba,ballsOnTable[i], 0.1, 0.1);
				
				//fac normalizarea vitezei ca sa impart pe speed si vellocity
								
				float modulSecond = sqrt(a.second.x * a.second.x + a.second.z * a.second.z);
				if (modulSecond != 0) {
					//daca modulul e 0 => va da eroareee !!!
					ballsOnTable[i].vellocityX = a.second.x / modulSecond;
					ballsOnTable[i].vellocityZ = a.second.z / modulSecond;
					}
					ballsOnTable[i].speed = modulSecond;

				if (ballsOnTable[i].speed <= 0 || (ballsOnTable[i].vellocityX == 0 && ballsOnTable[i].vellocityZ == 0)) {
					ballsOnTable[i].isMoving = false;
				}
				else {
					ballsOnTable[i].isMoving = true;
				}

				float modulFirst = sqrt(a.first.x * a.first.x + a.first.z * a.first.z);
				if (modulFirst != 0) {
					BilaAlba.vellocityX = a.first.x / modulFirst;
					BilaAlba.vellocityZ = a.first.z / modulFirst;
				}
				BilaAlba.speed = modulFirst;

				if (BilaAlba.speed <= 0 || (BilaAlba.vellocityX == 0 && BilaAlba.vellocityZ == 0)) {
					BilaAlba.isMoving = false;
				}
				else {
					BilaAlba.isMoving = true;
				}
			
			}//Aici se fac coliziunile bilei curente cu bila alba 
			else if (ballsOnTable[i].visible == true && ballsOnTable[i].speed != 0 && collision(&ballsOnTable[i], &BilaAlba, 0.1, 0.1)) {
	
				ajutor a = collisionWithBalls(ballsOnTable[i], BilaAlba, 0.1, 0.1);
				//fac normalizarea ca sa impart pe speed si vellocity
				float modulFirst = sqrt(a.first.x * a.first.x + a.first.z * a.first.z);
				if (modulFirst != 0) {
					ballsOnTable[i].vellocityX = a.first.x / modulFirst;
					ballsOnTable[i].vellocityZ = a.first.z / modulFirst;
				}
					ballsOnTable[i].speed = modulFirst;

				if (ballsOnTable[i].speed <= 0 || (ballsOnTable[i].vellocityX == 0 && ballsOnTable[i].vellocityZ == 0)) {
					ballsOnTable[i].isMoving = false;
				}
				else {
					ballsOnTable[i].isMoving = true;
				}

				float modulSecond = sqrt(a.second.x * a.second.x + a.second.z * a.second.z);
				if (modulSecond != 0) {
					BilaAlba.vellocityX = a.second.x / modulSecond;
					BilaAlba.vellocityZ = a.second.z / modulSecond;
				}
				BilaAlba.speed = modulSecond;

				if (BilaAlba.speed <= 0 || (BilaAlba.vellocityX == 0 && BilaAlba.vellocityZ == 0)) {
					BilaAlba.isMoving = false;
				}
				else {
					BilaAlba.isMoving = true;
				}
			
			}

		}
	
		//Coliziunile fiecarei bile cu celelalte
		//FOARTE IMPORTANT e ca bila sa nu se verifice cu ea insasi !!!!
		for (int i = 0; i < 15; i++) {

			for (int j = 0; j < 15; j++) {

				if ( ballsOnTable[i].visible == true && ballsOnTable[j].visible == true && i != j) {

					if (collision(&ballsOnTable[i], &ballsOnTable[j], 0.1, 0.1)) {

						ajutor a = collisionWithBalls(ballsOnTable[i], ballsOnTable[j], 0.1, 0.1);

						//Asa obtin lungimea vectorului de miscare
						float modulSecond = sqrt(a.second.x * a.second.x + a.second.z * a.second.z);
						float modulFirst = sqrt(a.first.x * a.first.x + a.first.z * a.first.z);
						if (modulSecond != 0) {
							//velocitatile sunt directiile
							ballsOnTable[j].vellocityX = a.second.x / modulSecond;
							ballsOnTable[j].vellocityZ = a.second.z / modulSecond;
							//viteza este lungimea vectorului
						}
						ballsOnTable[j].speed = modulSecond;						

						if (modulFirst != 0) {
							ballsOnTable[i].vellocityX = a.first.x / modulFirst;
							ballsOnTable[i].vellocityZ = a.first.z / modulFirst;
						}
						ballsOnTable[i].speed = modulFirst;

		
						if ((a.first.x == 0 && a.first.z == 0) || modulFirst == 0) {
							ballsOnTable[i].isMoving = false;

						}
						else {
							ballsOnTable[i].isMoving = true;
						}

					
						if ((a.second.x == 0 && a.second.z == 0) || modulSecond == 0) {
							ballsOnTable[j].isMoving = false;

						}
						else {
							ballsOnTable[j].isMoving = true;
						}

					}
				}
			}


		}	
		
		//Daca bilele sunt in miscare
		//Zona asta face deplasarea lor
		for (int k = 0; k < 15;k++) {

			if (ballsOnTable[k].visible == true) {
				//Daca bila curenta nu se misca.. nu am ce deplasare sa fac
				if (ballsOnTable[k].speed <= 0 || (ballsOnTable[k].vellocityX == 0 && 
					ballsOnTable[k].vellocityZ == 0)) {
					ballsOnTable[k].isMoving = false;
				}
				else {
					//altfel deplasez bila
					ballsOnTable[k].x += ballsOnTable[k].vellocityX  *
						deltaTimeSeconds * ballsOnTable[k].speed;
					ballsOnTable[k].z += ballsOnTable[k].vellocityZ  *
						deltaTimeSeconds* ballsOnTable[k].speed;

					//Ca un coeficient de frecare = incetineste bila
					ballsOnTable[k].speed -= 0.002 * 0.002 + 2 * 0.002;

					if (ballsOnTable[k].speed == 0) {
						//daca viteza devine 0, initializez
						//totul la 0 astfel incat sa nu am dupa
						//garbage
						ballsOnTable[k].vellocityX = 0;
						ballsOnTable[k].vellocityZ = 0;
					}
					//Trebuie ca bilele sa nu depaseasca marginile !!!
					if (ballsOnTable[k].x >= 1.75) {
						ballsOnTable[k].vellocityX = -ballsOnTable[k].vellocityX;
					}
					else if (ballsOnTable[k].x <= -1.75) {
						ballsOnTable[k].vellocityX = -ballsOnTable[k].vellocityX;
					}
					if (ballsOnTable[k].z <= -0.75) {
						ballsOnTable[k].vellocityZ = -ballsOnTable[k].vellocityZ;
					}
					else if (ballsOnTable[k].z >= 0.75) {
						ballsOnTable[k].vellocityZ = -ballsOnTable[k].vellocityZ;
					}
				}
			}
		}

		//Nimereste in vreo gaura?
		
		//Se verifica daca cumva bila alba intra in vreo gaura
		//Consider Fault daca player-ul curent a bagat bila alba
		//in gaura !!!!!!!!!!!!!!!!!!!!
		for (int i = 0; i < 6; i++) {

			if (collisionWithHoles(glm::vec3(holes[i].x, holes[i].y, holes[i].z),
				glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z), 0.15, 0.05)) {

				ballIsMoving = false;
				BilaAlba.speed = 0;
				//e un fault... daca bila alba intra in gaura
				//se schimba randul
				players[turn].faults++;
				turn = 1 - turn;
				//pot misca bila cu ASWD
				potPozitionaBila = true;
				//afisez aici statisici pt ca nu se mai ajunge la afisarea de jos
				//repozitionez bila in prima treime
				statistici();
				reinitializari();
				return;
			}
		}

		//Verific daca vreo bila colorata intra in vreo gaura
		for (int i = 0; i < 6; i++) {

			for (int j = 0; j < 15; j++) {

				if (ballsOnTable[j].visible == true && 
					collisionWithHoles(glm::vec3(holes[i].x, holes[i].y, holes[i].z),
						glm::vec3(ballsOnTable[j].x, ballsOnTable[j].y, ballsOnTable[j].z),
						0.15, 0.05)) {
					//dau jumatate din raza bilei ca sa para ca o inghite cand pe mijlocul ei
					//e pe margine
					//var asta spune ca a intrat o bila in runda asta
					//in gaura
					wasAnyBallInThePocket = true;
					//bila devine invizibila => nu se va mai randa
					ballsOnTable[j].visible = false;
					ballsOnTable[j].speed = 0; // imi trebuie la statistici
					if (j == 14) {
						//Zona pentru bila neagra
						statistici();
						cout << "GAME IS OVER \n";
						int nrBileInvizibile = 0;
						//Verific daca player-ul care a bagat bila neagra
						//A ramas fara bile pe masa ( daca toate sunt invisible)
						//Daca da, atunci e castigator
						//Altfel celalalt castiga
						if (players[turn].color == glm::vec3(1, 0, 0)) {
							for (int p = 0; p < 7; p++) {
								if (ballsOnTable[p].visible == false) {
									nrBileInvizibile++;
								}
							}
							if (nrBileInvizibile == 7) {
								cout << "Player " << turn << "wins !\n";
							}
							else {
								cout << "Player " << 1 - turn << "wins !\n";
							}
						}
						else if (players[turn].color == glm::vec3(1, 1, 0)) {
							for (int p = 7; p < 14; p++) {
								if (ballsOnTable[p].visible == false) {
									nrBileInvizibile++;
								}
							}
							if (nrBileInvizibile == 7) {
								cout << "Player " << turn << "wins !\n";
							}
							else {
								cout << "Player " << 1 - turn << "wins !\n";
							}
						}

						//Se afiseaza statisticile si se reinitializeaza jocul
						Init();
						return;
					}
					else if (decideThePlayer == true) {
						//Daca inca nu s-a decis player-ul si am bagat o bila in gaura
						//Vad ce culoare are bila si 
						//asignez culorile respective playerilor
						//aici nu se mai verifica prima bila lovita..
						//ca oricum nu are o bila setata
						if (j < 7) {

							players[turn].color = glm::vec3(1, 0, 0);
							players[turn].numberOfBalls = 1;
							players[1 - turn].color = glm::vec3(1, 1, 0);
						}
						else if (j < 14 && j >= 7) {

							players[turn].color = glm::vec3(1, 1, 0);
							players[turn].numberOfBalls = 1;
							players[1 - turn].color = glm::vec3(1, 0, 0);
						}
						decideThePlayer = false;
						//nu mai pot pozitiona bila!
						potPozitionaBila = false;
					}
					else {
						//Daca playerilor le-au fost decise culorile
						//atunci verific fault-urile (daca nu cumva s-a colizionat prima data
						//o bila care nu era a player-ului)
						//sau verific daca a bagat vreo bila in gaura
						//ce fel de bila e
						//CONSIDER FAULT si daca a bagat o bila de o culoare care
						//nu e a lui
						if (j < 7 && players[turn].color == glm::vec3(1, 0, 0)) {

							//DACA E O BILA CORECTA DAR PRIMA BILA NU E OK
							//NU SE IA IN CONSIDERARE la punctaj
							if (players[turn].primaBilaLovita == false && players[turn].primaBilaOk == true) {
								players[turn].numberOfBalls += 1;
							}
						}
						else if (j < 7 && players[turn].color == glm::vec3(1, 1, 0)) {
							//daca a intrat o bila care nu e a lui= > FAULT => pot pozitiona bila
							//Daca bila din gaura e gresita se considera 2 fault-uri
							//unul atunci cand ating bila colorata gresita (prima data)
							//si unul aici, pentru ca bag in gaura o bila care nu e a mea

							players[turn].faults++;
							turn = 1 - turn;
							potPozitionaBila = true;
							//DACA A BAGAT O BILA GRESITA, ORICUM RANDUL SE SCHIMBA
							//ASA CA TREC IN INITIALIZARI 
							//SI NU SE MAI FAC MODIFICARI JOS DE SCHIMBARE A PLAYERULUI
							reinitializari();
							//daca i-a intrat o singura bila care sa schimbe randul
							//se face break :(
							break;

						}
						else if (j >= 7 && j < 14 && players[turn].color == glm::vec3(1, 1, 0)) {
							//DACA E O BILA CORECTA DAR PRIMA BILA NU E OK
							//NU SE IA IN CONSIDERARE
							if (players[turn].primaBilaLovita == false && players[turn].primaBilaOk == true) {
								players[turn].numberOfBalls += 1;
							}
						}
						else if (j >= 7 && j < 14 && players[turn].color == glm::vec3(1, 0, 0)) {
							//Daca bila din gaura e gresita se considera 2 fault-uri
							//unul atunci cand ating bila colorata gresita (prima data)
							//si unul aici, pentru ca bag in gaura o bila care nu e a mea

							players[turn].faults++;
							turn = 1 - turn;
							potPozitionaBila = true;
							//DACA A BAGAT O BILA GRESITA, ORICUM RANDUL SE SCHIMBA
							//ASA CA TREC IN INITIALIZARI 
							//SI NU SE MAI FAC MODIFICARI JOS DE SCHIMBARE A PLAYERULUI
							reinitializari();
							break;
							//daca i-a intrat o singura bila care sa schimbe randul
							//se face break :(

						}
					}
				}
			}
		}
		
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
			RenderSimpleMesh(meshes["ractangle"], shaders["VertexColor"], modelMatrix);
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.275, -0.925));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
			RenderSimpleMesh(meshes["firstFoot"], shaders["VertexColor"], modelMatrix);
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.275, +0.925));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(180.0f), glm::vec3(0, 1, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
			RenderSimpleMesh(meshes["secondFoot"], shaders["VertexColor"], modelMatrix);
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.925, 0.275, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
			RenderSimpleMesh(meshes["thirdFoot"], shaders["VertexColor"], modelMatrix);
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(1.925, 0.275, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 1, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
			RenderSimpleMesh(meshes["fourthFoot"], shaders["VertexColor"], modelMatrix);
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.2, 0.85));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
			RenderSimpleMesh(meshes["hole1"], shaders["VertexColor"], modelMatrix);


		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(1.85, 0.2, 0.85));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
			RenderSimpleMesh(meshes["hole2"], shaders["VertexColor"], modelMatrix);
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.85, 0.2, 0.85));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
			RenderSimpleMesh(meshes["hole3"], shaders["VertexColor"], modelMatrix);


		}
		if (turn == 1) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(3, 1, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
			modelMatrix = glm::rotate(modelMatrix, deltaTimeSeconds,glm::vec3(0,1,0));
			RenderSimpleMeshBalls(meshes["sims"], shaders["ShaderLab6"], modelMatrix, players[turn].color);
		} else if (turn == 0) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(3, 1, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
			modelMatrix = glm::rotate(modelMatrix, deltaTimeSeconds, glm::vec3(0, 1, 0));
			RenderSimpleMeshBalls(meshes["sims"], shaders["ShaderLab6"], modelMatrix, players[turn].color);
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.2, -0.85));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
			RenderSimpleMesh(meshes["hole4"], shaders["VertexColor"], modelMatrix);

		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(1.85, 0.2, -0.85));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
			RenderSimpleMesh(meshes["hole5"], shaders["VertexColor"], modelMatrix);

		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.85, 0.2, -0.85));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
			RenderSimpleMesh(meshes["hole6"], shaders["VertexColor"], modelMatrix);

		}
		string name;
		glm::mat4 modelMatrix;
		if (ballsOnTable[0].visible == true) {
			name = "bilaRosie0";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[0].x, ballsOnTable[0].y, ballsOnTable[0].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 0, 0));
		}

		if (ballsOnTable[7].visible == true) {
			name = "bilaGalbena0";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[7].x, ballsOnTable[7].y, ballsOnTable[7].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 1, 0));
		}

		if (ballsOnTable[1].visible == true ) {
			name = "bilaRosie1";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[1].x, ballsOnTable[1].y, ballsOnTable[1].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 0, 0));
		}
		if (ballsOnTable[14].visible == true) {
			name = "bilaNeagra";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[14].x, ballsOnTable[14].y, ballsOnTable[14].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(0, 0, 0));
		}

		if (ballsOnTable[8].visible == true) {
			name = "bilaGalbena1";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[8].x, ballsOnTable[8].y, ballsOnTable[8].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 1, 0));
		}

		if (ballsOnTable[2].visible == true) {
			name = "bilaRosie2";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[2].x, ballsOnTable[2].y, ballsOnTable[2].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 0, 0));
		}

		if (ballsOnTable[9].visible == true) {
			name = "bilaGalbena2";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[9].x, ballsOnTable[9].y, ballsOnTable[9].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 1, 0));
		}
		if (ballsOnTable[3].visible == true) {
			name = "bilaRosie3";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[3].x, ballsOnTable[3].y, ballsOnTable[3].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 0, 0));
		}

		if (ballsOnTable[4].visible == true) {
			name = "bilaRosie4";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[4].x, ballsOnTable[4].y, ballsOnTable[4].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 0, 0));
		}
		if (ballsOnTable[10].visible == true) {
			name = "bilaGalbena3";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[10].x, ballsOnTable[10].y, ballsOnTable[10].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 1, 0));
		}

		if (ballsOnTable[5].visible == true) {
			name = "bilaRosie5";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[5].x, ballsOnTable[5].y, ballsOnTable[5].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 0, 0));
		}

		if (ballsOnTable[11].visible == true) {
			name = "bilaGalbena4";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[11].x, ballsOnTable[11].y, ballsOnTable[11].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 1, 0));
		}

		if (ballsOnTable[12].visible == true) {
			name = "bilaGalbena5";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[12].x, ballsOnTable[12].y, ballsOnTable[12].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 1, 0));
		}

		if (ballsOnTable[6].visible) {
			name = "bilaRosie6";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[6].x, ballsOnTable[6].y, ballsOnTable[6].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 0, 0));
		}
		if (ballsOnTable[13].visible == true) {
			name = "bilaGalbena6";
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballsOnTable[13].x, ballsOnTable[13].y, ballsOnTable[13].z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 1, 0));
		}
		name = "bilaAlba";
		modelMatrix = glm::mat4(1);
		if (ballIsMoving) {
			//logica de miscare a bilei :)
			//coliziunea cu peretii
			if (BilaAlba.x >= 1.75) {
				BilaAlba.vellocityX = -BilaAlba.vellocityX;
			}
			else if (BilaAlba.x <= -1.75) {
				BilaAlba.vellocityX = -BilaAlba.vellocityX;
			}
			if (BilaAlba.z >= 0.75) {
				BilaAlba.vellocityZ = -BilaAlba.vellocityZ;
			}
			else if (BilaAlba.z <= -0.75) {
				BilaAlba.vellocityZ = -BilaAlba.vellocityZ;
			}

			BilaAlba.x += BilaAlba.speed * deltaTimeSeconds * BilaAlba.vellocityX;
			BilaAlba.z += BilaAlba.speed * deltaTimeSeconds * BilaAlba.vellocityZ;

			//e ca un coeficient de frecare
			//face bila sa se opreasca
			BilaAlba.speed -= 0.001* 0.001 + 2 * 0.001;

			if (BilaAlba.speed <= 0.090) {
				//Am decis ca se misca prea incet spre sfarsitul deplasarii
				//asa ca daca trece de aceasta limita e ca si oprita
				allBallsAreStopped = 0;
				//allBallsAreStopped ma ajuta sa obtin corect oprirea tuturor bilelor
				//aici se reinitializeaza ,si are proprietatea
				//ca se poate reinitializa o singura data in cadrul unei runde
				//doar atunci cand se opreste bila alba.

				//variabila care tine cont daca nu mai se misca nicio bila
				//mai departe va fi modifica la sfarsitul lui Update
				//unde se va verifica daca si celelalte bile s-au oprit
				//Altfel, afisa de prea multe ori 
				//aceasta zona se utilizeaza o singura data in cadrul unei lovituri
				//deci utilizand-o nu se va calcula decat o singura data
				//daca s-au oprit sau nu toate
				ballIsMoving = false;
				releaseAngle = 0;
				BilaAlba.speed = 0;
				//pe releaseAngle asta il fac  0 ca tacul sa fie orientat normal
				//la urmatoarea lovitura :)

				//Modific vederea ca la inceput
				//nu e nici ca pe SPACE
				//nu e nici Top-Down
				//e un pic mai sus pe y ca pt SPACE
				//mi s-a parut ca asa imi e cel mai usor sa joc

				camera->Set(glm::vec3(BilaAlba.x - 0.3 - 1, BilaAlba.y + 0.6, BilaAlba.z), glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z), glm::vec3(0, 1, 0));


			}
		}

		//Bila Alba
		modelMatrix = glm::translate(modelMatrix, glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMeshBalls(meshes[name], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 1, 1));

		
		if (!ballIsMoving && topDownView == false) {
			//daca bila se misca tacul nu mai apare pe ecran :)

			if (leftMousePressed) {

				//Se realizeaza animatia cu ajutorul functiei sin
				//Care ma ajuta sa fac miscarea du-te - vino
				//se imparte la 10 pentru a fi proportionala cu bila 
				//aceasta miscare ( asta pt ca am scalat bila la 0.1 factor
				//de scalare)
				double elapsedTime = sin(Engine::GetElapsedTime()) / 10;

				//"0.2 - ceva"  pt ca viteza tb sa fie invers proportionala cu distanta
				//din animatie.
				// elapsedTime + 0.1 pt ca am si valori negative care ma incurca
				//la inmultire si vreau sa scap de ele :)
				// * 6.5 - cat de mare sa fie viteza 
				BilaAlba.speed = (0.2 - (elapsedTime + 0.1)) * 6.5;

				//cu cat elapsedTime e mai mic cu atat viteza e mai mare :D
				//-12.5 e inclinarea tacului
				//altfel intra in masa
				modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z));
				modelMatrix = glm::rotate(modelMatrix, releaseAngle, glm::vec3(0, 1, 0));
				modelMatrix = glm::rotate(modelMatrix, float(radians(-12.5f)), glm::vec3(0, 0, 1));


				// - 0.145 ca sa aiba loc sa faca animatia
				modelMatrix = glm::translate(modelMatrix, glm::vec3(-1 - 0.145, 0, 0));

				//Animatia in shader
				RenderMeshForTac(meshes["tac"], shaders["ShaderTac"], modelMatrix, elapsedTime);
			}
			else if (rightClickPressed) {

				modelMatrix = glm::mat4(1);

				modelMatrix = glm::translate(modelMatrix, glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z));
				modelMatrix = glm::rotate(modelMatrix, deltaXMouseMove, glm::vec3(0, 1, 0));
				modelMatrix = glm::rotate(modelMatrix, float(radians(-12.5f)), glm::vec3(0, 0, 1));

				modelMatrix = glm::translate(modelMatrix, glm::vec3(-1 - 0.05, 0, 0));
				//daca e apasata alta tasta in afara de leftClick 
				//tacul se afiseaza obisnuit, nu in shader
				RenderSimpleMesh(meshes["tac"], shaders["VertexColor"], modelMatrix);

			}
			else {
				modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z));
				modelMatrix = glm::rotate(modelMatrix, releaseAngle, glm::vec3(0, 1, 0));
				modelMatrix = glm::rotate(modelMatrix, float(radians(-12.50f)), glm::vec3(0, 0, 1));


				//fac asa pt ca initial si bila si tacul sunt in origine
				//asa ca scad jumatate din dim tacului si apoi mai scad cv care e dist fata de bila.	
				modelMatrix = glm::translate(modelMatrix, glm::vec3(-1 - 0.05, 0, 0));

				RenderSimpleMesh(meshes["tac"], shaders["VertexColor"], modelMatrix);

			}
		}

		//aici verific daca cumva toate bilele s-au oprit
		int count = 0;
		for (int i = 0; i < 15; i++) {
			if (ballsOnTable[i].speed <= 0) {
				count++;
			}
		}
		//daca toate bilele s-au oprit
		if (count == 15) {
			allBallsAreStopped++;
		}

		//daca s-a oprit bila alba, si s-a incrementat mai sus
		//allBallsAreStopped => ajunge la 1 all BallsAreStopped =>
		//pot vedea ce s-a intamplat in aceasta runda
		//asta pt ca nu mai exista miscare pe masa
		if (allBallsAreStopped == 1) {
			//o data ce ajunge bila alba sa se opreasca se reinitializeaza allBallAreStopped
			if (decideThePlayer == true ) {
				//inca nu s-a decis playeruil. nu am ce verifica de fault sa fac
				turn = 1 - turn; // doar se schimba player-ul
			}

			//Daca am bagat o bila incorecta si prima bila bagat era incorecta, nu se mai ajunge aici 
			//caci in functia de reinitializare se schimba variabilele din players
			
			//daca baga o bila corecta si prima bila nu e ok
			//se schimba randul
			else if (decideThePlayer == false && wasAnyBallInThePocket == true &&
				players[turn].primaBilaLovita == false && players[turn].primaBilaOk == false) {
				
				//a atins o bila dar nu e a lui
				//FAULT
				turn = 1 - turn;
				potPozitionaBila = true;
				reinitializari();

			}
			//daca nu am bagat nicio bila in buzunar si nici nu am lovit una
			else if (decideThePlayer == false && wasAnyBallInThePocket == false && players[turn].primaBilaLovita == true) {
				turn = 1 - turn;
			} //daca nu baga nicio bila
			//si prima bila atinsa e ok => se trece la celalat
			//daca prima bila nu e ok => urmatorul poate pune unde vrea
			else  if (decideThePlayer == false && wasAnyBallInThePocket == false && players[turn].primaBilaLovita == false) {
				if (players[turn].primaBilaOk == true) {
					turn = 1 - turn;
				}
				else if (players[turn].primaBilaOk == false) {
					turn = 1 - turn;
					potPozitionaBila = true;
					reinitializari();
				}
			}
			
			//se reinitiaza playerii
			for (int i = 0; i < 2; i++) {
				players[i].primaBilaLovita = true;
				players[i].primaBilaOk = true;
			}
			//se opreste vederea top-down caci si miscarea s-a oprit
			topDownView = false;
			//variabila asta stabileste daca de la 
			//inceputul miscarii bilei albe pana la sfarsitul bilelor nu a intrat 
			//nicio bila in buzunar => false
			//daca a intrat vreo bila in buzunar => true
			wasAnyBallInThePocket = false;
			//La sfarsitul miscarii afisez statisticile
			statistici();
			//dau o valoare dubioasa variabilei care
			//se ocupa de oprire
			//astfel incat
			//pana ce nu se petrece urmatoarea lovitura
			//si allBallsAreStopped devine 0
			//nu se poate sa mai intre aici
			allBallsAreStopped = 2;
		}
}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMeshForTac(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, double elapsedtime)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);


	int location = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	location = glGetUniformLocation(shader->program, "View");
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	location = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	location = glGetUniformLocation(shader->program, "ElapsedTime");
	glUniform1f(location, elapsedtime); // 1 dimensiune =  float


	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix)
{
	//e folosit in combinatie cu VertexColor acolo unde am pe mai multe fete diferite culori
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);
	int location = glGetUniformLocation(shader->program, "Model");

	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	location = glGetUniformLocation(shader->program, "View");

	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	location = glGetUniformLocation(shader->program, "Projection");
	glm::mat4 projectionMatrix2 = projectionMatrix;
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix2));


	//PT CULOARE
	GLint object_pos = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(object_pos, 1, glm::value_ptr(glm::vec3(1, 0, 0)));


	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);

}

void Tema2::RenderSimpleMeshBalls(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, glm::vec3 color)
{
	//e folosit in combinatie cu ShaderLab6 acolo unde am o singura culoare
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);
	int location = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	location = glGetUniformLocation(shader->program, "View");
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	location = glGetUniformLocation(shader->program, "Projection");
	glm::mat4 projectionMatrix2 = projectionMatrix;
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix2));


	//PT CULOARE
	GLint object_pos = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(object_pos, 1, glm::value_ptr(color));


	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// add key press event
	// Misc camera doar daca se tine apasat pe click Dreapta si nu s-a apasat deja stanga
		if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) == false && !leftMousePressed )
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			//Aici se misca doar camera
			// TODO : translate the camera forward

			camera->TranslateForward(deltaTime * cameraSpeed);

		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			camera->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->TranslateForward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpword(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpword(deltaTime * cameraSpeed);
		}
	
			///POTI MISCA BILA DOAR DACA NU AI APASAT PE ALTCEVA :)

	}
		if (potPozitionaBila && leftMousePressed == false) {
			//aici se misca si bila o data cu camera

				if (window->KeyHold(GLFW_KEY_W)) {
					// TODO : translate the camera forward
					if (BilaAlba.x <= -2.0f / 3 - 0.2f) { // scot si mantauaa :) 
						BilaAlba.x += deltaTime;
						camera->Set(glm::vec3(BilaAlba.x - 0.3 - 1, BilaAlba.y + 0.6, BilaAlba.z), glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z), glm::vec3(0, 1, 0));

					}
				}

				if (window->KeyHold(GLFW_KEY_A)) {
					// TODO : translate the camera to the left
					if (BilaAlba.z >= -0.75) {
						BilaAlba.z -= deltaTime;
						camera->Set(glm::vec3(BilaAlba.x - 0.3 - 1, BilaAlba.y + 0.6, BilaAlba.z), glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z), glm::vec3(0, 1, 0));

					}
				}

				if (window->KeyHold(GLFW_KEY_S)) {
					// TODO : translate the camera backwards
					if (BilaAlba.x >= -1.75) {
						BilaAlba.x -= deltaTime;
						camera->Set(glm::vec3(BilaAlba.x - 0.3 - 1, BilaAlba.y + 0.6, BilaAlba.z), glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z), glm::vec3(0, 1, 0));

					}
				}

				if (window->KeyHold(GLFW_KEY_D)) {
					// TODO : translate the camera to the right
					if (BilaAlba.z <= 0.75) {
						BilaAlba.z += deltaTime;
						camera->Set(glm::vec3(BilaAlba.x - 0.3 - 1, BilaAlba.y + 0.6, BilaAlba.z), glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z), glm::vec3(0, 1, 0));

					}
			}
		}
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE) {
		//Diferenta dintre space si vederea obisnuita(care e dif de top-down)
		//e ca space vede de mai jos 
		//Diferenta e doar de 0.15f pe axa OY.
		//Mi-a placut sa se vada aproape ca in space si in general
		//pentru ca pot lovi mai usor bilele cum trebuie
		camera->Set(glm::vec3(BilaAlba.x - 0.3 - 1, BilaAlba.y + 0.45, BilaAlba.z), glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z), glm::vec3(0, 1, 0));
		camera->RotateThirdPerson_OY(releaseAngle);

		topDownView = false;
	}

}

void Tema2::OnKeyRelease(int key, int mods)
{

	// add key release event


}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	//pt miscarea tacului in jurul bilei :)
	
	deltaXMouseMove += deltaX * 0.001;
	deltaZMouseMove += deltaY * 0.001;


	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		rightClickPressed = true;
		//La right-click se roteste mara stanga-dreapta;
		//camera third-person - rotiri dupa parametrii primiti
		camera->RotateThirdPerson_OX(deltaY * sensivityOY);
		camera->RotateThirdPerson_OY(deltaX * sensivityOX);

	}

}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		potPozitionaBila = false;
		leftMousePressed = true;
	}
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
		potPozitionaBila = false;
		//asez camera ca in space
		camera->Set(glm::vec3(BilaAlba.x - 0.3 - 1, BilaAlba.y + 0.45, BilaAlba.z), glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z), glm::vec3(0, 1, 0));
		camera->RotateThirdPerson_OY(deltaXMouseMove);
		
	}

}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		leftMousePressed = false;
		//se activeaza top-down doar cand bila alba 
		//se deplaseaza
		topDownView = true;
		ballIsMoving = true;
	}
	else if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
		//Foarte Important !
		//unghiul trebuie sa ramana cel care a fost
		//cand am dat drumul la right-click
		//se poate modifica dupa si sa nu mai faca bine rotatia
		//daca folosesc deltaXMouseMove
		releaseAngle = deltaXMouseMove;
		
		//O parte FOARTE IMPORTANTA a problemei e aici
		//Dupa pozitionare fac camera sa fie pe aceasi 
		//directie cu bila
		//ca sa iau usor
		//directiile (velocitatile !!!!)
		camera->Set(glm::vec3(BilaAlba.x - 0.3 - 1, BilaAlba.y + 0.45, BilaAlba.z), glm::vec3(BilaAlba.x, BilaAlba.y, BilaAlba.z), glm::vec3(0, 1, 0));
		camera->RotateThirdPerson_OY(releaseAngle);

		BilaAlba.vellocityX = BilaAlba.x - camera->position.x;
		BilaAlba.vellocityZ = BilaAlba.z - camera->position.z;

		rightClickPressed = false;

	}

}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
