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

	Const(value_type d) :value(d) {}
	inline value_type operator()() { return value; }
	inline value_type operator()(value_type) { return value; }

private:
	value_type value;
};


// 区分变量的思路，并且预计将const使用这种类型来表示，后续也许可以通过类型检测来进行表达式简化？

template<typename Scalar_type, int _Var_id>
class ExtVar
{
public:
	constexpr static int Var_id = _Var_id;
	typedef Scalar_type value_type;

	inline static void setVal(value_type v) { _v = v; }

	inline value_type operator()() { return _v; }
	inline value_type operator()(value_type) { return _v; } // 暂时兼容一下

private:
	static value_type _v;
};

template<typename VarType>
inline void setVar(typename VarType::value_type d)
{
	ExtVar<typename VarType::value_type, VarType::Var_id>::setVal(d);
}

#ifndef STATIC_VAR_DEFINATION
#define STATIC_VAR_DEFINATION

template<typename Scalar_type, int Var_id>
typename ExtVar<Scalar_type, Var_id>::value_type ExtVar<Scalar_type, Var_id>::_v = 0;

#endif