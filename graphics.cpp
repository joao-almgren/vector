
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "ddraw.lib")

#define STRICT
#include "graphics.h"

using namespace std;


Graphics::Graphics()
{
	pDD = 0;
	pDDSPrimary = 0;
	pDDSBack = 0;
}

Graphics::~Graphics()
{
	for (unsigned int i = 0; i < gfx.size(); i++)
		delete gfx[i];

	if (pDD)
	{
		if (pDDSPrimary)
		{
			pDDSPrimary->Release();
			pDDSPrimary = 0;
		}

		pDD->Release();
		pDD = 0;
	}
}

bool Graphics::init(const HWND hwnd)
{
	if (DirectDrawCreateEx(0, (void **)&pDD, IID_IDirectDraw7, 0) != DD_OK)
		return false;

	HRESULT hRet = pDD->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);
	if (hRet != DD_OK)
	{
		pDD->Release();
		pDD = 0;
		return false;
	}

	hRet = pDD->SetDisplayMode(SCREENWIDTH, SCREENHEIGHT, 16, 60, 0);
	if (hRet != DD_OK)
	{
		pDD->Release();
		pDD = 0;
		return false;
	}

	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof DDSURFACEDESC2);
	ddsd.dwSize = sizeof DDSURFACEDESC2;
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 2;
	hRet = pDD->CreateSurface(&ddsd, &pDDSPrimary, 0);
	if (hRet != DD_OK)
	{
		pDD->Release();
		pDD = 0;
		return false;
	}

	DDSCAPS2 ddscaps;
	memset(&ddscaps, 0, sizeof DDSCAPS2);
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	hRet = pDDSPrimary->GetAttachedSurface(&ddscaps, &pDDSBack);
	if (hRet != DD_OK)
	{
		pDDSPrimary->Release();
		pDDSPrimary = 0;
		pDD->Release();
		pDD = 0;
		return false;
	}

	return true;
}

int Graphics::load(const char filename[])
{
	GFX * g = new GFX();
	g->load(filename, pDD);

	int n = gfx.size();
	gfx.push_back(g);
	return n;
}

int Graphics::load(const char filename[], const WORD colorkey)
{
	GFX * g = new TransparentGFX();
	((TransparentGFX *)g)->loadKeyed(filename, pDD, colorkey);

	int n = gfx.size();
	gfx.push_back(g);
	return n;
}

int Graphics::create(const char name[], int width, int height)
{
	GFX * g = new GFX();
	if (!g->create(width, height, pDD))
	{
		delete g;
		return -1;
	}

	int n = gfx.size();
	gfx.push_back(g);
	return n;
}

int Graphics::create(const char name[], int width, int height, WORD colorkey)
{
	GFX * g = new TransparentGFX();
	if (!g->create(width, height, pDD))
	{
		delete g;
		return -1;
	}

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = colorkey;
	ddck.dwColorSpaceHighValue = colorkey;

	if (g->pDDS->SetColorKey(DDCKEY_SRCBLT, &ddck) != DD_OK)
	{
		delete g;
		return -1;
	}

	int n = gfx.size();
	gfx.push_back(g);
	return n;
}

void Graphics::draw(const int handle, Position where)
{
	gfx[handle]->draw(pDDSBack, where);
}

void Graphics::draw(const int handle, Position where, RECT source, bool mirror)
{
	gfx[handle]->draw(pDDSBack, where, source, mirror);
}

void Graphics::draw(const int source, Position where, const int target)
{
	gfx[source]->draw(gfx[target]->pDDS, where);
}

void Graphics::write(const char text[], Position where, const DWORD color)
{
	HDC hdc;
	pDDSBack->GetDC(&hdc);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, color);
	TextOut(hdc, where.x, where.y, text, strlen(text));
	pDDSBack->ReleaseDC(hdc);
}

void Graphics::clear(WORD color)
{
	DDBLTFX ddbltfx;
	memset(&ddbltfx, 0, sizeof DDBLTFX);
	ddbltfx.dwSize = sizeof DDBLTFX;
	ddbltfx.dwFillColor = color;
	pDDSBack->Blt(0, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
}

void Graphics::clear(int handle, WORD color)
{
	DDBLTFX ddbltfx;
	memset(&ddbltfx, 0, sizeof DDBLTFX);
	ddbltfx.dwSize = sizeof DDBLTFX;
	ddbltfx.dwFillColor = color;
	gfx[handle]->pDDS->Blt(0, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
}

void Graphics::flip()
{
	//pDDSPrimary->Flip(0, DDFLIP_NOVSYNC);
	pDDSPrimary->Flip(0, DDFLIP_WAIT);
}

bool Graphics::lock(void ** pBuffer, long * pitch)
{
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof DDSURFACEDESC2);
	ddsd.dwSize = sizeof DDSURFACEDESC2;

	if (!pDDSBack->Lock(0, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WRITEONLY | DDLOCK_WAIT, 0) == DD_OK)
		return false;

	*pBuffer = ddsd.lpSurface;
	*pitch = ddsd.lPitch;
	return true;
}

void Graphics::unlock()
{
	pDDSBack->Unlock(0);
}

bool Graphics::lock(const int handle, void ** pBuffer, long * pitch)
{
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof DDSURFACEDESC2);
	ddsd.dwSize = sizeof DDSURFACEDESC2;

	if (!gfx[handle]->pDDS->Lock(0, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_NOSYSLOCK | DDLOCK_WAIT, 0) == DD_OK)
		return false;

	*pBuffer = ddsd.lpSurface;
	*pitch = ddsd.lPitch;
	return true;
}

void Graphics::unlock(const int handle)
{
	gfx[handle]->pDDS->Unlock(0);
}

WORD lerpCol16(WORD col1, WORD col2, double f)
{
	BYTE r1 = R16(col1);
	BYTE g1 = G16(col1);
	BYTE b1 = B16(col1);

	BYTE r2 = R16(col2);
	BYTE g2 = G16(col2);
	BYTE b2 = B16(col2);

	BYTE r = (BYTE)(f * r2 + (1 - f) * r1);
	BYTE g = (BYTE)(f * g2 + (1 - f) * g1);
	BYTE b = (BYTE)(f * b2 + (1 - f) * b1);

	WORD col = RGB16(r, g, b);

	return col;
}

void Graphics::stretchBlt(unsigned short * pBack, long backPitch, int offsx, int offsy, int trgWidth, int trgHeight, int handle, int srcWidth, int srcHeight)
{
	bool hasKey = false;
	WORD colKey;
	if (gfx[handle]->getColorKey(colKey))
		hasKey = true;

	offsx = offsx - (trgWidth - srcWidth) / 2;
	offsy = offsy - (trgHeight - srcHeight) / 2;

	unsigned short * pBuffer = 0;
	long pitch = 0;
	if (lock(handle, (void**)&pBuffer, &pitch))
	{
		pitch >>= 1; // ingen vet

		double ratiox = (double)(srcWidth - 1) / (double)trgWidth;
		double ratioy = (double)(srcHeight - 1) / (double)trgHeight;

		int osy = 0;

		for (int y = 0; y < trgHeight; y++)
		{
			int osx = 0;

			double nsy = y * ratioy;
			while (nsy - osy >= 1)
			{
				osy++;
			}

			WORD col1a = pBuffer[(osx    ) + (osy    ) * pitch];
			WORD col1b = pBuffer[(osx    ) + (osy + 1) * pitch];
			WORD col2a = pBuffer[(osx + 1) + (osy    ) * pitch];
			WORD col2b = pBuffer[(osx + 1) + (osy + 1) * pitch];

			WORD col1, col2;

			if (hasKey)
			{
				if (col1a == colKey)
				{
					if (col1b == colKey)
						col1 = colKey;
					else
						col1 = col1b;
				}
				else
				{
					if (col1b == colKey)
						col1 = col1a;
					else
						col1 = lerpCol16(col1a, col1b, nsy - osy);
				}

				if (col2a == colKey)
				{
					if (col2b == colKey)
						col2 = colKey;
					else
						col2 = col2b;
				}
				else
				{
					if (col2b == colKey)
						col2 = col2a;
					else
						col2 = lerpCol16(col2a, col2b, nsy - osy);
				}
			}
			else
			{
				col1 = lerpCol16(col1a, col1b, nsy - osy);
				col2 = lerpCol16(col2a, col2b, nsy - osy);
			}

			for (int x = 0; x < trgWidth; x++)
			{
				bool dirty = false;
				int count = 0;
				double nsx = x * ratiox;
				while (nsx - osx >= 1)
				{
					osx++;
					dirty = true;
					count++;
				}
				if (dirty)
				{
					if (count == 1)
					{
						col1a = col2a;
						col1b = col2b;
					}
					else
					{
						col1a = pBuffer[(osx    ) + (osy    ) * pitch];
						col1b = pBuffer[(osx    ) + (osy + 1) * pitch];
					}

					col2a = pBuffer[(osx + 1) + (osy    ) * pitch];
					col2b = pBuffer[(osx + 1) + (osy + 1) * pitch];

					if (hasKey)
					{
						if (col1a == colKey)
						{
							if (col1b == colKey)
								col1 = colKey;
							else
								col1 = col1b;
						}
						else
						{
							if (col1b == colKey)
								col1 = col1a;
							else
								col1 = lerpCol16(col1a, col1b, nsy - osy);
						}

						if (col2a == colKey)
						{
							if (col2b == colKey)
								col2 = colKey;
							else
								col2 = col2b;
						}
						else
						{
							if (col2b == colKey)
								col2 = col2a;
							else
								col2 = lerpCol16(col2a, col2b, nsy - osy);
						}
					}
					else
					{
						col1 = lerpCol16(col1a, col1b, nsy - osy);
						col2 = lerpCol16(col2a, col2b, nsy - osy);
					}
				}

				WORD col;
				if (hasKey)
				{
					if (col1 == colKey)
					{
						if (col2 == colKey)
							continue;
						else
							col = col2;
					}
					else
					{
						if (col2 == colKey)
							col = col1;
						else
							col = lerpCol16(col1, col2, nsx - osx);
					}
				}
				else
				{
					col = lerpCol16(col1, col2, nsx - osx);
				}

				pBack[(x + offsx) + (y + offsy) * backPitch] = col;
			}
		}
		
		unlock(handle);
	}
}

void Graphics::stretchBltFast(unsigned short * pBack, long backPitch, int offsx, int offsy, int trgSize, int handle, int srcSize)
{
	bool hasKey = false;
	WORD colKey;
	if (gfx[handle]->getColorKey(colKey))
		hasKey = true;

	int corr = (trgSize - srcSize) / 2;
	offsx = offsx - corr;
	offsy = offsy - corr;

	unsigned short * pBuffer = 0;
	long pitch = 0;
	if (lock(handle, (void**)&pBuffer, &pitch))
	{
		pitch >>= 1; // ingen vet

		double ratio = (double)(srcSize - 1) / (double)trgSize;

		int osy = 0;

		for (int y = 0; y < trgSize; y++)
		{
			int osx = 0;

			double nsy = y * ratio;
			while (nsy - osy >= 1)
			{
				osy++;
			}

			WORD col = pBuffer[osx + osy * pitch];

			for (int x = 0; x < trgSize; x++)
			{
				bool dirty = false;
				double nsx = x * ratio;
				while (nsx - osx >= 1)
				{
					osx++;
					dirty = true;
				}
				if (dirty)
				{
					col = pBuffer[osx + osy * pitch];
				}

				if (hasKey && col == colKey)
					continue;

				pBack[(x + offsx) + (y + offsy) * backPitch] = col;
			}
		}
		
		unlock(handle);
	}
}

void Graphics::blendBlt(unsigned short * pBack, long backPitch, int offsx, int offsy, int percent, int handle)
{
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof DDSURFACEDESC2;
	gfx[handle]->pDDS->GetSurfaceDesc(&ddsd);

	bool hasKey = false;
	WORD colKey;
	if (gfx[handle]->getColorKey(colKey))
		hasKey = true;

	unsigned short * pBuffer = 0;
	long pitch = 0;
	if (lock(handle, (void**)&pBuffer, &pitch))
	{
		pitch >>= 1; // ingen vet

		for (unsigned int y = 0; y < ddsd.dwHeight; y++)
		{
			for (unsigned int x = 0; x < ddsd.dwWidth; x++)
			{
				WORD col1 = pBuffer[x + y * pitch];

				if (hasKey && col1 == colKey)
					continue;

				WORD col2 = pBack[(x + offsx) + (y + offsy) * backPitch];

				int r = (int)(0.01 * ((100 - percent) * R16(col2) + percent * R16(col1)));
				int g = (int)(0.01 * ((100 - percent) * G16(col2) + percent * G16(col1)));
				int b = (int)(0.01 * ((100 - percent) * B16(col2) + percent * B16(col1)));

				WORD col = RGB16(r, g, b);

				pBack[(x + offsx) + (y + offsy) * backPitch] = col;
			}
		}

		unlock(handle);
	}
}

Graphics::GFX::GFX()
{
	pDDS = NULL;

	memset(&fxMirror, 0, sizeof DDBLTFX);
	fxMirror.dwSize = sizeof DDBLTFX;
	fxMirror.dwDDFX = DDBLTFX_MIRRORLEFTRIGHT;
}

Graphics::GFX::~GFX()
{
	if (pDDS)
		pDDS->Release();
}

bool Graphics::GFX::load(const char filename[], IDirectDraw7 * pDD)
{
//	HBITMAP hbm = (HBITMAP)LoadImage(0, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	HBITMAP hbm = (HBITMAP)LoadImage(GetModuleHandle(0), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (!hbm)
		return false;

	BITMAP bm;
	GetObject(hbm, sizeof BITMAP, &bm);

	sx = bm.bmWidth;
	sy = bm.bmHeight;

	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof ddsd);
	ddsd.dwSize = sizeof ddsd;
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY;//DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = bm.bmWidth;
	ddsd.dwHeight = bm.bmHeight;
	if (pDD->CreateSurface(&ddsd, &pDDS, 0) != DD_OK)
	{
		DeleteObject(hbm);
		return false;
	}

	HDC hdcImage = CreateCompatibleDC(0);
	if (!hdcImage)
	{
		DeleteObject(hbm);
		pDDS->Release();
		pDDS = NULL;
		return false;
	}
	SelectObject(hdcImage, hbm);

	HDC hdc;
	if (pDDS->GetDC(&hdc) != DD_OK)
	{
		DeleteDC(hdcImage);
		DeleteObject(hbm);
		pDDS->Release();
		pDDS = NULL;
		return false;
	}

	if (!BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcImage, 0, 0, SRCCOPY))
	{
		pDDS->ReleaseDC(hdc);
		DeleteDC(hdcImage);
		DeleteObject(hbm);
		pDDS->Release();
		pDDS = NULL;
		return false;
	}

	pDDS->ReleaseDC(hdc);
	DeleteDC(hdcImage);
	DeleteObject(hbm);

	return true;
}

bool Graphics::GFX::create(int width, int height, IDirectDraw7 * pDD)
{
	sx = width;
	sy = height;

	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof ddsd);
	ddsd.dwSize = sizeof ddsd;
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY;//DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;
	if (pDD->CreateSurface(&ddsd, &pDDS, 0) != DD_OK)
		return false;

	return true;
}

bool Graphics::GFX::getColorKey(WORD & colorKey)
{
	return false;
}

bool Graphics::TransparentGFX::loadKeyed(const char filename[], IDirectDraw7 * pDD, const WORD colorkey)
{
	if (!load(filename, pDD))
		return false;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = colorkey;
	ddck.dwColorSpaceHighValue = colorkey;

	if (pDDS->SetColorKey(DDCKEY_SRCBLT, &ddck) != DD_OK)
		return false;

	return true;
}

bool Graphics::TransparentGFX::getColorKey(WORD & colorKey)
{
	DDCOLORKEY ddck;
	pDDS->GetColorKey(DDCKEY_SRCBLT, &ddck);
	colorKey = (WORD)ddck.dwColorSpaceLowValue;

	return true;
}

void Graphics::GFX::draw(IDirectDrawSurface7 * pDDSBack, Position where)
{
	RECT dim = {0, 0, sx, sy};
	pDDSBack->BltFast(where.x, where.y, pDDS, &dim, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
}

void Graphics::GFX::draw(IDirectDrawSurface7 * pDDSBack, Position where, RECT source, bool mirror)
{
	RECT dim = {where.x, where.y, where.x + source.right - source.left, where.y + source.bottom - source.top};

	if (mirror)
		pDDSBack->Blt(&dim, pDDS, &source, DDBLT_WAIT | DDBLT_DDFX, &fxMirror);
	else
		pDDSBack->Blt(&dim, pDDS, &source, DDBLT_WAIT, 0);
}

void Graphics::TransparentGFX::draw(IDirectDrawSurface7 * pDDSBack, Position where)
{
	RECT dim = {0, 0, sx, sy};
	pDDSBack->BltFast(where.x, where.y, pDDS, &dim, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
}

void Graphics::TransparentGFX::draw(IDirectDrawSurface7 * pDDSBack, Position where, RECT source, bool mirror)
{
	if (where.x + (source.right - source.left) <= 0)
		return;

	if (where.x < 0)
	{
		source.left += -where.x;
		where.x = 0;
	}

	if (where.x > SCREENWIDTH)
		return;

	if (where.x + (source.right - source.left) > SCREENWIDTH)
	{
		source.right -= (where.x + (source.right - source.left)) - SCREENWIDTH;
	}

	if (source.right <= source.left)
		return;

	RECT dim = {where.x, where.y, where.x + source.right - source.left, where.y + source.bottom - source.top};

	if (mirror)
		pDDSBack->Blt(&dim, pDDS, &source, DDBLT_KEYSRC | DDBLT_WAIT | DDBLT_DDFX, &fxMirror);
	else
		pDDSBack->Blt(&dim, pDDS, &source, DDBLT_KEYSRC | DDBLT_WAIT, 0);
}
