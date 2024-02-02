#include "TextureMgr.h"
#include "Texture.h"
#include "Include.h"

TextureMgr::TextureMgr()
{
}

TextureMgr::~TextureMgr()
{
	Release();
}

HRESULT TextureMgr::InsertTexture(const TCHAR* path, const TCHAR* objKey, TexType texType, const TCHAR* stateKey, const int& cnt)
{
	auto iter = texMap.find(objKey);

	if (iter != texMap.end())
		return E_FAIL;

	Texture* pTexture = NULL;
	switch (texType)
	{
	case TEX_SINGLE:
		pTexture = new SingleTexture;
		break;
	case TEX_MULTI:
		pTexture = new MultiTexture;
		break;
	}

	if (FAILED(pTexture->InsertTexture(path, stateKey, cnt)))
		return E_FAIL;

	texMap.insert(make_pair(objKey, pTexture));
	return S_OK;
}

const TEXINFO* TextureMgr::GetTexture(const TCHAR* pObjKey, const TCHAR* pStateKey, const int& iCnt)
{
	auto iter = texMap.find(pObjKey);

	if (iter == texMap.end())
		return nullptr;

	return iter->second->GetTexture(pStateKey, iCnt);
}

void TextureMgr::Release()
{
	for (auto iter = texMap.begin(); iter != texMap.end(); ++iter)
	{
		SafeDelete(iter->second);
	}
	texMap.clear();
}
