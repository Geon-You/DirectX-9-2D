#pragma once
#include "Define.h"
#include "Function.h"

class Object abstract
{
protected:
	bool isDead;
	INFO info;
	TCHAR* objKey;			// 객체의 키 값 (텍스처 키 값과 동일하게 사용하고 있음)
	TCHAR* stateKey;		// 모션의 키 값 (멀티 텍스처일 경우)	
	SortOrder sortOrder;	// 그리는 순서
	FRAME frame;

public:
	virtual void SetRenderInfo(const TCHAR* objKey, SortOrder sortOrder = SORT_FIRST, const TCHAR* stateKey = NULL)
	{
		this->objKey = (TCHAR*)objKey;
		this->stateKey = (TCHAR*)stateKey;
		this->sortOrder = sortOrder;
	}

public:
	virtual HRESULT Initialize() abstract;
	virtual SceneType Update() abstract;
	virtual void Render() abstract;
	virtual void Release() abstract;
	virtual Object* Clone() abstract;
	virtual void Dead() { isDead = true; }

public:
	bool IsDead() { return isDead; }
	const INFO* GetInfo() { return &info; }
	SortOrder GetSortOrder() { return sortOrder; }
	// 현재 프레임 갱신 기능
	void UpdateFrame();
	// 파라미터로 받은 위치에 해당하는 타일의 인덱스를 반환
	int GetTileIndex(const D3DXVECTOR3& pos);
	// 파라미터로 받은 타일 인덱스에 해당하는 타일 위치 반환
	const D3DXVECTOR3 GetTilePos(int idx);
	// 특정 포인트와 특정 타일이 충돌하는지 확인하는 기능
	bool CollPosToTile(const D3DXVECTOR3& pos, const TILE* tileInfo);
	const TCHAR* GetStateKey() { return stateKey; }
	Object* GetObjectInfo() { return this; }

public:
	Object();
	Object(const INFO& info);
	virtual ~Object();
};

