#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

int main() {
    const size_t N = 100000000;  // Размер массива
    std::vector<double> data(N, 1.0);  // Инициализация массива единицами

    // Параллельное вычисление суммы
    double parallel_sum = 0.0;
    auto start = std::chrono::high_resolution_clock::now();
    
    #pragma omp parallel for reduction(+:parallel_sum)
    for(size_t i = 0; i < N; ++i) {
        parallel_sum += data[i];
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> parallel_time = end - start;

    // Последовательное вычисление суммы для сравнения
    double serial_sum = 0.0;
    start = std::chrono::high_resolution_clock::now();
    
    for(size_t i = 0; i < N; ++i) {
        serial_sum += data[i];
    }
    
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> serial_time = end - start;

    // Вывод результатов
    std::cout << "Параллельная сумма: " << parallel_sum 
              << " \tВремя: " << parallel_time.count() << " с\n";
    std::cout << "Последовательная сумма: " << serial_sum 
              << " \tВремя: " << serial_time.count() << " с\n";

    return 0;
}