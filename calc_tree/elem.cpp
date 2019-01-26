#include "elem.h"
#include <variant>
#include <iostream>
#include <vld.h>

using namespace std;
struct Op
{
	inline virtual Ty Evaluate() const = 0;
};

struct Node
{
	Node(Ty _val) :val(_val), flag(5){}

	Node(shared_ptr<Op> in) : SomeOp(in), flag(0), val(NAN){}

	// Before build;
	const shared_ptr<Op> SomeOp = nullptr;

	// After build;
	inline Ty Evaluate() const
	{
		if (flag)
			return val;

		if (!SomeOp && !flag)
			return NAN;
		
		val = SomeOp->Evaluate();

		flag |= 1;

		return val;
	}

	void ChangeVal(Ty in) { val = in; flag |= 1; }

	mutable Ty val = NAN;
	mutable char flag = 0;
};


void no_delete(const Node*) {}
template<typename T>
shared_ptr<const Node> Initial_data(T&& input)
{
	if constexpr (is_same_v<decay_t<T&&>, Node> && is_lvalue_reference_v<T&&>)
	{
		return shared_ptr<const Node>(&input, &no_delete);
	}
	else
	{
		return make_shared<const Node>(input);
	}
}

struct UnaryOp :Op
{
	template<typename T>
	UnaryOp(T&& in) :data(Initial_data(forward<T&&>(in))) {}

	shared_ptr<const Node> data;
	Ty Evaluate() const override { return Evaluate(data->Evaluate()); }

	inline virtual Ty Evaluate(const Ty&) const = 0;
};

struct BinaryOp :Op
{
	template<typename T1, typename T2>
	BinaryOp(T1 && n1, T2 && n2)
		:data1(Initial_data(forward<T1&&>(n1))), data2(Initial_data(forward<T2&&>(n2))) {}

	
	shared_ptr<const Node> data1, data2;
	inline Ty Evaluate() const override 
	{ 
		return Evaluate(data1->Evaluate(), data2->Evaluate()); 
	}

	inline virtual Ty Evaluate(const Ty&, const Ty&) const = 0;
};

template<typename T1, typename T2>
struct Plus :BinaryOp
{
	Plus(T1 && n1, T2 && n2) :BinaryOp(forward< T1 && >(n1), forward< T2 && >(n2)) {}
	inline Ty Evaluate(const Ty& v1, const Ty& v2) const override
	{
		return v1 + v2;
	}
};

template<typename T1, typename T2>
struct Minus :BinaryOp
{
	Minus(T1 && n1, T2 && n2) :BinaryOp(forward< T1 && >(n1), forward< T2 && >(n2)) {}
	inline Ty Evaluate(const Ty& v1, const Ty& v2) const override
	{
		return v1 - v2;
	}
};

template<typename T1, typename T2>
struct Product :BinaryOp
{
	Product(T1 && n1, T2 && n2) :BinaryOp(forward< T1 && >(n1), forward< T2 && >(n2)) {}
	inline Ty Evaluate(const Ty& v1, const Ty& v2) const override
	{
		return v1 * v2;
	}
};

template<typename T1, typename T2>
struct Divide :BinaryOp
{
	Divide(T1 && n1, T2 && n2) :BinaryOp(forward< T1 && >(n1), forward< T2 && >(n2)) {}
	inline Ty Evaluate(const Ty& v1, const Ty& v2) const override
	{
		return v1 / v2;
	}
};

template<typename T>
struct Abs :UnaryOp
{
	Abs(T&& in) :UnaryOp(forward<T&&>(in)) {}
	inline Ty Evaluate(const Ty& v) const override
	{
		return std::abs(v);
	}
};

template<typename T1, typename T2>
Node operator+(T1&& n1, T2&& n2)
{
	return Node(std::make_shared<Plus<decltype(n1), decltype(n2)>>
		(forward< T1 && >(n1), forward< T2 && >(n2)));
}
template<typename T1, typename T2>
Node operator-(T1&& n1, T2&& n2)
{
	return Node(std::make_shared<Minus<decltype(n1), decltype(n2)>>
		(forward< T1 && >(n1), forward< T2 && >(n2)));
}
template<typename T1, typename T2>
Node operator*(T1&& n1, T2&& n2)
{
	return Node(std::make_shared<Product<decltype(n1), decltype(n2)>>
		(forward< T1 && >(n1), forward< T2 && >(n2)));
}
template<typename T1, typename T2>
Node operator/(T1&& n1, T2&& n2)
{
	return Node(std::make_shared<Divide<decltype(n1), decltype(n2)>>
		(forward< T1 && >(n1), forward< T2 && >(n2)));
}
template<typename T>
Node operator~(T&& in)
{
	return Node(std::make_shared<Abs<decltype(in)>>
		(forward<T&&>(in)));
}


int main()
{
	Node a(1.56), b(-2);
	auto ret1 = 2 + 3 + a + b + 2 + 3;
	auto ret2 = a * b;
	auto ret3 = a - b;
	auto ret4 = a / b;
	auto ret5 = ~b;
	auto ret6 = a + 7;

	cout << ret1.Evaluate() << endl;
	cout << ret2.Evaluate() << endl;
	cout << ret3.Evaluate() << endl;
	cout << ret4.Evaluate() << endl;
	cout << ret5.Evaluate() << endl;
	cout << ret6.Evaluate() << endl;
	return 0;
}