#pragma once

#include "ComplexString.h"
#include "ComplexConvert.h"
#include <math.h>

namespace ComplexLibrary
{
	class ComplexVariant
	{
	public:

		enum ComplexVariantType
		{
			CVT_NONE = 0,
			CVT_BOOL,
			CVT_INT,
			CVT_DOUBLE,
			CVT_STRING
		};

		ComplexVariant()
			: cvt(CVT_NONE), b_variant(false), i_variant(0), f_variant(0.0), s_variant()
		{

		}

		ComplexVariant(bool variant)
			: cvt(CVT_BOOL), b_variant(variant), i_variant(0), f_variant(0.0), s_variant()
		{

		}

		ComplexVariant(int variant)
			: cvt(CVT_INT), b_variant(false), i_variant(variant), f_variant(0.0), s_variant()
		{

		}

		ComplexVariant(double variant)
			: cvt(CVT_DOUBLE), b_variant(false), i_variant(0), f_variant(variant), s_variant()
		{

		}

		ComplexVariant(ComplexString variant)
			: cvt(CVT_STRING), b_variant(false), i_variant(0), f_variant(0.0), s_variant(variant)
		{

		}

		ComplexVariant(const char* variant)
			: cvt(CVT_STRING), b_variant(false), i_variant(0), f_variant(0.0), s_variant(variant)
		{

		}

		ComplexVariant(const ComplexVariant& variant)
			: cvt(CVT_NONE), b_variant(false), i_variant(0), f_variant(0.0), s_variant("")
		{
			cvt = variant.cvt;
			b_variant = variant.b_variant;
			i_variant = variant.i_variant;
			f_variant = variant.f_variant;
			s_variant = const_cast<ComplexString&>(variant.s_variant);
		}

		~ComplexVariant()
		{

		}

		ComplexVariantType GetVariantType() const
		{
			return cvt;
		}

		bool GetBoolean() const
		{
			return b_variant;
		}

		int GetInteger() const
		{
			return i_variant;
		}

		double GetDouble() const
		{
			return f_variant;
		}

		ComplexString GetString() const
		{
			return s_variant;
		}

		operator int()
		{
			if (cvt == CVT_BOOL)
			{
				i_variant = static_cast<int>(b_variant);
				b_variant = false;
			}
			else if (cvt == CVT_INT)
			{
				// do nothing
			}
			else if (cvt == CVT_DOUBLE)
			{
				i_variant = static_cast<int>(f_variant);
				f_variant = 0.0;
			}
			else if (cvt == CVT_STRING)
			{
				i_variant = ComplexConvert::StringToInt(s_variant);
				s_variant = "";
			}

			cvt = CVT_INT;

			return i_variant;
		}

		operator double()
		{
			if (cvt == CVT_BOOL)
			{
				f_variant = static_cast<double>(b_variant);
				b_variant = false;
			}
			else if (cvt == CVT_INT)
			{
				f_variant = static_cast<double>(i_variant);
				i_variant = 0;
			}
			else if (cvt == CVT_DOUBLE)
			{
				// do nothing
			}
			else if (cvt == CVT_STRING)
			{
				f_variant = ComplexConvert::StringToDouble(s_variant);
				s_variant = "";
			}

			cvt = CVT_DOUBLE;

			return f_variant;
		}

		operator bool()
		{
			if (cvt == CVT_BOOL)
			{
				// do nothing
			}
			else if (cvt == CVT_INT)
			{
				b_variant = static_cast<bool>(i_variant);
				i_variant = 0;
			}
			else if (cvt == CVT_DOUBLE)
			{
				b_variant = static_cast<bool>(f_variant);
				f_variant = 0.0;
			}
			else if (cvt == CVT_STRING)
			{
				b_variant = static_cast<bool>(ComplexConvert::StringToInt(s_variant));
				s_variant = "";
			}

			cvt = CVT_DOUBLE;

			return b_variant;
		}

		operator ComplexString()
		{
			if (cvt == CVT_BOOL)
			{
				ComplexString buf;
				buf.Format("%d", b_variant);
				s_variant = buf;
				b_variant = false;
			}
			else if (cvt == CVT_INT)
			{
				ComplexString buf;
				buf.Format("%d", i_variant);
				s_variant = buf;
				i_variant = 0;
			}
			else if (cvt == CVT_DOUBLE)
			{
				ComplexString buf;
				buf.Format("%f", f_variant);
				s_variant = buf;
				f_variant = 0.0;
			}
			else if (cvt == CVT_STRING)
			{
				// do nothing
			}

			return s_variant;
		}

		operator const char*()
		{
			if (cvt == CVT_BOOL)
			{
				ComplexString buf;
				buf.Format("%d", b_variant);
				s_variant = buf;
				b_variant = false;
			}
			else if (cvt == CVT_INT)
			{
				ComplexString buf;
				buf.Format("%d", i_variant);
				s_variant = buf;
				i_variant = 0;
			}
			else if (cvt == CVT_DOUBLE)
			{
				ComplexString buf;
				buf.Format("%f", f_variant);
				s_variant = buf;
				f_variant = 0.0;
			}
			else if (cvt == CVT_STRING)
			{
				// do nothing
			}

			return s_variant.GetBuffer();
		}

		ComplexVariant& operator = (bool variant)
		{
			if (cvt == CVT_BOOL)
			{
				// do nothing
			}
			else if (cvt == CVT_INT)
			{
				b_variant = variant;
				i_variant = 0;
			}
			else if (cvt == CVT_DOUBLE)
			{
				b_variant = variant;
				f_variant = 0.0;
			}
			else if (cvt == CVT_STRING)
			{
				b_variant = variant;
				s_variant = "";
			}

			cvt = CVT_BOOL;

			return *this;
		}

		ComplexVariant& operator = (int variant)
		{
			if (cvt == CVT_BOOL)
			{
				i_variant = variant;
				b_variant = false;
			}
			else if (cvt == CVT_INT)
			{
				// do nothing
			}
			else if (cvt == CVT_DOUBLE)
			{
				i_variant = variant;
				f_variant = 0.0;
			}
			else if (cvt == CVT_STRING)
			{
				i_variant = variant;
				s_variant = "";
			}

			cvt = CVT_INT;

			return *this;
		}

		ComplexVariant& operator = (double variant)
		{
			if (cvt == CVT_BOOL)
			{
				f_variant = variant;
				b_variant = false;
			}
			else if (cvt == CVT_INT)
			{
				f_variant = variant;
				i_variant = 0;
			}
			else if (cvt == CVT_DOUBLE)
			{
				// do nothing
			}
			else if (cvt == CVT_STRING)
			{
				f_variant = variant;
				s_variant = "";
			}

			cvt = CVT_DOUBLE;

			return *this;
		}

		ComplexVariant& operator = (ComplexString& variant)
		{
			if (cvt == CVT_BOOL)
			{
				s_variant = variant;
				i_variant = 0;
			}
			else if (cvt == CVT_INT)
			{
				s_variant = variant;
				i_variant = 0;
			}
			else if (cvt == CVT_DOUBLE)
			{
				s_variant = variant;
				f_variant = 0.0;
			}
			else if (cvt == CVT_STRING)
			{
				// do nothing
			}

			cvt = CVT_STRING;

			return *this;
		}

		ComplexVariant& operator = (ComplexVariant& variant)
		{
			cvt = variant.cvt;
			b_variant = variant.b_variant;
			i_variant = variant.i_variant;
			f_variant = variant.f_variant;
			s_variant = variant.s_variant;

			return *this;
		}

		ComplexVariant operator + (int variant)
		{
			ComplexVariant tmp;
			tmp = *this;
			if (cvt == CVT_INT)
			{
				tmp.i_variant += variant;
			}
			else if (cvt == CVT_DOUBLE)
			{
				tmp.f_variant += static_cast<double>(variant);
			}
			else if (cvt == CVT_STRING)
			{
				ComplexString buf;
				buf.Format("%d", variant);
				tmp.s_variant += buf;
			}
			return tmp;
		}

		ComplexVariant operator + (double variant)
		{
			ComplexVariant tmp;
			tmp = *this;
			if (cvt == CVT_INT)
			{
				tmp.i_variant += static_cast<int>(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				tmp.f_variant += variant;
			}
			else if (cvt == CVT_STRING)
			{
				ComplexString buf;
				buf.Format("%f", variant);
				tmp.s_variant += buf;
			}
			return tmp;
		}

		ComplexVariant operator + (ComplexString variant)
		{
			ComplexVariant tmp;
			tmp = *this;
			if (cvt == CVT_INT)
			{
				tmp.i_variant += ComplexConvert::StringToInt(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				tmp.f_variant += ComplexConvert::StringToDouble(variant);
			}
			else if (cvt == CVT_STRING)
			{
				tmp.s_variant += variant;
			}
			return tmp;
		}

		ComplexVariant operator + (const char* variant)
		{
			ComplexVariant tmp;
			tmp = *this;
			if (cvt == CVT_INT)
			{
				tmp.i_variant += ComplexConvert::StringToInt(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				tmp.f_variant += ComplexConvert::StringToDouble(variant);
			}
			else if (cvt == CVT_STRING)
			{
				tmp.s_variant += variant;
			}
			return tmp;
		}

		ComplexVariant operator + (ComplexVariant variant)
		{
			ComplexVariant tmp;
			tmp = *this;
			if (cvt == CVT_INT)
			{
				if (variant.cvt == CVT_INT)
				{
					tmp.i_variant += variant.i_variant;
				}
				else if (variant.cvt == CVT_DOUBLE)
				{
					tmp.i_variant += static_cast<int>(variant.f_variant);
				}
				else if (variant.cvt == CVT_STRING)
				{
					tmp.i_variant += ComplexConvert::StringToInt(variant.s_variant);
				}
			}
			else if (cvt == CVT_DOUBLE)
			{
				if (variant.cvt == CVT_INT)
				{
					tmp.f_variant += static_cast<double>(variant.i_variant);
				}
				else if (variant.cvt == CVT_DOUBLE)
				{
					tmp.f_variant += variant.f_variant;
				}
				else if (variant.cvt == CVT_STRING)
				{
					tmp.f_variant += ComplexConvert::StringToDouble(variant.s_variant);
				}
			}
			else if (cvt == CVT_STRING)
			{
				if (variant.cvt == CVT_INT)
				{
					ComplexString buf;
					buf.Format("%d", variant);
					tmp.s_variant += buf;
				}
				else if (variant.cvt == CVT_DOUBLE)
				{
					ComplexString buf;
					buf.Format("%f", variant);
					tmp.s_variant += buf;
				}
				else if (variant.cvt == CVT_STRING)
				{
					tmp.s_variant += variant.s_variant;
				}
			}
			return tmp;
		}

		ComplexVariant& operator += (int variant)
		{
			if (cvt == CVT_INT)
			{
				i_variant += variant;
			}
			else if (cvt == CVT_DOUBLE)
			{
				f_variant += static_cast<double>(variant);
			}
			else if (cvt == CVT_STRING)
			{
				ComplexString tmp;
				tmp.Format("%d", variant);
				s_variant += tmp;
			}
			return *this;
		}

		ComplexVariant& operator += (double variant)
		{
			if (cvt == CVT_INT)
			{
				i_variant += static_cast<int>(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				f_variant += variant;
			}
			else if (cvt == CVT_STRING)
			{
				ComplexString tmp;
				tmp.Format("%f", variant);
				s_variant += tmp;
			}
			return *this;
		}

		ComplexVariant& operator += (ComplexString variant)
		{
			if (cvt == CVT_INT)
			{
				i_variant += ComplexConvert::StringToInt(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				f_variant += ComplexConvert::StringToDouble(variant);
			}
			else if (cvt == CVT_STRING)
			{
				s_variant += variant;
			}
			return *this;
		}

		ComplexVariant& operator += (const char* variant)
		{
			if (cvt == CVT_INT)
			{
				i_variant += ComplexConvert::StringToInt(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				f_variant += ComplexConvert::StringToDouble(variant);
			}
			else if (cvt == CVT_STRING)
			{
				s_variant += variant;
			}
			return *this;
		}

		ComplexVariant operator - (int variant)
		{
			ComplexVariant tmp;
			tmp = *this;
			if (cvt == CVT_INT)
			{
				tmp.i_variant += variant;
			}
			else if (cvt == CVT_DOUBLE)
			{
				tmp.f_variant += static_cast<double>(variant);
			}
			return tmp;
		}

		ComplexVariant operator - (double variant)
		{
			ComplexVariant tmp;
			tmp = *this;
			if (cvt == CVT_INT)
			{
				tmp.i_variant += static_cast<int>(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				tmp.f_variant += variant;
			}
			return tmp;
		}

		ComplexVariant operator -= (int variant)
		{
			if (cvt == CVT_INT)
			{
				i_variant += variant;
			}
			else if (cvt == CVT_DOUBLE)
			{
				f_variant += static_cast<double>(variant);
			}
			return *this;
		}

		ComplexVariant& operator -= (double variant)
		{
			if (cvt == CVT_INT)
			{
				i_variant += static_cast<int>(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				f_variant += variant;
			}
			return *this;
		}

		ComplexVariant operator * (int variant)
		{
			ComplexVariant tmp;
			tmp = *this;
			if (cvt == CVT_INT)
			{
				tmp.i_variant *= variant;
			}
			else if (cvt == CVT_DOUBLE)
			{
				tmp.f_variant *= static_cast<double>(variant);
			}
			return tmp;
		}

		ComplexVariant operator * (double variant)
		{
			ComplexVariant tmp;
			tmp = *this;
			if (cvt == CVT_INT)
			{
				tmp.i_variant *= static_cast<int>(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				tmp.f_variant *= variant;
			}
			return tmp;
		}

		ComplexVariant& operator *= (int variant)
		{
			if (cvt == CVT_INT)
			{
				i_variant *= variant;
			}
			else if (cvt == CVT_DOUBLE)
			{
				f_variant *= static_cast<double>(variant);
			}
			return *this;
		}

		ComplexVariant& operator *= (double variant)
		{
			if (cvt == CVT_INT)
			{
				i_variant *= static_cast<int>(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				f_variant *= variant;
			}
			return *this;
		}

		ComplexVariant operator / (int variant)
		{
			ComplexVariant tmp;
			tmp = *this;
			if (cvt == CVT_INT)
			{
				tmp.i_variant /= variant;
			}
			else if (cvt == CVT_DOUBLE)
			{
				tmp.f_variant /= static_cast<double>(variant);
			}
			return tmp;
		}

		ComplexVariant operator / (double variant)
		{
			ComplexVariant tmp;
			tmp = *this;
			if (cvt == CVT_INT)
			{
				tmp.i_variant /= static_cast<int>(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				tmp.f_variant /= variant;
			}
			return tmp;
		}

		ComplexVariant& operator /= (int variant)
		{
			if (cvt == CVT_INT)
			{
				i_variant /= variant;
			}
			else if (cvt == CVT_DOUBLE)
			{
				f_variant /= static_cast<double>(variant);
			}
			return *this;
		}

		ComplexVariant& operator /= (double variant)
		{
			if (cvt == CVT_INT)
			{
				i_variant /= static_cast<int>(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				f_variant /= variant;
			}
			return *this;
		}

		ComplexVariant& operator % (int variant)
		{
			ComplexVariant tmp;
			tmp = *this;
			if (cvt == CVT_INT)
			{
				tmp.i_variant %= variant;
			}
			else if (cvt == CVT_DOUBLE)
			{
				tmp.f_variant = fmod(f_variant, static_cast<double>(variant));
			}
			return tmp;
		}

		ComplexVariant operator % (double variant)
		{
			ComplexVariant tmp;
			tmp = *this;
			if (cvt == CVT_INT)
			{
				tmp.i_variant %= static_cast<int>(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				tmp.f_variant = fmod(f_variant, static_cast<double>(variant));
			}
			return tmp;
		}

		ComplexVariant operator %= (int variant)
		{
			if (cvt == CVT_INT)
			{
				i_variant %= variant;
			}
			else if (cvt == CVT_DOUBLE)
			{
				f_variant = fmod(f_variant, static_cast<double>(variant));
			}
			return *this;
		}

		ComplexVariant& operator %= (double variant)
		{
			if (cvt == CVT_INT)
			{
				i_variant %= static_cast<int>(variant);
			}
			else if (cvt == CVT_DOUBLE)
			{
				f_variant = fmod(f_variant, static_cast<double>(variant));
			}
			return *this;
		}

		bool operator == (bool variant)
		{
			if (cvt == CVT_BOOL)
				return b_variant == variant;

			return false;
		}

		bool operator == (int variant)
		{
			if (cvt == CVT_INT)
				return i_variant == variant;

			return false;
		}

		bool operator == (double variant)
		{
			if (cvt == CVT_DOUBLE)
				return f_variant == variant;

			return false;
		}

		bool operator == (ComplexString variant)
		{
			if (cvt == CVT_STRING)
				return s_variant == variant;

			return false;
		}

		bool operator == (ComplexVariant& variant)
		{
			return (b_variant == variant.b_variant && i_variant == variant.i_variant && f_variant == variant.f_variant && s_variant == variant.s_variant);
		}

		bool operator != (bool variant)
		{
			if (cvt == CVT_BOOL)
				return b_variant != variant;

			return true;
		}

		bool operator != (int variant)
		{
			if (cvt == CVT_INT)
				return i_variant != variant;

			return true;
		}

		bool operator != (double variant)
		{
			if (cvt == CVT_DOUBLE)
				return f_variant != variant;

			return true;
		}

		bool operator != (ComplexString variant)
		{
			if (cvt == CVT_STRING)
				return s_variant != variant;

			return true;
		}

		bool operator != (ComplexVariant& variant)
		{
			return (b_variant != variant.b_variant || i_variant != variant.i_variant || f_variant != variant.f_variant || s_variant != variant.s_variant);
		}

	private:

		ComplexVariantType cvt;
		bool b_variant;
		int i_variant;
		double f_variant;
		ComplexString s_variant;

	};
}