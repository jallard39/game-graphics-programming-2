#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <vector>

#include "RayTracing.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "GameEntity.h"
#include "Lights.h"

class Game
{
public:
	// Basic OOP setup
	Game() = default;
	~Game();
	Game(const Game&) = delete; // Remove copy constructor
	Game& operator=(const Game&) = delete; // Remove copy-assignment operator

	// Primary functions
	void Initialize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void OnResize();

private:
	// Initialization helper methods
	void CreateCameras();
	void CreateGeometry();
	void CreateMaterials();
	void CreateEntities();
	void CreateLights();

	float RandomFloat(float low, float high)
	{
		return (((float)(rand()) / (float)(RAND_MAX)) * (high - low)) + low;
	}

	// Geometry
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	D3D12_INDEX_BUFFER_VIEW ibView{};

	// Other graphics data
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};

	// Game variables
	int activeCameraIndex = 0;
	int lightCount = 0;
	std::vector<std::shared_ptr<Camera>> cameras;
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<Material>> materials;
	std::vector<std::shared_ptr<GameEntity>> entities;
	std::vector<Light> lights;

	std::vector<float> sphereOffsets;
};

