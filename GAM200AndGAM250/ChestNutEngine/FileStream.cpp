#include "FileStream.h"
#include "HPComponent.h"
FileStream::FileStream()
{

}

FileStream::~FileStream()
{

}

ObjectType FileStream::GetObjectType(std::string string)
{
	if (string == "PLAYER")
	{
		return PLAYER;
	}
	else if (string == "ENEMY")
	{
		return ENEMY;
	}
	else
	{
		return UNKNOWN;
	}
}

void FileStream::ReadingPlayerFile(const char * filepath)
{
	FILE* fp;
	errno_t err = fopen_s(&fp, filepath, "r");

	if (err == 0)
	{
		std::cout << "Loading content of " << filepath << " file" << std::endl;
		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		rapidjson::Document d;
		d.ParseStream(is);

		std::cout << "Object to be create: " << d["Object"].GetString() << std::endl;
		std::string objectStr = d["Object"].GetString();

		ObjectType m_GameObjectType = GetObjectType(objectStr);

		/*std::vector<Component *> vector;
		


		if (d["HPComponent"].GetBool() == 1)
		{
			HPComponent * health = new HPComponent{ d["HPComponentInfo"]["m_HP"].GetUint() };
			std::cout << "Health constructed";
			vector.push_back(health);
		}


		GameObject * object = new GameObject{ m_GameObjectType, vector };*/


		

		
		
		std::cout << "The ID: " << d["ID"].GetUint() << std::endl;
		std::cout << "The hp: " << d["HPComponent"]["m_HP"].GetUint()<< std::endl;
		std::cout << "The m_PosX: " << d["PhysicsComponent"]["m_Pos"]["x"].GetFloat() << std::endl;
		std::cout << "The m_PosY: " << d["PhysicsComponent"]["m_Pos"]["y"].GetFloat() << std::endl;
		std::cout << "The m_VelX: " << d["PhysicsComponent"]["m_Vel"]["x"].GetFloat() << std::endl;
		std::cout << "The m_VelY: " << d["PhysicsComponent"]["m_Vel"]["y"].GetFloat() << std::endl;
		std::cout << "The m_Scale: " << d["PhysicsComponent"]["m_Scale"].GetFloat() << std::endl;
		//std::cout << "The CollisionComponent: " << d["CollisionComponent"].GetBool() << std::endl;
		
		fclose(fp);
	}
	else
	{
		std::cout << "Fail to open " << filepath << " file" << std::endl;
	}
}