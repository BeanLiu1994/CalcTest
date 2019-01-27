#pragma once

#include <memory>
#include <functional>
#include <cmath>
#include <limits>

using _calc_Ty = double;

#pragma region BasicTypes
struct Op
{
	inline virtual _calc_Ty Evaluate() const = 0;
};

struct Node
{
	Node(_calc_Ty _val) :val(_val), flag(5) {}

	Node(std::shared_ptr<Op> in) : SomeOp(in), flag(0), val(std::numeric_limits<_calc_Ty>::quiet_NaN()) {}

	// Before build;
	std::shared_ptr<Op> SomeOp = nullptr;

	// After build;
	inline _calc_Ty Evaluate() const
	{
		if (flag)
			return val;

		if (!SomeOp && !flag)
			return std::numeric_limits<_calc_Ty>::quiet_NaN();

		val = SomeOp->Evaluate();

		flag |= 1;

		return val;
	}

	void ChangeVal(_calc_Ty in) { val = in; flag |= 1; }

	mutable _calc_Ty val = std::numeric_limits<_calc_Ty>::quiet_NaN();
	mutable char flag = 0;
};


void no_delete(const Node*) {}
template<typename T>
std::shared_ptr<const Node> Initial_data(T&& input)
{
	if constexpr (std::is_same<typename std::decay<T&&>::type, Node>::value && std::is_lvalue_reference<T&&>::value)
	{
		return std::shared_ptr<const Node>(&input, &no_delete);
	}
	else
	{
		return std::make_shared<const Node>(input);
	}
}

struct UnaryOp :Op
{
	template<typename T>
	UnaryOp(T&& in) :data(Initial_data(std::forward<T&&>(in))) {}

	std::shared_ptr<const Node> data;
	_calc_Ty Evaluate() const override { return Evaluate(data->Evaluate()); }

	inline virtual _calc_Ty Evaluate(const _calc_Ty&) const = 0;
};

struct BinaryOp :Op
{
	template<typename T1, typename T2>
	BinaryOp(T1 && n1, T2 && n2)
		:data1(Initial_data(std::forward<T1&&>(n1))), data2(Initial_data(std::forward<T2&&>(n2))) {}


	std::shared_ptr<const Node> data1, data2;
	inline _calc_Ty Evaluate() const override
	{
		return Evaluate(data1->Evaluate(), data2->Evaluate());
	}

	inline virtual _calc_Ty Evaluate(const _calc_Ty&, const _calc_Ty&) const = 0;
};
#pragma endregion

#pragma region COMMON_OPERATION
template<typename T1, typename T2>
struct Plus :BinaryOp
{
	Plus(T1 && n1, T2 && n2) :BinaryOp(std::forward< T1 && >(n1), std::forward< T2 && >(n2)) {}
	inline _calc_Ty Evaluate(const _calc_Ty& v1, const _calc_Ty& v2) const override
	{
		return v1 + v2;
	}
};

template<typename T1, typename T2>
struct Minus :BinaryOp
{
	Minus(T1 && n1, T2 && n2) :BinaryOp(std::forward< T1 && >(n1), std::forward< T2 && >(n2)) {}
	inline _calc_Ty Evaluate(const _calc_Ty& v1, const _calc_Ty& v2) const override
	{
		return v1 - v2;
	}
};

template<typename T1, typename T2>
struct Product :BinaryOp
{
	Product(T1 && n1, T2 && n2) :BinaryOp(std::forward< T1 && >(n1), std::forward< T2 && >(n2)) {}
	inline _calc_Ty Evaluate(const _calc_Ty& v1, const _calc_Ty& v2) const override
	{
		return v1 * v2;
	}
};

template<typename T1, typename T2>
struct Divide :BinaryOp
{
	Divide(T1 && n1, T2 && n2) :BinaryOp(std::forward< T1 && >(n1), std::forward< T2 && >(n2)) {}
	inline _calc_Ty Evaluate(const _calc_Ty& v1, const _calc_Ty& v2) const override
	{
		return v1 / v2;
	}
};

template<typename T>
struct Abs :UnaryOp
{
	Abs(T&& in) :UnaryOp(std::forward<T&&>(in)) {}
	inline _calc_Ty Evaluate(const _calc_Ty& v) const override
	{
		return std::abs(v);
	}
};
template<typename T>
struct Exp :UnaryOp
{
	Exp(T&& in) :UnaryOp(std::forward<T&&>(in)) {}
	inline _calc_Ty Evaluate(const _calc_Ty& v) const override
	{
		return std::exp(v);
	}
};

template<typename T1, typename T2>
Node operator+(T1&& n1, T2&& n2)
{
	return Node(std::make_shared<Plus<decltype(n1), decltype(n2)>>
		(std::forward< T1 && >(n1), std::forward< T2 && >(n2)));
}
template<typename T1, typename T2>
Node operator-(T1&& n1, T2&& n2)
{
	return Node(std::make_shared<Minus<decltype(n1), decltype(n2)>>
		(std::forward< T1 && >(n1), std::forward< T2 && >(n2)));
}
template<typename T1, typename T2>
Node operator*(T1&& n1, T2&& n2)
{
	return Node(std::make_shared<Product<decltype(n1), decltype(n2)>>
		(std::forward< T1 && >(n1), std::forward< T2 && >(n2)));
}
template<typename T1, typename T2>
Node operator/(T1&& n1, T2&& n2)
{
	return Node(std::make_shared<Divide<decltype(n1), decltype(n2)>>
		(std::forward< T1 && >(n1), std::forward< T2 && >(n2)));
}
template<typename T>
Node abs(T&& in)
{
	return Node(std::make_shared<Abs<decltype(in)>>
		(std::forward<T&&>(in)));
}
template<typename T>
Node exp(T&& in)
{
	return Node(std::make_shared<Exp<decltype(in)>>
		(std::forward<T&&>(in)));
}
#pragma endregion

#pragma region GUIDE_FOR_ADD_AN_OPERATION
// To add a unary operation
template<typename T>
struct CustomUnaryOp :UnaryOp
{
	CustomUnaryOp(T&& in) :UnaryOp(std::forward<T&&>(in)) {}
	inline _calc_Ty Evaluate(const _calc_Ty& v) const override
	{
		// TO DO: fill in your custom evaluation.
		return v;
	}
};
template<typename T>
Node func_unary(T&& in)
{
	return Node(std::make_shared<CustomUnaryOp<decltype(in)>>
		(std::forward<T&&>(in)));
}

// To add a binary operation
template<typename T1, typename T2>
struct CustomBinaryOp :BinaryOp
{
	CustomBinaryOp(T1 && n1, T2 && n2) :BinaryOp(std::forward< T1 && >(n1), std::forward< T2 && >(n2)) {}
	inline _calc_Ty Evaluate(const _calc_Ty& v1, const _calc_Ty& v2) const override
	{
		// TO DO: fill in your custom evaluation.
		return std::round(v1 * v2);
	}
};
template<typename T1, typename T2>
Node func_binary(T1&& n1, T2&& n2)
{
	return Node(std::make_shared<CustomBinaryOp<decltype(n1), decltype(n2)>>
		(std::forward< T1 && >(n1), std::forward< T2 && >(n2)));
}
#pragma endregion
