#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <omp.h>
#include <list>
#include <utility>
#include <exception>
#include <math.h>

using namespace std;

struct Result
{
	double timestamp, area;
};

double f(const double x);
const Result rectangleMethod(const double, const double, const double);
const Result trapezoidalMethod(const double, const double, const double);

int main()
{
	const short steps = 4;
	short method;
	double x1, x2, dx;

	cout << fixed << setprecision(8) << endl;
	try
	{
		while (true)
		{
			cout << "   X1: "; cin >> x1;
			cout << "   X2: "; cin >> x2;
			cout << "   dx: "; cin >> dx;
			cout << "   Method (1 - rectangle, 2 - trapezoidal): "; cin >> method;

			list<pair<short, Result>> results;
			for (int i = 0; i < steps; i++)
			{
				Result result = (method == 1) ?
					rectangleMethod(x1, x2, dx) :
					trapezoidalMethod(x1, x2, dx);

				pair<short, Result> s_result(i + 1, result);
				results.push_back(s_result);
			}

			cout << endl << "   Results:" << endl;
			for (auto & result : results)
			{
				cout << "   Step: " << result.first;
				cout << ", timestamp: " << result.second.timestamp;
				cout << ", area: " << result.second.area << endl;
			}
			cout << endl;
		}
	}
	catch (exception & e)
	{
		cout << e.what() << endl;
	}
	cin.get();
	return 0;
}

const Result rectangleMethod(const double x1, const double x2, const double dx)
{
	const int N = static_cast<int>((x2 - x1) / dx);
	double now = omp_get_wtime();
	double s = 0;

	for (int i = 1; i <= N; i++) s += f(x1 + i * dx);

	s *= dx;
	 
	return { omp_get_wtime() - now, s };
}

const Result trapezoidalMethod(const double x1, const double x2, const double dx)
{
	const int N = static_cast<int>((x2 - x1) / dx);
	double now = omp_get_wtime();
	double s = 0;

	for (int i = 1; i < N; i++) s += f(x1 + i * dx);

	s = (s + (f(x1) + f(x2)) / 2) * dx;
	 
	return { omp_get_wtime() - now, s };
}

double f(const double x)
{
	return sin(x);
}
