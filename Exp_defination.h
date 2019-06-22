#pragma once

#include <utility>
#include "Variable.h"
// traits
#include <type_traits>

template<typename Ty>
struct _get_value_type
{
	typedef typename std::decay<Ty>::type::value_type type;
};

template<typename Ty1, typename Ty2>
struct _check_same_value_type
{
	typedef typename std::enable_if<
		std::is_same<typename _get_value_type<Ty1>::type, typename _get_value_type<Ty2>::type>::value,
		typename std::decay<Ty1>::type::value_type
	>::type type;
};


// 将arithmetic的类型包起来
template<typename Ty>
class _gen_non_value_type
{
	typedef typename std::decay<Ty>::type ty_decay;
public:
	typedef typename std::conditional<
		std::is_arithmetic<ty_decay>::value,
		Const<ty_decay>,
		ty_decay
	>::type type;
};

// defination

template<typename Ty, template<typename> typename _OP>
class UnaryExp
{
public:
	typedef typename _gen_non_value_type<Ty>::type param_type;
	typedef typename param_type::value_type value_type;
	typedef _OP<value_type> OP;

	param_type p_;

	template<typename Ty_>
	UnaryExp(Ty_&& p) :p_(std::forward<Ty_>(p)) {}

	inline value_type operator()(value_type d) { return OP::apply(p_(d)); }
};

template<typename Ty1, typename Ty2, template<typename> typename _OP>
class BinaryExp
{
public:
	typedef typename _gen_non_value_type<Ty1>::type left_param_type;
	typedef typename _gen_non_value_type<Ty2>::type right_param_type;
	typedef typename _check_same_value_type<left_param_type, right_param_type>::type value_type;
	typedef _OP<value_type> OP;

	left_param_type l_;
	right_param_type r_;

	template<typename Ty_l, typename Ty_r>
	BinaryExp(Ty_l&& l, Ty_r&& r) :l_(std::forward<Ty_l>(l)), r_(std::forward<Ty_r>(r)) 
	{

	}
	inline value_type operator()(value_type d) { return OP::apply(l_(d), r_(d)); }
};