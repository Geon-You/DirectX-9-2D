#include "SortMgr.h"
#include "Object.h"

SortMgr::SortMgr()
{
}

SortMgr::~SortMgr()
{
}

void SortMgr::AddObject(Object* obj)
{
	auto sortOrder = obj->GetSortOrder();
	objs[sortOrder].push_back(obj);
}

void SortMgr::RenderObject()
{
	for (int i = 0; i < SORT_END; ++i)
	{
		for (auto iter = objs[i].begin(); iter != objs[i].end(); ++iter)
		{
			(*iter)->Render();
		}
		objs[i].clear();
	}
}
