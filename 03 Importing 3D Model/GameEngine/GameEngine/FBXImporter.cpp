// FBX SDK 라이브러리 링크
#pragma comment (lib, "libfbxsdk-md.lib")

#include <vector>
#include <array>

#include "FBXImporter.h"
#include "FBXMemoryReadStream.h"
#include "Mesh.h"

static FbxManager* g_fbxManager;
static FbxIOSettings* g_fbxIOSettings;

bool FBXImporter::Start ()
{
	// FBX SDK 매니저를 생성한다.
	g_fbxManager = FbxManager::Create ();

	// 입출력 설정을 지정한다.
	g_fbxIOSettings = FbxIOSettings::Create (g_fbxManager, IOSROOT);
	g_fbxManager->SetIOSettings (g_fbxIOSettings);

	return true;
}
void FBXImporter::Shutdown ()
{
	// 입출력 설정을 지운다.
	g_fbxIOSettings->Destroy ();
	g_fbxIOSettings = nullptr;

	// FBX SDK 매니저를 지운다.
	g_fbxManager->Destroy ();
	g_fbxManager = nullptr;
}

bool FBXImporter::Import (Mesh& mesh, char* data, const int dataSize)
{
	// FBX 데이터를 읽기 위한 Memory stream과 Importer를 생성한다.
	FBXMemoryReadStream stream (dataSize, g_fbxManager);
	FbxImporter* fbxImporter = FbxImporter::Create (g_fbxManager, "Importer");

	// FBX Importer를 초기화한다.
	if (fbxImporter->Initialize (&stream, data, -1, g_fbxManager->GetIOSettings ()) == false)
	{
		const auto* error = fbxImporter->GetStatus ().GetErrorString ();
		fbxImporter->Destroy ();

		return false;
	}

	// 객체를 불러올 Scene을 생성한다.
	FbxScene* fbxScene = FbxScene::Create (g_fbxManager, "Scene");

	// 생성한 Scene에 객체를 불러온다.
	if (fbxImporter->Import (fbxScene) == false)
	{
		fbxScene->Destroy ();
		fbxImporter->Destroy ();

		return false;
	}

	// Importer는 사용이 끝났으므로 지운다.
	fbxImporter->Destroy ();

	// Scene의 최상위 노드를 가져온다.
	FbxNode* fbxRootNode = fbxScene->GetRootNode ();

	// Vertex를 저장할 버퍼
	std::vector<Vertex> vertices;

	if (fbxRootNode)
	{
		// Scene의 Geometry 노드를 삼각화한다.
		FbxGeometryConverter fbxGeometryConverter (g_fbxManager);

		if (fbxGeometryConverter.Triangulate (fbxScene, true) == false)
		{
			return false;
		}

		// 노드를 순회하며 버텍스 정보를 수집한다.
		TraverseNode (fbxRootNode, vertices);
	}

	// Scene의 사용이 끝났으므로 지운다.
	fbxScene->Destroy ();

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

void FBXImporter::TraverseNode (FbxNode* fbxNode, std::vector<Vertex>& vertices)
{
	// 현재 노드의 속성을 가져온다.
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute ();

	// 현재 노드가 Mesh 노드라면 정보를 파싱한다.
	if (fbxNodeAttribute != nullptr &&
		fbxNodeAttribute->GetAttributeType () == FbxNodeAttribute::eMesh)
	{
		// 노드의 Mesh를 가져온다.
		FbxMesh* fbxMesh = fbxNode->GetMesh ();

		// 노드의 피봇을 Vertex 위치와 Normal에 적용하고, Tangent를 생성한다.
		fbxMesh->ApplyPivot ();
		fbxMesh->GenerateTangentsData (0);

		// Mesh의 폴리곤 개수를 가져온다.
		int polygonCount = fbxMesh->GetPolygonCount ();
		int vertexId = 0;

		// Mesh에 적용할 World Matrix를 가져온다.
		FbxAMatrix matrix = GetWorldMatrix (fbxNode);

		for (int polygonIndex = 0; polygonIndex < polygonCount; polygonIndex++)
		{
			float positions[3][3] = { };
			float uvs[3][2] = { };
			float normals[3][3] = { };

			for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
			{
				// Control point 인덱스를 가져온다.
				int controlPointIndex = fbxMesh->GetPolygonVertex (polygonIndex, vertexIndex);

				// Vertex 위치, 텍스처 좌표, Normal을 가져온다.
				FbxVector4 controlPoint = fbxMesh->GetControlPointAt (controlPointIndex);
				FbxVector4 uv = GetMeshUV (fbxMesh, controlPointIndex, fbxMesh->GetTextureUVIndex (polygonIndex, vertexIndex));
				FbxVector4 normal = GetMeshNormal (fbxMesh, controlPointIndex, vertexId);
				FbxVector4 tangent = GetMeshTangent (fbxMesh, controlPointIndex, vertexId);
				float uvWinding = static_cast<float> (tangent.mData[3]);

				controlPoint = matrix.MultT (controlPoint);

				positions[vertexIndex][0] = static_cast<float> (controlPoint.mData[0]);
				positions[vertexIndex][1] = static_cast<float> (controlPoint.mData[1]);
				positions[vertexIndex][2] = static_cast<float> (controlPoint.mData[2]);

				uvs[vertexIndex][0] = static_cast<float> (uv.mData[0]);
				uvs[vertexIndex][1] = static_cast<float> (1.0f - uv.mData[1]);

				normals[vertexIndex][0] = static_cast<float> (normal.mData[0]);
				normals[vertexIndex][1] = static_cast<float> (normal.mData[1]);
				normals[vertexIndex][2] = static_cast<float> (normal.mData[2]);

				vertexId += 1;
			}

			// Vertex 위치, 텍스처 좌표, Normal을 버퍼에 추가한다.
			vertices.push_back ({ positions[0], uvs[0], normals[0] });
			vertices.push_back ({ positions[1], uvs[1], normals[1] });
			vertices.push_back ({ positions[2], uvs[2], normals[2] });
		}
	}

	// 현재 노드의 자식 노드를 순회한다.
	for (int childIndex = 0; childIndex < fbxNode->GetChildCount (); childIndex++)
	{
		TraverseNode (fbxNode->GetChild (childIndex), vertices);
	}
}

FbxVector4 FBXImporter::GetMeshUV (FbxMesh* fbxMesh, int controlPointIndex, int uvIndex)
{
	FbxVector4 uv;

	const FbxGeometryElementUV* fbxUV = fbxMesh->GetElementUV (0);
	const FbxLayerElement::EMappingMode uvMapping = fbxUV->GetMappingMode ();
	const FbxLayerElement::EReferenceMode uvReference = fbxUV->GetReferenceMode ();

	// Element의 매핑 모드와 레퍼런스 모드에 따라서 값을 가져온다.
	if (uvMapping == FbxGeometryElement::eByControlPoint)
	{
		if (uvReference == FbxGeometryElement::eDirect)
		{
			uv = fbxUV->GetDirectArray ().GetAt (controlPointIndex);
		}
		else if (uvReference == FbxGeometryElement::eIndexToDirect)
		{
			int index = fbxUV->GetIndexArray ().GetAt (controlPointIndex);
			uv = fbxUV->GetDirectArray ().GetAt (index);
		}
	}
	else if (uvMapping == FbxGeometryElement::eByPolygonVertex)
	{
		uv = fbxUV->GetDirectArray ().GetAt (uvIndex);
	}

	return uv;
}

FbxVector4 FBXImporter::GetMeshNormal (FbxMesh* fbxMesh, int controlPointIndex, int vertexId)
{
	FbxVector4 normal;

	const FbxGeometryElementNormal* fbxNormal = fbxMesh->GetElementNormal (0);
	const FbxLayerElement::EMappingMode normalMapping = fbxNormal->GetMappingMode ();
	const FbxLayerElement::EReferenceMode normalReference = fbxNormal->GetReferenceMode ();

	// Element의 매핑 모드와 레퍼런스 모드에 따라서 값을 가져온다.
	if (normalMapping == FbxGeometryElement::eByControlPoint)
	{
		if (normalReference == FbxGeometryElement::eDirect)
		{
			normal = fbxNormal->GetDirectArray ().GetAt (controlPointIndex);
		}
		else if (normalReference == FbxGeometryElement::eIndexToDirect)
		{
			int index = fbxNormal->GetIndexArray ().GetAt (controlPointIndex);
			normal = fbxNormal->GetDirectArray ().GetAt (index);
		}
	}
	else if (normalMapping == FbxGeometryElement::eByPolygonVertex)
	{
		if (normalReference == FbxGeometryElement::eDirect)
		{
			normal = fbxNormal->GetDirectArray ().GetAt (vertexId);
		}
		else if (normalReference == FbxGeometryElement::eIndexToDirect)
		{
			int index = fbxNormal->GetIndexArray ().GetAt (vertexId);
			normal = fbxNormal->GetDirectArray ().GetAt (index);
		}
	}

	return normal;
}

FbxVector4 FBXImporter::GetMeshTangent (FbxMesh* fbxMesh, int controlPointIndex, int vertexId)
{
	FbxVector4 tangent;

	const FbxGeometryElementTangent* fbxTangent = fbxMesh->GetElementTangent (0);
	const FbxLayerElement::EMappingMode tangentMapping = fbxTangent->GetMappingMode ();
	const FbxLayerElement::EReferenceMode tangentReference = fbxTangent->GetReferenceMode ();

	// Element의 매핑 모드와 레퍼런스 모드에 따라서 값을 가져온다.
	if (tangentMapping == FbxGeometryElement::eByControlPoint)
	{
		if (tangentReference == FbxGeometryElement::eDirect)
		{
			tangent = fbxTangent->GetDirectArray ().GetAt (controlPointIndex);
		}
		else if (tangentReference == FbxGeometryElement::eIndexToDirect)
		{
			int index = fbxTangent->GetIndexArray ().GetAt (controlPointIndex);
			tangent = fbxTangent->GetDirectArray ().GetAt (index);
		}
	}
	else if (tangentMapping == FbxGeometryElement::eByPolygonVertex)
	{
		if (tangentReference == FbxGeometryElement::eDirect)
		{
			tangent = fbxTangent->GetDirectArray ().GetAt (vertexId);
		}
		else if (tangentReference == FbxGeometryElement::eIndexToDirect)
		{
			int index = fbxTangent->GetIndexArray ().GetAt (vertexId);
			tangent = fbxTangent->GetDirectArray ().GetAt (index);
		}
	}

	return tangent;
}

FbxAMatrix FBXImporter::GetWorldMatrix (FbxNode* fbxNode)
{
	FbxAMatrix worldMatrix;
	worldMatrix.SetIdentity ();

	FbxNode* parentNode = fbxNode;

	while (parentNode != nullptr)
	{
		worldMatrix = parentNode->EvaluateLocalTransform () * worldMatrix;
		parentNode = parentNode->GetParent ();
	}

	FbxAMatrix geometryMatrix;
	geometryMatrix.SetT (fbxNode->GetGeometricTranslation (FbxNode::eSourcePivot));
	geometryMatrix.SetR (fbxNode->GetGeometricRotation (FbxNode::eSourcePivot));
	geometryMatrix.SetS (fbxNode->GetGeometricScaling (FbxNode::eSourcePivot));

	return worldMatrix * geometryMatrix;
}