
#define WIN_32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib, "winmm.lib")

#define _USE_MATH_DEFINES
#include <math.h>

#include <queue>
using namespace std;

#include "minifmod/minifmod.h"
#include "helper.h"

#include "resource.h"
#include "graphics.h"
#include "input.h"
#include "fontmap.h"
#include "torus.h"
#include "timer.h"


Graphics graphics;
Input input;
Timer * timer = getTimer();


Position project(Vector * pt)
{
	const double r = SCREENWIDTH / 2;
	return Position((int)(r * pt->x / pt->z + r), (int)(r * pt->y / pt->z + SCREENHEIGHT / 2));
}

class Bobber
{
public:
	Vector position, acceleration, velocity, rotation, angle, center;

	Bobber()
	{
		center = Vector(0, 0, 0);
		position = Vector(3, 2, 1);
		acceleration = Vector(0.05f, 0.05f, 0.05f);
		rotation = Vector((float)(1 * 2 * M_PI / 360), (float)(2 * 2 * M_PI / 360), 0);
		velocity = Vector(0, 0, 0);
		angle = Vector(0, 0, 0);
	}

	void bob()
	{
		for (int i = 0; i < 3; i++)
		{
			angle[i] += rotation[i];
			if (angle[i] >= (float)(2 * M_PI))
				angle[i] -= (float)(2 * M_PI);

			if (position[i] < 0)
				velocity[i] += acceleration[i];
			else if (position[i] > 0)
				velocity[i] -= acceleration[i];
			position[i] += velocity[i];
		}
	}

	bool goAway(Vector away)
	{
		bool ret = false;
		for (int i = 0; i < 3; i++)
			if (fabs(away[i] - center[i]) > 1)
			{
				center[i] += 0.01f * (away[i] - center[i]);
				ret = true;
			}
		return ret;
	}

	void comeBack()
	{
		Vector back = Vector(0, 0, 0);

		for (int i = 0; i < 3; i++)
			if (fabs(back[i] - center[i]) > 1)
				center[i] += 0.01f * (back[i] - center[i]);
	}
};

class Morph : public Bobber
{
public:
	static const int MORPHTARGETS = 4;
	class MorphTarget
	{
	public:
		Vector point[40];
		bool seenIt;
	} morphTarget[MORPHTARGETS];

	Vector point[40];

	int m0, m1;
	float d, v, p;

	float scale;

	Morph()
	{
		position = Vector(8.0f, 4.0f, 3.0f);
		acceleration = Vector(0.005f, 0.005f, 0.005f);
		rotation = Vector((float)(1 * 2 * M_PI / 360), (float)(2 * 2 * M_PI / 360), 0);
		center = Vector(SCREENWIDTH / 3.0f, SCREENHEIGHT / 2.0f, 40); // away

		d = 0;
		v = 0.005f;
		p = 0;
		m0 = 0;
		m1 = 0;
		scale = 0.75f;

		initMorphTargets();
	}

	void tick()
	{
		bob();

		if (p < 400)
		{
			p += 1;
		}
		else
		{
			d += v;
			if (v > 0 && d >= 1)
			{
				v = -v;
				d = 1;
				p = 0;

				bool seenItAll = true;
				for (int i = 0; i < MORPHTARGETS; i++)
					seenItAll = seenItAll && morphTarget[i].seenIt;
				if (seenItAll)
					for (int i = 0; i < MORPHTARGETS; i++)
						morphTarget[i].seenIt = false;
				do
				{
					m0 = rand() % MORPHTARGETS;
				}
				while (morphTarget[m0].seenIt);
				morphTarget[m0].seenIt = true;
			}
			else if (v < 0 && d <= 0)
			{
				v = -v;
				d = 0;
				p = 0;

				bool seenItAll = true;
				for (int i = 0; i < MORPHTARGETS; i++)
					seenItAll = seenItAll && morphTarget[i].seenIt;
				if (seenItAll)
					for (int i = 0; i < MORPHTARGETS; i++)
						morphTarget[i].seenIt = false;
				do
				{
					m1 = rand() % MORPHTARGETS;
				}
				while (morphTarget[m1].seenIt);
				morphTarget[m1].seenIt = true;
			}
		}

		if (scale > 0.75f)
			scale -= 0.05f;

		for (int i = 0; i < 40; i++)
		{
			Vector pt = morphTarget[m0].point[i] * (1 - d) + morphTarget[m1].point[i] * d;
			point[i] = rotate(pt, angle[0], angle[1], angle[2]);
			point[i] = translate(point[i], position.x, position.y, position.z + 25 + center.z);
		}

		for (int i = 0; i < 40; i++)
			for (int j = 0; j < 40 - 1; j++)
				if (point[j].z < point[j + 1].z)
					swap(point[j], point[j + 1]);
	}

	void draw(unsigned short * pBuffer, long pitch)
	{
		for (int i = 0; i < 40; i++)
		{
			Position pos2d = project(&point[i]);
			int s = (int)(scale * (48 - point[i].z));
			pos2d.y -= SCREENHEIGHT / 8;
			pos2d.y += (int)center.y;
			pos2d.x += (int)center.x;
			graphics.stretchBltFast(pBuffer, pitch, pos2d.x, pos2d.y, s, 1, 48);
		}
	}

	void initMorphTargets()
	{
		for (int i = 0; i < 20; i++)
		{
			morphTarget[0].point[i] = Vector(5 * cosf(0.16f + (float)M_PI * i / 10.0f), 5 * sinf(0.16f + (float)M_PI * i / 10.0f), 0);
			morphTarget[0].point[i + 20] = Vector(5 * cosf(0.16f + (float)M_PI * i / 10.0f), 0, 5 * sinf(0.16f + (float)M_PI * i / 10.0f));
		}

		for (int i = 0; i < 5; i++)
		{
			morphTarget[1].point[i     ] = Vector(2.0f * (i - 2), -2.0f, -4.0f);
			morphTarget[1].point[i +  5] = Vector(2.0f * (i - 2), -2.0f, 4.0f);
			morphTarget[1].point[i + 10] = Vector(-4.0f, -2.0f, 2.0f * (i - 2));
			morphTarget[1].point[i + 15] = Vector(4.0f, -2.0f, 2.0f * (i - 2));

			morphTarget[1].point[i + 20] = Vector((float)i, 2.0f * (4 - i) - 2, (float)i);
			morphTarget[1].point[i + 25] = Vector((float)i, 2.0f * (4 - i) - 2, (float)-i);
			morphTarget[1].point[i + 30] = Vector((float)-i, 2.0f * (4 - i) - 2, (float)-i);
			morphTarget[1].point[i + 35] = Vector((float)-i, 2.0f * (4 - i) - 2, (float)i);
		}

		for (int i = 0; i < 10; i++)
		{
			morphTarget[2].point[i     ] = Vector(1.4f * (i - 4.5f), 0.0f, 0.0f);
			morphTarget[2].point[i + 10] = Vector(0.0f, 1.4f * (i - 4.5f), 0.0f);
			morphTarget[2].point[i + 20] = Vector(1.0f * (i - 4.5f), 1.0f * (i - 4.5f), 0.0f);
			morphTarget[2].point[i + 30] = Vector(1.0f * (i - 4.5f), -1.0f * (i - 4.5f), 0.0f);
		}

		for (int i = 0; i < 10; i++)
		{
			morphTarget[3].point[i     ] = Vector(1.0f * (i - 4.5f), 0.0f, 0.0f);
			morphTarget[3].point[i + 10] = Vector(0.0f, 1.0f * (i - 4.5f), 0.0f);
			morphTarget[3].point[i + 20] = Vector(0.0f, 0.0f, 1.0f * (i - 4.5f));
			morphTarget[3].point[i + 30] = Vector(4.5f * cosf(0.314f + (float)M_PI * i / 5.0f), 0, 4.5f * sinf(0.314f + (float)M_PI * i / 5.0f));
		}

		morphTarget[0].seenIt = true;
		morphTarget[1].seenIt = false;
		morphTarget[2].seenIt = false;
		morphTarget[3].seenIt = false;
	}
} morph;

class SanDee : public Bobber
{
public:
	Vector * point;
	struct Face
	{
		int a, b, c;
		Vector n;
	} * face;
	int nPoints;
	int nFaces;

	Vector * pos3d;
	Position * pos2d;
	Vector * norm;

	SanDee()
	{
		position = Vector(60.0f, 30.0f, 15.0f);
		acceleration = Vector(0.05f, 0.05f, 0.05f);
		rotation = Vector((float)(1 * 2 * M_PI / 360), (float)(2.5f * 2 * M_PI / 360), 0);
		center = Vector(-SCREENWIDTH / 3.0f, SCREENHEIGHT / 2, 600); // away

		nFaces = nPoints = 0;
		point = NULL;
		face = NULL;
		pos3d = NULL;
		pos2d = NULL;
		norm = NULL;
	}

	~SanDee()
	{
		if (face)
			delete [] face;
		if (pos3d)
			delete [] pos3d;
		if (pos2d)
			delete [] pos2d;
		if (norm)
			delete [] norm;
	}

	bool load()
	{
		nPoints = NVTORUS;
		nFaces = NFTORUS;

		point = (Vector *)vtorus;
		face = new Face[nFaces];

		norm = new Vector[nFaces];
		pos3d = new Vector[nPoints];
		pos2d = new Position[nPoints];

		for (int i = 0; i < nFaces; i++)
		{
			face[i].a = ftorus[i][0];
			face[i].b = ftorus[i][1];
			face[i].c = ftorus[i][2];

			Vector u(point[face[i].b] - point[face[i].a]);
			Vector v(point[face[i].c] - point[face[i].a]);
			face[i].n = u.cross(v);
			face[i].n.normalize();
		}

		return true;
	}

	void tick()
	{
		bob();

		for (int i = 0; i < nPoints; i++)
		{
			pos3d[i] = rotate(point[i], angle[0], angle[1], angle[2]);
			pos3d[i] = translate(pos3d[i], position.x, position.y, position.z + 150 + center.z);
		}

		for (int i = 0; i < nFaces; i++)
			for (int j = 0; j < nFaces - 1; j++)
			{
				float z1 = pos3d[face[j].a].z + pos3d[face[j].b].z + pos3d[face[j].c].z;
				float z2 = pos3d[face[j + 1].a].z + pos3d[face[j + 1].b].z + pos3d[face[j + 1].c].z;
				if (z1 < z2)
					swap(face[j], face[j + 1]);
			}

		for (int i = 0; i < nFaces; i++)
		{
			norm[i] = rotate(face[i].n, angle[0], angle[1], angle[2]);
		}

		for (int i = 0; i < nPoints; i++)
		{
			pos2d[i] = project(&pos3d[i]);
			pos2d[i].y -= SCREENHEIGHT / 8;
			pos2d[i].y += (int)center.y;
			pos2d[i].x += (int)center.x;
		}
	}

	void draw(unsigned short * pBuffer, long pitch, float pow[3])
	{
		Vector light = Vector(-1, -1, -1);
		light.normalize();

		for (int i = 0; i < nFaces; i++)
		{
			Position a = pos2d[face[i].a];
			Position b = pos2d[face[i].b];
			Position c = pos2d[face[i].c];

			if (((a.x - b.x) * (c.y - b.y)) - ((a.y - b.y) * (c.x - b.x)) > 0)
			{
				float bright = (norm[i].dot(light) + 1) / 2;
				float backlit = 0.6f * (1 - (bright)) * (1 - (bright));
				BYTE cr = (BYTE)(min((backlit * pow[0] + bright), 1) * 255);
				BYTE cg = (BYTE)(min((backlit * pow[1] + bright), 1) * 255);
				BYTE cb = (BYTE)(min((backlit * pow[2] + bright), 1) * 255);
				WORD col = RGB16(cr, cg, cb);
				triangle(pBuffer, pitch, a, b, c, col);
			}
		}
	}

	void alphatriangle(unsigned short * pBuffer, long pitch, Position a, Position b, Position c, WORD color)
	{
		if (a.y > b.y)
			swap(a, b);
		if (b.y > c.y)
			swap(b, c);
		if (a.y > b.y)
			swap(a, b);

		if (c.y - a.y == 0)
			return;

		int colR = 3 * R16(color);
		int colG = 3 * G16(color);
		int colB = 3 * B16(color);

		if (b.y - a.y > 0)
		{
			int k1 = ((b.x - a.x) << 8) / (b.y - a.y);
			int k2 = ((c.x - a.x) << 8) / (c.y - a.y);
			int x1 = a.x << 8;
			int x2 = a.x << 8;

			if (k1 > k2)
			{
				swap(x1, x2);
				swap(k1, k2);
			}

			for (int y = a.y; y < b.y; y++)
			{
				int minx = (x1 >> 8);
				int maxx = (x2 >> 8);

				for (int x = minx; x < maxx; x++)
				{
					WORD C = pBuffer[x + y * pitch];
					unsigned char R = (1 * R16(C) + colR) >> 2;
					unsigned char G = (1 * G16(C) + colG) >> 2;
					unsigned char B = (1 * B16(C) + colB) >> 2;
					pBuffer[x + y * pitch] = RGB16(R, G, B);
				}

				x1 += k1;
				x2 += k2;
			}
		}

		if (c.y - b.y > 0)
		{
			int k1 = ((c.x - b.x) << 8) / (c.y - b.y);
			int k2 = ((c.x - a.x) << 8) / (c.y - a.y);
			int x1 = b.x << 8;
			int x2 = (a.x << 8) + k2 * (b.y - a.y);

			if (k2 > k1)
			{
				swap(x1, x2);
				swap(k1, k2);
			}

			for (int y = b.y; y < c.y; y++)
			{
				int minx = (x1 >> 8);
				int maxx = (x2 >> 8);

				for (int x = minx; x < maxx; x++)
				{
					WORD C = pBuffer[x + y * pitch];
					unsigned char R = (1 * R16(C) + colR) >> 2;
					unsigned char G = (1 * G16(C) + colG) >> 2;
					unsigned char B = (1 * B16(C) + colB) >> 2;
					pBuffer[x + y * pitch] = RGB16(R, G, B);
				}

				x1 += k1;
				x2 += k2;
			}
		}
	}

	void triangle(unsigned short * pBuffer, long pitch, Position a, Position b, Position c, WORD color)
	{
		if (a.y > b.y)
			swap(a, b);
		if (b.y > c.y)
			swap(b, c);
		if (a.y > b.y)
			swap(a, b);

		if (c.y - a.y == 0)
			return;

		if (b.y - a.y > 0)
		{
			int k1 = ((b.x - a.x) << 8) / (b.y - a.y);
			int k2 = ((c.x - a.x) << 8) / (c.y - a.y);
			int x1 = a.x << 8;
			int x2 = a.x << 8;

			if (k1 > k2)
			{
				swap(x1, x2);
				swap(k1, k2);
			}

			for (int y = a.y; y < b.y; y++)
			{
				int minx = (x1 >> 8);
				int maxx = (x2 >> 8);

				for (int x = minx; x < maxx; x++)
					pBuffer[x + y * pitch] = color;

				x1 += k1;
				x2 += k2;
			}
		}

		if (c.y - b.y > 0)
		{
			int k1 = ((c.x - b.x) << 8) / (c.y - b.y);
			int k2 = ((c.x - a.x) << 8) / (c.y - a.y);
			int x1 = b.x << 8;
			int x2 = (a.x << 8) + k2 * (b.y - a.y);

			if (k2 > k1)
			{
				swap(x1, x2);
				swap(k1, k2);
			}

			for (int y = b.y; y < c.y; y++)
			{
				int minx = (x1 >> 8);
				int maxx = (x2 >> 8);

				for (int x = minx; x < maxx; x++)
					pBuffer[x + y * pitch] = color;

				x1 += k1;
				x2 += k2;
			}
		}
	}
} sandee;

class Octopod : public Bobber
{
public:

	static const int OCTONUM = 100;

	Vector octo[8 * OCTONUM];
	float octopitch[OCTONUM + 1];
	float octoyaw[OCTONUM + 1];
	float octoroll[OCTONUM + 1];
	float octooffsx[OCTONUM + 1];
	float octooffsy[OCTONUM + 1];

	Octopod()
	{
		position = Vector(0.0f, 0.0f, 0.0f);
		acceleration = Vector(0.0f, 0.0f, 0.0f);
		rotation = Vector(0.0f, 0.0f, 0.0f);
		center = Vector(0, SCREENHEIGHT / 3, 500); // away

		for (int i = 0; i < OCTONUM; i++)
		{
			int s = i + 1;

			octo[0 * OCTONUM + i] = Vector(s * 0.25f, s * 0.25f, s * 0.25f);
			octo[1 * OCTONUM + i] = Vector(-(s * 0.25f), s * 0.25f, s * 0.25f);
			octo[2 * OCTONUM + i] = Vector(s * 0.25f, -(s * 0.25f), s * 0.25f);
			octo[3 * OCTONUM + i] = Vector(-(s * 0.25f), -(s * 0.25f), s * 0.25f);
			octo[4 * OCTONUM + i] = Vector(s * 0.25f, s * 0.25f, -(s * 0.25f));
			octo[5 * OCTONUM + i] = Vector(-(s * 0.25f), s * 0.25f, -(s * 0.25f));
			octo[6 * OCTONUM + i] = Vector(s * 0.25f, -(s * 0.25f), -(s * 0.25f));
			octo[7 * OCTONUM + i] = Vector(-(s * 0.25f), -(s * 0.25f), -(s * 0.25f));

			octopitch[i] = octoyaw[i] = octoroll[i] = 0;
			octooffsx[i] = octooffsy[i] = 0;
		}

		octopitch[OCTONUM] = octoyaw[OCTONUM] = octoroll[OCTONUM] = 0;
		octooffsx[OCTONUM] = octooffsy[OCTONUM] = 0;
		octooffsx[0] = 25;
		octooffsy[0] = 3;

		for (int i = 0; i < OCTONUM; i++)
			tick();
	}

	void tick()
	{
		if (rand() % 10 == 0)
			octopitch[OCTONUM] += (rand() % 3 - 1) * 0.25f * (float)M_PI / 180;
		if (rand() % 10 == 0)
			octoyaw[OCTONUM] += (rand() % 3 - 1) * 0.25f * (float)M_PI / 180;
		if (rand() % 10 == 0)
			octoroll[OCTONUM] += (rand() % 3 - 1) * 0.25f * (float)M_PI / 180;

		float vmax = 2 * (float)M_PI / 180;

		octopitch[OCTONUM] = (octopitch[OCTONUM] > vmax) ? vmax : (octopitch[OCTONUM] < -vmax) ? -vmax : octopitch[OCTONUM];
		octoyaw[OCTONUM] = (octoyaw[OCTONUM] > vmax) ? vmax : (octoyaw[OCTONUM] < -vmax) ? -vmax : octoyaw[OCTONUM];
		octoroll[OCTONUM] = (octoroll[OCTONUM] > vmax) ? vmax : (octoroll[OCTONUM] < -vmax) ? -vmax : octoroll[OCTONUM];

		octopitch[0] += octopitch[OCTONUM];
		octoyaw[0] += octoyaw[OCTONUM];
		octoroll[0] += octoroll[OCTONUM];

		if (octooffsx[0] > 0)
			octooffsx[OCTONUM] -= 0.0025f;
		else if (octooffsx[0] < 0)
			octooffsx[OCTONUM] += 0.0025f;
		if (octooffsy[0] > 0)
			octooffsy[OCTONUM] -= 0.0025f;
		else if (octooffsy[0] < 0)
			octooffsy[OCTONUM] += 0.0025f;

		octooffsx[0] += octooffsx[OCTONUM];
		octooffsy[0] += octooffsy[OCTONUM];

		for (int i = OCTONUM - 1; i > 0; i--)
		{
			octopitch[i] = octopitch[i - 1];
			octoyaw[i] = octoyaw[i - 1];
			octoroll[i] = octoroll[i - 1];

			octooffsx[i] = octooffsx[i - 1];
			octooffsy[i] = octooffsy[i - 1];
		}

	}

	void draw(unsigned short * pBuffer, long bufPitch)
	{
		Position octo2d[8 * OCTONUM];
		Vector octo3d[8 * OCTONUM];

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < OCTONUM; j++)
			{
				octo3d[i * OCTONUM + j] = rotate(octo[i * OCTONUM + j], octopitch[j], octoyaw[j], octoroll[j]);
				Vector tmp = translate(octo3d[i * OCTONUM + j], octooffsx[j], octooffsy[j] - 15, 100 + center.z);
				octo2d[i * OCTONUM + j] = project(&tmp);
			}

		for (int j = 0; j < 8; j++)
			for (int i = 0; i < OCTONUM; i++)
			{
				if (i % 5 == 0)
				{
					int x = octo2d[i + j * OCTONUM].x;
					int y = (int)(octo2d[i + j * OCTONUM].y + center.y);
					float pow = 1 - (float)i / OCTONUM;
					plotDiamond(pBuffer, bufPitch, x, y, pow, RGB16(128, 16, 8));
				}
			}
	}

	void plotDiamond(unsigned short * pBuffer, long bufPitch, int x, int y, float s, WORD col)
	{
		int r = R16(col);
		int g = G16(col);
		int b = B16(col);

		int s0 = (int)(-s * 3 + 2);
		int s1 = (int)(s0 + s * 7 + 3);

		for (int j = s0; j < s1; j++)
		{
			int snarf = abs(j - s1 / 2);
			for (int i = s0 + snarf; i < s1 - snarf; i++)
			{
				WORD c = pBuffer[(i + x) + (j + y) * bufPitch];
				int rc = r + R16(c);
				int gc = g + G16(c);
				int bc = b + B16(c);
				rc = (rc > 255) ? 255 : rc;
				gc = (gc > 255) ? 255 : gc;
				bc = (bc > 255) ? 255 : bc;
				pBuffer[(i + x) + (j + y) * bufPitch] = RGB16(rc, gc, bc);
			}
		}
	}
} octopod;


FMUSIC_MODULE * mod = 0;
float spectrum[8] = {0, 0, 0, 0, 0, 0, 0, 0};
bool goUFO = false;

bool tick()
{
	input.updateKeyboard();
	if (input.keyState[DIK_ESCAPE])
		return false;

	static int delay = 600;
	if (delay > 590)
	{
		delay--;
		graphics.clear();
		graphics.flip();
		return true;
	}
	else if (delay == 590)
	{
		delay--;
		morph.tick();
		sandee.tick();
		octopod.tick();
		FMUSIC_PlaySong(mod);
	}
	else if (delay > 0)
	{
		delay--;
	}
	else if (delay == 0)
	{
		delay--;
	}
	else
	{
		static int go = 0;
		static int cnt = 2700;
		if (--cnt <= 0)
		{
			cnt = 3000;
			if (++go > 2)
				go = 0;
		}
		if (go == 0)
		{
			morph.comeBack();
			morph.tick();

			if (sandee.goAway(Vector(-SCREENWIDTH / 3.0f, SCREENHEIGHT / 2.0f, 500)))
				sandee.tick();
			if (octopod.goAway(Vector(0.0f, SCREENHEIGHT / 3.0f, 500)))
				octopod.tick();
		}
		else if (go == 1)
		{
			sandee.comeBack();
			sandee.tick();

			if (morph.goAway(Vector(SCREENWIDTH / 3.0f, SCREENHEIGHT / 2.0f, 40)))
				morph.tick();
			if (octopod.goAway(Vector(0.0f, SCREENHEIGHT / 3.0f, 500)))
				octopod.tick();
		}
		else if (go == 2)
		{
			octopod.comeBack();
			octopod.tick();

			if (morph.goAway(Vector(SCREENWIDTH / 3.0f, SCREENHEIGHT / 2.0f, 40)))
				morph.tick();
			if (sandee.goAway(Vector(-SCREENWIDTH / 3.0f, SCREENHEIGHT / 2.0f, 600)))
				sandee.tick();
		}
	}

	static int waveangle = 0;
	waveangle += 3;
	if (waveangle > 360)
		waveangle -= 360;

	static float colPow[3] = {0, 0, 0};
	const float COLOR[][3] =
	{
		{0, 0, 0},
		{0, 0.75, 1},
		{1, 0, 0},
		{0, 1, 0},
		{1, 0, 1},
		{1, 0.75, 0},
		{1, 1, 1},
	};
	static int currCol = 0, nextCol = 1;
	static int colDist = 1000;
	static int colDelay = 950;
	if (colDelay <= 1000)
	{
		colDelay++;
	}
	else
	{
		if (colDist <= 0)
		{
			currCol = nextCol;
			nextCol++;
			if (nextCol > 6)
				nextCol = 1;
			colDist = 1000;
			colDelay = 0;
		}
		else
		{
			colDist -= 5;
			colPow[0] = 0.001f * (colDist * COLOR[currCol][0] + (1000 - colDist) * COLOR[nextCol][0]);
			colPow[1] = 0.001f * (colDist * COLOR[currCol][1] + (1000 - colDist) * COLOR[nextCol][1]);
			colPow[2] = 0.001f * (colDist * COLOR[currCol][2] + (1000 - colDist) * COLOR[nextCol][2]);
		}
	}

	struct Star
	{
		float x, y, v;
		Star()
		{
			x = (float)(rand() % SCREENWIDTH);
			y = (float)(rand() % (SCREENHEIGHT / 6));
			v = 0.25f * (1 + rand() % 3);
		}
	};
	static Star star[100];
	for (int i = 0; i < 100; i++)
	{
		star[i].x -= star[i].v;
		if (star[i].x < 0)
		{
			star[i].x += SCREENWIDTH;
			star[i].y = (float)(rand() % (SCREENHEIGHT / 6));
			star[i].v = 0.25f * (1 + rand() % 3);
		}
	}

	unsigned short * pBuffer = 0;
	long pitch = 0;
	if (graphics.lock(0, (void**)&pBuffer, &pitch))
	{
		pitch >>= 1; // ingen vet

		// background
		for (int y = 0; y < 3 * SCREENHEIGHT / 4; y++)
		{
			unsigned char t = (unsigned char)(y * 255.0 / (3 * SCREENHEIGHT / 4));
			unsigned char r = (unsigned char)(colPow[0] * t);
			unsigned char g = (unsigned char)(colPow[1] * t);
			unsigned char b = (unsigned char)(colPow[2] * t);
			WORD col = RGB16(r, g, b);
			int offs = y * pitch;

			for (int x = 0; x < SCREENWIDTH; x++)
				pBuffer[x + offs] = col;
		}

		// stars
		for (int i = 0; i < 100; i++)
		{
			unsigned char c = (unsigned char)(63 + star[i].v * 256);
			int x = (int)(star[i].x);
			int y = (int)(star[i].y);
			pBuffer[x + y * pitch] = RGB16(c, c, c);
		}

		// spectrum
		for (int x = 0; x < 8; x++)
		{
			int h = (int)(150 * spectrum[x]);
			spectrum[x] *= 0.9f;

			unsigned char r = (int)(196 * colPow[0]);
			unsigned char g = (int)(196 * colPow[1]);
			unsigned char b = (int)(196 * colPow[2]);
			WORD col = RGB16(r, g, b);

			for (int y = 3 * SCREENHEIGHT / 4; y > 3 * SCREENHEIGHT / 4 - h; y--)
			{
				if (y % 10 == 0)
				{
					y--;
					continue;
				}

				int x0 = x * 32 + 1;
				int x1 = (x + 1) * 32 - 1;

				if (y % 10 == 1 || y % 10 == 8)
				{
					x0++;
					x1--;
				}

				int offs = y * pitch + (SCREENWIDTH - 8 * (32 + 1)) / 2;

				for (int m = x0; m < x1; m++)
				{
					pBuffer[m + offs] = col;
				}
			}
		}

		graphics.unlock(0);
	}

	// ufo
	{
		static int framecounter = 0;
		framecounter++;
		if (framecounter > 22)
			framecounter = 0;
		int spriteframe = framecounter / 4;
		static float spritex = -64;

		if (goUFO)
		{
			spritex += 4.0f;
			if (spritex > SCREENWIDTH)
			{
				spritex = -64;
				goUFO = false;
			}

			bltTexture(graphics.gfx[3]->pDDS, makeRECT(spriteframe * 53, 0, (spriteframe + 1) * 53, 27), graphics.gfx[0]->pDDS, makePOINT((int)spritex, 5));
		}
	}

	// twister
	if (graphics.lock(6, (void**)&pBuffer, &pitch))
	{
		pitch >>= 1; // ingen vet

		for (int i = 0; i < 464 - 16; i += 16)
			for (int y = 0; y < 16; y++)
				for (int x = 0; x < SCREENWIDTH / 4; x++)
					pBuffer[x + (i + y) * pitch] = pBuffer[x + (i + y + 16) * pitch];

		for (int y = 0; y < 16; y++)
			for (int x = 0; x < SCREENWIDTH / 4; x++)
				pBuffer[x + (464 - (y + 1)) * pitch] = RGB16(0, 0, 0);

		static float speed = 0;
		static float angle = 90;

		for (int i = 0; i < 16; i++)
		{
			speed += (rand() % 7 - 3) * 0.004f;

			if (speed > 0.05f)
			{
				speed -= 0.0001f;
				speed *= 0.999f;
			}
			else if (speed < -0.05f)
			{
				speed += 0.0001f;
				speed *= 0.999f;
			}

			angle += speed;

			if (angle >= 180)
				angle -= 360;
			if (angle <= -180)
				angle += 360;

			float angle0 = angle - 90;
			float angle1 = angle;
			float angle2 = angle + 90;
			int rgb[6] = {255, 0, 255, 0, 255, 255};

			if (angle > 45)
			{
				angle0 = angle - 180;
				angle1 = angle - 90;
				angle2 = angle;
				rgb[0] = 0; rgb[1] = 0; rgb[2] = 255; rgb[3] = 255; rgb[4] = 0; rgb[5] = 255;
			}
			if (angle < -45)
			{
				angle0 = angle;
				angle1 = angle + 90;
				angle2 = angle + 180;
				rgb[0] = 0; rgb[1] = 255; rgb[2] = 255; rgb[3] = 255; rgb[4] = 128; rgb[5] = 0;
			}
			if (angle > (45 + 90))
			{
				angle0 = angle - 270;
				angle1 = angle - 180;
				angle2 = angle - 90;
				rgb[0] = 255; rgb[1] = 128; rgb[2] = 0; rgb[3] = 0; rgb[4] = 0; rgb[5] = 255;
			}
			if (angle < -(45 + 90))
			{
				angle0 = angle + 90;
				angle1 = angle + 180;
				angle2 = angle + 270;
				rgb[0] = 255; rgb[1] = 128; rgb[2] = 0; rgb[3] = 0; rgb[4] = 0; rgb[5] = 255;
			}

			int m0 = (int)(50 * sin(angle0 * M_PI / 180));
			int m1 = (int)(50 * sin(angle1 * M_PI / 180));
			int m2 = (int)(50 * sin(angle2 * M_PI / 180));

			float d = (m1 - m0) / 90.0f + 0.2f;
			WORD col0 = RGB16((int)(0.75f * d * rgb[0]), (int)(0.75f * d * rgb[1]), (int)(0.75f * d * rgb[2]));
			WORD col1 = RGB16((int)(d * d * rgb[0]), (int)(d * d * rgb[1]), (int)(d * d * rgb[2]));
			if (i > 7)
				swap(col0, col1);
			float R = (m1 - m0) / 4.0f;
			for (int x = m0; x < m1; x++)
			{
				float P = 0;
				if (R > 0)
				{
					float D = (x - m0) / R;
					P = (D - floor(D)) * R;
				}

				if (P < R / 2)
					pBuffer[(x + SCREENWIDTH / 8) + (464 - (16 - i + 1)) * pitch] = col0;
				else
					pBuffer[(x + SCREENWIDTH / 8) + (464 - (16 - i + 1)) * pitch] = col1;
			}

			d = (m2 - m1) / 90.0f + 0.2f;
			col0 = RGB16((int)(0.75f * d * rgb[3]), (int)(0.75f * d * rgb[4]), (int)(0.75f * d * rgb[5]));
			col1 = RGB16((int)(d * rgb[3]), (int)(d * rgb[4]), (int)(d * rgb[5]));
			if (i > 7)
				swap(col0, col1);
			R = (m2 - m1) / 4.0f;
			for (int x = m1; x < m2; x++)
			{
				float P = 0;
				if (R > 0)
				{
					float D = (x - m1) / R;
					P = (D - floor(D)) * R;
				}

				if (P < R / 2)
					pBuffer[(x + SCREENWIDTH / 8) + (464 - (16 - i + 1)) * pitch] = col0;
				else
					pBuffer[(x + SCREENWIDTH / 8) + (464 - (16 - i + 1)) * pitch] = col1;
			}
		}

		static bool firstTime = true;
		if (firstTime)
		{
			firstTime = false;
		
			for (int i = 0; i < 464 - 16; i += 16)
				for (int y = 0; y < 16; y++)
					for (int x = 0; x < SCREENWIDTH / 4; x++)
						pBuffer[x + (i + y) * pitch] = pBuffer[x + (464 - 17 + y) * pitch];
		}

		graphics.unlock(6);
	}

	graphics.draw(6, Position(0, 0), 0);

	// top scrolltext
	{
		char scrollTxt084[] = "WHAAAAZZZZZZAAAAAAAAAAAAAHHH!!! This is DragonL on the air, bringing on the vectors like you just don't care! Yeah, I should probably rephrase that, but it's 1:43AM and I'm too old for this shit. Bask in the warm glowing warming glow of your screen as you watch this tiny demo; it's my humble offering to the oldskool gods, hope y'all appreciate it. And thusly, the rant doth begin:    Oh heavenly Coder up above, why is it that everyone and their weird uncle's pet chameleon are now on the Internet polluting the airwaves, spouting inanities and affecting to be gamers? What did we do, that we deserve living in a world where the 4K demo is not praised as the greatest achievement of man? How did we arrive at this point in time where Flash and Ajax are touted as state of the art? Why, oh heavenly Coder, didst thou allow abominations such as XML and VB to emerge from the ancient hiding places of evil? We pray unto thee, bring back the bright young age of the C-64 and the Atari and the Amiga! Look favorably upon us, the coders who are your brave children, and return to us all that which is good and holy, so that we may once again separate ourselves from the commoners who are now known mainly as \"Youtubers\" and \"gankers\"... Amen.    Ok, rant over! Greetz go out to all my pals - you know perfectly well where my inbox lives, so drop me a @ some time, ok? Peace.";
		static int scrollx084 = 5 * SCREENWIDTH;
		if (scrollx084 > -scrollSize084(scrollTxt084))
		{
			scrollx084 -= 4;
			bltFont084(scrollTxt084, makePOINT(scrollx084, 64), graphics.gfx[0]->pDDS, graphics.gfx[2]->pDDS);
		}
	}

	// bottom scrolltext
	{
		char scrollTxt046[] = "The time has come for a second scroll, just to confuse you :) Anywho, here come the obligatory greetings! In no particular order, admiration and respect goes out to +Mahoney & Kaktus+ +The Carebears+ +The Spacepigs+ +Triton+ +Future Crew+ +Fairlight+ +rgba+ +tbc+ and +Farbrausch+ for inspiration and motivation. Also, greetz to my friends from +Atomic Elbow+ +Intellipool+ +Innovation & Media+ +Labhandleaderz+ +Powerhouse+ +Nordnet+ +Systemvaruhuset+ and of course +Frostforge+ while on a more personal level, kisses for my sweet +Bunny+ and manly man handshakes for +Iaoth+ +Mabuse+ +Zantac+ +Hippie+ and +Xela+ but seriously, why don't the rest of you people have proper handles? Or maybe you do? I haven't forgotten you, but how am I supposed to greet you guys properly if I don't know your handles? So just make sure I know for next time... :)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           That would be ESC to quit, just in case you were wondering.                                                                                                                                                                                                                         What are you waiting for? Secret messages? We'll have none of that, now run along and play outside.                                                                                                                                                                                                            Dude. That's all there is. It's over. Get a grip.                                                                                                                                                                                                                                                                                                                      EOF";
		static int scrollx046 = 18 * SCREENWIDTH;
		if (scrollx046 > -scrollSize046(scrollTxt046))
		{
			scrollx046 -= 3;
			bltFont046(scrollTxt046, makePOINT(scrollx046, 3 * SCREENHEIGHT / 4 - 16), graphics.gfx[0]->pDDS, graphics.gfx[5]->pDDS);
		}
	}

	if (graphics.lock(0, (void**)&pBuffer, &pitch))
	{
		pitch >>= 1; // ingen vet

		// sandee
		sandee.draw(pBuffer, pitch, colPow);

		// morph
		morph.draw(pBuffer, pitch);

		// octopod
		octopod.draw(pBuffer, pitch);

		// water
		for (int y = 0; y < SCREENHEIGHT / 4; y++)
		{
			int oy = (int)(9 * cos(0.15 * (y * y) * M_PI / (SCREENHEIGHT / 2) + 8 * waveangle * M_PI / 360));
			if (oy + y * 3 < 0)
				oy = 0;
			else if (oy + y * 3 > (3 * SCREENHEIGHT / 4) - 1)
				oy = 0;

			int soffs = (oy + y * 3) * pitch;
			int toffs = (SCREENHEIGHT - 1 - y) * pitch;

			for (int x = 0; x < SCREENWIDTH; x++)
			{
				WORD col = pBuffer[x + soffs];
				WORD r = 3 * (R16(col) >> 2);
				WORD g = 3 * (G16(col) >> 2);
				WORD b = 3 * (B16(col) >> 2) + 64;
				pBuffer[x + toffs] = RGB16(r, g, b);
			}
		}

		// credits
		static bool credits = true;
		if (credits)
		{
			static double percentage = 0;
			static double percdiff = -1.15;
			static int ofx = 0;
			static int ofy = 0;
			static int cred = 0;
			char * szcredit[4] = 
			{
				"Design: DragonL",
				"Code: DragonL",
				"Gfx: DragonL",
				"Music: DragonL"
			};
			if (percentage >= 100)
			{
				percentage = 100;
				percdiff = -percdiff;
			}
			else if (percentage <= 0)
			{
				if (cred > 3)
				{
					cred = 0;
					credits = false;
				}
				else
				{
					graphics.clear(4, RGB(0, 0, 0));
					bltFont046(szcredit[cred], makePOINT(0, 0), graphics.gfx[4]->pDDS, graphics.gfx[5]->pDDS);

					percentage = 0;
					percdiff = -percdiff;
					ofx += rand() % 20 + 100;
					ofy = rand() % 20 + (cred * 20) + 5 * SCREENHEIGHT / 8;

					cred++;
				}
			}
			if (credits)
			{
				percentage += percdiff;
				ofy--;
				graphics.blendBlt(pBuffer, pitch, ofx, ofy, (int)percentage, 4);
			}
		}

		graphics.unlock(0);
	}

	// blt to backbuffer
	graphics.draw(0, Position(0, 0));

	//// fps
	//{
	//	char szfps[20];
	//	static int numframes = 0;
	//	numframes++;
	//	static int tstart = timeGetTime();
	//	int tnow = timeGetTime();
	//	int fps = 0;
	//	if (tnow - tstart > 0)
	//		fps = 1000 * numframes / (tnow - tstart);
	//	sprintf_s(szfps, 20, "%i", fps);
	//	graphics.write(szfps, Position(0, 0), RGB(255, 255, 255));
	//	if (tnow - tstart > 5000)
	//	{
	//		numframes = 1;
	//		tstart = timeGetTime();
	//	}
	//}

	graphics.flip();
	return true;
}

queue< pair<__int64, unsigned char> > noteq;

void songcallback(FMUSIC_NOTE * note)
{
	if (note->note < 132 && note->number != 0)
	{
		__int64 time = timer->passed();
		noteq.push(pair<__int64, unsigned char>(time + 1000 + 800, note->number));

		while (noteq.size() > 0 && noteq.front().first < time)
		{
			unsigned char sample = noteq.front().second;
			noteq.pop();

			//float p = 1.0f * (float)(HIWORD(data)) / 64.0f;
			//if (spectrum[user - 1] < p)
			//	spectrum[user - 1] = p;
			spectrum[sample - 1] = 1.0f;

			switch (sample)
			{
				case 0x01: // bassdrum
					morph.scale = 1.5f;
					break;
				case 0x04: // sputnik
					goUFO = true;
					break;
				case 0x07: // snare
					morph.scale = 1.25f;
					break;
			}
		}
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			if (!graphics.init(hwnd))
				return -1;
			if (graphics.create("scratch0", SCREENWIDTH, SCREENHEIGHT) < 0)
				return -1;
			graphics.load(MAKEINTRESOURCE(IDB_BITMAP3), RGB16(0, 0, 0));
			graphics.load(MAKEINTRESOURCE(IDB_BITMAP2), RGB16(0, 0, 0));
			graphics.load(MAKEINTRESOURCE(IDB_BITMAP4), RGB16(255, 0, 0));
			if (graphics.create("scratch1", 320, 16, RGB(0, 0, 0)) < 0)
				return -1;
			graphics.load(MAKEINTRESOURCE(IDB_BITMAP1), RGB16(0, 0, 0));
			if (graphics.create("scratch2", 200, 464, RGB(0, 0, 0)) < 0)
				return -1;
			if (!sandee.load())
				return false;
			FSOUND_File_SetCallbacks(memopen, memclose, memread, memseek, memtell);
			mod = FMUSIC_LoadSong(MAKEINTRESOURCE(IDR_DATA1), NULL, songcallback);
			if (!mod)
				return -1;
			input.init(hwnd);
			input.initKeyboard();
			break;
		}
		case WM_ERASEBKGND:
			return 1;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char szWinName[] = "bfeVector";

	WNDCLASSEX wcl;
	wcl.cbSize = sizeof WNDCLASSEX;
	wcl.style = 0;
	wcl.lpfnWndProc = WindowProc;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hInstance = hInstance;
	wcl.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcl.hCursor = 0;
	wcl.hbrBackground = HBRUSH(COLOR_APPWORKSPACE + 1);
	wcl.lpszMenuName = 0;
	wcl.lpszClassName = szWinName;
	wcl.hIconSm = LoadIcon(0, IDI_WINLOGO);

	if (!RegisterClassEx(&wcl))
		return 0;

	HWND hwnd = CreateWindow(
		szWinName, szWinName, WS_POPUP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		HWND_DESKTOP, 0, hInstance, 0
	);

	if (!hwnd)
		return 0;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	SetFocus(hwnd);

	ShowCursor(FALSE);

	MSG msg;
	memset(&msg, 0, sizeof MSG);
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (!tick())
			PostQuitMessage(0);
	}

	FMUSIC_FreeSong(mod);
	delete timer;

	return (int)msg.wParam;
}
