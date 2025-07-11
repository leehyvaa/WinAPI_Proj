cmake_minimum_required(VERSION 3.10)
project(WinAPI_Proj)

# Force the generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add necessary preprocessor definitions
add_compile_definitions(UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS)

# Add all your source files here
add_executable(WinAPI_Proj
    "WinAPI_Proj/Source/CHook.cpp"
    "WinAPI_Proj/Source/Component/Animator/Animation/CAnimation.cpp"
    "WinAPI_Proj/Source/Component/Animator/CAnimator.cpp"
    "WinAPI_Proj/Source/Component/Collider/CCollider.cpp"
    "WinAPI_Proj/Source/Component/Gravity/CGravity.cpp"
    "WinAPI_Proj/Source/Component/RigidBody/CRigidBody.cpp"
    "WinAPI_Proj/Source/Core/CCore.cpp"
    "WinAPI_Proj/Source/Header/func.cpp"
    "WinAPI_Proj/Source/Main.cpp"
    "WinAPI_Proj/Source/Manager/BrushMgr/CBrushManager.cpp"
    "WinAPI_Proj/Source/Manager/CObjectPool.cpp"
    "WinAPI_Proj/Source/Manager/CSoundMgr/CSoundMgr.cpp"
    "WinAPI_Proj/Source/Manager/Camera/CCamera.cpp"
    "WinAPI_Proj/Source/Manager/CollisionMgr/CCollisionMgr.cpp"
    "WinAPI_Proj/Source/Manager/EffectMgr/CEffectMgr.cpp"
    "WinAPI_Proj/Source/Manager/EventMgr/CEventMgr.cpp"
    "WinAPI_Proj/Source/Manager/KeyMgr/CKeyMgr.cpp"
    "WinAPI_Proj/Source/Manager/PathMgr/CPathMgr.cpp"
    "WinAPI_Proj/Source/Manager/ResMgr/CResMgr.cpp"
    "WinAPI_Proj/Source/Manager/SceneMgr/CSceneMgr.cpp"
    "WinAPI_Proj/Source/Manager/TimeMgr/CTimeMgr.cpp"
    "WinAPI_Proj/Source/Manager/UIMgr/CUIMgr.cpp"
    "WinAPI_Proj/Source/Module/AI/AI.cpp"
    "WinAPI_Proj/Source/Module/AI/State/CState.cpp"
    "WinAPI_Proj/Source/Module/AI/State/Dead/CDeadState.cpp"
    "WinAPI_Proj/Source/Module/AI/State/Idle/CIdleState.cpp"
    "WinAPI_Proj/Source/Module/AI/State/Shooter/CAimingState.cpp"
    "WinAPI_Proj/Source/Module/AI/State/Spawning/CSpawningState.cpp"
    "WinAPI_Proj/Source/Module/AI/State/Subdued/CSubduedState.cpp"
    "WinAPI_Proj/Source/Module/AI/State/Trace/CTraceState.cpp"
    "WinAPI_Proj/Source/MouseCursor.cpp"
    "WinAPI_Proj/Source/Object/CBullet.cpp"
    "WinAPI_Proj/Source/Object/CEffect.cpp"
    "WinAPI_Proj/Source/Object/CMonster.cpp"
    "WinAPI_Proj/Source/Object/GameObject.cpp"
    "WinAPI_Proj/Source/Object/Ground/CGround.cpp"
    "WinAPI_Proj/Source/Object/Ground/CNormalGround.cpp"
    "WinAPI_Proj/Source/Object/Ground/CSkylineCar.cpp"
    "WinAPI_Proj/Source/Object/Ground/CWall.cpp"
    "WinAPI_Proj/Source/Object/Monster/CShooterHead.cpp"
    "WinAPI_Proj/Source/Object/Monster/CShooterMonster.cpp"
    "WinAPI_Proj/Source/Object/Tile/CBackGround.cpp"
    "WinAPI_Proj/Source/Object/Tile/CTile.cpp"
    "WinAPI_Proj/Source/Object/Trigger/CTrigger.cpp"
    "WinAPI_Proj/Source/Object/UI/BtnUI/CBtnUI.cpp"
    "WinAPI_Proj/Source/Object/UI/CUI.cpp"
    "WinAPI_Proj/Source/Object/UI/ChatUI/CTextUI.cpp"
    "WinAPI_Proj/Source/Object/UI/DamageEffect/CDamageEffectUI.cpp"
    "WinAPI_Proj/Source/Object/UI/PanelUI/CPanelUI.cpp"
    "WinAPI_Proj/Source/PlayerArm.cpp"
    "WinAPI_Proj/Source/Prefab/CGroundPrefab.cpp"
    "WinAPI_Proj/Source/Prefab/CMonPrefab.cpp"
    "WinAPI_Proj/Source/Raycast.cpp"
    "WinAPI_Proj/Source/Resource/CRes.cpp"
    "WinAPI_Proj/Source/Resource/Sound/CSound.cpp"
    "WinAPI_Proj/Source/Resource/Texture/CTexture.cpp"
    "WinAPI_Proj/Source/SPlayer.cpp"
    "WinAPI_Proj/Source/Scene/CScene.cpp"
    "WinAPI_Proj/Source/Scene/CScene_Start.cpp"
    "WinAPI_Proj/Source/Scene/Scene_Stage_01/Scene_Stage_01.cpp"
    "WinAPI_Proj/Source/Scene/Scene_Title/CScene_Title.cpp"
    "WinAPI_Proj/Source/Scene/Scene_Tool/CScene_Tool.cpp"
    "WinAPI_Proj/Source/Tool/CTool.cpp"
    "WinAPI_Proj/Source/Tool/Vec2.cpp"
)

# Include directories using absolute paths
target_include_directories(WinAPI_Proj PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Component"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Component/Animator"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Component/Animator/Animation"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Component/Collider"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Component/Gravity"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Component/RigidBody"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Header"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Manager"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Manager/BrushMgr"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Manager/Camera"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Manager/CollisionMgr"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Manager/CSoundMgr"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Manager/EffectMgr"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Manager/EventMgr"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Manager/KeyMgr"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Manager/PathMgr"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Manager/ResMgr"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Manager/SceneMgr"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Manager/TimeMgr"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Manager/UIMgr"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Module"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Module/AI"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Module/AI/State"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Module/AI/State/Dead"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Module/AI/State/Idle"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Module/AI/State/Shooter"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Module/AI/State/Spawning"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Module/AI/State/Subdued"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Module/AI/State/Trace"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Object"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Object/Ground"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Object/Monster"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Object/Tile"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Object/Trigger"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Object/UI"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Object/UI/BtnUI"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Object/UI/ChatUI"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Object/UI/DamageEffect"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Object/UI/PanelUI"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Prefab"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Resource"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Resource/Sound"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Resource/Texture"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Scene"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Scene/Scene_Stage_01"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Scene/Scene_Title"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Scene/Scene_Tool"
    "${CMAKE_CURRENT_SOURCE_DIR}/WinAPI_Proj/Source/Tool"
)

# Link necessary libraries
target_link_libraries(WinAPI_Proj
    gdi32
    winmm
    dsound
    d2d1
    dwrite
    windowscodecs
)
