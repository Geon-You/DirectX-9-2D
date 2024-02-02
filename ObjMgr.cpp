#include "ObjMgr.h"
#include "Object.h"
#include "Prototype.h"
#include "Include.h"

ObjMgr::ObjMgr()
{
}

ObjMgr::~ObjMgr()
{
	this->Release();
}

SceneType ObjMgr::Update()
{
	for (auto iter = objs.begin(); iter != objs.end(); ++iter)
	{
		for (auto sIter = iter->second.begin(); sIter != iter->second.end(); )
		{
			SceneType t = (*sIter)->Update();
			if (t > SCENE_NONE)
				return t;

			// 메모리 해제
			if ((*sIter)->IsDead())
			{
				SafeDelete(*sIter);
				sIter = iter->second.erase(sIter);
			}
			else
			{
				GET_SINGLE(SortMgr)->AddObject(*sIter);
				++sIter;
			}
		}
	}

	return SCENE_NONE;
}

void ObjMgr::Render()
{
	GET_SINGLE(SortMgr)->RenderObject();
}

void ObjMgr::Release()
{
	for (auto iter = objs.begin(); iter != objs.end(); ++iter)
	{
		for (auto sIter = iter->second.begin(); sIter != iter->second.end(); ++sIter)
		{
			SafeDelete(*sIter);
		}
		iter->second.clear();
	}
	objs.clear();
	GET_SINGLE(SortMgr)->Destroy();
}

const vector<TILE*>* ObjMgr::GetTiles()
{
	auto iter = objs.find(L"TileBG");

	if (iter == objs.end())
	{
		ERR_MSG(g_hWnd, L"TileBG 객체가 없습니다.");
		return nullptr;
	}

	return ((TileBackGround*)(iter->second.front()))->GetTiles();
}

D3DXVECTOR3* ObjMgr::GetScroll()
{
	auto iter = objs.find(L"TileBG");

	if (iter == objs.end())
	{
		ERR_MSG(g_hWnd, L"TileBG 객체가 없습니다.");
		return nullptr;
	}

	return ((TileBackGround*)(iter->second.front()))->GetScroll();
}

void ObjMgr::SetScroll(float scrollX, float scrollY)
{
	auto iter = objs.find(L"TileBG");

	if (iter == objs.end())
	{
		ERR_MSG(g_hWnd, L"TileBG 객체가 없습니다.");
	}

	((TileBackGround*)(iter->second.front()))->SetScroll(scrollX, scrollY);
}

const D3DXVECTOR3 ObjMgr::GetScrollMousePos()
{
	auto mousePos = Mouse::GetPos();
	auto scroll = GET_SINGLE(ObjMgr)->GetScroll();

	return D3DXVECTOR3((float)mousePos.x - scroll->x, (float)mousePos.y - scroll->y, 0);
}

const INFO* ObjMgr::GetInfo(const TCHAR* key, const int& cnt)
{
	auto iter = objs.find(key);

	if (iter == objs.end())
		return nullptr;

	auto sIter = iter->second.begin();
	for (int i = 0; i < cnt; ++i)
		++sIter;

	return (*sIter)->GetInfo();
}

list<Object*>* ObjMgr::GetInfoList(const TCHAR* key, const int& cnt)
{
	auto iter = objs.find(key);

	if (iter == objs.end())
		return nullptr;

	auto &sIter = iter->second;
	if (sIter.empty())
		return nullptr;

	return &sIter;
}

const TCHAR* ObjMgr::GetObjectStateKeyInfo(const TCHAR* key, const int& cnt)
{
	auto iter = objs.find(key);

	if (iter == objs.end())
		return nullptr;

	auto sIter = iter->second.begin();
	for (int i = 0; i < cnt; ++i)
		++sIter;

	return (*sIter)->GetStateKey();
}

Object* ObjMgr::GetObjectInfo(const TCHAR* key, const int& cnt)
{
	auto iter = objs.find(key);

	if (iter == objs.end())
		return nullptr;

	auto sIter = iter->second.begin();
	for (int i = 0; i < cnt; ++i)
		++sIter;

	return (*sIter)->GetObjectInfo();
}

Object* ObjMgr::AddObject(Prototype* proto, const TCHAR* key)
{
	// 맵에서 중복된 키값을 가진 객체가 있는지 조사
	auto iter = objs.find((TCHAR*)key);

	// 맵에 저장되어있는 원형객체를 가져옴
	Object* protoInstance = ((ObjProto*)proto)->GetProto(key);

	// 해당 원형객체를 통해 객체를 복제해서 생성
	Object* clone = protoInstance->Clone();
	clone->Initialize();

	// iter가 end와 같다면 해당 키값은 없다.
	if (iter == objs.end())
	{
		list<Object*> objList;
		objList.push_back(clone);

		objs.insert(make_pair(key, objList));
	}
	else
	{
		// 키값이 이미 존재한다면, 해당 객체 리스트에 추가한다.
		iter->second.push_back(clone);
	}

	return clone;
}


