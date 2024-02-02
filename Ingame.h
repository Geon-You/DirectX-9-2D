#pragma once
#include "Scene.h"

class Object;
class Ingame :
    public Scene
{
private:
    Object* bg;

    float lastKeyPressedTime = 0;
    float lastSpawnTime = 0; // enemy spawn ����
    float lastHealthPackSpawnTime = GetTickCount() + 30000.f; // item spawn ����, ���� ���� �� 30�� �������� ����
    DWORD delay;
    DWORD spawnDelay; // enemy ���� ������
    DWORD healthPackSpawnDelay; // ȸ���� ���� ������

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

