#include "Settings.h"

Settings::Settings()
{
	setTerrainTexture();
}

Settings::~Settings()
{
}
Settings* Settings::instance = nullptr;
int Settings::init_time = SDL_GetTicks();

Settings* Settings::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Settings();
		init_time = SDL_GetTicks();
    }
    return instance;
}

std::vector<Entity*> Settings::getEntities()
{
    return entities;
}

std::vector<Shader*> Settings::getShaders()
{
    return shaders;
}

Shader* Settings::addShader(Shader* s)
{
    shaders.push_back(s);
    return s;
}

void Settings::addEntity(Entity* e)
{
    entities.push_back(e);

	//actualizar bound
	vec3 min_ent, max_ent;
	e->getBounds(min_ent, max_ent);

	if (entities.size() == 1)
	{
		min_bound = min_ent;
		max_bound = max_ent;
	}
	else
	{
		if (min_ent.x < min_bound.x) min_bound.x = min_ent.x;
		if (min_ent.y < min_bound.y) min_bound.y = min_ent.y;
		if (min_ent.z < min_bound.z) min_bound.z = min_ent.z;
		if (max_ent.x > max_bound.x) max_bound.x = max_ent.x;
		if (max_ent.y > max_bound.y) max_bound.y = max_ent.y;
		if (max_ent.z > max_bound.z) max_bound.z = max_ent.z;
	}
}

// printShaderError
// Display (hopefully) useful error messages if shader fails to compile or link
void Settings::printShaderError(GLint shader)
{
	int maxLength = 0;
	int logLength = 0;
	GLchar* logMessage;

	// Find out how long the error message is
	if (glIsShader(shader))
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
	else
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	if (maxLength > 0) // If message has length > 0
	{
		logMessage = new GLchar[maxLength];
		if (glIsShader(shader))
			glGetProgramInfoLog(shader, maxLength, &logLength, logMessage);
		else
			glGetShaderInfoLog(shader, maxLength, &logLength, logMessage);
		cout << "Shader Info Log:" << endl << logMessage << endl;
		delete[] logMessage;
	}
}


GLuint Settings::initShaders(const char* vertFile, const char* fragFile)
{
	GLuint p, f, v;	// Handles for shader program & vertex and fragment shaders

	v = glCreateShader(GL_VERTEX_SHADER); // Create vertex shader handle
	f = glCreateShader(GL_FRAGMENT_SHADER);	// " fragment shader handle

	const char* vertSource = loadFile(vertFile); // load vertex shader source
	const char* fragSource = loadFile(fragFile);  // load frag shader source

	// Send the shader source to the GPU
	// Strings here are null terminated - a non-zero final parameter can be
	// used to indicate the length of the shader source instead
	glShaderSource(v, 1, &vertSource, 0);
	glShaderSource(f, 1, &fragSource, 0);

	GLint compiled, linked; // return values for checking for compile & link errors

	// compile the vertex shader and test for errors
	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		cout << "Vertex shader not compiled." << endl;
		printShaderError(v);
	}

	// compile the fragment shader and test for errors
	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		cout << "Fragment shader not compiled." << endl;
		printShaderError(f);
	}

	p = glCreateProgram(); 	// create the handle for the shader program
	glAttachShader(p, v); // attach vertex shader to program
	glAttachShader(p, f); // attach fragment shader to program

	glBindAttribLocation(p, 0, "in_Position"); // bind position attribute to location 0
	glBindAttribLocation(p, 1, "in_Color"); // bind color attribute to location 1

	glLinkProgram(p); // link the shader program and test for errors
	glGetProgramiv(p, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		cout << "Program not linked." << endl;
		printShaderError(p);
	}

	//glUseProgram(p);  // Make the shader program the current active program

	delete[] vertSource; // Don't forget to free allocated memory
	delete[] fragSource; // We allocated this in the loadFile function...

	return p; // Return the shader program handle
}

const char* Settings::loadFile(const char* fname)
{
	int size;
	char* memblock = NULL;

	// file read based on example in cplusplus.com tutorial
	// ios::ate opens file at the end
	ifstream file(fname, ios::in | ios::binary | ios::ate);
	if (file.is_open())
	{
		size = (int)file.tellg(); // get location of file pointer i.e. file size
		memblock = new char[size + 1]; // create buffer with space for null char
		file.seekg(0, ios::beg);
		file.read(memblock, size);
		file.close();
		memblock[size] = 0;
		cout << "file " << fname << " loaded" << endl;
	}
	else
	{
		cout << "Unable to open file " << fname << endl;
		// should ideally set a flag or use exception handling
		// so that calling function can decide what to do now
	}
	return memblock;
}

void Settings::getBounds(vec3& min, vec3& max)
{
	min = min_bound;
	max = max_bound;
}

void Settings::changeNowCamera(Camera* c)
{
	now_camera = c;
}

void Settings::setTerrainTexture()
{
	//Harcodeo las imagenes y los top, despues cuando se cargue desde el xml va a quedar mejor
	vector<FIBITMAP*> imagenes;
	vector<std::string> paths;
	vector<int> tops;
	paths.push_back("modelos/isla2.jpg");
	paths.push_back("modelos/montania.jpg");
	paths.push_back("modelos/isla3.jpg");
	tops.push_back(8);
	tops.push_back(45);
	tops.push_back(60);

	for (int i = 0; i < paths.size(); i++)
	{
		FIBITMAP* imagen;
		string filename = paths.at(i);
		imagen = FreeImage_Load(
			FreeImage_GetFileType(filename.c_str(), 0),
			filename.c_str());
		imagenes.push_back(imagen);
	}
	unsigned w = FreeImage_GetWidth(imagenes.at(0)); //Todas miden lo mismo
	unsigned h = FreeImage_GetHeight(imagenes.at(0));
	terrainTexture= FreeImage_Allocate(w, h, 24);
	for (int j = 0; j < w; j++)
	{
		for (int k = 0; k < h; k++)
		{
			RGBQUAD maxColor;
			bool primeraVez = true;
			for (int l = 0; l < imagenes.size(); l++)
			{
				RGBQUAD col;
				if (primeraVez)
				{
					primeraVez = false;
					FreeImage_GetPixelColor(imagenes.at(l), (int)(j), (int)(k), &col);
					maxColor.rgbRed = (int)((float)(col.rgbRed * tops.at(l)) / (float)60.0);
					maxColor.rgbGreen = 0;//Ojo con esto, lleno solo el rojo
					maxColor.rgbBlue = 0;
				}
				else
				{
					FreeImage_GetPixelColor(imagenes.at(l), (int)(j), (int)(k), &col);
					if ((float)(col.rgbRed * tops.at(l)) / (float)60.0 > maxColor.rgbRed)
					{
						maxColor.rgbRed = (int)((float)(col.rgbRed * tops.at(l)) / (float)60.0);
					}
				}
					
			}
			FreeImage_SetPixelColor(terrainTexture, j, k, &maxColor);
		}
	}
	if (FreeImage_Save(FIF_JPEG, terrainTexture, "terrenoFinal.jpg", 0)) {
		cout << "Image saved" << endl;
	}
	for (int i = 0; i < imagenes.size(); i++)
	{
		FreeImage_Unload(imagenes.at(i));
	}
}

float Settings::getHeightTerrain(float x, float z)
{
	//Voy a considerar que la escena esta entre x=0, x=200 y z=0, z=-200
	RGBQUAD color;
	unsigned w = FreeImage_GetWidth(terrainTexture); 
	unsigned h = FreeImage_GetHeight(terrainTexture);
	bool col = FreeImage_GetPixelColor(terrainTexture, (int)((x/200.0)*w), (int)((z/-200.0)*h), &color);
	//Considero que el top de todo es 60, por lo que para poner los colores, tengo que saber que despues lo voy a multiplicar por 60
	if (col)
		return (color.rgbRed / 255.0) * 60;
	else
		return -1;
}

Camera* Settings::getNowCamera()
{
	return now_camera;
}

std::vector<Light*> Settings::getLights()
{
	return lights;
}

void Settings::addLight(Light* l)
{
	lights.push_back(l);
}

int Settings::getInitTime()
{
	return init_time;
}

unsigned int Settings::TextureFromFile(const char* path) {
	string filename = string(path);
	FIBITMAP* bitmap = FreeImage_Load(
		FreeImage_GetFileType(filename.c_str(), 0),
		filename.c_str());

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
	int nWidth = FreeImage_GetWidth(pImage);
	int nHeight = FreeImage_GetHeight(pImage);

	if (pImage)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
			0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		FreeImage_Unload(pImage);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		FreeImage_Unload(pImage);
	}

	return textureID;
}

vec3 Settings::clampToScene(vec3 point) {
	if (point.x < min_bound.x) point.x = min_bound.x;
	if (point.z < min_bound.z) point.z = min_bound.z;
	if (point.x > max_bound.x) point.x = max_bound.x;
	if (point.z > max_bound.z) point.z = max_bound.z;
	return point;
}

void Settings::SetLightsToShader(Shader* shader) {

	//// directional light
	//shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	//shader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	//shader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	//shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	
	// point light 1
	shader->setVec3("pointLights[0].position", Settings::getInstance()->getLights()[0]->position);
	shader->setVec3("pointLights[0].color", Settings::getInstance()->getLights()[0]->color);
	shader->setFloat("pointLights[0].constant", 0.0f);
	shader->setFloat("pointLights[0].linear", 0.04f);
	shader->setFloat("pointLights[0].quadratic", 0.0f);

	// point light 2
	shader->setVec3("pointLights[1].position", Settings::getInstance()->getLights()[1]->position);
	shader->setVec3("pointLights[1].color", Settings::getInstance()->getLights()[1]->color);
	shader->setFloat("pointLights[1].constant", 0.0f);
	shader->setFloat("pointLights[1].linear", 0.04f);
	shader->setFloat("pointLights[1].quadratic", 0.0f);

	//// point light 3
	shader->setVec3("pointLights[2].position", Settings::getInstance()->getLights()[2]->position);
	shader->setVec3("pointLights[2].color", Settings::getInstance()->getLights()[2]->color);
	shader->setFloat("pointLights[2].constant", 0.0f);
	shader->setFloat("pointLights[2].linear", 0.04f);
	shader->setFloat("pointLights[2].quadratic", 0.0f);
	
	//// point light 4
	//shader->setVec3("pointLights[3].position", Settings::getInstance()->getLights()[3]->position);
	//shader->setVec3("pointLights[3].color", Settings::getInstance()->getLights()[3]->color);
	//shader->setFloat("pointLights[3].constant", 0.0f);
	//shader->setFloat("pointLights[3].linear", 0.04f);
	//shader->setFloat("pointLights[3].quadratic", 0.0f);
}

bool Settings::colliding(vec3 bound_min, vec3 bound_max)
{
	for (int i = 0; i < entities.size(); i++) if (entities[i]->intersectionBoxBounds(bound_min, bound_max)) return true;
	return false;
}