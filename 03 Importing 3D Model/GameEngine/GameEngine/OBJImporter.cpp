#include <vector>
#include <array>

#include "OBJImporter.h"

bool OBJImporter::Import (Mesh& mesh, char* data, const int dataSize)
{
	std::vector<std::array<float, 3>> positions;
	std::vector<std::array<float, 2>> uvs;
	std::vector<std::array<float, 3>> normals;
	std::vector<Vertex> vertices;

	int filePosition = 0;

	while (filePosition < dataSize)
	{
		char character = data[filePosition];

		if (character == '#')
		{
			while (data[filePosition] != '\n')
			{
				filePosition += 1;
			}

			filePosition += 1;
		}
		else
		{
			std::string token = ReadToken (&data[filePosition]);
			filePosition += token.size () + 1;

			if (token == "v")
			{
				std::array<float, 3> position;

				for (int i = 0; i < 3; i++)
				{
					token = ReadToken (&data[filePosition]);
					filePosition += token.size () + 1;

					position[i] = std::stof (token);
				}

				positions.push_back (position);
			}
			else if (token == "vn")
			{
				std::array<float, 3> normal;

				for (int i = 0; i < 3; i++)
				{
					token = ReadToken (&data[filePosition]);
					filePosition += token.size () + 1;

					normal[i] = std::stof (token);
				}

				normals.push_back (normal);
			}
			else if (token == "vt")
			{
				std::array<float, 2> uv;

				for (int i = 0; i < 2; i++)
				{
					token = ReadToken (&data[filePosition]);
					filePosition += token.size () + 1;

					uv[i] = std::stof (token);
				}

				uvs.push_back (uv);
			}
			else if (token == "f")
			{
				float facePositions[3][3];
				float faceUVs[3][2];
				float faceNormals[3][3];

				for (int i = 0; i < 3; i++)
				{
					token = ReadToken (&data[filePosition]);
					filePosition += token.size () + 1;

					size_t uvStart = token.find ('/', 0) + 1;
					size_t normalStart = token.find ('/', uvStart) + 1;

					size_t positionIndex = std::stoi (token.substr (0, uvStart - 1)) - 1;
					size_t uvIndex = std::stoi (token.substr (uvStart, normalStart - uvStart - 1)) - 1;
					size_t normalIndex = std::stoi (token.substr (normalStart, token.length () - normalStart)) - 1;

					bool hasNormal = normals.size () > 0;
					bool hasUV = uvs.size () > 0;

					facePositions[i][0] = positions.at (positionIndex)[0];
					facePositions[i][1] = positions.at (positionIndex)[1];
					facePositions[i][2] = positions.at (positionIndex)[2];

					faceUVs[i][0] = hasUV ? uvs.at (uvIndex)[0] : 0.0f;
					faceUVs[i][1] = hasUV ? uvs.at (uvIndex)[1] : 0.0f;

					faceNormals[i][0] = hasNormal ? normals.at (normalIndex)[0] : 0.0f;
					faceNormals[i][1] = hasNormal ? normals.at (normalIndex)[1] : 0.0f;
					faceNormals[i][2] = hasNormal ? normals.at (normalIndex)[2] : 0.0f;
				}

				Vertex v1 = { facePositions[0], faceUVs[0], faceNormals[0] };
				Vertex v2 = { facePositions[1], faceUVs[1], faceNormals[1] };
				Vertex v3 = { facePositions[2], faceUVs[2], faceNormals[2] };

				vertices.push_back (v1);
				vertices.push_back (v2);
				vertices.push_back (v3);

				//Vertex v1;
				//v1.position[0] = facePositions[0][0];
				//v1.position[1] = facePositions[0][1];
				//v1.position[2] = facePositions[0][2];
				//v1.uv[0] = faceUVs[0][0];
				//v1.uv[1] = faceUVs[0][1];
				//v1.normal[0] = faceNormals[0][0];
				//v1.normal[1] = faceNormals[0][1];
				//v1.normal[2] = faceNormals[0][2];

				//Vertex v2;
				//v2.position[0] = facePositions[1][0];
				//v2.position[1] = facePositions[1][1];
				//v2.position[2] = facePositions[1][2];
				//v2.uv[0] = faceUVs[1][0];
				//v2.uv[1] = faceUVs[1][1];
				//v2.normal[0] = faceNormals[1][0];
				//v2.normal[1] = faceNormals[1][1];
				//v2.normal[2] = faceNormals[1][2];

				//Vertex v3;
				//v3.position[0] = facePositions[2][0];
				//v3.position[1] = facePositions[2][1];
				//v3.position[2] = facePositions[2][2];
				//v3.uv[0] = faceUVs[2][0];
				//v3.uv[1] = faceUVs[2][1];
				//v3.normal[0] = faceNormals[2][0];
				//v3.normal[1] = faceNormals[2][1];
				//v3.normal[2] = faceNormals[2][2];

				//vertices.push_back (v1);
				//vertices.push_back (v2);
				//vertices.push_back (v3);
			}
			else
			{
				while (data[filePosition] != '\n')
				{
					filePosition += 1;
				}

				filePosition += 1;
			}
		}
	}

	std::vector<unsigned int> indices (vertices.size ());

	for (size_t i = 0; i < indices.size (); i++)
	{
		indices[i] = i;
	}

	if (mesh.Initialize (vertices, indices) == false)
	{
		return false;
	}

	return true;
}

std::string OBJImporter::ReadToken (const char* start)
{
	std::string result;

	for (int i = 0; start[i] != ' ' && start[i] && '\t' && start[i] != '\n'; i++)
	{
		result += start[i];
	}

	return result;
}
