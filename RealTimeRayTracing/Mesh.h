#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <string>

#include "Vertex.h"

struct MeshRaytracingData
{
	D3D12_GPU_DESCRIPTOR_HANDLE IndexBufferSRV { };
	D3D12_GPU_DESCRIPTOR_HANDLE VertexBufferSRV { };
	Microsoft::WRL::ComPtr<ID3D12Resource> BLAS;
	unsigned int HitGroupIndex = 0;
};

class Mesh {
	
public:
	Mesh(
		std::string name,
		Vertex* vertices, 
		int numVertices,
		unsigned int* indices,
		int numIndices
	);
	Mesh(
		std::string name,
		const wchar_t* filename
	);
	~Mesh();

	Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBuffer();
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBuffer();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
	int GetIndexCount();
	int GetVertexCount();
	std::string GetName();

	MeshRaytracingData GetRaytracingData() { return raytracingData; }

private:
	std::string name = "MyMesh";

	int numVertices;
	int numIndices;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	D3D12_INDEX_BUFFER_VIEW ibView{};

	MeshRaytracingData raytracingData;

	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
	void CreateBuffers(
		void* vertices, 
		int numVertices, 
		void* indices, 
		int numIndices
	);
};