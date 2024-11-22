﻿#define _USE_MATH_DEFINES
#include <locale.h>
#include <time.h>
#include <vector>
#include <deque>
#include <ppl.h>
#include <concurrent_vector.h>
#include <concurrent_queue.h>
#include <iostream>
#include <cmath>
#define N1 400 //Число узлов по каждому направлению
#define N2 50



double Func(double x, double y)
{
	double Tmp = 0;
	for (int k = 1; k <= N2; k++)
		for (int j = 1; j <= N2; j++)
			Tmp += cos(k * x) * sin(j * y) / ((1 + k + j) * sqrt(1 + pow(k, 4) + pow(j, 4)));
	return Tmp;
}

struct Point
{
	double x, y, f;
	Point(double _x, double _y, double _f)
	{
		x = _x; y = _y; f = _f;
	}
};

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, ".ACP");
	std::cout << "Сравнение для векторов:" << std::endl;
	std::vector<Point> Pts;
	double h = 2.0 * M_PI / N1;
	double Time = clock();
	for (int k = 0; k < N1; k++)
		for (int j = 0; j < N1; j++) {
			double x = h * k;
			double y = h * j;
			double f = Func(x, y);
			if (f <= 0)
				Pts.push_back(Point(x, y, f));
		}
	Time = (clock() - Time) / CLOCKS_PER_SEC;
	std::cout << "Вычисления завершены. Размер контейнера: " << Pts.size() << std::endl
		<< "Время : " << Time << " сек." << std::endl;
	Pts.clear();
	concurrency::concurrent_vector<Point> CPts;
	Time = clock();
	concurrency::parallel_for(0, N1,
		[&CPts, h](int k) {
			for (int j = 0; j < N1; j++) {
				double x = h * k;
				double y = h * j;
				double f = Func(x, y);
				if (f >= 0)
					CPts.push_back(Point(x, y, f));
			}}
	);
	Time = (clock() - Time) / CLOCKS_PER_SEC;
	std::cout << "Вычисления завершены. Размер вектора: " << CPts.size() << std::endl
		<< "Время : " << Time << " сек." << std::endl;
	CPts.clear();


	std::cout << std::endl << "Сравнение для очередей:" << std::endl;
	std::deque<Point> qPts;
	h = 2.0 * M_PI / N1;
	for (int k = 0; k < N1; k++)
		for (int j = 0; j < N1; j++) {
			double x = h * k;
			double y = h * j;
			double f = Func(x, y);
			if (f <= 0)
				Pts.push_back(Point(x, y, f));
		}
	Time = (clock() - Time) / CLOCKS_PER_SEC;
	std::cout << "Вычисления завершены. Размер контейнера: " << Pts.size() << std::endl
		<< "Время : " << Time << " сек." << std::endl;
	Pts.clear();
	concurrency::concurrent_queue<Point> CqPts;
	Time = clock();
	concurrency::parallel_for(0, N1,
		[&CqPts, h](int k) {
			for (int j = 0; j < N1; j++) {
				double x = h * k;
				double y = h * j;
				double f = Func(x, y);
				if (f >= 0)
					CqPts.push(Point(x, y, f));
			}}
	);
	Time = (clock() - Time) / CLOCKS_PER_SEC;
	std::cout << "Вычисления завершены. Размер вектора: " << CqPts.unsafe_size() << std::endl
		<< "Время : " << Time << " сек." << std::endl;
	CqPts.clear();
}