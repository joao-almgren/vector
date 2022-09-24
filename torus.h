
struct Vector
{
	float x, y, z;

	Vector() { x = y = z = 0; }
	Vector(const float a, const float b, const float c) { x = a; y = b; z = c; }

	Vector operator*(const float f) const { return Vector(f * x, f * y, f * z); }
	float dot(const Vector & v) const { return (x * v.x + y * v.y + z * v.z); }
	Vector cross(const Vector & v) const { return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

	Vector operator+(const Vector & v) const { return Vector(x + v.x, y + v.y, z + v.z); }
	Vector operator-() const { return Vector(-x, -y, -z); }
	Vector operator-(const Vector & v) const { return *this + (-v); }

	float & operator[](const int i) { if (i == 2) return z; else if (i == 1) return y; else return x; }

	float length() const { return sqrtf(x * x + y * y + z * z); }
	void normalize() { float l = length(); x /= l; y /= l; z /= l; }
};

Vector rotate(const Vector & v, const float pitch, const float yaw, const float roll)
{
	Vector result;
	const float x1 = v.x * cosf(roll) - v.y * sinf(roll);
	const float y1 = v.x * sinf(roll) + v.y * cosf(roll);
	const float z1 = x1 * sinf(yaw) + v.z * cosf(yaw);
	result.x = x1 * cosf(yaw) - v.z * sinf(yaw);
	result.z = z1 * cosf(pitch) - y1 * sinf(pitch);
	result.y = z1 * sinf(pitch) + y1 * cosf(pitch);
	return result;
}

Vector translate(const Vector & v, const float tx, const float ty, const float tz)
{
	Vector result;
	result.x = v.x + tx;
	result.y = v.y + ty;
	result.z = v.z + tz;
	return result;
}

//Vector project(const Vector & v)
//{
//	Vector result;
//	result.z = 1 / v.z;
// 	result.x = (SCREENWIDTH >> 1) + v.x * (SCREENWIDTH << 1) * result.z;
//	result.y = (SCREENHEIGHT >> 1) + v.y * (SCREENWIDTH << 1) * result.z;
//	return result;
//}

#define NVTORUS 64
Vector vtorus[NVTORUS] =
{
	Vector(30.0f, 0.0f, 0.0f),
	Vector(27.071068f, 0.000001f, 7.071067f),
	Vector(20.0f, 0.000001f, 9.999999f),
	Vector(12.928932f, 0.000001f, 7.071067f),
	Vector(10.0f, -0.0f, -0.000001f),
	Vector(12.928933f, -0.000001f, -7.071069f),
	Vector(20.0f, -0.000001f, -9.999999f),
	Vector(27.071066f, -0.000001f, -7.071068f),
	Vector(21.213203f, 21.213202f, -0.000003f),
	Vector(19.142136f, 19.142136f, 7.071064f),
	Vector(14.142135f, 14.142136f, 9.999997f),
	Vector(9.142136f, 9.142136f, 7.071066f),
	Vector(7.071068f, 7.071067f, -0.000002f),
	Vector(9.142137f, 9.142135f, -7.07107f),
	Vector(14.142136f, 14.142134f, -10.000001f),
	Vector(19.142134f, 19.142132f, -7.071072f),
	Vector(-0.000001f, 29.999998f, -0.000004f),
	Vector(-0.000001f, 27.071068f, 7.071063f),
	Vector(-0.000001f, 20.0f, 9.999997f),
	Vector(-0.000001f, 12.928932f, 7.071065f),
	Vector(-0.0f, 9.999999f, -0.000002f),
	Vector(-0.000001f, 12.928931f, -7.071071f),
	Vector(-0.000001f, 19.999998f, -10.000002f),
	Vector(-0.000001f, 27.071064f, -7.071073f),
	Vector(-21.213203f, 21.213202f, -0.000003f),
	Vector(-19.142136f, 19.142136f, 7.071064f),
	Vector(-14.142135f, 14.142136f, 9.999997f),
	Vector(-9.142136f, 9.142136f, 7.071066f),
	Vector(-7.071068f, 7.071067f, -0.000002f),
	Vector(-9.142137f, 9.142135f, -7.07107f),
	Vector(-14.142136f, 14.142134f, -10.000001f),
	Vector(-19.142134f, 19.142132f, -7.071072f),
	Vector(-30.0f, -0.000003f, 0.0f),
	Vector(-27.071068f, -0.000001f, 7.071067f),
	Vector(-20.0f, -0.0f, 9.999999f),
	Vector(-12.928932f, -0.0f, 7.071067f),
	Vector(-10.0f, -0.000001f, -0.000001f),
	Vector(-12.928933f, -0.000002f, -7.071069f),
	Vector(-20.0f, -0.000003f, -9.999999f),
	Vector(-27.071066f, -0.000003f, -7.071068f),
	Vector(-21.2132f, -21.213205f, 0.000003f),
	Vector(-19.142132f, -19.142136f, 7.071071f),
	Vector(-14.142133f, -14.142136f, 10.000001f),
	Vector(-9.142134f, -9.142135f, 7.071069f),
	Vector(-7.071066f, -7.071068f, 0.0f),
	Vector(-9.142135f, -9.142138f, -7.071068f),
	Vector(-14.142133f, -14.142138f, -9.999997f),
	Vector(-19.142132f, -19.142138f, -7.071065f),
	Vector(0.0f, -29.999998f, 0.000004f),
	Vector(0.0f, -27.071066f, 7.071072f),
	Vector(0.0f, -19.999998f, 10.000002f),
	Vector(0.0f, -12.92893f, 7.071069f),
	Vector(0.0f, -9.999999f, 0.000001f),
	Vector(0.0f, -12.928933f, -7.071067f),
	Vector(0.0f, -20.0f, -9.999997f),
	Vector(0.0f, -27.071066f, -7.071064f),
	Vector(21.2132f, -21.213205f, 0.000003f),
	Vector(19.142132f, -19.142136f, 7.071071f),
	Vector(14.142133f, -14.142136f, 10.000001f),
	Vector(9.142134f, -9.142135f, 7.071069f),
	Vector(7.071066f, -7.071068f, 0.0f),
	Vector(9.142135f, -9.142138f, -7.071068f),
	Vector(14.142133f, -14.142138f, -9.999997f),
	Vector(19.142132f, -19.142138f, -7.071065f)
};

#define NFTORUS 128
int ftorus[NFTORUS][3] =
{
	{0, 9, 1},
	{0, 8, 9},
	{1, 10, 2},
	{1, 9, 10},
	{2, 11, 3},
	{2, 10, 11},
	{3, 12, 4},
	{3, 11, 12},
	{4, 13, 5},
	{4, 12, 13},
	{5, 14, 6},
	{5, 13, 14},
	{6, 15, 7},
	{6, 14, 15},
	{7, 8, 0},
	{7, 15, 8},
	{8, 17, 9},
	{8, 16, 17},
	{9, 18, 10},
	{9, 17, 18},
	{10, 19, 11},
	{10, 18, 19},
	{11, 20, 12},
	{11, 19, 20},
	{12, 21, 13},
	{12, 20, 21},
	{13, 22, 14},
	{13, 21, 22},
	{14, 23, 15},
	{14, 22, 23},
	{15, 16, 8},
	{15, 23, 16},
	{16, 25, 17},
	{16, 24, 25},
	{17, 26, 18},
	{17, 25, 26},
	{18, 27, 19},
	{18, 26, 27},
	{19, 28, 20},
	{19, 27, 28},
	{20, 29, 21},
	{20, 28, 29},
	{21, 30, 22},
	{21, 29, 30},
	{22, 31, 23},
	{22, 30, 31},
	{23, 24, 16},
	{23, 31, 24},
	{24, 33, 25},
	{24, 32, 33},
	{25, 34, 26},
	{25, 33, 34},
	{26, 35, 27},
	{26, 34, 35},
	{27, 36, 28},
	{27, 35, 36},
	{28, 37, 29},
	{28, 36, 37},
	{29, 38, 30},
	{29, 37, 38},
	{30, 39, 31},
	{30, 38, 39},
	{31, 32, 24},
	{31, 39, 32},
	{32, 41, 33},
	{32, 40, 41},
	{33, 42, 34},
	{33, 41, 42},
	{34, 43, 35},
	{34, 42, 43},
	{35, 44, 36},
	{35, 43, 44},
	{36, 45, 37},
	{36, 44, 45},
	{37, 46, 38},
	{37, 45, 46},
	{38, 47, 39},
	{38, 46, 47},
	{39, 40, 32},
	{39, 47, 40},
	{40, 49, 41},
	{40, 48, 49},
	{41, 50, 42},
	{41, 49, 50},
	{42, 51, 43},
	{42, 50, 51},
	{43, 52, 44},
	{43, 51, 52},
	{44, 53, 45},
	{44, 52, 53},
	{45, 54, 46},
	{45, 53, 54},
	{46, 55, 47},
	{46, 54, 55},
	{47, 48, 40},
	{47, 55, 48},
	{48, 57, 49},
	{48, 56, 57},
	{49, 58, 50},
	{49, 57, 58},
	{50, 59, 51},
	{50, 58, 59},
	{51, 60, 52},
	{51, 59, 60},
	{52, 61, 53},
	{52, 60, 61},
	{53, 62, 54},
	{53, 61, 62},
	{54, 63, 55},
	{54, 62, 63},
	{55, 56, 48},
	{55, 63, 56},
	{56, 1, 57},
	{56, 0, 1},
	{57, 2, 58},
	{57, 1, 2},
	{58, 3, 59},
	{58, 2, 3},
	{59, 4, 60},
	{59, 3, 4},
	{60, 5, 61},
	{60, 4, 5},
	{61, 6, 62},
	{61, 5, 6},
	{62, 7, 63},
	{62, 6, 7},
	{63, 0, 56},
	{63, 7, 0}
};
