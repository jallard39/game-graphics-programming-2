#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <string>

#include "Vertex.h"

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
	std::string GetName();

private:
	std::string name = "MyMesh";

	int numVertices;
	int numIndices;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	D3D12_INDEX_BUFFER_VIEW ibView{};

	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
	void CreateBuffers(
		void* vertices, 
		int numVertices, 
		void* indices, 
		int numIndices
	);
};