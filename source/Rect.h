#ifndef _RECT_H
#define _RECT_H

#include "Vec.h"
#include "Lazy.h"
#include "Camera.h"
#include "PiE.h"
#include "Optional.h"

namespace PiE {

	struct AxisAlignedRect {

		struct Value {
			double v;
			enum {
				PX,
				VW,
				VH,
				NDC
			} unit;
			double operator()(PiE::EngineContext * ctx, AXIS axis) const {
				int & w = ctx->windowSize[0],
					& h = ctx->windowSize[1];
				switch (unit) {
				case AxisAlignedRect::Value::PX:
					switch (axis) {
					case X:
						return (double)v / (double)w;
					case Y:
						return (double)v / (double)h;
					default:
						return 0.0f;
					}
				case AxisAlignedRect::Value::VW:
					switch (axis) {
					case X:
						return (double)v;
					case Y:
						return (double)v * (double)w / (double)h;
					default:
						return 0.0f;
					}
				case AxisAlignedRect::Value::VH:
					switch (axis) {
					case X:
						return (double)v * (double)h / (double)w;
					case Y:
						return (double)v;
					default:
						return 0.0f;
					}
				case AxisAlignedRect::Value::NDC:
					return (double)v;
				default:
					return 0.0;
				}
			}
		};

		struct Vector : Vec<std::reference_wrapper<Optional<Value>>, 2> {
			Vector(std::array<std::reference_wrapper<Optional<Value>>, 2> list);
			const Vector & operator=(const Vec<Value, 2> & other) const;
			const Vector & operator=(const Vec<Value, 2> && other) const;
		};

		Optional<Value> xmin, xmid, xmax, ymin, ymid, ymax;
		Optional<Value> width, height;
		const Vector center, min, max, topLeft, bottomRight, & topRight = max, & bottomLeft = min;

		PiE::EngineContext * ctx;

		AxisAlignedRect();

		Matrix4f createGUIMatrix();

		double getXmin();
		double getXmid();
		double getXmax();
		double getYmin();
		double getYmid();
		double getYmax();
		double getWidth();
		double getHeight();

		Vec2d getCenter();
		Vec2d getMin();
		Vec2d getMax();
		Vec2d getTopLeft();
		Vec2d getBottomRight();

		bool testPoint(Vec2d point);

		void reset(Optional<Value> & a, Optional<Value> & b, Optional<Value> & c, Optional<Value> & d);

		void reset(Vector a, Optional<Value> & b, Optional<Value> & c);

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

}

#endif