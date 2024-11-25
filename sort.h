#include <vector>

// Стандартный Merge Sort
void merge(std::vector<int> &arr, int left, int mid, int right) {
  // Создание времных подмассивов
  int n1 = mid - left + 1;
  int n2 = right - mid;
  std::vector<int> L(n1);
  std::vector<int> R(n2);
  for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
  for (int i = 0; i < n2; ++i) R[i] = arr[mid + 1 + i];

  // Слияние временных массивов обратно в arr[left..right]
  int i = 0, j = 0, k = left;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k++] = L[i++];
    } else {
      arr[k++] = R[j++];
    }
  }
  // Копирование оставшихся элементов
  while (i < n1) arr[k++] = L[i++];
  while (j < n2) arr[k++] = R[j++];
}

void mergeSort(std::vector<int> &arr, int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }
}

// Insertion Sort
void insertionSort(std::vector<int> &arr, int left, int right) {
  for (int i = left + 1; i <= right; ++i) {
    int key = arr[i];
    int j = i - 1;
    while (j >= left && arr[j] > key) {
      arr[j + 1] = arr[j];
      --j;
    }
    arr[j + 1] = key;
  }
}

// Гибридный Merge Sort с переключением на Insertion Sort
void mergeSortHybrid(std::vector<int> &arr, int left, int right, int threshold) {
  if (right - left + 1 <= threshold) {
    insertionSort(arr, left, right);
  } else {
    if (left < right) {
      int mid = left + (right - left) / 2;
      mergeSortHybrid(arr, left, mid, threshold);
      mergeSortHybrid(arr, mid + 1, right, threshold);
      merge(arr, left, mid, right);
    }
  }
}