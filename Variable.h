#pragma once

template<typename Scalar_type>
class Var
{
public:
	typedef Scalar_type value_type;

	inline value_type operator()(value_type v) { return v; }
};

template<typename Scalar_type>
class Const
{
public:
	typedef Scalar_type value_type;
	value_type value;

	Const(value_type d) :value(d) {}
	inline value_type operator()(value_type) { return value; }
};