#include "Collision.h"

bool Collision::Virtualitys(XMFLOAT3 PlayerRay, XMFLOAT3 skyPos)
{
	float temp = sqrtf(pow(skyPos.x - PlayerRay.x, 2) + pow(skyPos.y - PlayerRay.y, 2) + pow(skyPos.z - PlayerRay.z, 2));

	return 500 - 17 > temp;
}

bool Collision::UnVirtualitys(XMFLOAT3 PlayerRay, XMFLOAT3 skyPos)
{
	float temp = sqrtf(pow(skyPos.x - PlayerRay.x, 2) + pow(skyPos.y - PlayerRay.y, 2) + pow(skyPos.z - PlayerRay.z, 2));

	return 500 - 17 < temp;
}

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

	return rad * 2 > temp[3];
}

bool Collision::CoaPlayerHit(XMFLOAT3 coa, XMFLOAT3 player)
{
	float temp = sqrtf(pow(coa.x - player.x, 2) + pow(coa.y - player.y, 2) + pow(coa.z - player.z, 2));

	return 100 - 50 > temp;
}

bool Collision::UnCoaPlayerHit(XMFLOAT3 coa, XMFLOAT3 player)
{
	float temp = sqrtf(pow(coa.x - player.x, 2) + pow(coa.y - player.y, 2) + pow(coa.z - player.z, 2));

	return 100 - 50 < temp;
}

bool Collision::SnoOrRainHit(XMFLOAT3 snow, XMFLOAT3 enemy, int enmeyFlag, char flag)
{
	float temp = sqrtf(pow(snow.x - enemy.x, 2) + pow(snow.y - enemy.y, 2) + pow(snow.z - enemy.z, 2));

	if (flag == 0)
	{
		if (enmeyFlag == 1)
		{
			return 100 - 50 > temp;
		}
		if (enmeyFlag == 2)
		{
			return 100 - 50 > temp;
		}
		if (enmeyFlag == 3)
		{
			return 100 - 50 > temp;
		}
		if (enmeyFlag == 4)
		{
			return 100 - 50 > temp;
		}
	}
	if (flag == 1)
	{
		if (enmeyFlag == 1)
		{
			return 120 > temp;
		}
		if (enmeyFlag == 2)
		{
			return 120 > temp;
		}
		if (enmeyFlag == 3)
		{
			return 120 > temp;
		}
		if (enmeyFlag == 4)
		{
			return 120 > temp;
		}
	}
	//return 100 - 17 > temp;
}

bool Collision::UnSnoOrRainHit(XMFLOAT3 snow, XMFLOAT3 enemy, int enmeyFlag, char flag)
{
	float temp = sqrtf(pow(snow.x - enemy.x, 2) + pow(snow.y - enemy.y, 2) + pow(snow.z - enemy.z, 2));

	if (flag == 0)
	{
		if (enmeyFlag == 1)
		{
			return 100 - 50 < temp;
		}
		if (enmeyFlag == 2)
		{
			return 100 - 50 < temp;
		}
		if (enmeyFlag == 3)
		{
			return 100 - 50 < temp;
		}
		if (enmeyFlag == 4)
		{
			return 100 - 50 < temp;
		}
	}
	if (flag == 1)
	{
		if (enmeyFlag == 1)
		{
			return 120 < temp;
		}
		if (enmeyFlag == 2)
		{
			return 120 < temp;
		}
		if (enmeyFlag == 3)
		{
			return 120 < temp;
		}
		if (enmeyFlag == 4)
		{
			return 120 < temp;
		}
	}
	//return 100 - 17 < temp;
}
