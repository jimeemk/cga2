#include "Settings.h"

Settings::Settings()
{
    
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
