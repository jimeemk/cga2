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
#include "Obligatorio2/Skybox.h"
#include "Obligatorio2/Water.h"
#include "Obligatorio2/Terrain.h"
#include "Obligatorio2/Xml.h"
#include <time.h>
using namespace std;

// global variables - normally would avoid globals, using in this demo
GLuint vao, vbo[2]; // handles for our VAO and two VBOs
float r = 0;
unsigned int last_time, current_time;
Entity* newObj;

//pos, target, up
float sensitivity;
float speed;

//mouse variables
int current_x, current_y, last_x, last_y;

bool wireframe;
bool draw_bounds;
bool panoramic;

Shader* lightShader;
Shader* material;
Shader* anim;
Skybox* skybox;

Camera* camera;
Camera* camera2;


// Something went wrong - print SDL error message and quit
void exitFatalError(char* message)
{
	cout << message << " ";
	cout << SDL_GetError();
	SDL_Quit();
	exit(1);
}


unsigned int loadCubemap(vector<std::string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(faces[i].c_str(), 0), faces[i].c_str());
		FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
		int width = FreeImage_GetWidth(pImage);
		int height = FreeImage_GetHeight(pImage);

		if (pImage) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*) FreeImage_GetBits(pImage));
			FreeImage_Unload(pImage);
		} else {
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			FreeImage_Unload(pImage);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


void init(void)
{
	Settings* settings = Settings::getInstance();
	
	lightShader = new Shader("simple.vert", "simple.frag");
	Shader* waterShader = new Shader("water.vert", "water.frag", "water.geom");
	Shader* hmShader = new Shader("heightMap.vert", "heightMap.frag");
	Shader* suelo = new Shader("suelo.vert", "suelo.frag");
	material = new Shader("material.vert", "material.frag");
	anim = new Shader("animated_model.vert", "animated_model.frag");
	
	settings->addShader(lightShader);
	settings->addShader(material);
	settings->addShader(anim);
	settings->addShader(waterShader);
	settings->addShader(hmShader);
	settings->addShader(suelo);
	
	AnimatedObject* policia = new AnimatedObject("models/policia/Dancing Twerk.dae", glm::vec3(0, 0, -1), 2, glm::vec3(172.5, 34, -189.3), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1), anim);
	AnimatedObject* zombie1 = new AnimatedObject("models/zombie2/Thriller Part 1.dae", glm::vec3(0, 0, -1), 2.5, glm::vec3(158.7, 33.8, -168), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), anim);
	//AnimatedObject* zombie2 = new AnimatedObject("models/zombie/Thriller Part 3.dae", glm::vec3(0, 0, -1), 3, glm::vec3(162.5, 34, -165), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), anim);
	Water* water = new Water(vec3(-60.f, 0.5f, 60.f), vec3(0.f, 1.f, 0.f), vec3(1.f, 0.f, 0.f), waterShader, 35, 1520, 1520, "modelos/water1.jpg");
	std::vector<Texture> text;
	Texture t2 = { Settings::TextureFromFile("modelos/grass2.jpg"), "texture_diffuse", "modelos/grass2.jpg" };
	text.push_back(t2);
	Texture t3 = { Settings::TextureFromFile("modelos/arena.jpg"), "texture_diffuse", "modelos/arena.jpg" };
	text.push_back(t3);
	Terrain* terrain = new Terrain(vec3(0.f), vec3(0.f, 1.f, 0.f), vec3(1.f, 0.f, 0.f), suelo, 50, 400, 400,38, text, "modelos/parteAdentro8.jpg");
	Terrain* terrain2 = new Terrain(vec3(0.f), vec3(0.f, 1.f, 0.f), vec3(1.f, 0.f, 0.f), hmShader, 50, 400, 400, 30, text, "modelos/parteAfuera.jpg");
	Terrain* terrain3 = new Terrain(vec3(0.f), vec3(0.f, 1.f, 0.f), vec3(1.f, 0.f, 0.f), hmShader, 50, 400, 400, 120, text, "modelos/m1.jpg");
	Terrain* terrain4 = new Terrain(vec3(0.f), vec3(0.f, 1.f, 0.f), vec3(1.f, 0.f, 0.f), hmShader, 50, 400, 400, 130, text, "modelos/m2.jpg");
	Terrain* terrain5 = new Terrain(vec3(0.f), vec3(0.f, 1.f, 0.f), vec3(1.f, 0.f, 0.f), hmShader, 50, 400, 400, 150, text, "modelos/m3.jpg");
	Terrain* terrain6 = new Terrain(vec3(0.f), vec3(0.f, 1.f, 0.f), vec3(1.f, 0.f, 0.f), hmShader, 50, 400, 400, 140, text, "modelos/m4.jpg");
	Terrain* terrain7 = new Terrain(vec3(0.f), vec3(0.f, 1.f, 0.f), vec3(1.f, 0.f, 0.f), hmShader, 50, 400, 400, 100, text, "modelos/m5.jpg");
	//Autos


	std::vector<Texture> text2;
	//Texture ta = { Settings::TextureFromFile("modelos/montania.jpg"), "texture_height", "modelos/montania.jpg" };
	//text2.push_back(ta);
	text2.push_back(t2);
	Texture t4 = { Settings::TextureFromFile("modelos/roca.jpg"), "texture_diffuse", "modelos/roca.jpg" };
	text2.push_back(t4);
	Terrain* terrainb = new Terrain(vec3(0.f), vec3(0.f, 1.f, 0.f), vec3(1.f, 0.f, 0.f), hmShader, 50, 200, 200, 60, text2, "modelos/montania.jpg");

	std::vector<Texture> text3;
	//Texture tc = { Settings::TextureFromFile("modelos/isla3.jpg"), "texture_height", "modelos/isla3.jpg" };
	//text3.push_back(tc);
	text3.push_back(t2);
	text3.push_back(t4);
	Terrain* terrainc = new Terrain(vec3(0.f), vec3(0.f, 1.f, 0.f), vec3(1.f, 0.f, 0.f), hmShader, 50, 200, 200, 75, text3, "modelos/isla3.jpg");


	newObj = new Object("modelos/vehiculos/car-kit-3d-model/Models/OBJ format/van.obj", glm::vec3(0, 0, 1), 3, glm::vec3(193, 33.29999, -191.6), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), material, false);
	Object* newObj2 = new Object("modelos/vehiculos/car-kit-3d-model/Models/OBJ format/suv.obj", glm::vec3(0, 0, 1), 3, glm::vec3(178, 33.29999, -161.6), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0), material, false);
	Object* newObj3 = new Object("modelos/vehiculos/car-kit-3d-model/Models/OBJ format/sedan.obj", glm::vec3(0, 0, 1), 3, glm::vec3(178, 33.29999, -206.6), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0), material, false);
	Object* newObj4 = new Object("modelos/vehiculos/car-kit-3d-model/Models/OBJ format/taxi.obj", glm::vec3(0, 0, 1), 3, glm::vec3(223, 33.29999, -161.6), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0), material, false);

	settings->addCar(newObj);
	settings->addCar(newObj2);
	settings->addCar(newObj3);
	settings->addCar(newObj4);
	settings->addEntity(newObj2);
	settings->addEntity(newObj3);
	settings->addEntity(newObj4);
	settings->addEntity(zombie1);
	settings->addEntity(policia);

	settings->addEntity(water);
	settings->addEntity(terrain);
	settings->addEntity(terrain2);
	settings->addEntity(terrain3);
	settings->addEntity(terrain4);
	settings->addEntity(terrain5);
	settings->addEntity(terrain6);
	settings->addEntity(terrain7);
	//set->addEntity(terrainc);
	settings->addEntity(newObj);

	std::cout << "Total entities: " << settings->getEntities().size() << std::endl;
	loadXMLEntities("xml/objetos.xml");
	
	glEnable(GL_DEPTH_TEST); // enable depth testing
	//glEnable(GL_CULL_FACE); // enable back face culling - try this and see what happens!

	//init camera
	camera = new Camera(vec3(210.f, 60.f, -200.f), half_pi<float>(), 0.f, 65.f, 4.0f / 3.0f, 0.01f, 500.f, 10.f, false);
	camera2 = new Camera(vec3(210.f, 200.f, -200.f), half_pi<float>(), 0.f, 65.f, 4.0f / 3.0f, 0.01f, 500.f, 10.f, true);
	Light* light1 = new Light(vec3(185,80, -206));
//	Light* light1 = new Light(vec3(0, 0, 0));
	Light* light2 = new Light(vec3(50, 50, -100));
	Light* light3 = new Light(vec3(100, 25, -150));
	Light* light4 = new Light(vec3(-500, 20, -30));
	skybox = new Skybox();

	settings->changeNowCamera(camera);
	settings->addLight(light1);
	settings->addLight(light2);
	settings->addLight(light3);
	//set->addLight(light4);
	panoramic = false;
	//init time
	last_time = 0;
	current_time = SDL_GetTicks();

	//init speed
	speed = 10.f;

	//init mouse variables
	sensitivity = 0.10f;

	wireframe = false;
	draw_bounds = false;
}


void drawLights(glm::mat4 projection, glm::mat4 view) {
	Settings* settings = Settings::getInstance();
	Shader lightCubeShader("light_cube.vert", "light_cube.frag");
	glm::mat4 model;

	for (int i = 0; i < settings->getLights().size(); i++) {
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, settings->getLights()[i]->position);
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubeShader.setMat4("model", model);
		settings->getLights()[i]->drawLight();
	}
}


void draw(SDL_Window* window)
{
	glClearColor(0.0, 0.66, 0.89, 1.0); // set background colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	// Create perspective projection matrix
	Settings* set = Settings::getInstance();
	glm::mat4 projection = set->getNowCamera()->getProjectionMatrix();
	glm::mat4 view = set->getNowCamera()->getViewMatrix();
	glm::mat4 model;

	Shader* actualShader;

	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (int i = 0; i < set->getEntities().size(); i++)
	{
		model = set->getEntities().at(i)->getModelMatrix();
		actualShader = set->getEntities().at(i)->getShaderProgram();
		actualShader->use();
		actualShader->setMat4("projection", projection);
		actualShader->setMat4("view", view);
		
		if (!draw_bounds) {
			actualShader->setMat4("model", model);
		} else {
			actualShader->setMat4("model", mat4(1.f));
		}
		
		vec3 center;
		float radio;
		set->getEntities().at(i)->getSphericalBounds(center, radio);
		if (set->getNowCamera()->intersectionSphereFrustum(center, radio) || true)
		{
			if (!draw_bounds) set->getEntities().at(i)->draw();
			else set->getEntities()[i]->drawBounds();
		}
	}

	drawLights(projection, view);
	//skybox->Draw(view, projection);

	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	SDL_GL_SwapWindow(window); // swap buffers

	//vec3 camPos=set->getNowCamera()->getPosition();
	//cout << "Camara en" << camPos.x << ";" << camPos.z << "\n";
	//cout << "Altura en este lugar:" << set->getHeightTerrain(camPos.x, camPos.z) << "\n";
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
	delete skybox;
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

	window = SDL_CreateWindow("Ventana", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);

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
	
	while (running)
	{
		//Calculo del tiempo que pasa entre frame y frame
		last_time = current_time;
		current_time = SDL_GetTicks();
		float delta_time = (current_time - last_time) / 1000.f; //en segundos
		Settings* set = Settings::getInstance();
		for (int k = 0; k < set->getCars().size(); k++)
		{
			dynamic_cast<Object*>(set->getCars().at(k))->runCar(delta_time*speed);
		}
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
				case SDLK_r:
					newObj->aumentarX();
					break;
				case SDLK_t:
					newObj->aumentarY();
					break;
				case SDLK_y:
					newObj->aumentarZ();
					break;
				case SDLK_f:
					newObj->disminuirX();
					break;
				case SDLK_g:
					newObj->disminuirY();
					break;
				case SDLK_h:
					newObj->disminuirZ();
					break;
				case SDLK_u:
					newObj->rotarMasX();
					break;
				case SDLK_i:
					newObj->rotarMasY();
					break;
				case SDLK_o:
					newObj->rotarMasZ();
					break;
				case SDLK_j:
					newObj->rotarMenosX();
					break;
				case SDLK_k:
					newObj->rotarMenosY();
					break;
				case SDLK_l:
					newObj->rotarMenosZ();
					break;
				case SDLK_1:
					newObj->escMasX();
					break;
				case SDLK_2:
					newObj->escMenosX();
					break;
				case SDLK_3:
					newObj->escMasY();
					break;
				case SDLK_4:
					newObj->escMenosY();
					break;
				case SDLK_5:
					newObj->escMasZ();
					break;
				case SDLK_6:
					newObj->escMenosZ();
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
				case SDLK_z:
					wireframe = !wireframe;
					break;
				case SDLK_b:
					draw_bounds = !draw_bounds;
					break;
				case SDLK_7:
					newObj->guardarEntity();
					saveXMLEntities("xml/objetos.xml");
					newObj = new Object("modelos/vehiculos/watercraft-pack-3d-model/Models/watercraftPack_024.obj", glm::vec3(1, 0, 0), dynamic_cast<Object*>(newObj)->getScale(), newObj->getPosition(), glm::vec3(0, 1, 0), dynamic_cast<Object*>(newObj)->getDirection(), material, false);
					set->addEntity(newObj);
					break;
				case SDLK_0:
					panoramic = !panoramic;
					if (panoramic)
					{
						Settings::getInstance()->getNowCamera()->changeMode();
						camera2 = Settings::getInstance()->getNowCamera();
						vec3 exPos = camera2->getPosition();
						exPos.y = 100;
						camera2->setPosition(exPos);
						set->changeNowCamera(camera2);
					}
					else {
						Settings::getInstance()->getNowCamera()->changeMode();
						set->changeNowCamera(camera);
						Settings::getInstance()->getNowCamera()->updatePosition(delta_time * speed, movement_direction::FRONT);
						Settings::getInstance()->getNowCamera()->updatePosition(delta_time * speed, movement_direction::BACK);
					}
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
