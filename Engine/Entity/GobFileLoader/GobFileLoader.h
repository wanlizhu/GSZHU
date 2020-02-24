
#pragma once

#include <fstream>
#include <vector>
#include <DirectXMath.h>

#include "Global.h"

using namespace Engine;

namespace GobFileLoader {

// 4 channel color value.
union ColorRGBA {
	uint32_t mColor;

	struct {
		uint8_t r, g, b, a;
	};

	explicit ColorRGBA(uint32_t aColor = 0) : mColor(aColor) {}

	ColorRGBA(uint8_t aRed, uint8_t aGreen, uint8_t aBlue, uint8_t aAlpha = 255U) :
		r(aRed), g(aGreen), b(aBlue), a(aAlpha) {}

	friend std::ostream& operator<<(std::ostream& aOutStream, const ColorRGBA& aColor) {
		aOutStream << "r = " << aColor.r << "g = " << aColor.g << "b" << aColor.b;

		return aOutStream;
	}
};


struct vec2 {
	float x, y;

	static float getRatio(vec2 min, vec2 max, float width, float height) {
		auto rangex = (max.x - min.x);
		auto rangey = (max.y - min.y);
		auto ratioX = width / rangex;
		auto ratioY = height / rangey;
		return ratioX < ratioY ? ratioX : ratioY;
	}

	vec2 operator*(float f) const {
		return vec2{ x * f, y * f };
	}

	vec2 normal(vec2 min, vec2 max, float width, float height) const {
		float t = getRatio(min, max, width, height);
		return {(x - min.x) * t, (y - min.y) * t};
	}
};



struct box2 {
	vec2 min, max;
};


// PointInstanceVertex contains per-instance point data.
struct PointInstanceVertex {
	vec2 mOrigin; // Origin (2 floats)
	ColorRGBA mColor; // Color (4 bytes)
	float mWidth; // Line width
	int mTexture; // Texture index (0 for solid)
	float mDepth; // Depth for draw order (0~1)

	friend std::ostream& operator<<(std::ostream& aOutStream, const PointInstanceVertex& aInfo) {
		aOutStream << "mOrigin = (" << aInfo.mOrigin.x << " " << aInfo.mOrigin.y << ")\n"
			<< "mColor = " << aInfo.mColor.mColor << std::endl
			//<< "mColor = " << "r = " << aInfo.mColor.r << "g = " << aInfo.mColor.g << "b" << aInfo.mColor.b << "a" << aInfo.mColor.a << std::endl
			<< "mWidth = " << aInfo.mWidth << std::endl
			<< "mTexture = " << aInfo.mTexture << std::endl
			<< "mDepth = " << aInfo.mDepth << std::endl;
		return aOutStream;
	}

	PointInstanceVertex normal(const vec2& min, const vec2& max, const float width, const float height) const {
		PointInstanceVertex res;
		res.mOrigin = mOrigin.normal(min, max, width, height);
		res.mColor = mColor;
		float t = vec2::getRatio(min, max, width, height);
		res.mWidth = mWidth * t;
		res.mTexture = mTexture;
		res.mDepth = mDepth;
		return res;
	}
};


// LineInstanceVertex contains per-instance line data.
struct LineInstanceVertex {
	vec2 mStartPoint; // Start point (2 floats)
	vec2 mEndPoint; // End point (2 floats)
	ColorRGBA mColor; // Color (4 bytes)
	float mWidth; // Line Width
	int mTexture; // Texture index (0 for solid)
	float mDepth; // Depth for draw order
	float mLength; // Segment length (cumulative up to segment)
	float mTotalLength; // Length of the full curve

	LineInstanceVertex normal(vec2 min, vec2 max, float width, float height) const {
		LineInstanceVertex res;
		res.mStartPoint = mStartPoint.normal(min, max, width, height);
		res.mEndPoint = mEndPoint.normal(min, max, width, height);
		res.mDepth = mDepth;
		res.mColor = mColor;
		return res;
	}

	friend std::ostream& operator<<(std::ostream& aOutStream, const LineInstanceVertex& aInfo) {
		aOutStream << "mStartPoint = (" << aInfo.mStartPoint.x << " " << aInfo.mStartPoint.y << ")\n"
			<< "mEndPoint = (" << aInfo.mEndPoint.x << " " << aInfo.mEndPoint.y << ")\n"
			<< "mColor = " << aInfo.mColor.mColor << std::endl
			//<< "mColor = " << "r = " << aInfo.mColor.r << "g = " << aInfo.mColor.g << "b" << aInfo.mColor.b << "a" << aInfo.mColor.a << std::endl
			<< "mWidth = " << aInfo.mWidth << std::endl
			<< "mTexture = " << aInfo.mTexture << std::endl
			<< "mDepth = " << aInfo.mDepth << std::endl
			<< "mLength = " << aInfo.mLength << std::endl
			<< "mTotalLength = " << aInfo.mTotalLength << std::endl;

		return aOutStream;

	}
};


// EllipseInstanceVertex contains per-instance ellipse data.
struct EllipseInstanceVertex {
	vec2 mCenter; // Center (2 floats)
	ColorRGBA mColor; // Color ( 4 bytes)
	vec2 mRadius; // x, y radius
	float mWidth; // Line width
	float mRotation; // Rotation
	int mTexture; // Texture index (0 for solid)
	float mDepth; // Depth for draw order (0~1)
	float mLength; // Segment length (cumulative up to segment)
	float mTotalLength; // Length of the full ellipse curve

	EllipseInstanceVertex normal(const vec2& min, const vec2& max, const float width, const float height) const {
		EllipseInstanceVertex res;
		res.mCenter = mCenter.normal(min, max, width, height);
		float t = vec2::getRatio(min, max, width, height);
		res.mRadius = mRadius * t;
		res.mDepth = mDepth;
		res.mColor = mColor;
		res.mWidth = mWidth;
		res.mRotation = mRotation;
		res.mLength = mLength * t;
		res.mTotalLength = mTotalLength * t;
		res.mTexture = 0;
		return res;
	}

	friend std::ostream& operator<<(std::ostream& aOutStream, const EllipseInstanceVertex& aInfo) {
		aOutStream << "mCenter = (" << aInfo.mCenter.x << " " << aInfo.mCenter.y << ")\n"
			<< "mColor = " << aInfo.mColor.mColor << std::endl
			//<< "mColor = " << "r = " << aInfo.mColor.r << "g = " << aInfo.mColor.g << "b" << aInfo.mColor.b << "a" << aInfo.mColor.a << std::endl
			<< "mRadius = (" << aInfo.mRadius.x << " " << aInfo.mRadius.y << ")\n"
			<< "mWidth = " << aInfo.mWidth << std::endl
			<< "mRotation = " << aInfo.mRotation << std::endl
			<< "mTexture = " << aInfo.mTexture << std::endl
			<< "mDepth = " << aInfo.mDepth << std::endl
			<< "mLength = " << aInfo.mLength << std::endl
			<< "mTotalLength = " << aInfo.mTotalLength << std::endl;

		return aOutStream;
	}
};


// EllipseArcInstanceVertex contains per-instance ellipse arc data.
struct EllipseArcInstanceVertex {
	vec2 mCenter; // Center (2 floats)
	ColorRGBA mColor; // Color ( 4 bytes)
	vec2 mRadius; // x, y radius
	float mWidth; // Line width
	float mRotation; // Rotation
	float mStartTheta; // Start angle
	float mEndTheta; // End angle
	int mTexture; // Texture index (0 for solid)
	float mDepth; // Depth for draw order (0~1)
	float mLength; // Segment length (cumulative up to segment)
	float mTotalLength; // Length of the full ellipse curve

	EllipseArcInstanceVertex normal(const vec2& min, const vec2& max, const float width, const float height) const {
		EllipseArcInstanceVertex res;
		res.mCenter = mCenter.normal(min, max, width, height);
		res.mColor = mColor;
		float t = vec2::getRatio(min, max, width, height);
		res.mRadius = mRadius * t;
		res.mWidth = mWidth;
		res.mRotation = mRotation;
		res.mStartTheta = mStartTheta;
		res.mEndTheta = mEndTheta;
		res.mTexture = 0;
		res.mDepth = mDepth;
		res.mLength = mLength * t;
		res.mTotalLength = mTotalLength * t;
		return res;
	}



	friend std::ostream& operator<<(std::ostream& aOutStream, const EllipseArcInstanceVertex& aInfo) {
		aOutStream << "mCenter = (" << aInfo.mCenter.x << " " << aInfo.mCenter.y << ")\n"
			<< "mColor = " << aInfo.mColor.mColor << std::endl
			//<< "mColor = " << "r = " << aInfo.mColor.r << "g = " << aInfo.mColor.g << "b" << aInfo.mColor.b << "a" << aInfo.mColor.a << std::endl
			<< "mRadius = (" << aInfo.mRadius.x << " " << aInfo.mRadius.y << ")\n"
			<< "mWidth = " << aInfo.mWidth << std::endl
			<< "mRotation = " << aInfo.mRotation << std::endl
			<< "mStartTheta = " << aInfo.mStartTheta << std::endl
			<< "mEndTheta = " << aInfo.mEndTheta << std::endl
			<< "mTexture = " << aInfo.mTexture << std::endl
			<< "mDepth = " << aInfo.mDepth << std::endl
			<< "mLength = " << aInfo.mLength << std::endl
			<< "mTotalLength = " << aInfo.mTotalLength << std::endl;

		return aOutStream;
	}
};


// CircleInstanceVertex contains per-instance circle data.
struct CircleArcInstanceVertex {
	vec2 mCenter; // The origin of the circle (2 floats)
	float mRadius; // Radius ( 1 floats)
	float mWidth; // Line width (1 floats)
	ColorRGBA mColor; // Color (4 bytes)
	float mStartTheta; // Start angle
	float mEndTheta; // End angle
	int mTexture; // The texture index (0 for solid)
	float mDepth; // Depth for draw order (0~1)
	float mLength; // Segment length (cumulative up to segment)
	float mTotalLength; // Length of the full circle curve

	CircleArcInstanceVertex normal(const vec2& min, const vec2& max, const float width, const float height) const {
		CircleArcInstanceVertex res;
		res.mCenter = mCenter.normal(min, max, width, height);
		float t = vec2::getRatio(min, max, width, height);
		res.mRadius = mRadius * t;
		res.mDepth = mDepth;
		res.mColor = mColor;
		res.mWidth = mWidth;
		res.mStartTheta = mStartTheta;
		res.mEndTheta = mEndTheta;
		return res;
	}

	friend std::ostream& operator<<(std::ostream& aOutStream, const CircleArcInstanceVertex& aInfo) {
		aOutStream << "mCenter = (" << aInfo.mCenter.x << " " << aInfo.mCenter.y << ")\n"
			<< "mColor = " << aInfo.mColor.mColor << std::endl
			//<< "mColor = " << "r = " << aInfo.mColor.r << "g = " << aInfo.mColor.g << "b" << aInfo.mColor.b << "a" << aInfo.mColor.a << std::endl
			<< "mRadius = (" << aInfo.mRadius << std::endl
			<< "mWidth = " << aInfo.mWidth << std::endl
			<< "mStartTheta = " << aInfo.mStartTheta << std::endl
			<< "mEndTheta = " << aInfo.mEndTheta << std::endl
			<< "mTexture = " << aInfo.mTexture << std::endl
			<< "mDepth = " << aInfo.mDepth << std::endl
			<< "mLength = " << aInfo.mLength << std::endl
			<< "mTotalLength = " << aInfo.mTotalLength << std::endl;

		return aOutStream;
	}

};


struct CircleInstanceVertex {
	vec2 mCenter; // The origin of the circle (2 floats)
	float mRadius; // Radius ( 1 floats)
	float mWidth; // Line width (1 floats)
	int mTexture; // The texture index (0 for solid)
	ColorRGBA mColor; // Color (4 bytes)
	float mDepth; // Depth for draw order (0~1)
	float mLength; // Segment length (cumulative up to segment)
	float mTotalLength; // Length of the full circle curve

	CircleInstanceVertex normal(vec2 min, vec2 max, float width, float height) const {
		CircleInstanceVertex res;
		res.mCenter = mCenter.normal(min, max, width, height);
		float t = vec2::getRatio(min, max, width, height);
		res.mRadius = mRadius * t;
		res.mDepth = mDepth;
		res.mColor = mColor;
		res.mWidth = mWidth;
		return res;
	}

	friend std::ostream& operator<<(std::ostream& aOutStream, const CircleInstanceVertex& aInfo) {
		aOutStream << "mCenter = (" << aInfo.mCenter.x << " " << aInfo.mCenter.y << ")\n"
			<< "mColor = " << aInfo.mColor.mColor << std::endl
			//<< "mColor = " << "r = " << aInfo.mColor.r << "g = " << aInfo.mColor.g << "b" << aInfo.mColor.b << "a" << aInfo.mColor.a << std::endl
			<< "mRadius = " << aInfo.mRadius << std::endl
			<< "mWidth = " << aInfo.mWidth << std::endl
			<< "mTexture = " << aInfo.mTexture << std::endl
			<< "mDepth = " << aInfo.mDepth << std::endl
			<< "mLength = " << aInfo.mLength << std::endl
			<< "mTotalLength = " << aInfo.mTotalLength << std::endl;

		return aOutStream;
	}
};


struct LineTypeInfo {
	enum {
		TEXTURE_ROW_SIZE = 1024,
		DASH = 1,
		DOT = 2,
		SPACE = 3,
		NAMEBUFFERSIZE = 32
	};

	uint8_t mName[NAMEBUFFERSIZE];
	int mId;
	uint8_t mOnOff[TEXTURE_ROW_SIZE];
	float mPhaseOffset;

	friend std::ostream& operator<<(std::ostream& aOutStream, const LineTypeInfo& aInfo) {
		aOutStream << "mName = (" << aInfo.mName << std::endl
			<< "mId = (" << aInfo.mId << std::endl
			//<< "mOnOff = (" << aInfo.mOnOff << std::endl
			<< "mPhaseOffset = (" << aInfo.mPhaseOffset << std::endl;

		return aOutStream;
	}
};

// some base class
struct BaseStruct { };
struct point : public BaseStruct {
	point() = default;

	point(float x, float y): x(x), y(y) {}

	point(const vec2& v): x(v.x), y(v.y) {}

	point(const std::initializer_list<float>& l) {
		auto iter = l.begin();
		x = *iter;
		++iter;
		y = *iter;
	}

	point& operator+=(const point&& p) {
		x += p.x;
		y += p.y;
		return *this;
	}

	void zoom(short direct, short x, short y) {
		float f;
		if (direct < 0) {
			f = 0.9f;
		} else {
			f = 1.1f;
		}
		this->x = x * f + (1 - f) * x;
		this->y = y * f + (1 - f) * y;
	}

	static point randomPoint() {
		return {float(rand() % gpGlobal->GetConfiguration<AppConfiguration>().GetWidth()), float(rand() % gpGlobal->GetConfiguration<AppConfiguration>().GetHeight())};
	};

	float length() const {
		return sqrt(x * x + y * y);
	}

	point operator+(const point p2) const {
		return point(x + p2.x, y + p2.y);
	}

	point operator-(const point p2) const {
		return point(x - p2.x, y - p2.y);
	}

	point operator/(float f) const {
		return point(x / f, y / f);
	}

	float x, y;
	unsigned int color = 0xff0000ff;
	float depth = 0.5f;
};

struct GlowPoint : public point {
	GlowPoint(float x, float y): point(x, y) {}

	GlowPoint(PointInstanceVertex p): point(p.mOrigin.x, p.mOrigin.y) {
		depth = p.mDepth;
		color = p.mColor.mColor;
	}

	unsigned int glow_color = 0xff00ff00;
};

struct line : public BaseStruct {
	point p1, p2;
	float depth = 0.5f;
	unsigned int color = 0xff0000ff;

	line(const point& p1, const point& p2, int color, float depth) : p1(p1),
																		p2(p2),
																		color(color),
																		depth(depth) {}

	line(const line& l) = default ;

	line(const LineInstanceVertex& l) {
		p1 = {l.mStartPoint};
		p2 = {l.mEndPoint};
		color = l.mColor.mColor;
		depth = l.mDepth;
	}

	line(const point& p1, const point& p2) : p1(p1),
												p2(p2) { }

	line(const std::initializer_list<point>& l) {
		auto iter = l.begin();
		p1 = *iter;
		++iter;
		p2 = *iter;
	}

	void toFloatArray(float* out) {
		out[0] = p1.x;
		out[1] = p1.y;
		out[2] = p2.x;

		out[3] = p2.y;
		out[4] = depth;
		out[5] = *reinterpret_cast<float*>(&color);
	}

	static line randomLine() {
		auto delta = point::randomPoint();
		auto start = point::randomPoint();
		return line{start, delta};
	}
};


struct triangle : public BaseStruct {
	triangle(point p1, point p2, point p3): p1(p1), p2(p2), p3(p3) {
		DirectX::XMVECTOR v1{p2.x - p1.x, p2.y - p1.y};
		DirectX::XMVECTOR v2{p3.x - p1.x, p3.y - p1.y};
		DirectX::XMFLOAT2 res;
		DirectX::XMStoreFloat2(&res, DirectX::XMVector2Cross(v1, v2));
		if (res.x > 0) {
			this->p1 = p2;
			this->p2 = p1;
		}
	}

	point p1, p2, p3;
	unsigned int color = 0xff00ff00;
	float depth = 0.5f;

};

#define PI 3.141592653589793f

struct ellipse : BaseStruct {
	//construction for ellipse
	ellipse(const point& p1, const point& p2, const point& p3, const point& p4, const point& p5) {
		center = p1;
		radius = point((p1 - p2).length(), (p1 - p3).length());

		point v1 = p4 - p1;
		float r1 = atan(v1.y / v1.x);
		if (v1.x < 0) {
			r1 += PI;
		}
		r1 += 2 * PI;
		r1 = r1 - int(r1 / (2 * PI)) * 2 * PI;

		point v2 = p5 - p1;
		float r2 = atan(v2.y / v2.x);
		if (v2.x < 0) {
			r2 += PI;
		}
		r2 += 2 * PI;
		r2 = r2 - int(r2 / (2 * PI)) * 2 * PI;
		if (r2 < r1) {
			r2 += 2 * PI;
		}
		range = point(r1, r2);

	}

	//construction for circle
	ellipse(const point& p1, const point& p2, const point& p3, const point& p4) {
		center = p1;
		float length = (p1 - p2).length();
		radius = point(length, length);
		point v1 = p3 - p1;
		float r1 = atan(v1.y / v1.x);
		if (v1.x < 0) {
			r1 += PI;
		}
		r1 += 2 * PI;
		r1 = r1 - int(r1 / (2 * PI)) * 2 * PI;

		point v2 = p4 - p1;
		float r2 = atan(v2.y / v2.x);
		if (v2.x < 0) {
			r2 += PI;
		}
		r2 += 2 * PI;
		r2 = r2 - int(r2 / (2 * PI)) * 2 * PI;
		if (r2 < r1) {
			r2 += 2 * PI;
		}
		range = point(r1, r2);

	}
	
	ellipse(const ellipse &e) = default;

	//construction for ellipse
	ellipse(const CircleInstanceVertex& c) {
		center = {c.mCenter};
		radius = {c.mRadius, c.mRadius};
		color = c.mColor.mColor;
		range = {0, 2 * PI};
		weight = unsigned int(c.mWidth);
		drawZ = c.mDepth;
	}

	//construction for ellipse
	ellipse(const CircleArcInstanceVertex& a) {
		center = {a.mCenter};
		radius = {a.mRadius, a.mRadius};
		color = a.mColor.mColor;
		range = {a.mStartTheta , a.mEndTheta};
		//		weight = unsigned int(a.mWidth);
		weight = 1;
		drawZ = a.mDepth;
	}

	ellipse(const EllipseInstanceVertex& e) {
		center = {e.mCenter};
		radius = e.mRadius;
		color = e.mColor.mColor;
		range = {0, 2 * PI};
		weight = unsigned int(e.mWidth);
		drawZ = e.mDepth;
	}

	ellipse(const EllipseArcInstanceVertex& e) {
		center = {e.mCenter};
		radius = e.mRadius;
		color = e.mColor.mColor;
		range = {e.mStartTheta, e.mEndTheta};
		weight = unsigned int(e.mWidth);
		drawZ = e.mDepth;
	}

	static ellipse randomCircle() {
		return ellipse(point::randomPoint(), point::randomPoint(), point::randomPoint(), point::randomPoint());
	}

	point center;
	point radius;
	point range;
	float rotate = 0;
	unsigned int color = 0xffff0000;
	unsigned int weight = 2;
	unsigned int captype = 0;
	unsigned int glowColor = 0xff0000ff;
	float drawZ = 0.5;
};

class CGobFileLoader {
private:
	const static auto GOB_FILE_VERSION_PREV = 0x3;
	const static auto GOB_FILE_VERSION = 0x4;
	constexpr static auto GOB_FILE_MAGIC = "GOB";
	const static auto GOB_FILE_MAGIC_LEN = 3;

	struct Header {
		uint8_t mMagic[GOB_FILE_MAGIC_LEN];
		int mVersion;
		int mPointCount;
		int mLineCount;
		int mEllipseCount;
		int mEllipseArcCount;
		int mCircleCount;
		int mCircleArcCount;
		int mLineTypeCount;
		box2 mBounds;
		box2 mDWGBounds;
	};

	box2 mBoundingBox;
	std::vector<PointInstanceVertex> mPoints;
	std::vector<LineInstanceVertex> mLines;
	std::vector<EllipseArcInstanceVertex> mEllipseArcs;
	std::vector<EllipseInstanceVertex> mEllipses;
	std::vector<CircleArcInstanceVertex> mCircleArcs;
	std::vector<CircleInstanceVertex> mCircles;
	std::vector<LineTypeInfo> mLineTypes;

public:
	std::vector<BaseStruct *> lines;
	std::vector<BaseStruct *> points;
	std::vector<BaseStruct *> ellipses;
	std::vector<BaseStruct *> circles;
	enum EntityType {
		POINT = 0,
		LINE,
		ELLIPSE_ARC,
		ELLIPSE,
		CIRCLE_ARC,
		CIRCLE,
		LINE_TYPE,

		TYPE_COUNT
	};

	explicit CGobFileLoader(const std::string& aFilename);

	const box2& BoundingBox(void) const {
		return mBoundingBox;
	}

	const std::vector<PointInstanceVertex>& Points(void) const {
		return mPoints;
	}

	const std::vector<LineInstanceVertex>& Lines(void) const {
		return mLines;
	}

	const std::vector<EllipseArcInstanceVertex>& EllipseArcs(void) const {
		return mEllipseArcs;
	}

	const std::vector<EllipseInstanceVertex>& Ellipses(void) const {
		return mEllipses;
	}

	const std::vector<CircleArcInstanceVertex>& CircleArcs(void) const {
		return mCircleArcs;
	}

	const std::vector<CircleInstanceVertex>& Circles(void) const {
		return mCircles;
	}

	const std::vector<LineTypeInfo>& LineTypes(void) const {
		return mLineTypes;
	}
};
}
