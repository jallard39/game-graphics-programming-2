#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl/client.h>

class Material {

public:
	Material(
		DirectX::XMFLOAT3 colorTint,
		float roughness,
		DirectX::XMFLOAT2 uvScale,
		DirectX::XMFLOAT2 uvOffset
	);
	Material(
		float r, float g, float b,
		float roughness,
		float scaleX, float scaleY,
		float offsetX, float offsetY
	);
	Material(DirectX::XMFLOAT3 colorTint, float roughness);
	Material(float r, float g, float b, float roughness);
	Material(DirectX::XMFLOAT3 colorTint);
	Material(float r, float g, float b);
	~Material();

	DirectX::XMFLOAT4 GetColorTintAndRoughness();
	DirectX::XMFLOAT3 GetColorTint();
	float GetRoughness();
	DirectX::XMFLOAT2 GetUVScale();
	DirectX::XMFLOAT2 GetUVOffset();
	D3D12_GPU_DESCRIPTOR_HANDLE GetFinalGPUHandleForSRVs();

	void SetColorTint(DirectX::XMFLOAT3 colorTint);
	void SetColorTint(float r, float g, float b);
	void SetRoughness(float roughness);
	void SetUVScale(DirectX::XMFLOAT2 scale);
	void SetUVScale(float x, float y);
	void SetUVOffset(DirectX::XMFLOAT2 offset);
	void SetUVOffset(float x, float y);

	void AddTexture(D3D12_CPU_DESCRIPTOR_HANDLE srv, int slot);
	void FinalizeMaterial();

private:
	
	DirectX::XMFLOAT3 colorTint;
	float roughness;
	DirectX::XMFLOAT2 uvScale = { 1.0f, 1.0f };
	DirectX::XMFLOAT2 uvOffset = { 0.0f, 0.0f };

	bool finalized = false;

	D3D12_CPU_DESCRIPTOR_HANDLE textureSRVsBySlot[128]; 
	int lastIndexUsed = 0;

	D3D12_GPU_DESCRIPTOR_HANDLE finalGPUHandleForSRVs;


};