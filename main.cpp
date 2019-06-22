#include "Frame.h"
#include <iostream>
//#define RUN_TEST

#include <cmath>
using std::abs;

#ifdef RUN_TEST
const static double x_val = 2.0;

void test_binary()
{
	Var<double> x;
	auto r1 = x + 3.0;
	auto r2 = x + x;
	auto r3 = x + Const<double>(3);
	auto r4 = x - 2.0;
	auto r5 = x * x * x;
	auto r6 = x * 3.0 * x;
	auto r7 = x / 3.0 / x;
	auto r8 = x / x;
	auto r9 = 1.0 / x;

	assert(r1(x_val) == x_val + 3.0);
	assert(r2(x_val) == x_val + x_val);
	assert(r3(x_val) == x_val + 3);
	assert(r4(x_val) == x_val - 2);
	assert(r5(x_val) == x_val * x_val*x_val);
	assert(r6(x_val) == x_val * x_val * 3);
	assert(r7(x_val) == x_val / x_val / 3.0);
	assert(r8(x_val) == x_val / x_val);
	assert(r9(x_val) == 1 / x_val);

	std::cout << "test_binary passed." << std::endl;
}

void test_unary()
{
	Var<double> x;
	auto r1 = -x;
	auto r2 = abs(x);
	auto r3 = abs(-x);
	auto r4 = -abs(-abs(-x));

	assert(r1(x_val) == -x_val);
	assert(r2(x_val) == abs(x_val));
	assert(r3(x_val) == abs(-x_val));
	assert(r4(x_val) == -abs(-abs(-x_val)));

	std::cout << "test_unary passed." << std::endl;
}

void test_combine()
{
	Var<double> x;
	auto r1 = abs(-x) + x / 4.0 * x + 3.0;
	auto r2 = 4.0 - abs(-x) + x / 2.0 - 2.0;
	auto r3 = 5.0 * -abs(-x) + x / 4.0 * (x + 3.0);

	assert(r1(x_val) == abs(-x_val) + x_val / 4.0 * x_val + 3.0);
	assert(r2(x_val) == 4.0 - abs(-x_val) + x_val / 2.0 - 2.0);
	assert(r3(x_val) == 5.0 * -abs(-x_val) + x_val / 4.0 * (x_val + 3.0));

	std::cout << "test_combine passed." << std::endl;
}

void test_extVar()
{
	Var<double> x;
	ExtVar<double, 0> a;
	ExtVar<double, 1> b;
	auto c = abs(-x) + x / 4.0 * x + 3.0 + a - b;

	setVar<decltype(a)>(100);

	setVar<decltype(b)>(100);
	assert(c(x_val) == abs(-x_val) + x_val / 4.0 * x_val + 3.0 + a() - b());

	b.setVal(200);
	assert(c(x_val) == abs(-x_val) + x_val / 4.0 * x_val + 3.0 + a() - b());

	std::cout << "test_extVar passed." << std::endl;
}
#endif

void diy()
{


	//double x = 1, y=0.4;
	//double c = x + y * x - 3;


	Var<double> x;
	ExtVar<double, 0> a;
	ExtVar<double, 1> b;
	auto c = abs(-x) + x / 4.0 * x + 3.0 + a - b;
	auto d = c - c + x;

	setVar<decltype(a)>(100);
	setVar<decltype(b)>(100);

	std::cout << c(4) << std::endl;
	b.setVal(200);
	std::cout << c(4) << std::endl;

	auto r8 = x / x;
	r8(9.0);
}

int main()
{
#ifdef RUN_TEST
	std::cout << "program started." << std::endl;
	test_binary();
	test_unary();
	test_combine();
	test_extVar();
	std::cout << "program ended." << std::endl;
#else
	diy();
#endif

	return 0;
}