
#include "Material.h"
#include "Graphics.h"

Material::Material(
	DirectX::XMFLOAT3 colorTint,
	float roughness,
	DirectX::XMFLOAT2 uvScale,
	DirectX::XMFLOAT2 uvOffset
) :
	colorTint(colorTint),
	roughness(roughness),
	uvScale(uvScale),
	uvOffset(uvOffset)
{
	
}

Material::Material(
	float r, float g, float b,
	float roughness,
	float scaleX, float scaleY,
	float offsetX, float offsetY
) {
	SetColorTint(r, g, b);
	SetRoughness(roughness);
	SetUVScale(scaleX, scaleY);
	SetUVOffset(offsetX, offsetY);
}

Material::Material(DirectX::XMFLOAT3 colorTint, float roughness) :
	colorTint(colorTint),
	roughness(roughness)
{
	SetUVScale(1.0, 1.0);
	SetUVOffset(0.0, 0.0);
}

Material::Material(float r, float g, float b, float roughness)
{
	SetColorTint(r, g, b);
	SetRoughness(roughness);
	SetUVScale(1.0, 1.0);
	SetUVOffset(0.0, 0.0);
}

Material::Material(DirectX::XMFLOAT3 colorTint) : 
	colorTint(colorTint)
{
	SetRoughness(1.0f);
	SetUVScale(1.0, 1.0);
	SetUVOffset(0.0, 0.0);
}

Material::Material(float r, float g, float b)
{
	SetColorTint(r, g, b);
	SetRoughness(1.0f);
	SetUVScale(1.0, 1.0);
	SetUVOffset(0.0, 0.0);
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

DirectX::XMFLOAT4 Material::GetColorTintAndRoughness()
{
	return DirectX::XMFLOAT4(colorTint.x, colorTint.y, colorTint.z, roughness);
}

DirectX::XMFLOAT3 Material::GetColorTint()
{
	return colorTint;
}

float Material::GetRoughness()
{
	return roughness;
}

DirectX::XMFLOAT2 Material::GetUVScale()
{
	return uvScale;
}

DirectX::XMFLOAT2 Material::GetUVOffset()
{
	return uvOffset;
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

void Material::SetRoughness(float roughness)
{
	this->roughness = roughness;
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