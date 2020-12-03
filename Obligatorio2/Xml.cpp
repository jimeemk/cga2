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
						d = vec3(x, y, z);
					}
					std::string path = obj_ele->Attribute("path");
					float scale = stof(obj_ele->Attribute("scale"));
					unsigned int shaderId = stof(obj_ele->Attribute("shader"));

					Settings* set = Settings::getInstance();
					Object* obj = new Object(path, o, scale, p, u, d, set->getShaders().at(shaderId));
					set->addEntity(obj);
					obj_ele = obj_ele->NextSiblingElement("objeto");
				}

		}
	}
}

void saveXMLEntities(const char* ruta, vector<Entity*> entities)
{
	TiXmlDocument doc;
	TiXmlDeclaration* declaracion = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(declaracion);

	TiXmlElement* root = new TiXmlElement("file");
	doc.LinkEndChild(root);

	TiXmlElement* objetos = new TiXmlElement("objetos");
	for (int i = 0; i < entities.size(); i++)
	{
		if (Object* o = dynamic_cast<Object*>(entities.at(i)))
		{

			TiXmlElement* objeto = new TiXmlElement("objeto");
			objeto->SetAttribute("path", o->getPath().c_str());
			objeto->SetAttribute("scale", o->getScale());
			TiXmlElement* orientation = new TiXmlElement("orientation");
			orientation->SetAttribute("x", o->getOrientation().x);
			orientation->SetAttribute("y", o->getOrientation().y);
			orientation->SetAttribute("z", o->getOrientation().z);
			objeto->LinkEndChild(orientation);
			TiXmlElement* position = new TiXmlElement("position");
			position->SetAttribute("x", o->getPosition().x);
			position->SetAttribute("y", o->getPosition().y);
			position->SetAttribute("z", o->getPosition().z);
			objeto->LinkEndChild(position);
			TiXmlElement* up = new TiXmlElement("up");
			up->SetAttribute("x", o->getUp().x);
			up->SetAttribute("y", o->getUp().y);
			up->SetAttribute("z", o->getUp().z);
			objeto->LinkEndChild(up);
			TiXmlElement* direction = new TiXmlElement("direction");
			direction->SetAttribute("x", o->getDirection().x);
			direction->SetAttribute("y", o->getDirection().y);
			direction->SetAttribute("z", o->getDirection().z);
			objeto->LinkEndChild(direction);

			
			objetos->LinkEndChild(objeto);
		}
	}
	root->LinkEndChild(objetos);
	doc.SaveFile(ruta);
}