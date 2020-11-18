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
#include "Obligatorio2/Camera.h"
#include "Obligatorio2/Object.h"
#include "Obligatorio2/Plane.h"
#include "Obligatorio2/Settings.h"
#include "Obligatorio2/Light.h"
#include "Obligatorio2/Shader.h"

using namespace std;

// global variables - normally would avoid globals, using in this demo
GLuint vao, vbo[2]; // handles for our VAO and two VBOs
float r = 0;

unsigned int last_time, current_time;

//pos, target, up
Camera* camera;
float sensitivity;
float speed;

//mouse variables
int current_x, current_y, last_x, last_y;

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
	//Los objetos son asi: path, orientation, esc, pos, up, dir,numShader
	GLuint textu=set->addShader(Settings::initShaders("simple.vert", "simple.frag"));
	GLuint arco=set->addShader(Settings::initShaders("arcoiris.vert", "arcoiris.frag"));
	Object* o1 = new Object("modelos/12221_Cat_v1_l3.obj",glm::vec3(0, -1, 0), 0.8, glm::vec3(8, -0.6, -8.6),glm::vec3(0, 0, 1), glm::vec3(-1,0,-1), textu);
	Object* o2 = new Object("modelos/Japanese_Temple.obj", glm::vec3(0,0,-1),10, glm::vec3(0,4.2,0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), textu);
	//Manzana 1: plaza
	Plane* p1 = new Plane("modelos/asfalto.jpg", 100, 100, glm::vec3(20, 1, 20), glm::vec3(10, -1, -10), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), textu);//Calle
	Plane* p2 = new Plane("modelos/cordon.jpg", 20, 20, glm::vec3(19, 1, 19), glm::vec3(9.5, -0.9, -9.5), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), textu);//Cordon
	Plane* p3 = new Plane("modelos/cordon.jpg", 20, 1, glm::vec3(1, 0.09, 19), glm::vec3(9.5, -0.99, 9.5), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), textu);//Cordon
	Plane* p4 = new Plane("modelos/cordon.jpg", 20, 1, glm::vec3(19, 0.09, 1), glm::vec3(9.5, -0.99, -9.5), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1), textu);//Cordon
	Plane* p5 = new Plane("modelos/cordon.jpg", 20, 1, glm::vec3(1, 0.09, 19), glm::vec3(-9.5, -0.99, -9.5), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0), textu);//Cordon
	Plane* p6 = new Plane("modelos/cordon.jpg", 20, 1, glm::vec3(19, 0.09, 1), glm::vec3(-9.5, -0.99, 9.5), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), textu);//Cordon
	Plane* p7 = new Plane("modelos/vereda.jpg", 40, 40, glm::vec3(18.9, 1, 18.9), glm::vec3(9.45, -0.89, -9.45), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), textu);//Vereda
	Plane* p8 = new Plane("modelos/grass.jpg", 10, 10, glm::vec3(15, 1, 15), glm::vec3(7.5, -0.88, -7.5), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), textu);//Terreno

	set->addEntity(o1);
	set->addEntity(o2);
	set->addEntity(p1);
	set->addEntity(p2);
	set->addEntity(p3);
	set->addEntity(p4);
	set->addEntity(p5);
	set->addEntity(p6);
	set->addEntity(p7);
	set->addEntity(p8);

	 // Create and start shader program
	glEnable(GL_DEPTH_TEST); // enable depth testing
	//glEnable(GL_CULL_FACE); // enable back face culling - try this and see what happens!

	//init camera
	camera = new Camera(vec3(0.f, 0.f, -10.f), vec3(0.f), 45.f, 1.f, 1.f, 100.f);

	//init time
	last_time = 0;
	current_time = SDL_GetTicks();

	//init speed
	speed = 3.f;

	//init mouse variables
	SDL_GetMouseState(&current_x, &current_y);
	sensitivity = 0.005f;
}


void draw(SDL_Window* window)
{
	glClearColor(0.3, 0.3, 0.3, 1.0); // set background colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear window
	
	
	// Create perspective projection matrix
	glm::mat4 projection = camera->getProjectionMatrix();

	glm::mat4 view = camera->getViewMatrix();
	// Create view matrix for the camera
	//r += 0.001; //for camera rotation
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
	//view = glm::rotate(view, r, glm::vec3(0.0f, 1.0f, 0.0f));
	// Create model matrix for model transformations
	Settings* set = Settings::getInstance();
	for (int i = 0; i < set->getEntities().size(); i++)
	{
		glm::mat4 model = set->getEntities().at(i)->getModelMatrix();
		GLuint sp = set->getEntities().at(i)->getShaderProgram();
		glUseProgram(sp);
		int projectionIndex = glGetUniformLocation(sp, "projection");
		glUniformMatrix4fv(projectionIndex, 1, GL_FALSE, glm::value_ptr(projection));
		// pass model as uniform into shader
		int viewIndex = glGetUniformLocation(sp, "view");
		glUniformMatrix4fv(viewIndex, 1, GL_FALSE, glm::value_ptr(view));
		// pass model as uniform into shader
		int modelIndex = glGetUniformLocation(sp, "model");
		glUniformMatrix4fv(modelIndex, 1, GL_FALSE, glm::value_ptr(model));
		
		Shader lightingShader("simple.vert", "simple.frag");
		Shader lightCubeShader("light_cube.vert", "light_cube.frag");

		Light light1 = Light(vec3(0, 0, 10));

		lightingShader.use();
		lightingShader.setVec3("lightColor", light1.color);
		lightingShader.setVec3("lightPos", light1.position);
		lightingShader.setVec3("viewPos", camera->getPosition());
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);

		set->getEntities().at(i)->draw();

		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, light1.position);
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubeShader.setMat4("model", model);
		light1.drawLight();
	}
	
	
	//model= glm::rotate(model, r, glm::vec3(0.0f, 1.0f, 0.0f));
	// pass model as uniform into shader
	SDL_GL_SwapWindow(window); // swap buffers
}


void cleanup(void)
{
	glUseProgram(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	// could also detach shaders
	for(int i=0; i<Settings::getInstance()->getShaders().size();i++)
		glDeleteProgram(Settings::getInstance()->getShaders().at(i));
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, &vao);
}


int main(int argc, char *argv[]) {
	//INICIALIZACION
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	SDL_Window *window = NULL;
	SDL_GLContext gl_context;

	window = SDL_CreateWindow("Ventana", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
																	800, 800, SDL_WINDOW_OPENGL);

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
					camera->changeMode();
					break;
				}
				break;
				
			case SDL_MOUSEMOTION:
				SDL_WarpMouseInWindow(window, width * 0.5, height * 0.5);
				last_x = current_x;
				last_y = current_y;
				SDL_GetMouseState(&current_x, &current_y);
				float h_cant = delta_time * sensitivity * (width * 0.5 - current_x);
				float v_cant = delta_time * sensitivity * (current_y - height * 0.5);
				camera->moveCamera(h_cant, v_cant);
				break;
			}
			//chequeo de teclas
			
			if (keys[FRONT]) camera->updatePosition(delta_time * speed, movement_direction::FRONT);
			if (keys[LEFT]) camera->updatePosition(delta_time * speed, movement_direction::LEFT);
			if (keys[BACK]) camera->updatePosition(delta_time * speed, movement_direction::BACK);
			if (keys[RIGHT]) camera->updatePosition(delta_time * speed, movement_direction::RIGHT);
			if (keys[UP]) camera->updatePosition(delta_time * speed, movement_direction::UP);
			if (keys[DOWN]) camera->updatePosition(delta_time * speed, movement_direction::DOWN);
		}

		//update();
		draw(window); // call the draw function
	}

	cleanup();

	//FIN LOOP PRINCIPAL
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
