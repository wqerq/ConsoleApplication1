#include <agents.h>
#include <cmath>
#include <iostream>
#include <locale.h>
#include <time.h>

using namespace concurrency;
using namespace std;

int const N = 50; //Размерность рядов
int const NNN = 500; //Длина массива на входе конвейера 
int const Chunk = 100; //Порция обрабатываемых элементов

double Func1(double x)//Первая ступень конвейера
{
	double totalSum = 0;
	for (int n = 0; n <= N; ++n) {
		for (int k = 0; k <= N; ++k) {
			for (int j = 0; j <= N; ++j) {
				double denominator = 1 + sqrt(pow(x, 2) + pow(n, 4)) + pow(k, 2) + pow(j, 3);
				if (denominator != 0) {  // Проверка на деление на ноль
					totalSum += (x + j) / denominator;
				}
			}
		}
	}
	return totalSum;
}

double Func2(double x)//Вторая ступень конвейера
{
	double totalSum = 0;
	for (int n = 0; n <= N; ++n) {
		for (int k = 0; k <= N; ++k) {
			for (int j = 0; j <= N; ++j) {
				double numerator = pow(x, 2) - k;
				double denominator = 1 + pow(x, 2) + pow(n, 3) + pow(k, 3) + pow(j, 2);

				if (denominator != 0) {  // Проверка на деление на ноль
					totalSum += numerator / denominator;
				}
			}
		}
	}
	return totalSum;
}

class Stage_Agent : public agent
{
private:
	// Буфер - источник информации
	ISource<double>& _source;
	// Буфер - приемник информации
	ITarget<double>& _target;
	//Функция для обработки
	double(*_func) (double);
public:
	Stage_Agent(ISource<double>& source, ITarget<double>& target, double func(double))
		: _target(target), _source(source)
	{
		_func = func;
	}
protected:
	void run()
	{
		while (true) {
			asend(_target, _func(receive(_source)));
		}

	}

};

int main()
{
	setlocale(LC_ALL, ".ACP");
	cout << "Конвейер на основе блоков сообщений и программных агентов:" << endl;
	vector<double> X(NNN), Y(NNN), Z(NNN);
	//Заполнение входного вектора
	for (int k = 0; k < X.size(); k++)
		X[k] = 10 * sin((double)k);
	double Tms = clock();
	//Последовательные вычисления
	for (int k = 0; k < X.size(); k++) {
		Y[k] = Func1(X[k]);
		Y[k] = Func2(Y[k]);
	}
	Tms = (clock() - Tms) / CLOCKS_PER_SEC;
	cout << "- Время последовательного алгоритма: " << Tms << " c." << endl;
	unbounded_buffer<double> buf0, buf1, buf2;
	//Первая ступень конвейера	
	Stage_Agent Stage_Agent1(buf0, buf1, Func1);
	//Вторая ступень конвейера
	Stage_Agent Stage_Agent2(buf1, buf2, Func2);
	Stage_Agent1.start();
	Stage_Agent2.start();

	Tms = clock();
	//Параллельные вычисления
	for (int k = 0; k < Chunk; k++)
		send(buf0, X[k]);

	for (int k = Chunk; k < X.size(); k++) {
		Z[k - Chunk] = receive(buf2);
		send(buf0, X[k]);
	}
	for (int k = X.size() - Chunk; k < X.size(); k++) {
		Z[k] = receive(buf2);
	}
	Tms = (clock() - Tms) / CLOCKS_PER_SEC;
	cout << "- Время параллельного алгоритма: " << Tms << " c." << endl;
}
