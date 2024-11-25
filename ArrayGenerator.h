#include <algorithm>
#include <functional>
#include <random>
#include <sstream>
#include <string>
#include <vector>

class ArrayGenerator {
    public:
        // Конструктор, инициализирующий массив максимальной длины
        ArrayGenerator(int max_size = 10000, int value_range = 6000)
            : max_size(max_size), value_range(value_range) {
            generateRandomArray();
        }

        // Метод для получения подмассива заданного размера и типа
        std::vector<int> getArray(int size, const std::string &type) {
            std::vector<int> subarray;
            if (size > max_size) size = max_size;
            switch (type_enum(type)) {
                case ARRAY_TYPE::RANDOM:
                    subarray.assign(random_array.begin(), random_array.begin() + size);
                    break;
                case ARRAY_TYPE::REVERSE_SORTED:
                    subarray.assign(reverse_sorted_array.begin(), reverse_sorted_array.begin() + size);
                    break;
                case ARRAY_TYPE::ALMOST_SORTED:
                    subarray.assign(almost_sorted_array.begin(), almost_sorted_array.begin() + size);
                    break;
                default:
                    subarray.assign(random_array.begin(), random_array.begin() + size);
                    break;
            }
            return subarray;
        }

    private:
        enum class ARRAY_TYPE { RANDOM, REVERSE_SORTED, ALMOST_SORTED };

        int max_size;
        int value_range;
        std::vector<int> random_array;
        std::vector<int> reverse_sorted_array;
        std::vector<int> almost_sorted_array;

        // Генерация случайного массива
        void generateRandomArray() {
            random_array.reserve(max_size);
            std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<int> dist(0, value_range);
            for (int i = 0; i < max_size; ++i) {
                random_array.push_back(dist(rng));
            }

            // Создание обратно отсортированного массива
            reverse_sorted_array = random_array;
            std::sort(reverse_sorted_array.begin(), reverse_sorted_array.end(), std::greater<int>());

            // Создание почти отсортированного массива
            almost_sorted_array = random_array;
            std::sort(almost_sorted_array.begin(), almost_sorted_array.end());
            // Выполнение небольшого количества обменов (например, 1% от размера)
            int swaps = max_size / 100;
            for (int i = 0; i < swaps; ++i) {
                std::swap(almost_sorted_array[i], almost_sorted_array[max_size - i - 1]);
            }
        }

        // Преобразование строки в ENUM
        ARRAY_TYPE type_enum(const std::string &type) {
            if (type == "random") return ARRAY_TYPE::RANDOM;
            if (type == "reverse_sorted") return ARRAY_TYPE::REVERSE_SORTED;
            if (type == "almost_sorted") return ARRAY_TYPE::ALMOST_SORTED;
            return ARRAY_TYPE::RANDOM;
        }
};
