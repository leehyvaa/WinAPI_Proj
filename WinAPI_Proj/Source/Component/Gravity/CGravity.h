#pragma once
class GameObject;

class CGravity
{
private:
	GameObject* m_pOwner;
	bool m_bApplyGravity;

public:
	void SetApplyGravity(bool _b);
    bool IsApplyGravity() {return m_bApplyGravity; }

	void FinalUpdate();
public:
	CGravity();
	~CGravity();

	friend class GameObject;
};

