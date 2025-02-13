
#include "Material.h"
#include "Graphics.h"

Material::Material(
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState,
	DirectX::XMFLOAT3 colorTint,
	DirectX::XMFLOAT2 uvScale,
	DirectX::XMFLOAT2 uvOffset
) :
	pipelineState(pipelineState),
	colorTint(colorTint),
	uvScale(uvScale),
	uvOffset(uvOffset)
{
	
}

Material::Material(
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState,
	float r, float g, float b,
	float scaleX, float scaleY,
	float offsetX, float offsetY
) :
	pipelineState(pipelineState)
{
	SetColorTint(r, g, b);
	SetUVScale(scaleX, scaleY);
	SetUVOffset(offsetX, offsetY);
}

Material::~Material()
{

}

void Material::AddTexture(D3D12_CPU_DESCRIPTOR_HANDLE srv, int slot)
{
	textureSRVsBySlot[slot] = srv;
	if (slot > lastIndexUsed) lastIndexUsed = slot;
}

void Material::FinalizeMaterial()
{
	if (!finalized)
	{
		finalGPUHandleForSRVs = Graphics::CopySRVsToDescriptorHeapAndGetGPUDescriptorHandle(textureSRVsBySlot[0], 1);
		for (int i = 1; i <= lastIndexUsed; i++) 
		{
			Graphics::CopySRVsToDescriptorHeapAndGetGPUDescriptorHandle(textureSRVsBySlot[i], 1);
		}
		finalized = true;
	}
}


// ----------------------
// GETTERS
// ----------------------

DirectX::XMFLOAT3 Material::GetColorTint()
{
	return colorTint;
}

DirectX::XMFLOAT2 Material::GetUVScale()
{
	return uvScale;
}

DirectX::XMFLOAT2 Material::GetUVOffset()
{
	return uvOffset;
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> Material::GetPipelineState()
{
	return pipelineState;
}

D3D12_GPU_DESCRIPTOR_HANDLE Material::GetFinalGPUHandleForSRVs()
{
	return finalGPUHandleForSRVs;
}

// ----------------------
// SETTERS
// ----------------------

void Material::SetColorTint(DirectX::XMFLOAT3 colorTint)
{
	this->colorTint = colorTint;
}

void Material::SetColorTint(float r, float g, float b)
{
	this->colorTint = { r, g, b };
}

void Material::SetUVOffset(DirectX::XMFLOAT2 offset)
{
	uvOffset = offset;
}

void Material::SetUVOffset(float x, float y)
{
	uvOffset = { x, y };
}

void Material::SetUVScale(DirectX::XMFLOAT2 scale)
{
	uvScale = scale;
}

void Material::SetUVScale(float x, float y)
{
	uvScale = { x, y };
}