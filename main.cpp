#include <vector>
#include <random>
#include <chrono>
#include "ArrayGenerator.h"
#include <string>
#include <map>
#include "sort.h"
#include <iostream>
#include <fstream>
class SortTester {
    public:
        SortTester(int num_iterations = 10)
            : iterations(num_iterations) {
        }

        // Метод для замера времени сортировки
        double measureTime(const std::vector<int> &arr, const std::string &sort_type, int threshold = 0) {
            std::vector<int> temp = arr;
            long long total_time = 0;
            for (int i = 0; i < iterations; ++i) {
                temp = arr; // Сброс массива
                auto start = std::chrono::high_resolution_clock::now();
                if (sort_type == "merge") {
                    mergeSort(temp, 0, temp.size() - 1);
                } else if (sort_type == "merge_insertion") {
                    mergeSortHybrid(temp, 0, temp.size() - 1, threshold);
                }
                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                long long msec = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                total_time += msec;
            }
            return static_cast<double>(total_time) / iterations;
        }

    private:
        int iterations;
};


int main(){
    ArrayGenerator generator;
    SortTester tester(10); // 10 итераций для усреднения

    // Определение пороговых значений
    std::vector<int> thresholds = {5, 10, 20, 30, 50};

    // Открытие файлов для записи результатов
    std::ofstream random_file("random_sort_times.csv");
    std::ofstream reverse_file("reverse_sort_times.csv");
    std::ofstream almost_sorted_file("almost_sorted_sort_times.csv");

    // Проверка успешного открытия файлов
    if(!random_file.is_open() || !reverse_file.is_open() || !almost_sorted_file.is_open()){
        std::cerr << "Не удалось открыть один из CSV файлов для записи.\n";
        return 1;
    }

    // Запись заголовков динамически на основе thresholds
    auto write_header = [&](std::ofstream& file) {
        file << "Size,MergeSort(ms)";
        for(auto threshold : thresholds){
            file << ",MergeInsertionSort_Threshold" << threshold << "(ms)";
        }
        file << "\n";
    };

    write_header(random_file);
    write_header(reverse_file);
    write_header(almost_sorted_file);

    // Диапазон размеров массивов
    for(int size = 500; size <= 10000; size += 100){
        // Генерация массивов разных типов
        std::vector<int> random_arr = generator.getArray(size, "random");
        std::vector<int> reverse_arr = generator.getArray(size, "reverse_sorted");
        std::vector<int> almost_sorted_arr = generator.getArray(size, "almost_sorted");

        // Замер времени для стандартного MergeSort
        double time_merge_random = tester.measureTime(random_arr, "merge");
        double time_merge_reverse = tester.measureTime(reverse_arr, "merge");
        double time_merge_almost = tester.measureTime(almost_sorted_arr, "merge");

        // Замер времени для гибридного Merge+InsertionSort с различными порогами
        std::map<int, double> hybrid_times_random;
        std::map<int, double> hybrid_times_reverse;
        std::map<int, double> hybrid_times_almost;

        for(auto threshold : thresholds){
            hybrid_times_random[threshold] = tester.measureTime(random_arr, "merge_insertion", threshold);
            hybrid_times_reverse[threshold] = tester.measureTime(reverse_arr, "merge_insertion", threshold);
            hybrid_times_almost[threshold] = tester.measureTime(almost_sorted_arr, "merge_insertion", threshold);
        }

        // Запись результатов в файлы
        random_file << size << "," << time_merge_random;
        reverse_file << size << "," << time_merge_reverse;
        almost_sorted_file << size << "," << time_merge_almost;

        for(auto threshold : thresholds){
            random_file << "," << hybrid_times_random[threshold];
            reverse_file << "," << hybrid_times_reverse[threshold];
            almost_sorted_file << "," << hybrid_times_almost[threshold];
        }
        random_file << "\n";
        reverse_file << "\n";
        almost_sorted_file << "\n";
    }

    random_file.close();
    reverse_file.close();
    almost_sorted_file.close();

    std::cout << "Done. Results are saved in .csv files.\n";
    return 0;
}
