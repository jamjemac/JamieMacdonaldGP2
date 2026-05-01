#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;


MainGame::MainGame()
{
	_currentState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{

	_sceneDisplay.initDisplay();

	// Intialises the camera
	myCamera.initCamera(glm::vec3(10, 0.55, -0.65), 70.0f, (float)_sceneDisplay.getWidth() / _sceneDisplay.getHeight(), 0.01f, 1000.0f);

	// Loads in the audio that the scene uses
	sceneAudio.addAudioTrack("..\\res\\background.wav");
	sceneSFX.addSoundEffect("..\\res\\ChangeModel.wav");

	// Loads in all the models into the meshes array
	meshes[0].loadModel("..\\res\\Sheep.obj");
	meshes[1].loadModel("..\\res\\Cabin.obj");
	meshes[2].loadModel("..\\res\\Wizard.obj");

	// Loads in all the textures into the texture array
	textures[0].init("..\\res\\Brick.jpg");
	textures[1].init("..\\res\\Water.jpg");
	textures[2].init("..\\res\\Bricks.jpg");

	// Loads in all the shaders
	shader.init("..\\res\\shader.vert", "..\\res\\shader.frag");
	ADS.init("..\\res\\ADS.vert", "..\\res\\ADS.frag");

	// Applies inital values to the scene
	currentMesh = 0; 

	for (int i = 0; i < 3; i++) // Starts a for loop to input values into each of the float arrays
	{
		meshesScale[i] = 1.0f;
		meshesXRotation[i] = 0.0f;
		meshesYRotation[i] = 0.0f;
	}

	// Stores the position of all meshes in the scene
	meshesPosition[0] = 10;
	meshesPosition[1] = 0;
	meshesPosition[2] = -10;

}

void MainGame::linkADS()
{
	// Controls the position and colour of the light
	glm::vec3 lightPosition(meshesPosition[currentMesh], 50.0f, 0.0f); // Updates the position of the light to reflect the current object the player is viewing
	glm::vec3 lightColor(0.5f, 0.5f, 1.0f);

	// Controls the colour of the rim light
	glm::vec3 rimColour(0.0f, 0.0f, 0.0f);

	// Recieves the modelMatrix 
	glm::mat4 modelMatrix = transform.GetModel();

	// Sets the value of the uniforms in the shader
	ADS.setVec3("lightPosition", lightPosition);
	ADS.setVec3("lightColour", lightColor);
	ADS.setVec3("rimColour", rimColour);
	ADS.setVec3("viewPosition", myCamera.getPos());
	ADS.setMat4("model", modelMatrix);
}

void MainGame::gameLoop()
{
	// Ensures the gameLoop is opperating as long as the state isn't exit
	while (_currentState != GameState::EXIT)
	{

		// Calls all required functions to ensure scene opperates smoothly
		sceneAudio.playAudioTrack();
		processInput();
		adjustObjects();
		drawGame();

	}
}

void MainGame::processInput()
{
	// Handles user input events
	SDL_Event event;

	// Processes user events
	while(SDL_PollEvent(&event))
	{

		switch (event.type)
		{
		// Checks for player inputs from the keyboard
		case SDL_KEYDOWN:
			
			// Controls the current mesh being selected and having the program update to accomidate that
			if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
				if (currentMesh == 0) { return; } // Returns value since no meshes exist past the mesh stored at 0
				resetMesh(); // Resets the current mesh
				currentMesh--;
				myCamera.MoveLeft(10);
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
				if (currentMesh == 2) { return; } // Returns value since no meshes exist past the mesh stored at 2
				resetMesh(); // Resets the current mesh
				currentMesh++;
				myCamera.MoveRight(10);
			}

			// Controls scaling the current mesh
			if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
				scalingKeys[0] = true;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
				scalingKeys[1] = true;
			}

			// Controls rotating current mesh
			if (event.key.keysym.scancode == SDL_SCANCODE_A) {
				rotatingKeys[0] = true;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_D) {
				rotatingKeys[1] = true;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_W) {
				rotatingKeys[2] = true;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_S) {
				rotatingKeys[3] = true;
			}

			// Controls adjusting the shaders
			if (event.key.keysym.scancode == SDL_SCANCODE_1) {
				adsOff = true;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_2) {
				adsOff = false;
			}

			// Closes the program
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				_currentState = GameState::EXIT;
			}

			break;

		// Checks for the user releasing the key
		case SDL_KEYUP:

			// Checks if the user is no longer scaling the model
			if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
				scalingKeys[0] = false;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
				scalingKeys[1] = false;
			}

			// Checks if the user is no longer rotating the model
			if (event.key.keysym.scancode == SDL_SCANCODE_A) {
				rotatingKeys[0] = false;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_D) {
				rotatingKeys[1] = false;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_W) {
				rotatingKeys[2] = false;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_S) {
				rotatingKeys[3] = false;
			}

			break;
		// Checks if user has closed the window to shutdown the game
		case SDL_QUIT:
			_currentState = GameState::EXIT;
			break;
		}
	}
}

void MainGame::resetMesh()
{
	// Resets mesh variables back to the default
	meshesScale[currentMesh] = 1.0f;
	meshesXRotation[currentMesh] = 0.0f;
	meshesYRotation[currentMesh] = 0.0f;
	sceneSFX.playSoundEffect(0);
}

void MainGame::adjustObjects() 
{

	// Handles scaling the users current mesh 
	if (scalingKeys[0] == true) {
		meshesScale[currentMesh] += 0.01f; // This scales the current mesh up
	}
	if (scalingKeys[1] == true) {
		if (meshesScale[currentMesh] < 0.01f) 
		{ 
			meshesScale[currentMesh] = 0.01f; // Sets meshesScale to be 0.1 since thats the smallest it can be without disapearing and turning upside down
			return;
		}
		meshesScale[currentMesh] -= 0.01f; // This scales the current mesh down
	}

	// Handles rotating the users current mesh
	if (rotatingKeys[0] == true) {
		meshesXRotation[currentMesh] -= 0.01f; // This rotates the current mesh to the left
	}
	if (rotatingKeys[1] == true) {
		meshesXRotation[currentMesh] += 0.01f; // This rotates the current mesh to the right
	}
	if (rotatingKeys[2] == true) {
		meshesYRotation[currentMesh] += 0.01f; // This rotates the current mesh backwards
	}
	if (rotatingKeys[3] == true) {
		meshesYRotation[currentMesh] -= 0.01f; // This rotates the current mesh forwards
	}
}

void MainGame::drawGame()
{
	_sceneDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	// Adjusts the background colour of the window
	glClearColor(0.0, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Loads in the first mesh in the meshes array
	transform.SetPos(glm::vec3(10.0, 0.0, 3.0));
	transform.SetRot(glm::vec3(meshesYRotation[0], meshesXRotation[0], 0.0));
	transform.SetScale(glm::vec3(meshesScale[0], meshesScale[0], meshesScale[0]));
	
	// Checks if the user has meshes enabled
	if (!adsOff) {
		ADS.Bind();
		linkADS();
		ADS.Update(transform, myCamera);
		shader.Update(transform, myCamera);
	}
	else {
		shader.Bind();
		shader.Update(transform, myCamera);
	}
	textures[0].Bind(0); // Binds the texture to the object
	meshes[0].draw(); // Draws the object with it's correct shader and texture
	meshes[0].updateSphereData(*transform.GetPos(), 0.6f); 
	
	// Loads in the second mesh in the meshes array
	transform.SetPos(glm::vec3(0.0, 0.0, 3.0));
	transform.SetRot(glm::vec3(meshesYRotation[1], meshesXRotation[1], 0.0));
	transform.SetScale(glm::vec3(meshesScale[1], meshesScale[1], meshesScale[1]));
	
	// Checks if the user has meshes enabled
	if (!adsOff) {
		ADS.Bind();
		linkADS();
		ADS.Update(transform, myCamera);
		shader.Update(transform, myCamera);
	}
	else {
		shader.Bind();
		shader.Update(transform, myCamera);
	}
	textures[1].Bind(0); // Binds the texture to the object
	meshes[1].draw();
	meshes[1].updateSphereData(*transform.GetPos(), 0.6f);

	// Loads in the third mesh in the meshes array
	transform.SetPos(glm::vec3(-10.0, 0.0, 3.0));
	transform.SetRot(glm::vec3(meshesYRotation[2], meshesXRotation[2], 0.0));
	transform.SetScale(glm::vec3(meshesScale[2], meshesScale[2], meshesScale[2]));

	// Checks if the user has meshes enabled
	if (!adsOff) {
		ADS.Bind();
		linkADS();
		ADS.Update(transform, myCamera);
		shader.Update(transform, myCamera);
	}
	else {
		shader.Bind();
		shader.Update(transform, myCamera);
	}
	textures[2].Bind(0); // Binds the texture to the object
	meshes[2].draw();
	meshes[2].updateSphereData(*transform.GetPos(), 0.6f);

	_sceneDisplay.swapBuffer();
} 