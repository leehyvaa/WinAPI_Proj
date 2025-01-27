#pragma once
class GameObject;

class CGravity
{
private:
	GameObject* m_pOwner;
	bool m_bGround;

public:
	void SetGround(bool _b);


	void FinalUpdate();
public:
	CGravity();
	~CGravity();

	friend class GameObject;
};

