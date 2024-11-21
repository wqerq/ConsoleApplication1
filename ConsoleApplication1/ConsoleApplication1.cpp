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


int main()
{
    setlocale(LC_ALL, ".ACP");
    double V1[NNN];
    double Time = clock();
    std::cout << "Сравнение последовательной версии и версии на основе concurrency::parallel_for:\n";

    for (int k = 0; k < NNN; k++) {
        V1[k] = My_Task(100 * cos(k));
    }


    // сравнение времени    вычисления сумм
    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время выполнения последовательной версии: " << Time << " сек." << std::endl;

    Time = clock();
    Concurrency::parallel_for(0, NNN, [&V1](size_t k) { V1[k] = My_Task(100 * cos(k)); });

    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время выполнения параллельной версии: " << Time << " сек." << std::endl;



    // сравнение эффективности для vector
    std::vector<double> V(NNN); // Создаём вектор
    for (int k = 0; k < NNN; k++)
        V[k] = 100 * cos(k);
    std::vector<double> VP(V); // Конструируем копию

    std::cout << "\nСравнение std::for_each и parallel_for_each для вектора:\n";

    Time = clock();
    std::for_each(V.begin(), V.end(),
        [](double& x) {x = My_Task(x); });
    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время посл. обр: " << Time << " сек." << std::endl;
    Time = clock();
    concurrency::parallel_for_each(VP.begin(), VP.end(), [](double& x) {x = My_Task(x); });
    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время парал. обр.: " << Time << " сек." << std::endl;

    // сравнение эффективности для deque
    std::deque<double> Dq;
    for (int k = 0; k < NNN; k++)
        Dq.push_back(100 * cos(k));
    std::deque<double> DqP(Dq);

    std::cout << "\nСравнение std::for_each и parallel_for_each для дека:\n";

    Time = clock();
    std::for_each(Dq.begin(), Dq.end(),
        [](double& x) {x = My_Task(x); });
    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время посл. обр: " << Time << " сек." << std::endl;
    Time = clock();
    concurrency::parallel_for_each(DqP.begin(), DqP.end(),
        [](double& x) {x = My_Task(x); });
    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время парал. обр.: " << Time << " сек." << std::endl;

    // сравнение эффективности для list
    std::list<double> Lst;
    for (int k = 0; k < NNN; k++)
        Lst.push_back(100 * cos(k));
    std::list<double> LstP(Lst);

    std::cout << "\nСравнение std::for_each и parallel_for_each для списка:\n";

    Time = clock();
    std::for_each(Lst.begin(), Lst.end(),
        [](double& x) {x = My_Task(x); });
    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время посл. обр: " << Time << " сек." << std::endl;
    Time = clock();
    concurrency::parallel_for_each(LstP.begin(), LstP.end(),
        [](double& x) {x = My_Task(x); });
    Time = (clock() - Time) / CLOCKS_PER_SEC;
    std::cout << "Подзадачи завершены" << std::endl
        << "- Время парал. обр.: " << Time << " сек." << std::endl;


    return 0;
}