
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ddraw.h>

#pragma warning (disable:4786) // blabla templates blabla
#include <vector>
#include <string>

#define RGB15(r, g, b)	(WORD)((((BYTE)r >> 3) << 10) + (((BYTE)g >> 3) << 5) + (((BYTE)b >> 3)))
#define RGB16(r, g, b)	(WORD)((((BYTE)r >> 3) << 11) + (((BYTE)g >> 2) << 5) + (((BYTE)b >> 3)))
#define R16(col)		(BYTE)((col >> 11) << 3)
#define G16(col)		(BYTE)(((col >> 5) & 0x003F) << 2)
#define B16(col)		(BYTE)(((col) & 0x001F) << 3)

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 600;

class Position
{
	public:
		int x, y;
		Position() { x = y = 0; }
		Position(int a, int b) { x = a; y = b; }
		Position(Position & p) { x = p.x; y = p.y; }
};

class Graphics
{
	public:
		Graphics();
		~Graphics();
		bool init(const HWND hwnd);

		int load(const char filename[]);
		int load(const char filename[], const WORD colorkey);
		void draw(const int handle, Position where);
		void draw(const int source, Position where, const int target);
		void draw(const int handle, Position where, RECT source, bool mirror = false);
		void write(const char text[], Position where, const DWORD color);
		void clear(WORD color = 0);
		void clear(int handle, WORD color);
		void flip();

		bool lock(void ** pBuffer, long * pitch);
		void unlock();

		bool lock(const int handle, void ** pBuffer, long * pitch);
		void unlock(const int handle);

		void stretchBlt(unsigned short * pBack, long backPitch, int offsx, int offsy, int trgWidth, int trgHeight, int handle, int srcWidth, int srcHeight);
		void stretchBltFast(unsigned short * pBack, long backPitch, int offsx, int offsy, int trgSize, int handle, int srcSize);

		void blendBlt(unsigned short * pBack, long backPitch, int offsx, int offsy, int percent, int handle);

		int create(const char name[], int width, int height);
		int create(const char name[], int width, int height, WORD colorkey);

	// public access is easier
		IDirectDraw7 * pDD;
		IDirectDrawSurface7 * pDDSPrimary;
		IDirectDrawSurface7 * pDDSBack;

		class GFX
		{
			public:
				GFX();
				~GFX();

				bool load(const char filename[], IDirectDraw7 * pDD);
				virtual void draw(IDirectDrawSurface7 * pDDSBack, Position where);
				virtual void draw(IDirectDrawSurface7 * pDDSBack, Position where, RECT source, bool mirror = false);
		
				virtual bool getColorKey(WORD & colorKey);

				bool create(int width, int height, IDirectDraw7 * pDD);

				IDirectDrawSurface7 * pDDS;
				DDBLTFX fxMirror;
				int sx, sy;
		};

		class TransparentGFX : public GFX
		{
			public:
				bool loadKeyed(const char filename[], IDirectDraw7 * pDD, const WORD colorkey);
				void draw(IDirectDrawSurface7 * pDDSBack, Position where);
				void draw(IDirectDrawSurface7 * pDDSBack, Position where, RECT source, bool mirror = false);

				bool getColorKey(WORD & colorKey);
		};

		std::vector< GFX * > gfx;
};
