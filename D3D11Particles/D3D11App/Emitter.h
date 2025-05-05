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

	DirectX::XMFLOAT3 StartVelocity;
};


class Emitter
{
public:
	Emitter(
		float x, float y, float z,
		int maxParticles,
		float maxParticleLifetime,
		float secondsPerParticle,
		DirectX::XMFLOAT4 startColor,
		DirectX::XMFLOAT4 endColor,
		float startSize,
		float endSize,
		float fadeOut,
		DirectX::XMFLOAT3 startVelocity,
		DirectX::XMFLOAT3 velcityRandomRange,
		DirectX::XMFLOAT3 acceleration,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler,
		std::shared_ptr<SimpleVertexShader> particleVS,
		std::shared_ptr<SimplePixelShader> particlePS);

	~Emitter();

	// Getters and Setters
	std::shared_ptr<Transform> GetTransform();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetParticleTexture();
	int GetIndexFirstAlive();
	int GetIndexFirstDead();
	int GetNumLivingParticles();
	float GetTimeSinceLastEmit();

	// Pause functions
	bool IsPaused();
	void Pause();
	void Unpause();

	// Emitter-level variables (same for all particles)
	DirectX::XMFLOAT3 startVelocity;
	DirectX::XMFLOAT3 acceleration;

	// Particle visuals
	DirectX::XMFLOAT4 startColor;
	DirectX::XMFLOAT4 endColor;
	float startSize;
	float endSize;
	float fadeOut;

	// Randomization ranges
	DirectX::XMFLOAT3 velocityRandomRange;

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
	float secondsPerParticle;
	float timeSinceLastEmit;

	bool paused;

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