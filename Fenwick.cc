#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

template<typename T>
class FenwickTree { 
private:
    std::vector<T> tree;
    int m;

    static int lowbit(int x) {
        return x & -x;
    }

    T query(int i) {
        T sum = 0;
        while (i > 0) {
            sum += tree[i];
            i -= lowbit(i);
        }
        return sum;
    }
    
public:
    explicit FenwickTree(int n) : m(n) {
        tree.resize(n + 1, T{});
    }

    void update(int i, T val) {
        while (i <= m) {
            tree[i] += val;
            i += lowbit(i);
        }
    }

    T query (int l, int r) {
        return query(r) - query(l - 1);
    }
};

int main() {
    std::vector<int> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // Example array
    int n = arr.size();
    std::vector<int> prefixSum(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        prefixSum[i] = prefixSum[i - 1] + arr[i - 1];
    }
    FenwickTree<int> ft(n);
    for (int i = 0; i < n; ++i) {
        ft.update(i + 1, arr[i]);
    }
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, n);
    for (int i = 0; i < 10; ++i) {
        int l = dist(rng);
        int r = dist(rng);
        if (l > r) std::swap(l, r);
        int sumFenwick = ft.query(l, r);
        int sumPrefix = prefixSum[r] - prefixSum[l - 1];
        std::cout << "Querying range [" << l << ", " << r << "]: "
                  << "Fenwick Tree Sum = " << sumFenwick
                  << ", Prefix Sum = " << sumPrefix
                  << std::endl;
    }
    return 0;
}