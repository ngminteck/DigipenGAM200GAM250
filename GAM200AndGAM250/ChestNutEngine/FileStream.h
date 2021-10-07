#pragma once

#include "rapidjson/rapidjson.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"

#include <iostream>
#include <sstream>
#include "GameObject.h"

class FileStream
{
public:
	FileStream();
	~FileStream();
	void ReadingPlayerFile(const char * filepath);
	ObjectType GetObjectType(std::string string);

private:
	/*FILE * m_file;
	errno_t m_Errno_t;
	const char readBuffer[65536];
	//rapidjson::FileReadStream m_FileReadStream;
	rapidjson::Document m_Document;*/

};