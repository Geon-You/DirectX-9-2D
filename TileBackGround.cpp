#include "TileBackground.h"
#include "Include.h"

TileBackGround::TileBackGround()
	: isDrawTileIndex(false), scroll(0, 0, 0)
{
}

TileBackGround::TileBackGround(const INFO& info)
	:StaticObj(info), isDrawTileIndex(false), scroll(0, 0, 0)
{
}

TileBackGround::~TileBackGround()
{
	Release();
}

void TileBackGround::LoadTileFromFile()
{
	HANDLE	hFile;
	DWORD	dwByte;

	hFile = CreateFile(L"Data/Map/TileMap.dat", GENERIC_READ, 0
		, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	while (true)
	{
		TILE* pTile = new TILE;
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
		if (dwByte == 0)
		{
			SafeDelete(pTile);
			break;
		}
		tiles.push_back(pTile);
	}
	CloseHandle(hFile);
}

void TileBackGround::DrawTileIndex(int index)
{
	TCHAR temp[128] = L"";
	wsprintf(temp, L"%d", index);

	RECT rc = { 0, };

	GET_SINGLE(Device)->GetFont()->DrawTextW(
	GET_SINGLE(Device)->GetSprite(),
		temp,
		lstrlen(temp),
		&rc,
		DT_NOCLIP,
		D3DCOLOR_ARGB(255, 0, 0, 0));
}

HRESULT TileBackGround::Initialize()
{
	LoadTileFromFile();

	objKey = (TCHAR*)L"TileBG";
	stateKey = NULL;
	sortOrder = SORT_FIRST;

	return S_OK;
}

SceneType TileBackGround::Update()
{
	//UpdateScroll();

    return SCENE_NONE;
}

void TileBackGround::Render()
{
	D3DXMATRIX matTrans;

	for (int i = 0; i < TILE_CNT_Y; ++i)
	{
		for (int j = 0; j < TILE_CNT_X; ++j)
		{
			int idx = i * TILE_CNT_X + j;

			auto texInfo = GET_SINGLE(TextureMgr)->GetTexture(objKey, stateKey, tiles[idx]->drawId);

			if (texInfo == nullptr)
				return;

			D3DXMatrixTranslation(&matTrans,
				tiles[idx]->posX + scroll.x,
				tiles[idx]->posY + scroll.y,
				0);

			info.world = matTrans;
			info.center = D3DXVECTOR3(TILE_SIZE_X * .5f, TILE_SIZE_Y * .5f, 0);

			auto rc = RECT{tiles[idx]->drawId * TILE_SIZE_X, 0, tiles[idx]->drawId * TILE_SIZE_X + TILE_SIZE_X, TILE_SIZE_Y };
			GET_SINGLE(Device)->GetSprite()->SetTransform(&info.world);
			GET_SINGLE(Device)->GetSprite()->Draw(texInfo->texture, 
				&rc,
				&info.center,
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			if (isDrawTileIndex)
				DrawTileIndex(idx);
		}
	}
}

void TileBackGround::Release()
{
	for (auto iter = tiles.begin(); iter != tiles.end(); ++iter)
	{
		SafeDelete(*iter);
	}
	tiles.clear();
}

Object* TileBackGround::Clone()
{
	return new TileBackGround(*this);
}
