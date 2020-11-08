#pragma once

#include <string>

#include "Mesh.h"

class OBJImporter
{
public:
	static bool Import (Mesh& mesh, char* data, const int dataSize);

private:
	static std::string ReadToken (const char* start);
};

