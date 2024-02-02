#pragma once
#include "Define.h"
#include "Function.h"

class Object;
class Prototype;
class ObjMgr
{
public:
	DECLARE_SINGLETON(ObjMgr);

private:
	ObjMgr();
public:
	~ObjMgr();

private:
	map<const TCHAR*, list<Object*>>	objs;

public:
	const vector<TILE*>* GetTiles();
	D3DXVECTOR3* GetScroll();
	void SetScroll(float scrollX, float scrollY);
	const D3DXVECTOR3 GetScrollMousePos();
	const INFO* GetInfo(const TCHAR* key, const int& cnt = 0);
	list<Object*>* GetInfoList(const TCHAR* key, const int& cnt = 0);
	const TCHAR* GetObjectStateKeyInfo(const TCHAR* key, const int& cnt = 0);
	Object* GetObjectInfo(const TCHAR* key, const int& cnt = 0);
	map<const TCHAR*, list<Object*>>* GetObjsAddress() { return &objs; }

public:
	Object* AddObject(Prototype* proto, const TCHAR* key);
	SceneType Update();
	void Render();
	void Release();
};

