#ifndef _RECT_H_
#define _RECT_H_

#include "Vec.h"
#include "Lazy.h"
#include "Camera.h"
#include "PiE.h"
#include "Optional.h"

struct AxisAlignedRect {

	struct Value {
		double v;
		enum {
			PX,
			VW,
			VH,
			NDC
		} unit;
		float operator()(PiE::EngineContext * ctx, AXIS axis) const {
			int &w = ctx->windowSize[0],
				&h = ctx->windowSize[1];
			switch (unit)
			{
			case AxisAlignedRect::Value::PX:
				switch (axis) 
				{
				case X:
					return (float)v / (float)w;
				case Y:
					return (float)v / (float)h;
				default:
					return 0.0f;
				}
			case AxisAlignedRect::Value::VW:
				switch (axis)
				{
				case X:
					return (float)v;
				case Y:
					return (float)v * (float)w / (float)h;
				default:
					return 0.0f;
				}
			case AxisAlignedRect::Value::VH:
				switch (axis)
				{
				case X:
					return (float)v * (float)h / (float)w;
				case Y:
					return (float)v;
				default:
					return 0.0f;
				}
			case AxisAlignedRect::Value::NDC:
				return (float)v;
			default:
				return 0.0f;
			}
		}
	};

	struct Vector : Vec<std::reference_wrapper<Optional<Value>>, 2> {
		Vector(std::array<std::reference_wrapper<Optional<Value>>, 2> list);
		Vector &operator=(const Vec<Value, 2> &other);
	};

	Optional<Value> xmin, xmid, xmax, ymin, ymid, ymax;
	Vector center, min, max, topLeft, bottomRight, &topRight = max, &bottomLeft = min;
	Optional<Value> width, height;

	PiE::EngineContext * ctx;

	AxisAlignedRect();

	Matrix4f createGUIMatrix();

	float getXmin();
	float getXmid();
	float getXmax();
	float getYmin();
	float getYmid();
	float getYmax();
	float getWidth();
	float getHeight();

	Vec2f getCenter();
	Vec2f getMin();
	Vec2f getMax();
	Vec2f getTopLeft();
	Vec2f getBottomRight();

	bool testPoint(Vec2d point);

	void reset(Optional<Value>& a, Optional<Value>& b, Optional<Value>& c, Optional<Value>& d);

	void reset(Vector a, Optional<Value>& b, Optional<Value>& c);

	void reset(Vector a, Vector b);
};

AxisAlignedRect::Value operator "" _px(long double d);
AxisAlignedRect::Value operator "" _vw(long double d);
AxisAlignedRect::Value operator "" _vh(long double d);
AxisAlignedRect::Value operator "" _ndc(long double d);
//
//AxisAlignedRect::Value operator+ (const AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right);
//AxisAlignedRect::Value operator+ (const double & left, const AxisAlignedRect::Value & right);
//AxisAlignedRect::Value operator+ (const AxisAlignedRect::Value & left, const double & right);
//AxisAlignedRect::Value operator- (const AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right);
//AxisAlignedRect::Value operator- (const double & left, const AxisAlignedRect::Value & right);
//AxisAlignedRect::Value operator- (const AxisAlignedRect::Value & left, const double & right);
//AxisAlignedRect::Value operator* (const AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right);
//AxisAlignedRect::Value operator* (const double & left, const AxisAlignedRect::Value & right);
//AxisAlignedRect::Value operator* (const AxisAlignedRect::Value & left, const double & right);
//AxisAlignedRect::Value operator/ (const AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right);
//AxisAlignedRect::Value operator/ (const double & left, const AxisAlignedRect::Value & right);
//AxisAlignedRect::Value operator/ (const AxisAlignedRect::Value & left, const double & right);
//
//AxisAlignedRect::Value operator+= (const AxisAlignedRect::Value & left, const double & right);
//AxisAlignedRect::Value operator+= (const AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right);
//AxisAlignedRect::Value operator-= (const AxisAlignedRect::Value & left, const double & right);
//AxisAlignedRect::Value operator-= (const AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right);
//AxisAlignedRect::Value operator*= (const AxisAlignedRect::Value & left, const double & right);
//AxisAlignedRect::Value operator*= (const AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right);
//AxisAlignedRect::Value operator/= (const AxisAlignedRect::Value & left, const double & right);
//AxisAlignedRect::Value operator/= (const AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right);
//
//AxisAlignedRect::Value operator- (const AxisAlignedRect::Value & value);
#endif