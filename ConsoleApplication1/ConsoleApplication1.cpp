#include <locale.h>
#include <time.h>
#include <vector>
#include <complex>
#include <algorithm>
#include <ppl.h>
#include <iostream>
#define NNN 20000000

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, ".ACP");
	vector<double> V0(NNN);
	for (int k = 0; k < NNN; k++)
		V0[k] = cos(k*sqrt(k)/(1+sqrt(k)));
	vector<double> V1(V0);//Конструируем копию

	double Time = clock();
	sort(V0.begin(), V0.end(),
		[](const double& left, const double& right) {
			return left < right; });
	Time = (clock() - Time) / CLOCKS_PER_SEC;
	cout << "Сортировка sort завершена" << endl
		<< "Время посл. сортировки: " << Time << " сек." << endl;

	V0 = V1;
	Time = clock();
	concurrency::parallel_sort(V0.begin(), V0.end(),
		[](const double& left, const double& right) {
			return left < right; });
	Time = (clock() - Time) / CLOCKS_PER_SEC;
	cout << "\nСортировка parallel_sort завершена" << endl
		<< "Время парал. сортировки: " << Time << " сек." << endl;

	V0 = V1;
	Time = clock();
	concurrency::parallel_buffered_sort(V0.begin(), V0.end(),
		[](const double& left, const double& right) {
			return left < right; });
	Time = (clock() - Time) / CLOCKS_PER_SEC;
	cout << "\nСортировка parallel_buffered_sort завершена" << endl
		<< "Время парал. сортировки: " << Time << " сек." << endl;
}