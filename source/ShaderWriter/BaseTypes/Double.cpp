/*
See LICENSE file in root folder
*/
#include "ShaderWriter/BaseTypes/Double.hpp"

namespace
{
	void compileDouble()
	{
		bool ignore;
		auto a = 0.0_d;
		auto b = 1.0_d;
		a += b;
		a -= b;
		a *= b;
		a /= b;
		ignore = a == b;
		ignore = a != b;
		ignore = a <= b;
		ignore = a >= b;
		ignore = a < b;
		ignore = a > b;
		a + b;
		a - b;
		a * b;
		a / b;
		a += 1.0;
		a -= 1.0;
		a *= 1.0;
		a /= 1.0;
		ignore = a == 1.0;
		ignore = a != 1.0;
		ignore = a <= 1.0;
		ignore = a >= 1.0;
		ignore = a < 1.0;
		ignore = a > 1.0;
		a + 1.0;
		a - 1.0;
		a * 1.0;
		a / 1.0;
		ignore = 1.0 == a;
		ignore = 1.0 != a;
		ignore = 1.0 <= a;
		ignore = 1.0 >= a;
		ignore = 1.0 < a;
		ignore = 1.0 > a;
		1.0 + a;
		1.0 - a;
		1.0 * a;
		1.0 / a;

		auto c = sdw::Optional< sdw::Double >{ b, false };
		a += c;
		a -= c;
		a *= c;
		a /= c;
		ignore = a == c;
		ignore = a != c;
		ignore = a <= c;
		ignore = a >= c;
		ignore = a < c;
		ignore = a > c;
		a + c;
		a - c;
		a * c;
		a / c;
		c += b;
		c -= b;
		c *= b;
		c /= b;
		ignore = c == b;
		ignore = c != b;
		ignore = c <= b;
		ignore = c >= b;
		ignore = c < b;
		ignore = c > b;
		c + b;
		c - b;
		c * b;
		c / b;
		c += 1.0;
		c -= 1.0;
		c *= 1.0;
		c /= 1.0;
		ignore = c == 1.0;
		ignore = c != 1.0;
		ignore = c <= 1.0;
		ignore = c >= 1.0;
		ignore = c < 1.0;
		ignore = c > 1.0;
		c + 1.0;
		c - 1.0;
		c * 1.0;
		c / 1.0;
		ignore = 1.0 == c;
		ignore = 1.0 != c;
		ignore = 1.0 <= c;
		ignore = 1.0 >= c;
		ignore = 1.0 < c;
		ignore = 1.0 > c;
		1.0 + c;
		1.0 - c;
		1.0 * c;
		1.0 / c;

		auto d = sdw::Optional< sdw::Double >{ b, false };
		a += d;
		a -= d;
		a *= d;
		a /= d;
		ignore = a == d;
		ignore = a != d;
		ignore = a <= d;
		ignore = a >= d;
		ignore = a < d;
		ignore = a > d;
		a + d;
		a - d;
		a * d;
		a / d;
		d += b;
		d -= b;
		d *= b;
		d /= b;
		ignore = d == b;
		ignore = d != b;
		ignore = d <= b;
		ignore = d >= b;
		ignore = d < b;
		ignore = d > b;
		d + b;
		d - b;
		d * b;
		d / b;
		d += 1.0;
		d -= 1.0;
		d *= 1.0;
		d /= 1.0;
		ignore = d == 1.0;
		ignore = d != 1.0;
		ignore = d <= 1.0;
		ignore = d >= 1.0;
		ignore = d < 1.0;
		ignore = d > 1.0;
		d + 1.0;
		d - 1.0;
		d * 1.0;
		d / 1.0;
		ignore = 1.0 == d;
		ignore = 1.0 != d;
		ignore = 1.0 <= d;
		ignore = 1.0 >= d;
		ignore = 1.0 < d;
		ignore = 1.0 > d;
		1.0 + d;
		1.0 - d;
		1.0 * d;
		1.0 / d;

		auto e = sdw::MaybeOptional< sdw::Double >{ c };
		a += e;
		a -= e;
		a *= e;
		a /= e;
		ignore = a == e;
		ignore = a != e;
		ignore = a <= e;
		ignore = a >= e;
		ignore = a < e;
		ignore = a > e;
		a + e;
		a - e;
		a * e;
		a / e;
		e += b;
		e -= b;
		e *= b;
		e /= b;
		ignore = e == b;
		ignore = e != b;
		ignore = e <= b;
		ignore = e >= b;
		ignore = e < b;
		ignore = e > b;
		e + b;
		e - b;
		e * b;
		e / b;
		e += 1.0;
		e -= 1.0;
		e *= 1.0;
		e /= 1.0;
		ignore = e == 1.0;
		ignore = e != 1.0;
		ignore = e <= 1.0;
		ignore = e >= 1.0;
		ignore = e < 1.0;
		ignore = e > 1.0;
		e + 1.0;
		e - 1.0;
		e * 1.0;
		e / 1.0;
		ignore = 1.0 == e;
		ignore = 1.0 != e;
		ignore = 1.0 <= e;
		ignore = 1.0 >= e;
		ignore = 1.0 < e;
		ignore = 1.0 > e;
		1.0 + e;
		1.0 - e;
		1.0 * e;
		1.0 / e;

		auto f = sdw::MaybeOptional< sdw::Double >{ b };
		a += f;
		a -= f;
		a *= f;
		a /= f;
		ignore = a == f;
		ignore = a != f;
		ignore = a <= f;
		ignore = a >= f;
		ignore = a < f;
		ignore = a > f;
		a + f;
		a - f;
		a * f;
		a / f;
		f += b;
		f -= b;
		f *= b;
		f /= b;
		ignore = f == b;
		ignore = f != b;
		ignore = f <= b;
		ignore = f >= b;
		ignore = f < b;
		ignore = f > b;
		f + b;
		f - b;
		f * b;
		f / b;
		f += 1.0;
		f -= 1.0;
		f *= 1.0;
		f /= 1.0;
		ignore = f == 1.0;
		ignore = f != 1.0;
		ignore = f <= 1.0;
		ignore = f >= 1.0;
		ignore = f < 1.0;
		ignore = f > 1.0;
		f + 1.0;
		f - 1.0;
		f * 1.0;
		f / 1.0;
		ignore = 1.0 == f;
		ignore = 1.0 != f;
		ignore = 1.0 <= f;
		ignore = 1.0 >= f;
		ignore = 1.0 < f;
		ignore = 1.0 > f;
		1.0 + f;
		1.0 - f;
		1.0 * f;
		1.0 / f;
	}
}
