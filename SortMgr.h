#pragma once
#include "Define.h"
#include "Function.h"

class Object;
class SortMgr
{
public:
	DECLARE_SINGLETON(SortMgr);

private:
	SortMgr();
public:
	~SortMgr();

private:
	list<Object*> objs[SORT_END];

public:
	void AddObject(Object* obj);
	void RenderObject();
};

