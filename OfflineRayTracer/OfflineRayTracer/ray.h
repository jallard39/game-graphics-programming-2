#pragma once
#ifndef RAY_H
#define RAY_H

#include "DirectXMath.h"

class ray {
public:
	ray() {}

	ray(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& direction) :
		orig(origin),
		dir(direction) {}

	const DirectX::XMFLOAT3& origin() const	{ return orig; }
	const DirectX::XMFLOAT3& direction() const { return dir;  }

	DirectX::XMFLOAT3 at(float t) const {
		DirectX::XMVECTOR dirVec = DirectX::XMLoadFloat3(&dir);
		DirectX::XMVECTOR scaleVec = DirectX::XMVectorScale(dirVec, t);
		DirectX::XMFLOAT3 result;
		DirectX::XMStoreFloat3(&result, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&orig), scaleVec));
		return result;
	}

private:
	DirectX::XMFLOAT3 orig;
	DirectX::XMFLOAT3 dir;
};


#endif