#include <functional>
#include <iostream>
//#include <omp.h>
#include <string>
#include <vector>
#include "omp.h"
#include <cmath>


const double STEP = 0.01;//Шаг
double a;//Нижняя граница
double b;//Верхняя граница

//Функция
double fTest(double x) {
    return x * x;
    // return sin(x);
    // return 0.5 * x;
}

double rectIntegral(
        const std::function<double(double)> &fun,
        const double a, const double b, const int n) {
    double h = std::abs((b - a) / n);
    double sum = 0;

    ///

#pragma omp parallel reduction (+: sum)
    {
#pragma omp for
        //Считаем интеграл
        for (int i = 0; i < n; ++i) {
            sum += fun(a + i * h) * h;
        }
#pragma omp critical
        //Критическая секция вывод текущей посчитанной суммы
        {
            std::cout << omp_get_thread_num() << ": sum = " << sum << "\n";
        }
    }  // pragma omp parallel
    return sum;
}

///Считывание одной границы интегрирования [double]
double getDoubleValue(const std::string &mes) {
    double k;//Граница
    std::string str;//Вводимая строка
    while (true) // цикл продолжается до тех пор, пока пользователь не введет корректное значение
    {
        try {
            std::cout << mes;//Сообщение для пользователя
            std::cin >> str;
            k = std::stod(str);//ПЕревод string -> double (либо эксепшн)

            return k;
        } catch (std::exception e) {
            std::cout << e.what();
        }
    }
}

///Считывание пределов интегрирования a b
void ReadAB() {

    a = getDoubleValue("Input double a:\n");
    b = getDoubleValue("Input double b:\n");

    //В случае некорректного порялка меняем местами границы
    if (a > b) {
        std::swap(a, b);
    }
}


int main() {

//ВВод границ
    ReadAB();
//    a = 2;
//    b = 4;

    //a=-10;
    //b=0;

    //Максимальное число потоков
    int max = static_cast<int>(abs((a + b)) / STEP ) + 1;
    auto integralValie = rectIntegral(fTest, a, b, max);
    std::cout << "Integral value = " << integralValie << "\n";


    return 0;
}