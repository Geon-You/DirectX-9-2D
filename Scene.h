#pragma once
#include "Define.h"

class Prototype;
class Scene
{
protected:
	// 씬 별로 원형객체를 생성하고 관리할 객체
	Prototype* proto;

public:
	virtual HRESULT Initialize() abstract;
	virtual void Update() abstract;
	virtual void Render() abstract;
	virtual void Release() abstract;

public:
	Scene();
	virtual ~Scene();
};

