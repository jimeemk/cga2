#include "Object.h"
#include <time.h>

Object::Object(string p, glm::vec3 ori, float esc, glm::vec3 pos, glm::vec3 u, glm::vec3 dir, Shader* sh, bool ht) : Entity(pos,u,dir,sh)
{
    path = p;
    orientation = ori;
    scale = esc;
    hasTexture = ht;
    ultimaEsquina = glm::vec2(pos.x,pos.z);
    avance = 0.f;
    modelMatrix = mat4(1.f);
    model = new Model(p);
    initObject();

}

Object::~Object()
{
}

glm::vec3 Object::getPosition()
{
    return position;
}

glm::vec3 Object::getDirection()
{
    return direction;
}

glm::vec3 Object::getUp()
{
    return up;
}

glm::vec3 Object::getOrientation()
{
    return orientation;
}

glm::mat4 Object::getModelMatrix()
{
    return modelMatrix;
}

bool Object::getHasTexture()
{
    return hasTexture;
}

float Object::getScale()
{
    return scale;
}

string Object::getPath()
{
    return path;
}

Shader* Object::getShaderProgram()
{
    return shaderProgram;
}

void Object::draw()
{
    Settings::SetLightsToShader(shaderProgram);
    shaderProgram->setVec3("viewPos", Settings::getInstance()->getNowCamera()->getPosition());
    model->Draw(shaderProgram->ID);
}

void Object::initObject()
{
    glm::vec3 centro = glm::vec3(0.0);
    min = model->min;
    max = model->max;
    centro.x = (min.x + max.x) / 2;
    centro.y = (min.y + max.y) / 2;
    centro.z = (min.z + max.z) / 2;

    pointsInitialAABB[0] = glm::vec3(min.x, min.y, min.z);
    pointsInitialAABB[1] = glm::vec3(min.x, min.y, max.z);
    pointsInitialAABB[2] = glm::vec3(min.x, max.y, min.z);
    pointsInitialAABB[3] = glm::vec3(min.x, max.y, max.z);
    pointsInitialAABB[4] = glm::vec3(max.x, min.y, min.z);
    pointsInitialAABB[5] = glm::vec3(max.x, min.y, max.z);
    pointsInitialAABB[6] = glm::vec3(max.x, max.y, min.z);
    pointsInitialAABB[7] = glm::vec3(max.x, max.y, max.z);

    
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position.x, position.y, position.z));
    rotation();
    float maxFac = getMaxScale(max.x-min.x,max.y-min.y, max.z-min.z);
    modelMatrix = glm::scale(modelMatrix,glm::vec3(scale/maxFac, scale / maxFac, scale / maxFac));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-centro.x, -centro.y, -centro.z));//Por si el objeto no viene en el origen
    calcBounds();
    glm::vec3 maxAux;
    glm::vec3 minAux;
    getBounds(minAux,maxAux);//Esto no va aca, es para probar
}
float Object::getMaxScale(float x, float y, float z)
{
    if (x > y)
    {
        if (x > z)
            return x;
        else
            return z;
    }
    else
    {
        if (y > z)
            return y;
        else
            return z;
    }
}
void Object::rotation()
{
    glm::vec3 initialDir = orientation;
    float tita1, tita2 = 0;
    glm::vec3 dir1,dir2=glm::vec3(1,0,0);
    glm::vec3 upv = up;
    if (glm::cross(initialDir, direction)!=glm::vec3(0.0))//Verifico si de entrada son iguales
    {
        if (initialDir.x != 0 || initialDir.z != 0)//La primera rotacion la hago por eje x o y dependiendo cual conviene. Por ejemplo, si initialDir es 1 0 0, no me sirve rotar en x.
        {
            dir1 = glm::vec3(0.0, 1.0, 0.0);//Segun el eje y
            tita1 = glm::orientedAngle(normalize(initialDir), normalize(direction), dir1);
        }
        else if (initialDir.y != 0 || initialDir.z != 0)
        {
            dir1 = glm::vec3(1.0, 0.0, 0.0);//Segun el eje x
            tita1 = glm::orientedAngle(normalize(initialDir), normalize(direction), dir1);
        }

        upv = glm::rotate(upv, tita1, dir1);//Actualizo up
        initialDir = glm::rotate(initialDir, tita1, dir1);//Actualizo vector "initialDir"

        if (glm::cross(initialDir, direction) != glm::vec3(0.0))//Verifico que ya no esten paralelos
        {
            dir2 = normalize(glm::cross(initialDir, direction));//En el paso anterior, pase mi problema 3d a uno 2d, ahora en ese plano (calculando el vector perpendicular con producto cruz y tomandolo como eje de referencia), voy a calcular el angulo entre los dos vectores
            tita2 = glm::orientedAngle(normalize(initialDir), normalize(direction), dir2);
            upv = glm::rotate(upv, tita2, dir2);
            initialDir = glm::rotate(initialDir, tita2, glm::cross(initialDir, direction));
        }
    }
    
    if (glm::cross(direction, upv) != glm::vec3(0.0))//Me cubro del caso dir= 0,1,0
    {
        float titaFinal = glm::orientedAngle(normalize(upv),glm::vec3(0, 1, 0), normalize(direction));//Esto es para alinear el up con el (0,1,0)
        modelMatrix = glm::rotate(modelMatrix, titaFinal, normalize(direction));
        upv = glm::rotate(upv, titaFinal, normalize(direction));
    }

    if (glm::dot(initialDir,direction)<0)//Si en realidad obtuve un vector opuesto, es decir, estoy mirando para el otro lado
    {
        if (glm::cross(initialDir, glm::vec3(0, 0, 1)) != glm::vec3(0.0))
        {
            modelMatrix = glm::rotate(modelMatrix, glm::pi<float>(), glm::vec3(0, 0, 1));
        }
        else
        {
            modelMatrix = glm::rotate(modelMatrix, glm::pi<float>(), glm::vec3(0, 1, 0));
        }
    }
    modelMatrix = glm::rotate(modelMatrix, tita2, dir2);
    modelMatrix = glm::rotate(modelMatrix, tita1, dir1);//Recordar que esta rotacion se hace primero

}


void Object::aumentarX() //r
{
    modelMatrix = mat4(1.f);
    position.x += 0.05;
    initObject();
}

void Object::disminuirX() //f
{
    modelMatrix = mat4(1.f);
    position.x -= 0.05;
    initObject();
}

void Object::aumentarY() //t
{
    modelMatrix = mat4(1.f);
    position.y += 0.2;
    initObject();
}

void Object::disminuirY()//g
{
    modelMatrix = mat4(1.f);
    position.y -= 0.1;
    initObject();
}

void Object::aumentarZ()//y
{
    modelMatrix = mat4(1.f);
    position.z += 0.05;
    initObject();
}

void Object::disminuirZ() //h
{
    modelMatrix = mat4(1.f);
    position.z -= 0.05;
    initObject();
}

void Object::rotarMasX() //u
{
    modelMatrix = mat4(1.f);
    direction.x += 0.2;
    initObject();
}

void Object::rotarMenosX()//j
{
    modelMatrix = mat4(1.f);
    direction.x -= 0.2;
    initObject();
}

void Object::rotarMasY()//i
{
    modelMatrix = mat4(1.f);
    direction.y += 0.2;
    initObject();
}

void Object::rotarMenosY() //k
{
    modelMatrix = mat4(1.f);
    direction.y -= 0.2;
    initObject();
}

void Object::rotarMasZ()//o
{
    modelMatrix = mat4(1.f);
    direction.z += 0.2;
    initObject();
}

void Object::rotarMenosZ()//l
{
    modelMatrix = mat4(1.f);
    direction.z -= 0.2;
    initObject();
}

void Object::escMasX()//1
{
    modelMatrix = mat4(1.f);
    scale += 0.2;
    initObject();
}

void Object::escMenosX()//2
{
    modelMatrix = mat4(1.f);
    scale -= 0.2;
    cout <<"Scale: "<< scale <<'\n';
    initObject();
}

void Object::escMasY()//3
{
    modelMatrix = mat4(1.f);
    initObject();
}

void Object::escMenosY()//4
{
    modelMatrix = mat4(1.f);
    initObject();
}

void Object::escMasZ()//5
{
    modelMatrix = mat4(1.f);
    initObject();
}

void Object::escMenosZ()//6
{
    modelMatrix = mat4(1.f);
    initObject();
}

void Object::guardarEntity()//7
{
    cout << "GUARDAR: position:" << position.x<< "; " << position.y << "; " << position.z << "; \n";
    cout << "dir:" << direction.x << "; " << direction.y << "; " << direction.z << "; \n";
    cout << "escala:" << scale << "; \n";

}

bool Object::esEsquina()
{
    if (position.x - floor(position.x) < 0.01 && (position.z+0.6)*-1 - floor((position.z+0.6)*-1) < 0.01)
    {
        if (int((position.x) - 118) % 15 == 0 && int((position.z + 0.6) + 161) % 15 == 0)
        {
            return true;
            cout << "Es esquina \n";
        }
    }
    return false;
}

void Object::runCar(float meters)
{
    float avanzado = 0;
        if (normalize(direction) == vec3(1, 0, 0))
        {
            if (avance+0.05>15 || avance==0) //si estoy en una esquina, tomo una decision
            {
                if (avance != 0)
                {
                    position.x = ultimaEsquina.x + 15; //Llegue a la esquina en +x
                    position.z = ultimaEsquina.y;
                    ultimaEsquina.x = position.x;
                    ultimaEsquina.y = position.z;
                }
                int num = 0;
                
                srand(time(NULL));
                num = rand() % 2 + 1;

                if (position.x < 238 && ((num == 1 && avance!=0) || avance==0))//Si puedo, y toca seguir, sigo
                {
                    aumentarX();
                    if (avance != 0)
                    {
                        avance = 0;
                    }
                    avance += 0.05;
                    avanzado += 0.05;
                }
                else
                {
                    if (num == 1)
                    {
                        direction = vec3(0, 0, 1);
                    }
                    else
                    {
                        direction = vec3(0, 0, -1);
                    }
                }
            }
            else
            {
                aumentarX();
                avance += 0.05;
                avanzado += 0.05;
            }
        }
        if (normalize(direction) == vec3(-1, 0, 0))
        {
            if (avance + 0.05 > 15 || avance == 0) //si estoy en una esquina, tomo una decision
            {
                if (avance != 0)
                {
                    position.x = ultimaEsquina.x - 15; //Llegue a la esquina en -x
                    position.z = ultimaEsquina.y;
                    ultimaEsquina.x = position.x;
                    ultimaEsquina.y = position.z;
                    avance = 0;
                }
                int num = 0;

                srand(time(NULL));
                num = rand() % 2 + 1;

                if ((position.x > 148 || (position.x > 118 && position.z < -176) ) && ((num == 1 && avance != 0) || avance == 0))//Si puedo, y toca seguir, sigo
                {
                    disminuirX();
                    if (avance != 0)
                    {
                        avance = 0;
                    }
                    avance += 0.05;
                    avanzado += 0.05;
                }
                else
                {
                    if (num == 1)
                    {
                        direction = vec3(0, 0, 1);
                    }
                    else
                    {
                        direction = vec3(0, 0, -1);
                    }
                }
            }
            else
            {
                disminuirX();
                avance += 0.05;
                avanzado += 0.05;
            }
        }
        if (normalize(direction) == vec3(0, 0, 1))
        {
            if (avance + 0.05 > 15 || avance == 0) //si estoy en una esquina, tomo una decision
            {
                if (avance != 0)
                {
                    position.x = ultimaEsquina.x; //Llegue a la esquina en +z
                    position.z = ultimaEsquina.y + 15;
                    ultimaEsquina.x = position.x;
                    ultimaEsquina.y = position.z;
                    avance = 0;
                }
                int num = 0;

                srand(time(NULL));
                num = rand() % 2 + 1;

                if ((position.z < -177 || (position.x > 148 && position.z < -162)) && ((num == 1 && avance != 0) || avance == 0))//Si puedo, y toca seguir, sigo
                {
                    aumentarZ();
                    if (avance != 0)
                    {
                        avance = 0;
                    }
                    avance += 0.05;
                    avanzado += 0.05;
                }
                else
                {
                    if (num == 1)
                    {
                        direction = vec3(1, 0, 0);
                    }
                    else
                    {
                        direction = vec3(-1, 0, 0);
                    }
                }
            }
            else
            {
                aumentarZ();
                avance += 0.05;
                avanzado += 0.05;
            }
        }
        if (normalize(direction) == vec3(0, 0, -1))
        {
            if (avance + 0.05 > 15 || avance == 0) //si estoy en una esquina, tomo una decision
            {
                if (avance != 0)
                {
                    position.x = ultimaEsquina.x; //Llegue a la esquina en +x
                    position.z = ultimaEsquina.y - 15;
                    ultimaEsquina.x = position.x;
                    ultimaEsquina.y = position.z;
                    avance = 0;
                }
                int num = 0;

                srand(time(NULL));
                num = rand() % 2 + 1;

                if (position.z > -220 && ((num == 1 && avance != 0) || avance == 0))//Si puedo, y toca seguir, sigo
                {
                    disminuirZ();
                    if (avance != 0)
                    {
                        avance = 0;
                    }
                    avance += 0.05;
                    avanzado += 0.05;
                }
                else
                {
                    if (num == 1)
                    {
                        direction = vec3(1, 0, 0);
                    }
                    else
                    {
                        direction = vec3(-1, 0, 0);
                    }
                }
            }
            else
            {
                disminuirZ();
                avance += 0.05;
                avanzado += 0.05;
            }
        }
    
}


    
