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
    bool isHandgun; // 캐릭터 무기 소지 여부
    bool isShotgun; // 캐릭터 무기 소지 여부
    static float playerHP; // 캐릭터 체력 관리

private:
    DWORD lastKeyPressedTime;
    DWORD lastHitTime; // 피격 관련 딜레이 설정 멤버
    bool canKeyPress; // 키 입력 상태

    D3DXMATRIX matRotZ; // 회전 행렬 (z축 회전)
    D3DXMATRIX matTrans; // 이동 행렬

    // 캐릭터 이동 범위 제한 멤버
    float upperLimit;
    float leftLimit;
    float bottomLimit;
    float rightLimit;

private:
    void UpdateMotion(); // 캐릭터 모션 업데이트
    void HitEnemy(); // 적과 충돌 검사 메서드
    void GetItem(); // 아이템과 충돌 검사 메서드

public:
    // DynamicObj을(를) 통해 상속됨
    HRESULT Initialize() override;
    SceneType Update() override;
    void Render() override;
    void Release() override;
    Object* Clone() override;
};

