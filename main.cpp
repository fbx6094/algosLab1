#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct Sort {
    long long swaps = 0;
    double time = 0.0;
};

vector<int> readData(const string& filename) {
    vector<int> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return data;
    }

    int value = 0;
    while (file >> value) {
        data.push_back(value);
    }
    file.close();
    return data;
}

vector<int> generateRandom(int n) {
    vector<int> data(n);
    const int max_value = max(10, n / 2);

    for (int i = 0; i < n; ++i) {
        data[i] = rand() % max_value + 1;
    }
    return data;
}

Sort selectionSort(vector<int>& arr) {
    Sort result;
    const auto start = chrono::high_resolution_clock::now();

    const int n = static_cast<int>(arr.size());
    for (int i = 0; i < n - 1; ++i) {
        int min_index = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        if (min_index != i) {
            swap(arr[i], arr[min_index]);
            ++result.swaps;
        }
    }

    const auto end = chrono::high_resolution_clock::now();
    result.time = chrono::duration<double, milli>(end - start).count();
    return result;
}

void quickSortInternal(vector<int>& arr, int left, int right, Sort& result) {
    if (left >= right) {
        return;
    }

    int i = left;
    int j = right;
    const int pivot = arr[(left + right) / 2];

    while (i <= j) {
        while (arr[i] < pivot) {
            ++i;
        }
        while (arr[j] > pivot) {
            --j;
        }

        if (i <= j) {
            if (i != j && arr[i] != arr[j]) {
                swap(arr[i], arr[j]);
                ++result.swaps;
            }
            ++i;
            --j;
        }
    }

    if (left < j) {
        quickSortInternal(arr, left, j, result);
    }
    if (i < right) {
        quickSortInternal(arr, i, right, result);
    }
}

Sort quickSort(vector<int>& arr) {
    Sort result;
    if (arr.empty()) {
        return result;
    }

    const auto start = chrono::high_resolution_clock::now();
    quickSortInternal(arr, 0, static_cast<int>(arr.size()) - 1, result);
    const auto end = chrono::high_resolution_clock::now();
    result.time = chrono::duration<double, milli>(end - start).count();
    return result;
}

Sort insertionSort(vector<int>& arr) {
    Sort result;
    if (arr.empty()) {
        return result;
    }

    const auto start = chrono::high_resolution_clock::now();
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        int j = static_cast<int>(i) - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            ++result.swaps;
            --j;
        }
        arr[j + 1] = key;
    }

    const auto end = chrono::high_resolution_clock::now();
    result.time = chrono::duration<double, milli>(end - start).count();
    return result;
}

void printOut(vector<int>& arr, int limit = 30) {
    cout << "Elements: " << arr.size() << endl;
    const size_t max_items = static_cast<size_t>(limit);
    for (size_t i = 0; i < arr.size() && i < max_items; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    if (arr.size() > static_cast<size_t>(limit)) {
        cout << "... [there are " << arr.size() << " elements]";
    }
    cout << endl;
}

int main() {
    cout << "RANDOM DATA GENERATION" << endl;
    srand(static_cast<unsigned>(time(nullptr)));

    int n = 0;
    cout << "enter the number of elements: ";
    cin >> n;
    if (n <= 0) {
        cout << "n must be positive" << endl;
        return 1;
    }

    vector<int> arr = generateRandom(n);
    vector<int> arr1 = arr;
    const Sort res1 = selectionSort(arr1);
    vector<int> arr2 = arr;
    const Sort res2 = quickSort(arr2);
    vector<int> arr3 = arr;
    const Sort res3 = insertionSort(arr3);

    cout << "---Selection sort---" << endl;
    cout << "Swaps: " << res1.swaps << endl;
    cout << "Time: " << res1.time / 1000.0 << " s" << endl;
    arr1.erase(unique(arr1.begin(), arr1.end()), arr1.end());
    cout << "---Unique elements---" << endl;
    printOut(arr1);

    cout << "---Quick sort---" << endl;
    cout << "Swaps: " << res2.swaps << endl;
    cout << "Time: " << res2.time / 1000.0 << " s" << endl;
    arr2.erase(unique(arr2.begin(), arr2.end()), arr2.end());
    printOut(arr2);

    cout << "---Insertion sort---" << endl;
    cout << "Swaps: " << res3.swaps << endl;
    cout << "Time: " << res3.time / 1000.0 << " s" << endl;
    arr3.erase(unique(arr3.begin(), arr3.end()), arr3.end());
    printOut(arr3);

    cout << "SORTING DATA FROM FILE" << endl;
    vector<int> fileArr = readData("21.txt");
    cout << "total elements from file: " << fileArr.size() << endl;

    vector<int> file_sel = fileArr;
    const Sort selection_file_sort = selectionSort(file_sel);
    vector<int> file_quick = fileArr;
    const Sort quick_file_sort = quickSort(file_quick);
    vector<int> file_ins = fileArr;
    const Sort insertion_file_sort = insertionSort(file_ins);

    cout << "[selection srt]: time = " << selection_file_sort.time / 1000.0
         << " s, swaps = " << selection_file_sort.swaps << endl;
    file_sel.erase(unique(file_sel.begin(), file_sel.end()), file_sel.end());
    printOut(file_sel, 30);

    cout << "[quick sort]:     time = " << quick_file_sort.time / 1000.0
         << " s, swaps = " << quick_file_sort.swaps << endl;
    file_quick.erase(unique(file_quick.begin(), file_quick.end()), file_quick.end());
    printOut(file_quick, 30);

    cout << "[insertion srt]: time = " << insertion_file_sort.time / 1000.0
         << " s, swaps = " << insertion_file_sort.swaps << endl;
    file_ins.erase(unique(file_ins.begin(), file_ins.end()), file_ins.end());
    printOut(file_ins, 30);

    return 0;
}

// MOSCOW 2021
// Elements: 32423
// Selection sort: swaps = 32402, time = 0.441918 s
// Quick sort: swaps = 48974, time = 0.0029175 s
// Unique elements: 58