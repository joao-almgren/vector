
#pragma once
#define WIN_32_LEAN_AND_MEAN
#include <windows.h>
#include <ddraw.h>


struct makeRECT : public tagRECT
{
	makeRECT(LONG left, LONG top, LONG right, LONG bottom)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
};

struct makePOINT : public tagPOINT
{
	makePOINT(LONG x, LONG y)
	{
		this->x = x;
		this->y = y;
	}
};


/*
"046.bmp"
 !"., +'()  _- /0123
456789:;   ? ABCDEFG
HIJKLMNOPQRSTUVWXYZ 
*/

RECT mapChar046(char c)
{
	RECT r;

	switch (c)
	{
		case '0':
			r = makeRECT(16 * 16, 0 * 16, 0, 0);
			break;
		case '1':
			r = makeRECT(17 * 16, 0 * 16, 0, 0);
			break;
		case '2':
			r = makeRECT(18 * 16, 0 * 16, 0, 0);
			break;
		case '3':
			r = makeRECT(19 * 16, 0 * 16, 0, 0);
			break;
		case '4':
			r = makeRECT(0 * 16, 1 * 16, 0, 0);
			break;
		case '5':
			r = makeRECT(1 * 16, 1 * 16, 0, 0);
			break;
		case '6':
			r = makeRECT(2 * 16, 1 * 16, 0, 0);
			break;
		case '7':
			r = makeRECT(3 * 16, 1 * 16, 0, 0);
			break;
		case '8':
			r = makeRECT(4 * 16, 1 * 16, 0, 0);
			break;
		case '9':
			r = makeRECT(5 * 16, 1 * 16, 0, 0);
			break;
		case '(':
			r = makeRECT(8 * 16, 0 * 16, 0, 0);
			break;
		case ')':
			r = makeRECT(9 * 16, 0 * 16, 0, 0);
			break;
		case ':':
			r = makeRECT(6 * 16, 1 * 16, 0, 0);
			break;
		case ';':
			r = makeRECT(7 * 16, 1 * 16, 0, 0);
			break;
		case '_':
			r = makeRECT(12 * 16, 0 * 16, 0, 0);
			break;
		case '-':
			r = makeRECT(13 * 16, 0 * 16, 0, 0);
			break;
		case '.':
			r = makeRECT(3 * 16, 0 * 16, 0, 0);
			break;
		case ',':
			r = makeRECT(4 * 16, 0 * 16, 0, 0);
			break;
		case '&':
			r = makeRECT(5 * 16, 0 * 16, 0, 0);
			break;
		case '+':
			r = makeRECT(6 * 16, 0 * 16, 0, 0);
			break;
		case '!':
			r = makeRECT(1 * 16, 0 * 16, 0, 0);
			break;
		case '?':
			r = makeRECT(11 * 16, 1 * 16, 0, 0);
			break;
		case '\'':
			r = makeRECT(7 * 16, 0 * 16, 0, 0);
			break;
		case '\"':
			r = makeRECT(2 * 16, 0 * 16, 0, 0);
			break;
		case '/':
			r = makeRECT(15 * 16, 0 * 16, 0, 0);
			break;
		case 'a':
		case 'A':
			r = makeRECT(13 * 16, 1 * 16, 0, 0);
			break;
		case 'b':
		case 'B':
			r = makeRECT(14 * 16, 1 * 16, 0, 0);
			break;
		case 'c':
		case 'C':
			r = makeRECT(15 * 16, 1 * 16, 0, 0);
			break;
		case 'd':
		case 'D':
			r = makeRECT(16 * 16, 1 * 16, 0, 0);
			break;
		case 'e':
		case 'E':
			r = makeRECT(17 * 16, 1 * 16, 0, 0);
			break;
		case 'f':
		case 'F':
			r = makeRECT(18 * 16, 1 * 16, 0, 0);
			break;
		case 'g':
		case 'G':
			r = makeRECT(19 * 16, 1 * 16, 0, 0);
			break;
		case 'h':
		case 'H':
			r = makeRECT(0 * 16, 2 * 16, 0, 0);
			break;
		case 'i':
		case 'I':
			r = makeRECT(1 * 16, 2 * 16, 0, 0);
			break;
		case 'j':
		case 'J':
			r = makeRECT(2 * 16, 2 * 16, 0, 0);
			break;
		case 'k':
		case 'K':
			r = makeRECT(3 * 16, 2 * 16, 0, 0);
			break;
		case 'l':
		case 'L':
			r = makeRECT(4 * 16, 2 * 16, 0, 0);
			break;
		case 'm':
		case 'M':
			r = makeRECT(5 * 16, 2 * 16, 0, 0);
			break;
		case 'n':
		case 'N':
			r = makeRECT(6 * 16, 2 * 16, 0, 0);
			break;
		case 'o':
		case 'O':
			r = makeRECT(7 * 16, 2 * 16, 0, 0);
			break;
		case 'p':
		case 'P':
			r = makeRECT(8 * 16, 2 * 16, 0, 0);
			break;
		case 'q':
		case 'Q':
			r = makeRECT(9 * 16, 2 * 16, 0, 0);
			break;
		case 'r':
		case 'R':
			r = makeRECT(10 * 16, 2 * 16, 0, 0);
			break;
		case 's':
		case 'S':
			r = makeRECT(11 * 16, 2 * 16, 0, 0);
			break;
		case 't':
		case 'T':
			r = makeRECT(12 * 16, 2 * 16, 0, 0);
			break;
		case 'u':
		case 'U':
			r = makeRECT(13 * 16, 2 * 16, 0, 0);
			break;
		case 'v':
		case 'V':
			r = makeRECT(14 * 16, 2 * 16, 0, 0);
			break;
		case 'w':
		case 'W':
			r = makeRECT(15 * 16, 2 * 16, 0, 0);
			break;
		case 'x':
		case 'X':
			r = makeRECT(16 * 16, 2 * 16, 0, 0);
			break;
		case 'y':
		case 'Y':
			r = makeRECT(17 * 16, 2 * 16, 0, 0);
			break;
		case 'z':
		case 'Z':
			r = makeRECT(18 * 16, 2 * 16, 0, 0);
			break;
		case ' ':
		default:
			r = makeRECT(0 * 16, 0 * 16, 0, 0);
			break;
	}

	r.right = r.left + 16;
	r.bottom = r.top + 16;

	return r;
}

/*
"084.bmp"
 !""   '()
  ,-. 0123
456789:;  
 ? ABCDEFG
HIJKLMNOPQ
RSTUVWXYZ 
*/

RECT mapChar084(char c)
{
	RECT r;

	switch (c)
	{
		case '@':
			r = makeRECT(3 * 32, 0 * 32, 0, 0);
			break;
		case '0':
			r = makeRECT(6 * 32, 1 * 32, 0, 0);
			break;
		case '1':
			r = makeRECT(7 * 32, 1 * 32, 0, 0);
			break;
		case '2':
			r = makeRECT(8 * 32, 1 * 32, 0, 0);
			break;
		case '3':
			r = makeRECT(9 * 32, 1 * 32, 0, 0);
			break;
		case '4':
			r = makeRECT(0 * 32, 2 * 32, 0, 0);
			break;
		case '5':
			r = makeRECT(1 * 32, 2 * 32, 0, 0);
			break;
		case '6':
			r = makeRECT(2 * 32, 2 * 32, 0, 0);
			break;
		case '7':
			r = makeRECT(3 * 32, 2 * 32, 0, 0);
			break;
		case '8':
			r = makeRECT(4 * 32, 2 * 32, 0, 0);
			break;
		case '9':
			r = makeRECT(5 * 32, 2 * 32, 0, 0);
			break;
		case '(':
			r = makeRECT(8 * 32, 0 * 32, 0, 0);
			break;
		case ')':
			r = makeRECT(9 * 32, 0 * 32, 0, 0);
			break;
		case ':':
			r = makeRECT(6 * 32, 2 * 32, 0, 0);
			break;
		case ';':
			r = makeRECT(7 * 32, 2 * 32, 0, 0);
			break;
		case '-':
			r = makeRECT(3 * 32, 1 * 32, 0, 0);
			break;
		case '.':
			r = makeRECT(4 * 32, 1 * 32, 0, 0);
			break;
		case ',':
			r = makeRECT(2 * 32, 1 * 32, 0, 0);
			break;
		case '!':
			r = makeRECT(1 * 32, 0 * 32, 0, 0);
			break;
		case '?':
			r = makeRECT(1 * 32, 3 * 32, 0, 0);
			break;
		case '\'':
			r = makeRECT(7 * 32, 0 * 32, 0, 0);
			break;
		case '\"':
			r = makeRECT(2 * 32, 0 * 32, 0, 0);
			break;
		case 'A':
		case 'a':
			r = makeRECT(3 * 32, 3 * 32, 0, 0);
			break;
		case 'B':
		case 'b':
			r = makeRECT(4 * 32, 3 * 32, 0, 0);
			break;
		case 'C':
		case 'c':
			r = makeRECT(5 * 32, 3 * 32, 0, 0);
			break;
		case 'D':
		case 'd':
			r = makeRECT(6 * 32, 3 * 32, 0, 0);
			break;
		case 'E':
		case 'e':
			r = makeRECT(7 * 32, 3 * 32, 0, 0);
			break;
		case 'F':
		case 'f':
			r = makeRECT(8 * 32, 3 * 32, 0, 0);
			break;
		case 'G':
		case 'g':
			r = makeRECT(9 * 32, 3 * 32, 0, 0);
			break;
		case 'H':
		case 'h':
			r = makeRECT(0 * 32, 4 * 32, 0, 0);
			break;
		case 'I':
		case 'i':
			r = makeRECT(1 * 32, 4 * 32, 0, 0);
			break;
		case 'J':
		case 'j':
			r = makeRECT(2 * 32, 4 * 32, 0, 0);
			break;
		case 'K':
		case 'k':
			r = makeRECT(3 * 32, 4 * 32, 0, 0);
			break;
		case 'L':
		case 'l':
			r = makeRECT(4 * 32, 4 * 32, 0, 0);
			break;
		case 'M':
		case 'm':
			r = makeRECT(5 * 32, 4 * 32, 0, 0);
			break;
		case 'N':
		case 'n':
			r = makeRECT(6 * 32, 4 * 32, 0, 0);
			break;
		case 'O':
		case 'o':
			r = makeRECT(7 * 32, 4 * 32, 0, 0);
			break;
		case 'P':
		case 'p':
			r = makeRECT(8 * 32, 4 * 32, 0, 0);
			break;
		case 'Q':
		case 'q':
			r = makeRECT(9 * 32, 4 * 32, 0, 0);
			break;
		case 'R':
		case 'r':
			r = makeRECT(0 * 32, 5 * 32, 0, 0);
			break;
		case 'S':
		case 's':
			r = makeRECT(1 * 32, 5 * 32, 0, 0);
			break;
		case 'T':
		case 't':
			r = makeRECT(2 * 32, 5 * 32, 0, 0);
			break;
		case 'U':
		case 'u':
			r = makeRECT(3 * 32, 5 * 32, 0, 0);
			break;
		case 'V':
		case 'v':
			r = makeRECT(4 * 32, 5 * 32, 0, 0);
			break;
		case 'W':
		case 'w':
			r = makeRECT(5 * 32, 5 * 32, 0, 0);
			break;
		case 'X':
		case 'x':
			r = makeRECT(6 * 32, 5 * 32, 0, 0);
			break;
		case 'Y':
		case 'y':
			r = makeRECT(7 * 32, 5 * 32, 0, 0);
			break;
		case 'Z':
		case 'z':
			r = makeRECT(8 * 32, 5 * 32, 0, 0);
			break;
		case ' ':
		default:
			r = makeRECT(0 * 32, 0 * 32, 0, 0);
			break;
	}

	r.right = r.left + 32;
	r.bottom = r.top + 32;

	return r;
}

int scrollSize084(char * s)
{
	return (int)(strlen(s) + 1) * (32 + 2);
}

int scrollSize046(char * s)
{
	return (int)(strlen(s) + 1) * (16 + 2);
}

void updateSurface(IDirectDrawSurface7 * pSource, RECT sourceRect, IDirectDrawSurface7 * pDest, POINT destPoint)
{
	DDCOLORKEY ddck;
	if (DDERR_NOCOLORKEY == pSource->GetColorKey(DDCKEY_SRCBLT, &ddck))
	{
		pDest->BltFast(destPoint.x, destPoint.y, pSource, &sourceRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
	}
	else
	{
		WORD colorKey = (WORD)ddck.dwColorSpaceLowValue;
		pDest->BltFast(destPoint.x, destPoint.y, pSource, &sourceRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
	}
}

void bltTexture(IDirectDrawSurface7 * pSource, RECT sourceRect, IDirectDrawSurface7 * pDest, POINT destPoint)
{
	if (destPoint.x + (sourceRect.right - sourceRect.left) <= 0)
		return;

	if (destPoint.x < 0)
	{
		sourceRect.left += -destPoint.x;
		destPoint.x = 0;
	}

	DDSURFACEDESC2 destDesc;
	destDesc.dwSize = sizeof DDSURFACEDESC2;

	HRESULT hr = pDest->GetSurfaceDesc(&destDesc);
	if (FAILED(hr))
		return;

	if (destPoint.x > (LONG)destDesc.dwWidth)
		return;

	if (destPoint.x + (sourceRect.right - sourceRect.left) > (LONG)destDesc.dwWidth)
	{
		sourceRect.right -= (destPoint.x + (sourceRect.right - sourceRect.left)) - destDesc.dwWidth;
	}

	if (sourceRect.right <= sourceRect.left)
		return;

	updateSurface(pSource, sourceRect, pDest, destPoint);
}

void bltFont084(char * s, POINT destPoint, IDirectDrawSurface7 * pDest, IDirectDrawSurface7 * pSource)
{
	for (int i = 0; s[i] != '\0'; i++)
	{
		RECT r = mapChar084(s[i]);

		POINT p = makePOINT(destPoint.x, (LONG)(destPoint.y + sin(4 * M_PI * (float)destPoint.x / SCREENWIDTH) * 10));

		bltTexture(pSource, r, pDest, p);

		destPoint.x += 32 + 2;
	}
}

void bltFont046(char * s, POINT destPoint, IDirectDrawSurface7 * pDest, IDirectDrawSurface7 * pSource)
{
	for (int i = 0; s[i] != '\0'; i++)
	{
		RECT r = mapChar046(s[i]);

		bltTexture(pSource, r, pDest, destPoint);

		destPoint.x += 16 + 2;
	}
}
