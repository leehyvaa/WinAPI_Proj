#include "Player.h"

Player::Player()
{
}

Player::Player(POINT pos, float scale)
{
    position.x = pos.x;
    position.y = pos.y;

    this->scale = scale;
    moveSpeed = 10;




}

Player::~Player()
{
}

void Player::Update()
{
	if ((GetAsyncKeyState(VK_LEFT) & 0x8000))
	{

		position.x += -1;
	}
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000))
	{
		position.x += 1;
	}
	if ((GetAsyncKeyState(VK_UP) & 0x8000))
	{
		position.y += -1;
	}
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000))
	{
		position.y += 1;

	}

}

void Player::Draw(HDC dc)
{
	Rectangle(dc, position.x - scale, position.y - scale, position.x + scale, position.y + scale);
}

bool Player::Collision(GObject& vObj)
{
    return false;
}
