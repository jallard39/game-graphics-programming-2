#pragma once

#include <memory>
#include "Mesh.h"
#include "Transform.h"

class GameEntity {

public: 
	GameEntity(std::shared_ptr<Mesh> mesh);
	~GameEntity();

	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Transform> GetTransform();

	void SetMesh(std::shared_ptr<Mesh> mesh);

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Transform> transform;

};