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
    NON_GROUND,
       
    PLAYER,
    PLAYER_WEAPON,
    MONSTER,
    PLAYER_ARM,
    MONSTER_HEAD,
	HOOK,
	PROJ_MONSTER,
    TRIGGER,
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

// Legacy GDI brush and pen types - kept for compatibility
/*
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
    BIGGREEN,
    HOLLOW,
	END,
};
*/

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
    SPAWNING,
	IDLE,
	PATROL,
    AIMING,
	TRACE,
    EXECUTED,
    
    SUBDUED,
	ATTACK,
	DEAD,
	END,
};

enum class VERTEX_POSITION
{
    NONE,
    TOP_LEFT,
    BOT_RIGHT,
};

enum class TILE_COLLIDE_TYPE {
    NONE,
    SOLID,        // 완전 충돌
    TOP_PLATFORM, // 플랫폼(위에서만 충돌)
    BOT_PLATFORM, // 플랫폼(밑에서만 충돌)
    SLOPE_LEFT,   // 왼쪽 경사
    SLOPE_RIGHT,   // 오른쪽 경사
    END,
};

enum class GROUND_TYPE
{
    NONE,
	NORMAL,
    UNWALKABLE,
	DAMAGEZONE,
	DEADZONE,
	END,
};

enum class PLAYER_STATE
{
	IDLE,
	RUN,
	EXECUTE,
	JUMP,
	FALL,
	CLIMB,
	SHOT,
	SWING,
	DAMAGED,
	DEAD,

};

enum class PLAYER_CLIMB_STATE
{
    NONE,
    UP,
    DOWN,
    END,
};

enum class WALL_STATE
{
    CLOSE,
    OPENING,
    OPEN,
    CLOSING,
    END,
};