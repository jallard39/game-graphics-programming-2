#include "GameEntity.h"

GameEntity::GameEntity(std::shared_ptr<Mesh> mesh) : 
	mesh(mesh)
{
	transform = std::make_shared<Transform>();
}

GameEntity::~GameEntity() 
{

}

// ======================
// GETTERS
// ======================

std::shared_ptr<Mesh> GameEntity::GetMesh()
{
	return mesh;
}

std::shared_ptr<Transform> GameEntity::GetTransform()
{
	return transform;
}

// ======================
// SETTERS
// ======================

void GameEntity::SetMesh(std::shared_ptr<Mesh> mesh)
{
	this->mesh = mesh;
}