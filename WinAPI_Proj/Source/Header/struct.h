#pragma once

enum ObjectType
{
	Player,
	Enemy,
	Boss,
};

enum PlayerDir
{
	UP,
	RIGHT,
	DOWN,
	LEFT,
	DEFAULT,
};

struct MovePoint
{
	int x;
	int y;
	PlayerDir dir;

};

