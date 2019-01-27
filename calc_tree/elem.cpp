#include "Timer.h"
#include "elem.h"
#include <iostream>
//#include <vld.h>
using namespace std;

int main()
{
	Timer<chrono::nanoseconds> timer;

	timer.StartTimer();
	{
		Node a(1.56), b(-2);
		Node n1 = a + b + 3;
		Node n2 = exp(a) * n1 - abs(b);
		Node n3 = n1 / n2; 
	
		timer.EndTimer("build time");
		timer.StartTimer();
	
		cout << n3.Evaluate() << endl;
	}
	timer.EndTimer("evaluate time");
	return 0;
}