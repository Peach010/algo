#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>
#include <ranges>

template <typename T>
struct Node {
    T val;
    T t;
    T mx;

    Node(T v = T{}, T s = T{}, T m = T{}) : val(v), t(s), mx(m) {}
    void print(){};
};

template <typename T>
class SegmentTree {
private:
    int n;
    std::vector<Node<T>> tree;

    void build(int node, int start, int end, const std::vector<T>& arr) {
        if (start > end) {
            return;
        }
        if (start == end) {
            tree[node].val = arr[start];
            tree[node].mx = arr[start];
        } else {
            int mid = (start + end) / 2;
            int node_l = 2 * node, node_r = 2 * node + 1;
            build(node_l, start, mid, arr);
            build(node_r, mid + 1, end, arr);
            tree[node].val = tree[node_l].val + tree[node_r].val;
            tree[node].mx = std::max(tree[node_l].mx, tree[node_r].mx);
        }
    }

    void update(int node, int l, int r, int start, int end, T val) {
        if (l > end || r < start) {
            return;
        }
        if (l >= start && r <= end) {
            tree[node].val += (r - l + 1) * val;
            tree[node].t += val;
            tree[node].mx += val;
        } else {
            int mid = l + (r - l) / 2;
            int node_l = node * 2, node_r = node * 2 + 1;
            if(tree[node].t && l != r) {
                T x = tree[node].t;
                tree[node].t = 0;
                tree[node_l].val += (mid - l + 1) * x;
                tree[node_l].mx += x;
                tree[node_l].t += x;
                tree[node_r].val += (r - mid) * x;
                tree[node_r].mx += x;
                tree[node_r].t += x;
                tree[node].mx = std::max(tree[node_l].mx, tree[node_r].mx);
            } 
            update(node_l, l, mid, start, end, val);
            update(node_r, mid + 1, r, start, end, val);
            tree[node].val = tree[node_l].val + tree[node_r].val;
            tree[node].mx = std::max(tree[node_l].mx, tree[node_r].mx);
        }
    }

    Node<T> query(int node, int l, int r, int start, int end) {
        if (l > end || r < start) {
            return Node<T>(0, 0, std::numeric_limits<T>::min());
        }
        if (l >= start && r <= end) {
            return tree[node];
        }
        int mid = l + (r - l) / 2;
        int node_l = node << 1, node_r = (node << 1) + 1;
        if (tree[node].t) {
            T x = tree[node].t;
            tree[node].t = 0;
            tree[node_l].val += (mid - l + 1) * x;
            tree[node_l].mx += x;
            tree[node_l].t += x;
            tree[node_r].val += (r - mid) * x;
            tree[node_r].mx += x;
            tree[node_r].t += x;
            tree[node].mx = std::max(tree[node_l].mx, tree[node_r].mx);
        }
        Node<T> l_node = query(node_l, l, mid, start, end);
        Node<T> r_node = query(node_r, mid + 1, r, start, end);
        return Node<T>(l_node.val + r_node.val, 0, std::max(l_node.mx, r_node.mx));
    }
    
public:
    explicit SegmentTree(const std::vector<T>& arr) {
        n = static_cast<int>(arr.size());
        tree.resize(4 * n + 5, Node<T>{});
        build(1, 0, n - 1, arr);
    }

    void update(int start, int end, T val) {
        if (start <= end && start >= 0 && end < n) {
            update(1, 0, n - 1, start, end, val);
        }
    }

    Node<T> query(int start, int end) {
        if (start > end || start < 0 || end >= n) {
            return Node<T>{};
        }
        return query(1, 0, n - 1, start, end);
    }


};

template<typename T>
void printArr(std::vector<T> &arr) {
    for (const auto &t : arr) {
        std::cout << t << " ";
    } 
    std::cout << std::endl;
}

void print() {
    std::cout << std::endl;
}

template<typename T, typename... Args>
void print(T t, Args... args) {
    std::cout << t;
    print(args...);
}

int main() {
    std::vector<int> arr(500);
    int n = static_cast<int>(arr.size());
    std::random_device rd{};
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, n - 1);
    for (int i = 0; i < n; i++) {
        arr[i] = dist(gen);
    }
    std::cout << "arr: ";
    printArr(arr);
    std::vector<int> preSum(n + 1, 0);
    for (int i = 0; i < n; i++) {
        preSum[i + 1] = preSum[i] + arr[i];
    }
    std::cout << "pre_sum: ";
    printArr(preSum);


    SegmentTree<int> st(arr);

    for (int i = 0; i < 1e3; i++) {
        int l = dist(gen);
        int r = dist(gen);
        int mod_val = dist(gen);
        if (l > r) {
            std::swap(l, r);
        }
        // int pre_range_res = preSum[r + 1] - preSum[l];
        // print("pre_sum[", l, ", ", r, "]: sum = ", pre_range_res, 
        //         ", mx = ", *std::max_element(arr.begin() + l, arr.begin() + r + 1));
        int sum = 0, mx = INT_MIN;
        for (int i = l; i <= r; i++) {
            arr[i] += mod_val;
            sum += arr[i];
            mx = std::max(mx, arr[i]);
        }
        // printArr(arr);
        std::cout << "pre_sum[" << l << ", " << r << "]: sum = " << sum
                    << ", mx = " << mx << std::endl;
        st.update(l, r, mod_val);
        
        auto node = st.query(l, r);
        std::cout << "segment[" << l << ", " << r << "]: sum = " << node.val 
                    << ", mx = " << node.mx << std::endl;
        if (sum != node.val || mx != node.mx) {
            std::cout << "sum != node.val || mx != node.mx" << std::endl;
            break;
        }
    }


}