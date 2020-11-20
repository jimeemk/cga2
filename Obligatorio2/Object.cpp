#include "Object.h"

Object::Object(string p, glm::vec3 ori, float esc, glm::vec3 pos, glm::vec3 u, glm::vec3 dir, GLuint sh) : Entity(pos,u,dir,sh)
{
    path = p;
    orientation = ori;
    scale = esc;
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

glm::mat4 Object::getModelMatrix()
{
    return modelMatrix;
}

GLuint Object::getShaderProgram()
{
    return shaderProgram;
}

void Object::draw()
{
    model->Draw(shaderProgram);
}

void Object::initObject()
{
    glm::vec3 centro = glm::vec3(0.0);
    min = model->min;
    max = model->max;
    centro.x = (min.x + max.x) / 2;
    centro.y = (min.y + max.y) / 2;
    centro.z = (min.z + max.z) / 2;
    cout << "min: " << min.x << " ... " << min.y << " ... " << min.z << "\n";
    cout << "max: " << max.x << " ... " << max.y << " ... " << max.z << "\n";
    cout <<"Centro: "<< centro.x << " ... " << centro.y << " ... " << centro.z << "\n";
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

        up = glm::rotate(up, tita1, dir1);//Actualizo up
        initialDir = glm::rotate(initialDir, tita1, dir1);//Actualizo vector "initialDir"

        if (glm::cross(initialDir, direction) != glm::vec3(0.0))//Verifico que ya no esten paralelos
        {
            dir2 = normalize(glm::cross(initialDir, direction));//En el paso anterior, pase mi problema 3d a uno 2d, ahora en ese plano (calculando el vector perpendicular con producto cruz y tomandolo como eje de referencia), voy a calcular el angulo entre los dos vectores
            tita2 = glm::orientedAngle(normalize(initialDir), normalize(direction), dir2);
            up = glm::rotate(up, tita2, dir2);
            initialDir = glm::rotate(initialDir, tita2, glm::cross(initialDir, direction));
        }
    }
    
    if (glm::cross(direction, up) != glm::vec3(0.0))//Me cubro del caso dir= 0,1,0
    {
        float titaFinal = glm::orientedAngle(normalize(up),glm::vec3(0, 1, 0), normalize(direction));//Esto es para alinear el up con el (0,1,0)
        modelMatrix = glm::rotate(modelMatrix, titaFinal, normalize(direction));
        up = glm::rotate(up, titaFinal, normalize(direction));
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
    
