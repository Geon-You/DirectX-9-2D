#pragma once
#include "DynamicObj.h"

class Player :
    public DynamicObj
{
public:
    Player();
    Player(const INFO& info);
    virtual ~Player();

public:
    bool isHandgun; // ĳ���� ���� ���� ����
    bool isShotgun; // ĳ���� ���� ���� ����
    static float playerHP; // ĳ���� ü�� ����

private:
    DWORD lastKeyPressedTime;
    DWORD lastHitTime; // �ǰ� ���� ������ ���� ���
    bool canKeyPress; // Ű �Է� ����

    D3DXMATRIX matRotZ; // ȸ�� ��� (z�� ȸ��)
    D3DXMATRIX matTrans; // �̵� ���

    // ĳ���� �̵� ���� ���� ���
    float upperLimit;
    float leftLimit;
    float bottomLimit;
    float rightLimit;

private:
    void UpdateMotion(); // ĳ���� ��� ������Ʈ
    void HitEnemy(); // ���� �浹 �˻� �޼���
    void GetItem(); // �����۰� �浹 �˻� �޼���

public:
    // DynamicObj��(��) ���� ��ӵ�
    HRESULT Initialize() override;
    SceneType Update() override;
    void Render() override;
    void Release() override;
    Object* Clone() override;
};

