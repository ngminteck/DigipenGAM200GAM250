#include "AnimationFactory.h"

AnimationFactory::AnimationFactory()
{

}
AnimationFactory::~AnimationFactory()
{
	for (auto& it : m_AnimationMap)
	{
		delete it.second;
		it.second = nullptr;
	}
}


bool AnimationFactory::LoadFromFile(const char * filename)
{
	AnimationMap* ani2D;
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, filename, "r");
	if (err == 0)
	{
		std::cout << "Loading content of " << filename << " file" << std::endl;
		char readBuffer[65536]; // assign the buffersize
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer)); // rapidjson file reader stream
		rapidjson::Document document; // rapidjson document for using rpaidjson document format
		document.ParseStream(is); // pass in the file stream into the document
		const rapidjson::Value& animationJson = document["Animation"];

		for (rapidjson::SizeType i = 0; i < animationJson.Size(); i++)
		{
			const rapidjson::Value& animList = animationJson[i]["Anim"];
			const char * hash = animationJson[i]["Hash"].GetString();
			ani2D = new AnimationMap{ animList };
			m_AnimationMap[hash] = ani2D;
			ani2D = nullptr;
		}
	}
	else
	{
		std::cout << "Animation Load Failed!" << std::endl;
		return false;
	}
	return true;
}
std::unordered_map<std::string, AnimationMap*> * AnimationFactory::GetAnimationList()
{
	return &m_AnimationMap;
}