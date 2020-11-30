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
		// 현재 파일 위치의 문자를 가져온다.
		char character = data[filePosition];

		if (character == '#')
		{
			// 현재 위치가 주석의 시작점이면
			// 다음 줄의 시작점으로 파일 위치를 이동한다.
			while (true)
			{
				if (data[filePosition++] == '\n')
				{
					break;
				}
			}
		}
		else
		{
			std::string token = ReadToken (&data[filePosition]);
			filePosition += token.size () + 1;

			if (token == "v")
			{
				// Vertex 위치를 읽는다.
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
				// Vertex normal을 읽는다.
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
				// Vertex 텍스처 좌표를 읽는다.
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
				// 삼각형의 Vertex 정보를 읽는다.
				float facePositions[3][3];
				float faceUVs[3][2];
				float faceNormals[3][3];

				for (int i = 0; i < 3; i++)
				{
					token = ReadToken (&data[filePosition]);
					filePosition += token.size () + 1;

					// 텍스처 좌표와 Normal의 인덱스 시작점을 가져온다.
					size_t uvStart = token.find ('/', 0) + 1;
					size_t normalStart = token.find ('/', uvStart) + 1;

					// 위치, 텍스처 좌표, Normal의 인덱스를 구한다.
					size_t positionIndex = std::stoi (token.substr (0, uvStart - 1)) - 1;
					size_t uvIndex = std::stoi (token.substr (uvStart, normalStart - uvStart - 1)) - 1;
					size_t normalIndex = std::stoi (token.substr (normalStart, token.length () - normalStart)) - 1;

					// 인덱스로 Vertex의 각 정보를 채운다.
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

				// 삼각형을 이루는 버텍스를 Vertex 리스트에 추가한다.
				Vertex v1 = { facePositions[0], faceUVs[0], faceNormals[0] };
				Vertex v2 = { facePositions[1], faceUVs[1], faceNormals[1] };
				Vertex v3 = { facePositions[2], faceUVs[2], faceNormals[2] };

				vertices.push_back (v1);
				vertices.push_back (v2);
				vertices.push_back (v3);
			}
			else
			{
				// 파싱하지 않는 정보라면
				// 다음 줄의 시작점으로 파일 위치를 이동한다.
				while (true)
				{
					if (data[filePosition++] == '\n')
					{
						break;
					}
				}
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

	// 공백 문자가 나올때까지 문자를 읽는다.
	for (int i = 0; start[i] != ' ' && start[i] && '\t' && start[i] != '\n'; i++)
	{
		result += start[i];
	}

	return result;
}
