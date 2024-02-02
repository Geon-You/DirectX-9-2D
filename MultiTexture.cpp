#include "MultiTexture.h"
#include "Device.h"

MultiTexture::MultiTexture()
{
}

MultiTexture::~MultiTexture()
{
	Release();
}

const TEXINFO* MultiTexture::GetTexture(const TCHAR* key, const int& cnt)
{
	auto iter = texMap.find(key);

	if (iter == texMap.end())
	{
		return nullptr;
	}

	return iter->second[cnt];
}

HRESULT MultiTexture::InsertTexture(const TCHAR* path, const TCHAR* key, const int& cnt)
{
	TCHAR szPath[128] = L"";
	vector<TEXINFO*> texInfos;
	texInfos.reserve(cnt);

	for (int i = 0; i < cnt; ++i)
	{
		wsprintf(szPath, path, i);

		TEXINFO* texInfo = new TEXINFO;
		ZeroMemory(texInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szPath, &texInfo->ImgInfo)))
		{
			return E_FAIL;
		}

		if (FAILED(D3DXCreateTextureFromFileEx(
			GET_SINGLE(Device)->GetDevice()
			, szPath
			, texInfo->ImgInfo.Width
			, texInfo->ImgInfo.Height
			, 1
			, 0
			, texInfo->ImgInfo.Format
			, D3DPOOL_MANAGED
			, D3DX_DEFAULT
			, D3DX_DEFAULT
			, D3DCOLOR_ARGB(255, 255, 0, 255)	// 투명하게 만들 색상값 지정
			, &texInfo->ImgInfo
			, NULL
			, &texInfo->texture)))
		{
			return E_FAIL;
		}

		texInfos.push_back(texInfo);
	}

	texMap.insert(make_pair(key, texInfos));
	return S_OK;
}

void MultiTexture::Release()
{
	for (auto iter = texMap.begin(); iter != texMap.end(); ++iter)
	{
		for (int i = 0; i < iter->second.size(); ++i)
		{
			if (iter->second[i] != nullptr)
			{
				iter->second[i]->texture->Release();
				delete iter->second[i];
				iter->second[i] = nullptr;
			}
		}
		iter->second.clear();
	}

	texMap.clear();
}
