#include <locale.h>
#include <time.h>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <ppl.h>
#include <iostream>
#include <cmath> 
#define NNN 1200

double TaskOne(int x) {
    double ans = 0;
    int end = std::max(20, 20 * std::abs(x));
    for (int k = 1; k <= end; k++) {
        for (int j = 1; j <= end; j++) {

            double denominator = x * x + k * k  + j * j;
            if (denominator != 0) {
                ans += ((x*x) / denominator) *  cos((k+j) * x);
            }
        }
    }
    return ans;
}

double My_Task(double x) {
    return TaskOne(static_cast<int>(x)); // Приведение к int, если нужно
}


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, ".ACP");
    //Обработка векторов
    std::cout << "Сравнение std::transform и concurrency::parallel_transform для векторов:" << std::endl;
    std::vector<double> V(NNN);
    for (int k = 0; k < NNN; k++)
        V[k] = 100 * cos(k);
    std::vector<double> VP(V);//Конструируем копию
    double Time = clock();
    std::for_each(V.begin(), V.end(), [](double& x) {x = My_Task(x); });
    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время посл. обр: " << Time << " сек." << std::endl;
    Time = clock();
    concurrency::parallel_for_each(VP.begin(), VP.end(), [](double& x) {x = My_Task(x); });
    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время парал. обр.: " << Time << " сек." << std::endl;

    //Обработка деков
    std::cout << "\nСравнение std::transform и concurrency::parallel_transform для деков:" << std::endl;
    std::deque<double> Dq;
    for (int k = 0; k < NNN; k++)
        Dq.push_back(100 * cos(k));
    std::deque<double> DqP(Dq);
    Time = clock();
    std::for_each(Dq.begin(), Dq.end(), [](double& x) {x = My_Task(x); });
    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время посл. обр: " << Time << " сек." << std::endl;
    Time = clock();
    concurrency::parallel_for_each(DqP.begin(), DqP.end(), [](double& x) {x = My_Task(x); });
    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время парал. обр.: " << Time << " сек." << std::endl;

    //Обработка списков
    std::cout << "\nСравнение std::transform и concurrency::parallel_transform для списков:" << std::endl;
    std::list<double> Lst;
    for (int k = 0; k < NNN; k++)
        Lst.push_back(100 * cos(k));
    std::list<double> LstP(Lst);
    Time = clock();
    std::for_each(Lst.begin(), Lst.end(), [](double& x) {x = My_Task(x); });
    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время посл. обр: " << Time << " сек." << std::endl;
    Time = clock();
    concurrency::parallel_for_each(LstP.begin(), LstP.end(), [](double& x) {x = My_Task(x); });
    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время парал. обр.: " << Time << " сек." << std::endl;
}