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

	static bool Virtualitys(XMFLOAT3 PlayerRay, XMFLOAT3 skyPos);
	static bool UnVirtualitys(XMFLOAT3 PlayerRay, XMFLOAT3 skyPos);

	static bool territory(XMFLOAT3 Player, XMFLOAT3 Enemy,char flag);
	static bool ATKcollision(XMFLOAT3 Player, XMFLOAT3 Enemy);
	static bool CoaHit(XMFLOAT3 Coa, XMFLOAT3 Enemy, float rad);

	static bool CoaPlayerHit(XMFLOAT3 coa, XMFLOAT3 player);
	static bool UnCoaPlayerHit(XMFLOAT3 coa, XMFLOAT3 player);

	static bool SnoOrRainHit(XMFLOAT3 snow, XMFLOAT3 enemy,int enmeyFlag,char flag);
	static bool UnSnoOrRainHit(XMFLOAT3 snow, XMFLOAT3 enemy,int enmeyFlag, char flag);

};