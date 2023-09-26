#include <iostream>
#include <vector>
#include <cmath>

int binarySearch(const std::vector<double>& arr, double target) {
    int low = 0;
    int high = arr.size() - 1;
    int closestIndex = -1; // 保存最接近目标值的索引

    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        if (arr[mid] == target) {
            return mid;
        }
        
        if (closestIndex == -1 || std::abs(arr[mid] - target) < std::abs(arr[closestIndex] - target)) {
            closestIndex = mid;
        }
        
        if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    
    return closestIndex;
}

int main() {
    std::vector<double> arr = {1.1, 2.2, 3.3, 4.4, 5.5};
    double target = 3.4;
    int closestIndex = binarySearch(arr, target);
    
    std::cout << closestIndex << std::endl;
    
    return 0;
}
