#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>

#include "LabCamera.h"

struct Player {
	int numberOfBalls;
	glm::vec3 color;
	bool primaBilaLovita = true;
	int faults = 0;
	bool primaBilaOk = true;
	//daca e a doua bila ..nu mai e true aici
};
struct hole{
	Mesh* mesh;
	float x;
	float y;
	float z;
};

struct Bile {
	Mesh* mesh;
	float x;
	float y;
	float z;
	bool visible;
	float speed;
	float vellocityX;
	float vellocityZ;
	float isMoving = false;
	bool colided = false;
};
struct ajutor {
	vec3 first;
	vec3 second;

};
class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

		Mesh * CreateMesh(const char * name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices);

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix);
		void RenderSimpleMeshBalls(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, glm::vec3 color);
		void RenderMeshForTac(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix,double elapsedTime);
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		bool collisionWithHoles(glm::vec3 first, glm::vec3 second, float firstRadix, float seconRadix);
		bool collision(Bile* first, Bile* second, float firstRadix, float seconRadix);
		ajutor collisionWithBalls(Bile first, Bile second, float firstRadix, float seconRadix);
		void randerString(float x, float y, char * string);
		void statistici();
		void reinitializari();

		Laborator::Camera *camera;
		Shader * shader;
		Mesh* bile[15];
		Bile BilaAlba;
		hole holes[6];
		Bile ballsOnTable[15];

		bool ballDissapeared = false;

		int FOVAngle;
		glm::mat4 projectionMatrixForTac;
		glm::mat4 projectionMatrix;

		bool nuAMutatBila;
		bool leftMousePressed; //create an animation of the pool cue

		bool rightClickPressed = false;
		float releaseAngle = 0.0f;
		bool potPozitionaBila = true;

		bool topDownView = false;
		bool ontTheTacView = false;
		bool ballIsMoving = false;

		float deltaXMouseMove;
		float deltaZMouseMove;

		Player players[2];
		bool decideThePlayer = true;
		int turn = 0;
		int allBallsAreStopped = -1;
		bool wasAnyBallInThePocket = false;
		
		
};
