#include <fstream>
#include <iomanip>
#include "SLE.h"
#include "Analysis.h"
using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");

/*
* --------------------------------------------------------Список методов--------------------------------------------------------
* Решение СЛАУ и поиск собственных значений и векторов:
* --- Прямые методы решения СЛАУ (применяются к объектам типа SLE) ---
* - метод Гаусса: Gauss()
* - метод отражений: HR()
* --- Итерационные методы решения СЛАУ (применяются к объектам типа SLE) ---
* - метод Зейделя: HZ(const double& - точность, const Vector& - начальное приближение)
* - метод Якоби: Jacobi(const double& - точность, const Vector& - начальное приближение)
* - метод сопряжённых градиентов: SGrd(const double& - точность, const Vector& - начальное приближение)
* - трёхчленная формула реализации метода Ричардсона с чебышёвскими коэффициентами: Rchd3(const double& - точность,
* const Vector& - начальное приближение, const double& - левая граница спектра, const double& - правая граница спектра)
* --- Методы поиска собственных значений (применя.тся к объектам типа Matrix)
* - QR-метод: QR(const double& - точность)
* - метод обратных итераций со сдвигом с соотношением Рэлея: RQI(const double& - точность, const double& - приближение с. ч.)
* Численный анализ:
* --- Интерполирование:
* - Интерполяционный многочлен Лагранжа: 
* -- Vector Lagrange_interpolation_builder(const Vector& - узлы, const Vector& - значения в узлах)
* -- double Lagrange_interpolation(const double - нужный x0, const double - левая граница отрезка, const double - правая граница
* отрезка, const Vector& - узлы, const Vector& - значения в узлах)
*/

int main()
{
    fout << fixed << setprecision(8); // установка точности на 8 знаков после запятой

    /*//////////////////////////////////////////////////////////
    * //////////////////////////////////////////////////////////
    * //////////////////////////////////////////////////////////
    * Раскомментировать нужное или написать своё, если требуется
    * //////////////////////////////////////////////////////////
    * //////////////////////////////////////////////////////////
    *///////////////////////////////////////////////////////////

    /*
    * Ввод начальных данных
    */

    //int N; fin >> N; // размерность матрицы или системы

    ////////////////////////////////////////////////////////////

    //double e; fin >> e; // точность вычислений (не для прямых методов)
    //Vector v0(N); fin >> v0; // начальное приближение (для итерационных методов)
    //double alpha, beta; fin >> alpha >> beta; // границы собственных чисел (для метода Ричардсона)
    //double lambda_e; fin >> lambda_e; // начальное приближение с. ч. (для метода обратных итераций)
    //SLE system(N); fin >> system; // для решения систем
    //Matrix A(N); fin >> A; // для поиска собственных значений

    /*
    * Вызов методов и печать их результата
    */

    // Прямые методы
    //fout << system.Gauss(); // метод Гаусса
    //fout << system.HR(); // метод отражений

    // Итерационные методы
    //system.HZ(e, v0); // метод Зейделя
    //system.Jacobi(e, v0); // метод Якоби
    //system.SGrd(e, v0); // метод сопряжённых градиентов
    //system.Rchd3(e, v0, alpha, beta); // метод Ричардсона

    // Методы поиска собственных значений и векторов
    //A.QR(e); // QR-метод
    //A.RQI(e, lambda_e); // метод обратных итераций

    ////////////////////////////////////////////////////////////

    double a, b; fin >> a >> b; // границы отрезка
    double x0; fin >> x0; // икс, в котором нужно найти значение полинома
    size_t N; fin >> N; // кол-во узлов

    // запись узлов

    Vector nodes_x(N), nodes_y(N);
    for (size_t i = 0; i < N; i++)
        fin >> nodes_x[i] >> nodes_y[i];

    /*
    * Вызов метода
    */

    fout << "f(x0) = " << calculatePolynomial(Lagrange_interpolation_builder(nodes_x, nodes_y), x0) << endl << endl; // значение полинома Лагранжа в точке
    printPolynomial(Lagrange_interpolation_builder(nodes_x, nodes_y)); // вывод полинома Лагранжа

    fin.close();
    fout.close();
    return 0;
}
