#pragma once


#define SINGLE(type) public:\
static type* GetInst()\
{\
	static type mgr;\
	return &mgr;\
}\
private:\
type();\
~type();



#define fDT CTimeMgr::GetInst()->GetfDT()
#define DT CTimeMgr::GetInst()->GetDT()

#define CLONE(type) type* Clone(){return new type(*this);}

#define KEY_CHECK(key, state) CKeyMgr::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define MOUSE_POS CKeyMgr::GetInst()->GetMousePos()



#define TILE_SIZE 48

enum class GROUP_TYPE
{
	DEFAULT,
	BACKGROUND,
	TILE,
	GROUND,

	PLAYER,
	MONSTER,
	PLAYERARM,

	PROJ_PLAYER,
	PROJ_MONSTER,

	Ray = 30,
	UI = 31,
	END = 32,
};

enum class SCENE_TYPE
{
	START,
	TOOL,

	STAGE_01,
	STAGE_02,

	END,
};

enum class BRUSH_TYPE
{
	HOLLOW,
	BLACK,
	RED,
	MAGENTA,
	END,
};

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,
	PURPLE,
	ORANGE,
	END,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,
	CHANGE_AI_STATE,

	END,
};

enum class MON_STATE
{
	IDLE,
	PATROL,
	TRACE,
	ATTACK,
	RUN,
	DEAD,
	END,
};

enum class GROUND_TYPE
{
	GROUND,
	NONGROUND,
	DAMAGEZONE,
	DEADZONE,
	END,
};

enum class PLAYER_STATE
{
	IDLE,
	RUN,
	ATTACK,
	JUMP,
	CLIMB,
	CLIMBUP,
	CLIMBDOWN,
	SHOT,
	SWING,
	FALL,
	DAMAGED,
	DEAD,

};