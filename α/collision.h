#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Collision
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

<<<<<<< HEAD
=======
	static bool Virtualitys(XMFLOAT3 PlayerRay, XMFLOAT3 skyPos);
	static bool UnVirtualitys(XMFLOAT3 PlayerRay, XMFLOAT3 skyPos);

>>>>>>> kinjo(sin)
	static bool territory(XMFLOAT3 Player, XMFLOAT3 Enemy);
	static bool ATKcollision(XMFLOAT3 Player, XMFLOAT3 Enemy);
	static bool CoaHit(XMFLOAT3 Coa, XMFLOAT3 Enemy,float rad);

};