#pragma once
#include "StaticObj.h"
class ObstaclesBackGround :
    public StaticObj
{
public:
    ObstaclesBackGround();
    ObstaclesBackGround(const INFO& info);
    virtual ~ObstaclesBackGround();

public:
    // StaticObj��(��) ���� ��ӵ�
    virtual HRESULT Initialize() override;
    virtual SceneType Update() override;
    virtual void Render() override;
    virtual void Release() override;
    virtual Object* Clone() override;
};

