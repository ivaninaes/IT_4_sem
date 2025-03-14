#include <iostream>
#include <vector>
#include <list>

using namespace std;

class Hashtable {
private:
    std::vector<std::list<int>> table;
    int size;

    int hash_function(int key) {
        int square = key * key;
        std::string str1 = std::to_string(square);
        char str2 = str1[(str1.length()/2)] + str1[str1.length()/2 + 1];
        return (str2 - '0')%size;
    }
public:
    explicit Hashtable(int tablesize) : size(tablesize) {
        table.resize(size);
    }
    void insert(int key) {
        int index = hash_function(key);
        table[index].push_back(key);
    }
    bool search(int key) {
        int index = hash_function(key);
        for (int el : table[index]) {
            if (el == key) {
                return true;
            }
        }
        return false;
    }
    void show() {
        for (int i = 0; i < size; ++i) {
            std::cout << "index " << i << ": ";
            for (int el : table[i]) {
                std::cout << el << " ";
            }
        }
    }
};
int main() {
    Hashtable ahaha(10);
    ahaha.insert(0);
    ahaha.insert(123);
    ahaha.insert(345);
    ahaha.insert(456);
    ahaha.insert(789);
    ahaha.insert(999);
    ahaha.insert(9999 );
    ahaha.show();
    return 0;
}
