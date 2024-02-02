#pragma once
#include "Scene.h"

class Object;
class Ingame :
    public Scene
{
private:
    Object* bg;

    float lastKeyPressedTime = 0;
    float lastSpawnTime = 0; // enemy spawn 관련
    float lastHealthPackSpawnTime = GetTickCount() + 30000.f; // item spawn 관련, 게임 시작 후 30초 간격으로 생성
    DWORD delay;
    DWORD spawnDelay; // enemy 스폰 딜레이
    DWORD healthPackSpawnDelay; // 회복약 스폰 딜레이

    list<Object*> enemys;

public:
    HRESULT Initialize() override;
    void Update() override;
    void Render() override;
    void Release() override;

public:
    Ingame();
    virtual ~Ingame();
};

