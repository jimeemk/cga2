#include "xml.h"

void loadXMLEntities(const char* ruta)
{
	vector<Entity*> entities;
	TiXmlDocument doc(ruta);
	doc.LoadFile();
	TiXmlElement* root = doc.RootElement();
	if (root != NULL)
	{
		TiXmlElement* objetos_ele = root->FirstChildElement("objetos");
		//Cargar objetos y agregarlos a settings
		if (objetos_ele != NULL)
		{
				TiXmlElement* obj_ele = objetos_ele->FirstChildElement("objeto");
				while (obj_ele != NULL)
				{

					TiXmlElement* ori = obj_ele->FirstChildElement("orientation");
					TiXmlElement* pos = obj_ele->FirstChildElement("position");
					TiXmlElement* up = obj_ele->FirstChildElement("up");
					TiXmlElement* dir = obj_ele->FirstChildElement("direction");
					glm::vec3 o=glm::vec3(0.f);
					glm::vec3 p=glm::vec3(0.f);
					glm::vec3 u= glm::vec3(0.f);
					glm::vec3 d= glm::vec3(0.f);
					if (ori != NULL)
					{
						float x = stof(ori->Attribute("x"));
						float y = stof(ori->Attribute("y"));
						float z = stof(ori->Attribute("z"));
						o = vec3(x, y, z);
					}

					if (pos != NULL)
					{
						float x = stof(pos->Attribute("x"));
						float y = stof(pos->Attribute("y"));
						float z = stof(pos->Attribute("z"));
						p = vec3(x, y, z);
					}

					if (up != NULL)
					{
						float x = stof(up->Attribute("x"));
						float y = stof(up->Attribute("y"));
						float z = stof(up->Attribute("z"));
						u = vec3(x, y, z);
					}

					if (dir != NULL)
					{
						float x = stof(dir->Attribute("x"));
						float y = stof(dir->Attribute("y"));
						float z = stof(dir->Attribute("z"));
						if (x > 0 && z == 0 && y == 0)
							d = vec3(x, y, z + 0.0001);
						else
							d = vec3(x, y, z);
					}
					std::string path = obj_ele->Attribute("path");
					float scale = stof(obj_ele->Attribute("scale"));
					unsigned int shaderId = stof(obj_ele->Attribute("shader"));
					unsigned int hasTexture = stof(obj_ele->Attribute("hasTexture"));
					bool ht = false;
					int se = 1;
					if (hasTexture == 1)
					{
						ht = true;
						se = 0;
					}
					Settings* set = Settings::getInstance();

					Object* obj = new Object(path, o, scale, p, u, d, set->getShaders().at(se),ht);
					set->addEntity(obj);
					obj_ele = obj_ele->NextSiblingElement("objeto");
				}

		}
	}
}

void saveXMLEntities(const char* ruta)
{
	TiXmlDocument doc;
	TiXmlDeclaration* declaracion = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(declaracion);

	TiXmlElement* root = new TiXmlElement("file");
	doc.LinkEndChild(root);

	TiXmlElement* objetos = new TiXmlElement("objetos");
	Settings* set = Settings::getInstance();
	for (int i = 0; i < set->getEntities().size(); i++)
	{
		if (Object* o = dynamic_cast<Object*>(set->getEntities().at(i)))
		{

			TiXmlElement* objeto = new TiXmlElement("objeto");
			objeto->SetAttribute("path", o->getPath().c_str());
			objeto->SetDoubleAttribute("scale", o->getScale());
			if (o->getHasTexture())
			{
				objeto->SetAttribute("hasTexture", 1);
				objeto->SetAttribute("shader", 0);
			}
			else {
				objeto->SetAttribute("hasTexture", 0);
				objeto->SetAttribute("shader", 1);
			}
			TiXmlElement* orientation = new TiXmlElement("orientation");
			orientation->SetDoubleAttribute("x", o->getOrientation().x);
			orientation->SetDoubleAttribute("y", o->getOrientation().y);
			orientation->SetDoubleAttribute("z", o->getOrientation().z);
			objeto->LinkEndChild(orientation);
			TiXmlElement* position = new TiXmlElement("position");
			position->SetDoubleAttribute("x", o->getPosition().x);
			position->SetDoubleAttribute("y", o->getPosition().y);
			position->SetDoubleAttribute("z", o->getPosition().z);
			objeto->LinkEndChild(position);
			TiXmlElement* up = new TiXmlElement("up");
			up->SetDoubleAttribute("x", o->getUp().x);
			up->SetDoubleAttribute("y", o->getUp().y);
			up->SetDoubleAttribute("z", o->getUp().z);
			objeto->LinkEndChild(up);
			TiXmlElement* direction = new TiXmlElement("direction");
			direction->SetDoubleAttribute("x", o->getDirection().x);
			direction->SetDoubleAttribute("y", o->getDirection().y);
			direction->SetDoubleAttribute("z", o->getDirection().z);
			objeto->LinkEndChild(direction);

			
			objetos->LinkEndChild(objeto);
		}
	}
	root->LinkEndChild(objetos);
	doc.SaveFile(ruta);
}