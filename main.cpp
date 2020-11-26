#include <windows.h>

#include <glad/glad.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "FreeImage.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include "Obligatorio2/Model.h"
#include "Obligatorio2/AnimatedObject.h"
#include "Obligatorio2/AnimatedModel.h"
#include "Obligatorio2/Camera.h"
#include "Obligatorio2/Object.h"
#include "Obligatorio2/Plane.h"
#include "Obligatorio2/Settings.h"
#include "Obligatorio2/Shader.h"
#include "Obligatorio2/Water.h"
#include "Obligatorio2/Terrain.h"

using namespace std;

// global variables - normally would avoid globals, using in this demo
GLuint vao, vbo[2]; // handles for our VAO and two VBOs
float r = 0;
unsigned int last_time, current_time;

//pos, target, up
float sensitivity;
float speed;

//mouse variables
int current_x, current_y, last_x, last_y;
bool wireframe;

// loadFile - loads text file into char* fname
// allocates memory - so need to delete after use


// Something went wrong - print SDL error message and quit
void exitFatalError(char* message)
{
	cout << message << " ";
	cout << SDL_GetError();
	SDL_Quit();
	exit(1);
}


void init(void)
{
	Settings* set = Settings::getInstance();
	Shader* lightShader = new Shader("simple.vert", "simple.frag");
	Shader* waterShader = new Shader("water.vert", "water.frag");
	//Shader* anim = new Shader("animated_model.vert", "animated_model.frag");
	Shader* hmShader = new Shader("heightMap.vert", "heightMap.frag");
	//AnimatedObject* ao1 = new AnimatedObject("models/negro/Rumba Dancing.dae", glm::vec3(0, 0, -1), 2, glm::vec3(0, 0.8, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), anim);
	//Object* o1 = new Object("modelos/12221_Cat_v1_l3.obj",glm::vec3(0, -1, 0), 0.8, glm::vec3(8, -0.6, -8.6),glm::vec3(0, 0, 1), glm::vec3(-1,0,-1), lightShader);
	//Object* o2 = new Object("modelos/Japanese_Temple.obj", glm::vec3(0,0,-1),10, glm::vec3(0,4.2,0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), lightShader);
	//Manzana 1: plaza
	Plane* p1 = new Plane("modelos/asfalto.jpg", 100, 100, glm::vec3(20, 1, 20), glm::vec3(10, -1, -10), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), lightShader);//Calle
	Plane* p2 = new Plane("modelos/cordon.jpg", 20, 20, glm::vec3(19, 1, 19), glm::vec3(9.5, -0.9, -9.5), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), lightShader);//Cordon
	Plane* p3 = new Plane("modelos/cordon.jpg", 20, 1, glm::vec3(1, 0.09, 19), glm::vec3(9.5, -0.99, 9.5), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), lightShader);//Cordon
	Plane* p4 = new Plane("modelos/cordon.jpg", 20, 1, glm::vec3(19, 0.09, 1), glm::vec3(9.5, -0.99, -9.5), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1), lightShader);//Cordon
	Plane* p5 = new Plane("modelos/cordon.jpg", 20, 1, glm::vec3(1, 0.09, 19), glm::vec3(-9.5, -0.99, -9.5), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0), lightShader);//Cordon
	Plane* p6 = new Plane("modelos/cordon.jpg", 20, 1, glm::vec3(19, 0.09, 1), glm::vec3(-9.5, -0.99, 9.5), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), lightShader);//Cordon
	Plane* p7 = new Plane("modelos/vereda.jpg", 40, 40, glm::vec3(18.9, 1, 18.9), glm::vec3(9.45, -0.89, -9.45), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), lightShader);//Vereda
	Plane* p8 = new Plane("modelos/grass.jpg", 10, 10, glm::vec3(200, 1.f, 200), glm::vec3(0.f,0.2,0.f), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), lightShader);//Terreno
	Water* water = new Water(vec3(0.f, 1.5f, 0.f), vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, -1.f), waterShader, 35, 200, 200, "modelos/water1.jpg");
	Terrain* terrain = new Terrain(vec3(0.f), vec3(0.f, 1.f, 0.f), vec3(1.f, 0.f, 0.f), hmShader, 50, 200, 200, "modelos/bariloche.jpg");
	//set->addEntity(o1);
	//set->addEntity(o2);
	//set->addEntity(p1);
	//set->addEntity(p2);
	//set->addEntity(p3);
	//set->addEntity(p4);
	//set->addEntity(p5);
	//set->addEntity(p6);
	//set->addEntity(p7);
	set->addEntity(p8);
	//set->addEntity(ao1);

	set->addEntity(water);
	set->addEntity(terrain);

	std::cout << "Total entities: " << set->getEntities().size() << std::endl;

	 // Create and start shader program
	glEnable(GL_DEPTH_TEST); // enable depth testing
	//glEnable(GL_CULL_FACE); // enable back face culling - try this and see what happens!

	//init camera
	Camera* camera = new Camera(vec3(0.f, 60.f, -100.f), half_pi<float>(), 0.f, 45.f, 4.0f / 3.0f, 1.f, 300.f);
	Light* light1 = new Light(vec3(0, 50, -100));
	set->changeNowCamera(camera);
	set->addLight(light1);

	//init time
	last_time = 0;
	current_time = SDL_GetTicks();

	//init speed
	speed = 25.f;

	//init mouse variables
	sensitivity = 0.10f;

	wireframe = false;
}


void draw(SDL_Window* window)
{
	glClearColor(0.3, 0.3, 0.3, 1.0); // set background colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	// Create perspective projection matrix
	Settings* set = Settings::getInstance();
	glm::mat4 projection = set->getNowCamera()->getProjectionMatrix();
	glm::mat4 view = set->getNowCamera()->getViewMatrix();
	glm::mat4 model;

	//Shader lightingShader("simple.vert", "simple.frag");
	Shader lightCubeShader("light_cube.vert", "light_cube.frag");
	Shader* actualShader;

	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (int i = 0; i < set->getEntities().size(); i++)
	{
		model = set->getEntities().at(i)->getModelMatrix();
		actualShader = set->getEntities().at(i)->getShaderProgram();
		actualShader->use();
		actualShader->setMat4("projection", projection);
		actualShader->setMat4("view", view);
		actualShader->setMat4("model", model);
		vec3 center;
		float radio;
		set->getEntities().at(i)->getSphericalBounds(center, radio);
		if (set->getNowCamera()->intersectionSphereFrustum(center, radio))
		{
			set->getEntities().at(i)->draw();
		}
	}

	lightCubeShader.use();
	lightCubeShader.setMat4("projection", projection);
	lightCubeShader.setMat4("view", view);
	model = glm::mat4(1.0f);
	model = glm::translate(model, set->getLights()[0]->position);
	model = glm::scale(model, glm::vec3(0.2f));
	lightCubeShader.setMat4("model", model);
	set->getLights()[0]->drawLight();

	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	SDL_GL_SwapWindow(window); // swap buffers
}


void cleanup(void)
{
	glUseProgram(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	// could also detach shaders
	for(int i=0; i<Settings::getInstance()->getShaders().size();i++)
		glDeleteProgram(Settings::getInstance()->getShaders().at(i)->ID);
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, &vao);
}


int main(int argc, char *argv[]) {
	//INICIALIZACION
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return 1;
	}
	// SET ATTRIBUTE ONLY after initialize
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); // on antialiasing sdl
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); //subsamples for each pixel

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // set to 1 to require hardware acceleration

	SDL_Window *window = NULL;
	SDL_GLContext gl_context;

	window = SDL_CreateWindow("Ventana", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
																	800, 600, SDL_WINDOW_OPENGL);

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	gl_context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(0);
	printf("OpenGL loaded\n");
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	init();

	bool running = true; // set running to true
	SDL_Event sdlEvent;  // variable to detect SDL events

	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_WarpMouseInWindow(window, width / 2, height / 2);

	//para multiples teclas presionadas
	bool keys[6] = {false};

	while (running)		// the event loop
	{
		//Calculo del tiempo que pasa entre frame y frame
		last_time = current_time;
		current_time = SDL_GetTicks();
		float delta_time = (current_time - last_time) / 1000.f; //en segundos

		while (SDL_PollEvent(&sdlEvent))
		{
			switch (sdlEvent.type) 
			{
			case SDL_QUIT: 
				running = false;
				break;

			//se presiona tecla
			case SDL_KEYDOWN:
				switch (sdlEvent.key.keysym.sym) 
				{
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_w:
					keys[FRONT] = true;
					break;
				case SDLK_a:
					keys[LEFT] = true;
					break;
				case SDLK_s:
					keys[BACK] = true;
					break;
				case SDLK_d:
					keys[RIGHT] = true;
					break;
				case SDLK_SPACE:
					keys[UP] = true;
					break;
				case SDLK_LCTRL:
					keys[DOWN] = true;
					break;
				case SDLK_LEFT:
					//camera->moveCamera();
					break;
				}
				break;

			//se suelta una tecla
			case SDL_KEYUP:
				switch (sdlEvent.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_w:
					keys[FRONT] = false;
					break;
				case SDLK_a:
					keys[LEFT] = false;
					break;
				case SDLK_s:
					keys[BACK] = false;				
					break;
				case SDLK_d:
					keys[RIGHT] = false;
					break;
				case SDLK_SPACE:
					keys[UP] = false;
					break;
				case SDLK_LCTRL:
					keys[DOWN] = false;
					break;
				case SDLK_c:
					Settings::getInstance()->getNowCamera()->changeMode();
					break;
				case SDLK_l:
					wireframe = !wireframe;
					break;
				}
				break;
				
			case SDL_MOUSEMOTION:
				SDL_GetRelativeMouseState(&current_x, &current_y);
				float h_cant = -delta_time * sensitivity * current_x;
				float v_cant = -delta_time * sensitivity * current_y;
				Settings::getInstance()->getNowCamera()->moveCamera(h_cant, v_cant);
				SDL_WarpMouseInWindow(window, width * 0.5, height * 0.5);
				break;
			}
			//chequeo de teclas

			if (keys[FRONT]) Settings::getInstance()->getNowCamera()->updatePosition(delta_time * speed, movement_direction::FRONT);
			if (keys[LEFT]) Settings::getInstance()->getNowCamera()->updatePosition(delta_time * speed, movement_direction::LEFT);
			if (keys[BACK]) Settings::getInstance()->getNowCamera()->updatePosition(delta_time * speed, movement_direction::BACK);
			if (keys[RIGHT]) Settings::getInstance()->getNowCamera()->updatePosition(delta_time * speed, movement_direction::RIGHT);
			if (keys[UP]) Settings::getInstance()->getNowCamera()->updatePosition(delta_time * speed, movement_direction::UP);
			if (keys[DOWN]) Settings::getInstance()->getNowCamera()->updatePosition(delta_time * speed, movement_direction::DOWN);
		}

		draw(window); // call the draw function
	}

	cleanup();

	//FIN LOOP PRINCIPAL
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
