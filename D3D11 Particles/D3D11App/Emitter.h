#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include "Transform.h"
#include "SimpleShader.h"
#include "Camera.h"

struct Particle 
{
	float EmitTime;
	DirectX::XMFLOAT3 StartPos;
};


class Emitter
{
public:
	Emitter(
		int maxParticles,
		float maxParticleLifetime,
		int particlesPerEmit,
		float secondsPerEmit,
		std::shared_ptr<SimpleVertexShader> particleVS,
		std::shared_ptr<SimplePixelShader> particlePS);

	~Emitter();

	std::shared_ptr<Transform> GetTransform();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetParticleTexture();
	int GetIndexFirstAlive();
	int GetIndexFirstDead();
	int GetNumLivingParticles();
	float GetTimeSinceLastEmit();

	void SetParticleTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);
	void SetSampler(Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler);

	void Update(float dt, float currentTime);
	void Draw(float currentTime, std::shared_ptr<FPSCamera> camera);

private:
	int maxParticles;		// Maximum number of particles
	Particle* particles;	// All possible particles

	int indexFirstAlive;
	int indexFirstDead;
	int numLivingParticles;

	float maxParticleLifetime;
	int particlesPerEmit;
	float secondsPerEmit;
	float timeSinceLastEmit;

	std::shared_ptr<Transform> transform;

	Microsoft::WRL::ComPtr<ID3D11Buffer> particleDataBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> particleDataSRV;
	Microsoft::WRL::ComPtr<ID3D11Buffer> particleIndexBuffer;
	int numIndices;

	std::shared_ptr<SimpleVertexShader> particleVS;
	std::shared_ptr<SimplePixelShader> particlePS;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;

	void CreateBuffers();
	void EmitParticle(float currentTime);
	void CopyResourcesToGPU();
};