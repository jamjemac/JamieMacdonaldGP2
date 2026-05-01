#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <SDL/SDL_mixer.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "SDLAudio.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();
	void linkADS();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void adjustObjects();
	void drawGame();
	void resetMesh();
	//void playAudio(unsigned int Source, glm::vec3 pos);

	Display _sceneDisplay; // Stores the window that contains the scene
	GameState _currentState; // Stores the current state of the scene
	Mesh meshes[3]; // Stores all the meshes within the scene
	Camera myCamera; // Stores the camera used to showcase the scene in the window
	Texture textures[3]; // Stores all the textures used for the meshes
	Shader shader; // Stores the shader utalised for the scene
	Shader ADS; // Stores the ambient, diffuse and specular lighting shader
	SDLAudio sceneAudio; // Stores the audio used for the scene
	SDLAudio sceneSFX; // Stores the sound effect used for the scene

	// Contains the transform values of all interactable objects
	float meshesScale[3];
	float meshesXRotation[3];
	float meshesYRotation[3];
	float meshesPosition[3];
	int currentMesh;

	// Contains bools used to store user inputs
	bool scalingKeys[2]; // Stores bool values that track what keys the user has pressed regarding scaling
	bool rotatingKeys[4]; // Stores bool values that track what keys the user has pressed regarding rotation
	bool adsOff; // Stores wether of not ads is off or not


};

