#pragma once

#include "Variable.h"

#include <cassert>
#include <cmath>

// unary

template<typename _value_type>
class Abs
{
public:
	typedef _value_type value_type;
	static value_type apply(value_type v)
	{
		return std::abs(v);
	}
};
template<typename Ty>
inline auto abs(Ty&& l)
{
	return UnaryExp<Ty, Abs>(std::forward<Ty>(l));
}

template<typename _value_type>
class Neg
{
public:
	typedef _value_type value_type;
	static value_type apply(value_type v)
	{
		return -v;
	}
};
template<typename Ty>
inline auto operator-(Ty&& l)
{
	return UnaryExp<Ty, Neg>(std::forward<Ty>(l));
}


// binary

template<typename _value_type>
class Plus
{
public:
	typedef _value_type value_type;
	static value_type apply(value_type v1, value_type v2)
	{
		return v1 + v2;
	}
};

template<typename Ty_l, typename Ty_r>
inline auto operator+(Ty_l&& l, Ty_r&& r)
{
	return BinaryExp<Ty_l, Ty_r, Plus>(std::forward<Ty_l>(l), std::forward<Ty_r>(r));
}



template<typename _value_type>
class Minus
{
public:
	typedef _value_type value_type;
	static value_type apply(value_type v1, value_type v2)
	{
		return v1 - v2;
	}
};

template<typename Ty_l, typename Ty_r>
inline auto operator-(Ty_l&& l, Ty_r&& r)
{
	return BinaryExp<Ty_l, Ty_r, Minus>(std::forward<Ty_l>(l), std::forward<Ty_r>(r));
}



template<typename _value_type>
class Product
{
public:
	typedef _value_type value_type;
	static value_type apply(value_type v1, value_type v2)
	{
		return v1 * v2;
	}
};

template<typename Ty_l, typename Ty_r>
inline auto operator*(Ty_l&& l, Ty_r&& r)
{
	return BinaryExp<Ty_l, Ty_r, Product>(std::forward<Ty_l>(l), std::forward<Ty_r>(r));
}



template<typename _value_type>
class Divide
{
public:
	typedef _value_type value_type;
	static value_type apply(value_type v1, value_type v2)
	{
		assert(v2 != 0);
		return v1 / v2;
	}
};

template<typename Ty_l, typename Ty_r>
inline auto operator/(Ty_l&& l, Ty_r&& r)
{
	return BinaryExp<Ty_l, Ty_r, Divide>(std::forward<Ty_l>(l), std::forward<Ty_r>(r));
}
