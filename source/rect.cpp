#include "rect.h"

AxisAlignedRect::AxisAlignedRect() :
	min({ std::ref(xmin), std::ref(ymin) }),
	center({ std::ref(xmid), std::ref(ymid) }),
	max({ std::ref(xmax), std::ref(ymax) }),
	topLeft({ std::ref(xmin), std::ref(ymax) }),
	bottomRight({ std::ref(xmax), std::ref(ymin) })
{}
//	xmin({ [this]() -> Value {
//		if (xmid.v && xmax.v) {
//			return xmid() * 2.0 - xmax();
//		}
//		else if (width) {
//			if (xmid) {
//				return xmid() - width() / 2.0;
//			}
//			else if (xmax) {
//				return xmax() - width();
//			}
//		}
//		puts("WARNING: Incomplete Rect");
//		return 0.0_ndc;
//	}, AxisAlignedRect::Value::NDC }),
//xmid([this]() -> Value {
//	if (xmin && xmax) {
//		return (xmin() - xmax()) / 2.0;
//	}
//	else if (width) {
//		if (xmin) {
//			return xmin() + width() / 2.0;
//		}
//		else if (xmax) {
//			return xmax() - width() / 2.0;
//		}
//	}
//	puts("WARNING: Incomplete Rect");
//	return 0.0_ndc;
//}),
//xmax([this]() -> Value {
//	if (xmin && xmid) {
//		return xmid() * 2.0 - xmin();
//	}
//	else if (width) {
//		if (xmin) {
//			return xmin() + width();
//		}
//		else if (xmid) {
//			return xmid() + width() / 2.0;
//		}
//	}
//	puts("WARNING: Incomplete Rect");
//	return 0.0_ndc;
//}),
//ymin([this]() -> Value {
//	if (ymid && ymax) {
//		return ymid() * 2.0 - ymax();
//	}
//	else if (height) {
//		if (ymid) {
//			return ymid() - height() / 2.0;
//		}
//		else if (ymax) {
//			return ymax() - height();
//		}
//	}
//	puts("WARNING: Incomplete Rect");
//	return 0.0_ndc;
//}),
//ymid([this]() -> Value {
//	if (ymin && ymax) {
//		return (ymin() - ymax()) / 2.0;
//	}
//	else if (height) {
//		if (ymin) {
//			return ymin() + height() / 2.0;
//		}
//		else if (ymax) {
//			return ymax() - height() / 2.0;
//		}
//	}
//	puts("WARNING: Incomplete Rect");
//	return 0.0_ndc;
//}),
//ymax([this]() -> Value {
//	if (ymin && ymid) {
//		return ymid() * 2.0 - ymin();
//	}
//	else if (height) {
//		if (ymin) {
//			return ymin() + height();
//		}
//		else if (ymid) {
//			return ymid() + height() / 2.0;
//		}
//	}
//	puts("WARNING: Incomplete Rect");
//	return 0.0_ndc;
//}),
//width([this]() -> Value {
//	if (xmin && xmid) {
//		return (xmid() - xmin()) * 2.0;
//	}
//	else if (xmid && xmax) {
//		return (xmax() - xmid()) * 2.0;
//	}
//	else if (xmin && xmax) {
//		return xmax() - xmin();
//	}
//	puts("WARNING: Incomplete Rect");
//	return 0.0_ndc;
//}),
//height([this]() -> Value {
//	if (ymin && ymid) {
//		return (ymid() - ymin()) * 2.0;
//	}
//	else if (ymid && ymax) {
//		return (ymax() - ymid()) * 2.0;
//	}
//	else if (ymin && ymax) {
//		return ymax() - ymin();
//	}
//	puts("WARNING: Incomplete Rect");
//	return 0.0_ndc;
//}),
//center({ xmid, ymid }),
//min({ xmin, ymin }),
//max({ xmax, ymax }),
//topLeft({ xmin, ymax }),
//bottomRight({ xmax, ymin })

//min([this]() {
//	if (bottomRight && width) {
//		return Vec2d{ {width, 0} } + bottomRight;
//	}
//	else if (topLeft && height) {
//		return -Vec2d{ {0, height} } + topLeft;
//	}
//	else if (center && width && height) {
//		return -Vec2d{ {width, height} } * 0.5 + center;
//	}
//	else if (max && width && height) {
//		return -Vec2d{ {width, height} } + max;
//	}
//	else if (center && bottomRight && height) {
//		return -Vec2d{ {width, height} } *0.5 + center;
//	}
//	else {
//		return Vec2d{ {0, 0} };
//	}
//}),
//max([this]() {
//	if (bottomRight && height) {
//		return Vec2d{ {0, height} } + bottomRight;
//	}
//	else if (topLeft && width) {
//		return Vec2d{ {width, 0} } + topLeft;
//	}
//	else if (center && width && height) {
//		return Vec2d{ {width, height} } * 0.5 + center;
//	}
//	else if (min && width && height) {
//		return Vec2d{ {width, height} } + min;
//	}
//	else {
//		return Vec2d{ {0, 0} };
//	}
//}),
//topLeft([this]() {
//	if (max && width) {
//		return -Vec2d{ {width, 0} } + max;
//	}
//	else if (min && height) {
//		return Vec2d{ {0, height} } + min;
//	}
//	else if (center && width && height) {
//		return Vec2d{ {-width(), height} } * 0.5 + center;
//	}
//	else if (bottomRight && width && height) {
//		return Vec2d{ {-width(), height} } + bottomRight;
//	}
//	else {
//		return Vec2d{ {0, 0} };
//	}
//}),
//bottomRight([this]() {
//	if (max && height) {
//		return -Vec2d{ {0, height} } + max;
//	}
//	else if (min && width) {
//		return Vec2d{ {0, width} } + min;
//	}
//	else if (center && width && height) {
//		return Vec2d{ {width, -height()} } * 0.5 + center;
//	}
//	else if (bottomRight && width && height) {
//		return Vec2d{ {width, -height()} } +bottomRight;
//	}
//	else {
//		return Vec2d{ {0, 0} };
//	}
//}),
//center([this]() {
//	if (min && max) {
//		return (min() + max()) * 0.5;
//	}
//	else if (topLeft && bottomRight) {
//		return (topLeft() + bottomRight()) * 0.5;
//	}
//	else if (min && topLeft && width) {
//		return (min() + topLeft() + Vec2d{ {width, 0} }) * 0.5;
//	}
//	else if (topLeft && max && height) {
//		return (topLeft() + max() - Vec2d{ {0, height} }) * 0.5;
//	}
//	else if (max && bottomRight && width) {
//		return (max() + bottomRight() - Vec2d{ {width, 0} }) * 0.5;
//	}
//	else if (bottomRight && min && height) {
//		return (bottomRight() + min() + Vec2d{ {0, height} }) * 0.5;
//	}
//	else if (min && width && height) {
//		return (min() + Vec2d{ {width, height} }) * 0.5;
//	}
//	else if (topLeft && width && height) {
//		return (topLeft() + Vec2d{ {width, -height()} }) * 0.5;
//	}
//	else if (max && width && height) {
//		return (max() + Vec2d{ {-width(), -height()} }) * 0.5;
//	}
//	else if (bottomRight && width && height) {
//		return (bottomRight() + Vec2d{ {-width(), height} }) * 0.5;
//	}
//	else {
//		return Vec2d{ {0, 0} };
//	}
//}),
//width([this]() {
//	if (bottomRight && min) {
//		return bottomRight()[0] - min()[0];
//	}
//	else if (bottomRight && topLeft) {
//		return bottomRight()[0] - topLeft()[0];
//	}
//	else if (bottomRight && center) {
//		return (bottomRight()[0] - center()[0]) * 2.0;
//	}
//	else if (max && min) {
//		return max()[0] - min()[0];
//	}
//	else if (max && topLeft) {
//		return max()[0] - topLeft()[0];
//	}
//	else if (max && center) {
//		return (max()[0] - center()[0]) * 2.0;
//	}
//	else if (center && min) {
//		return (center()[0] - min()[0]) * 2.0;
//	}
//	else if (center && topLeft) {
//		return (center()[0] - topLeft()[0]) * 2.0;
//	}
//	else {
//		return 0.0;
//	}
//}),
//height([this]() {
//	if (topLeft && min) {
//		return topLeft()[1] - min()[1];
//	}
//	else if (topLeft && bottomRight) {
//		return topLeft()[1] - bottomRight()[1];
//	}
//	else if (topLeft && center) {
//		return (topLeft()[1] - center()[1]) * 2.0;
//	}
//	else if (max && min) {
//		return max()[1] - min()[1];
//	}
//	else if (max && bottomRight) {
//		return max()[1] - bottomRight()[1];
//	}
//	else if (max && center) {
//		return (max()[1] - center()[1]) * 2.0;
//	}
//	else if (center && min) {
//		return (center()[1] - min()[1]) * 2.0;
//	}
//	else if (center && bottomRight) {
//		return (center()[1] - min()[1]) * 2.0;
//	}
//	else {
//		return 0.0;
//	}
//}) {}

bool AxisAlignedRect::testPoint(Vec2d point) {

	bool w = getWidth() > 0;
	bool h = getHeight() > 0;

	double x1 = w ? getXmin() : getXmax();
	double y1 = h ? getYmin() : getYmax();
	double x2 = w ? getXmax() : getXmin();
	double y2 = h ? getYmax() : getYmin();

	return point[0] >= x1 && point[0] <= x2 && point[1] >= y1 && point[1] <= y2;
}

Matrix4f AxisAlignedRect::createGUIMatrix() {
	return Matrix4f::Identity()
		.scale({ {getWidth(), getHeight(), 1.0f, 1.0f} })
		.translate({ {getXmin(), getYmin(), 0} })
		.scale({ {2.0f, 2.0f, 1.0f, 1.0f} })
		.translate({ {-1.0f, -1.0f, 0.0f} });
}

#define V(val) val.transform<float>(resolve)

float AxisAlignedRect::getXmin() {
	auto resolve = [this](Value &v) -> Optional<float> { return v(ctx, AXIS::X); };
	return V(xmin).value_or(
		V(xmid).value_or(
			V(xmax).value_or(1) - V(width).value_or(1) / 2.0f
		) * 2.0f
		-
		V(xmax).value_or(
			V(xmid).value_or(0.5f) + V(width).value_or(1) / 2.0f
		)
	);
}

float AxisAlignedRect::getXmid() {
	auto resolve = [this](Value &v) -> Optional<float> { return v(ctx, AXIS::X); };
	return V(xmid).value_or(
		(
			V(xmax).value_or(
				V(xmin).value_or(0) + V(width).value_or(1)
			)
			+
			V(xmin).value_or(
				V(xmax).value_or(1) - V(width).value_or(1)
			)
		) / 2.0f
	);
}

float AxisAlignedRect::getXmax() {
	auto resolve = [this](Value &v) -> Optional<float> { return v(ctx, AXIS::X); };
	return V(xmax).value_or(
		V(xmid).value_or(
			V(xmin).value_or(0) + V(width).value_or(1) / 2.0f
		) * 2.0f
		-
		V(xmin).value_or(
			V(xmid).value_or(0.5) - V(width).value_or(1) / 2.0f
		)
	);
}

float AxisAlignedRect::getYmin() {
	auto resolve = [this](Value &v) -> Optional<float> { return v(ctx, AXIS::Y); };
	return V(ymin).value_or(
		V(ymid).value_or(
			V(ymax).value_or(1) - V(height).value_or(1) / 2.0f
		) * 2.0f
		-
		V(ymax).value_or(
			V(ymid).value_or(0.5f) + V(height).value_or(1) / 2.0f
		)
	);
}

float AxisAlignedRect::getYmid() {
	auto resolve = [this](Value &v) -> Optional<float> { return v(ctx, AXIS::Y); };
	return V(ymid).value_or(
		(
			V(ymax).value_or(
				V(ymin).value_or(0) + V(height).value_or(1)
			)
			+
			V(ymin).value_or(
				V(ymax).value_or(1) - V(height).value_or(1)
			)
		) / 2.0f
	);
}

float AxisAlignedRect::getYmax() {
	auto resolve = [this](Value &v) -> Optional<float> { return v(ctx, AXIS::Y); };
	return V(ymax).value_or(
		V(ymid).value_or(
			V(ymin).value_or(0) + V(height).value_or(1) / 2.0f
		) * 2.0f
		-
		V(ymin).value_or(
			V(ymid).value_or(0.5) - V(height).value_or(1) / 2.0f
		)
	);
}

float AxisAlignedRect::getWidth() {
	auto resolve = [this](Value &v) -> Optional<float> { return v(ctx, AXIS::X); };
	return V(width).value_or(
		V(xmax).value_or(
			V(xmid).value_or(0.5f) * 2.0f - V(xmin).value_or(0)
		)
		-
		V(xmin).value_or(
			V(xmax).value_or(1) - V(xmax).value_or(0.5) * 2.0f
		)
	);
}

float AxisAlignedRect::getHeight() {
	auto resolve = [this](Value &v) -> Optional<float> { return v(ctx, AXIS::Y); };
	return V(height).value_or(
		V(ymax).value_or(
			V(ymid).value_or(0.5f) * 2.0f - V(ymin).value_or(0)
		)
		-
		V(ymin).value_or(
			V(ymax).value_or(1) - V(ymax).value_or(0.5) * 2.0f
		)
	);
}

#undef V

Vec2f AxisAlignedRect::getCenter() {
	return { {getXmid(), getYmid()} };
}

Vec2f AxisAlignedRect::getMin(){
	return { {getXmin(), getYmin()} };
}

Vec2f AxisAlignedRect::getMax() {
	return { {getXmax(), getYmax()} };
}

Vec2f AxisAlignedRect::getTopLeft() {
	return { {getXmin(), getYmax()} };
}

Vec2f AxisAlignedRect::getBottomRight() {
	return { {getXmax(), getYmin()} };
}

AxisAlignedRect::Vector::Vector(std::array<std::reference_wrapper<Optional<Value>>, 2> list) : Vec<std::reference_wrapper<Optional<Value>>, 2>(list) {}

AxisAlignedRect::Vector & AxisAlignedRect::Vector::operator=(const Vec<Value, 2> & other) {
	(*this)[0].get() = other[0];
	(*this)[1].get() = other[1];
	return *this;
}

void AxisAlignedRect::reset(Optional<Value>& a, Optional<Value>& b, Optional<Value>& c, Optional<Value>& d) {
#define TEST(z) if (z && &z != &a && &z != &b && &z != &c && &z != &d) z.reset()
	TEST(xmin);
	TEST(xmid);
	TEST(xmax);
	TEST(ymin);
	TEST(ymid);
	TEST(ymax);
	TEST(width);
	TEST(height);
#undef  TEST
}

void AxisAlignedRect::reset(Vector a, Optional<Value>& b, Optional<Value>& c) {
#define TEST(z) if (z && &z != &(a[0].get()) && &z != &(a[1].get()) && &z != &b && &z != &c) z.reset()
	TEST(xmin);
	TEST(xmid);
	TEST(xmax);
	TEST(ymin);
	TEST(ymid);
	TEST(ymax);
	TEST(width);
	TEST(height);
#undef  TEST
}

void AxisAlignedRect::reset(Vector a, Vector b) {
#define TEST(z) if (z && &z != &(a[0].get()) && &z != &(a[1].get()) && &z != &(b[0].get()) && &z != &(b[1].get())) z.reset()
	TEST(xmin);
	TEST(xmid);
	TEST(xmax);
	TEST(ymin);
	TEST(ymid);
	TEST(ymax);
	TEST(width);
	TEST(height);
#undef  TEST
}

AxisAlignedRect::Value operator "" _px(long double d) {
	return AxisAlignedRect::Value{ (double)d, AxisAlignedRect::Value::PX };
}
AxisAlignedRect::Value operator "" _vw(long double d) {
	return AxisAlignedRect::Value{ (double)d, AxisAlignedRect::Value::VW };
}
AxisAlignedRect::Value operator "" _vh(long double d) {
	return AxisAlignedRect::Value{ (double)d, AxisAlignedRect::Value::VH };
}
AxisAlignedRect::Value operator "" _ndc(long double d) {
	return AxisAlignedRect::Value{ (double)d, AxisAlignedRect::Value::NDC };
}

//AxisAlignedRect::Value operator+(const AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right)
//{
//	return { (double)left + (double)right, AxisAlignedRect::Value::NDC };
//}
//
//AxisAlignedRect::Value operator+(const double & left, const AxisAlignedRect::Value & right)
//{
//	return { left + (double)right, AxisAlignedRect::Value::NDC };
//}
//
//AxisAlignedRect::Value operator+(const AxisAlignedRect::Value & left, const double & right)
//{
//	return { (double)left + right, AxisAlignedRect::Value::NDC };
//}
//
//AxisAlignedRect::Value operator-(const AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right)
//{
//	return { (double)left - (double)right, AxisAlignedRect::Value::NDC };
//}
//
//AxisAlignedRect::Value operator-(const double & left, const AxisAlignedRect::Value & right)
//{
//	return { (double)left - (double)right, AxisAlignedRect::Value::NDC };
//}
//
//AxisAlignedRect::Value operator-(const AxisAlignedRect::Value & left, const double & right)
//{
//	return { (double)left - (double)right, AxisAlignedRect::Value::NDC };
//}
//
//AxisAlignedRect::Value operator*(const AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right)
//{
//	return { (double)left * (double)right, AxisAlignedRect::Value::NDC };
//}
//
//AxisAlignedRect::Value operator*(const double & left, const AxisAlignedRect::Value & right)
//{
//	return { (double)left * (double)right, AxisAlignedRect::Value::NDC };
//}
//
//AxisAlignedRect::Value operator*(const AxisAlignedRect::Value & left, const double & right)
//{
//	return { (double)left * (double)right, AxisAlignedRect::Value::NDC };
//}
//
//AxisAlignedRect::Value operator/(const AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right)
//{
//	return { (double)left / (double)right, AxisAlignedRect::Value::NDC };
//}
//
//AxisAlignedRect::Value operator/(const double & left, const AxisAlignedRect::Value & right)
//{
//	return { (double)left / (double)right, AxisAlignedRect::Value::NDC };
//}
//
//AxisAlignedRect::Value operator/(const AxisAlignedRect::Value & left, const double & right)
//{
//	return { (double)left / (double)right, AxisAlignedRect::Value::NDC };
//}
//
//AxisAlignedRect::Value & operator+=(AxisAlignedRect::Value & left, const double & right)
//{
//	left.v += right;
//	return left;
//}
//
//AxisAlignedRect::Value & operator+=(AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right)
//{
//	left.v += right.v;
//	left.unit = AxisAlignedRect::Value::NDC;
//	return left;
//}
//
//AxisAlignedRect::Value & operator-=(AxisAlignedRect::Value & left, const double & right)
//{
//	left.v -= right;
//	return left;
//}
//
//AxisAlignedRect::Value & operator-=(AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right)
//{
//	left.v -= right.v;
//	left.unit = AxisAlignedRect::Value::NDC;
//	return left;
//}
//
//AxisAlignedRect::Value & operator*=(AxisAlignedRect::Value & left, const double & right)
//{
//	left.v *= right;
//	return left;
//}
//
//AxisAlignedRect::Value & operator*=(AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right)
//{
//	left.v *= right.v;
//	left.unit = AxisAlignedRect::Value::NDC;
//	return left;
//}
//
//AxisAlignedRect::Value & operator/=(AxisAlignedRect::Value & left, const double & right)
//{
//	left.v /= right;
//	return left;
//}
//
//AxisAlignedRect::Value & operator/=(AxisAlignedRect::Value & left, const AxisAlignedRect::Value & right)
//{
//	left.v /= right.v;
//	left.unit = AxisAlignedRect::Value::NDC;
//	return left;
//}
//
//AxisAlignedRect::Value operator-(const AxisAlignedRect::Value & value)
//{
//	return { -value.v, value.unit };
//}

