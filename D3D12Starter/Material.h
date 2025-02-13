#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl/client.h>

class Material {

public:
	Material(
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState,
		DirectX::XMFLOAT3 colorTint,
		DirectX::XMFLOAT2 uvScale,
		DirectX::XMFLOAT2 uvOffset
	);
	Material(
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState,
		float r, float g, float b,
		float scaleX, float scaleY,
		float offsetX, float offsetY
	);
	~Material();

	DirectX::XMFLOAT3 GetColorTint();
	DirectX::XMFLOAT2 GetUVScale();
	DirectX::XMFLOAT2 GetUVOffset();
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState();
	D3D12_GPU_DESCRIPTOR_HANDLE GetFinalGPUHandleForSRVs();

	void SetColorTint(DirectX::XMFLOAT3 colorTint);
	void SetColorTint(float r, float g, float b);
	void SetUVScale(DirectX::XMFLOAT2 scale);
	void SetUVScale(float x, float y);
	void SetUVOffset(DirectX::XMFLOAT2 offset);
	void SetUVOffset(float x, float y);

	void AddTexture(D3D12_CPU_DESCRIPTOR_HANDLE srv, int slot);
	void FinalizeMaterial();

private:
	
	DirectX::XMFLOAT3 colorTint;
	DirectX::XMFLOAT2 uvScale = { 1.0f, 1.0f };
	DirectX::XMFLOAT2 uvOffset = { 0.0f, 0.0f };

	bool finalized = false;

	D3D12_CPU_DESCRIPTOR_HANDLE textureSRVsBySlot[128]; 
	int lastIndexUsed = 0;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	D3D12_GPU_DESCRIPTOR_HANDLE finalGPUHandleForSRVs;


};