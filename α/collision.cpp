#include "collision.h"

<<<<<<< HEAD
=======
bool Collision::Virtualitys(XMFLOAT3 PlayerRay, XMFLOAT3 skyPos)
{
	float temp = sqrtf(pow(skyPos.x - PlayerRay.x, 2) + pow(skyPos.z - PlayerRay.y, 2) + pow(skyPos.z - PlayerRay.z, 2));
	
	return 500 - 17 > temp;
}

bool Collision::UnVirtualitys(XMFLOAT3 PlayerRay, XMFLOAT3 skyPos)
{
	float temp = sqrtf(pow(skyPos.x - PlayerRay.x, 2) + pow(skyPos.z - PlayerRay.y, 2) + pow(skyPos.z - PlayerRay.z, 2));

	return 500 - 17 < temp;
}

>>>>>>> kinjo(sin)
bool Collision::territory(XMFLOAT3 Player, XMFLOAT3 Enemy)
{
	float temp[4] = {};
	temp[0] = Player.x - Enemy.x;
	temp[1] = Player.y - Enemy.y;
	temp[2] = Player.z - Enemy.z;

	temp[3] = sqrtf((temp[0] * temp[0]) + (temp[1] * temp[1]) + (temp[2] * temp[2]));

	//float sqrtf(temp[3]);

	return temp[3] < sqrtf(10000.0f);
}

bool Collision::ATKcollision(XMFLOAT3 Player, XMFLOAT3 Enemy)
{
	float temp[4];
	temp[0] = Player.x - Enemy.x;
	temp[1] = Player.y - Enemy.y;
	temp[2] = Player.z - Enemy.z;

	temp[3] = sqrtf((temp[0] * temp[0]) + (temp[1] * temp[1]) + (temp[2] * temp[2]));

	return temp[3] <= 10.0f;
}

bool Collision::CoaHit(XMFLOAT3 Coa, XMFLOAT3 Enemy, float rad)
{
	float temp[4] = {};
	temp[0] = Coa.x - Enemy.x;
	temp[1] = Coa.y - Enemy.y;
	temp[2] = Coa.z - Enemy.z;

	temp[3] = sqrtf((temp[0] * temp[0]) + (temp[1] * temp[1]) + (temp[2] * temp[2]));

	return rad*2>temp[3];
}
