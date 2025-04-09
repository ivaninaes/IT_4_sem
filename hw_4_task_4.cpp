#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <random>
#include <fstream>

// Структура книги
struct Book {
    int id;
    std::string author;
    std::string title;

    bool operator<(const Book& other) const {
        return id < other.id;
    }
};

// Узел дерева
struct Node {
    Book book;
    Node* parent;
    Node* left;
    Node* right;
    int color; // 0 -> Black, 1 -> Red

    Node(Book b) : book(b), parent(nullptr), left(nullptr), right(nullptr), color(1) {}
};

// Класс дерева
class RedBlackTree {
private:
    Node* root;
    Node* TNULL;

    // Балансировка после вставки
    void fixInsert(Node* k) {
        Node* u;
        while (k->parent->color == 1) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->color == 1) {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right;
                if (u->color == 1) {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = 0;
    }

    // Левый поворот
    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != TNULL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // Правый поворот
    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != TNULL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    // Поиск узла по ключу
    Node* searchTreeHelper(Node* node, int key) const {
        if (node == TNULL || key == node->book.id) {
            return node;
        }

        if (key < node->book.id) {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }

    // Вставка узла
    void insert(Node* node) {
        Node* y = nullptr;
        Node* x = this->root;

        while (x != TNULL) {
            y = x;
            if (node->book < x->book) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        node->parent = y;
        if (y == nullptr) {
            root = node;
        } else if (node->book < y->book) {
            y->left = node;
        } else {
            y->right = node;
        }

        if (node->parent == nullptr) {
            node->color = 0;
            return;
        }

        if (node->parent->parent == nullptr) {
            return;
        }

        fixInsert(node);
    }

    // Обход дерева в порядке
    void inorderHelper(Node* node, std::ostream& out) const {
        if (node != TNULL) {
            inorderHelper(node->left, out);
            out << "ID: " << node->book.id << ", Author: " << node->book.author << ", Title: " << node->book.title << "\n";
            inorderHelper(node->right, out);
        }
    }

public:
    RedBlackTree() {
        TNULL = new Node(Book{0, "", ""});
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }

    // Поиск
    bool search(int k) const {
        return searchTreeHelper(this->root, k) != TNULL;
    }

    // Вставка
    void insert(const Book& key) {
        Node* node = new Node(key);
        node->parent = nullptr;
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1;

        insert(node);
    }

    // Вывод в файл
    void printTree(const std::string& filename) const {
        std::ofstream file(filename);
        inorderHelper(this->root, file);
        file.close();
    }
};

// Генерация случайных данных
std::vector<Book> generateRandomBooks(int count) {
    std::vector<Book> books;
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 10000);

    for (int i = 0; i < count; ++i) {
        books.push_back(Book{dist(rng), "Author" + std::to_string(i), "Title" + std::to_string(i)});
    }

    return books;
}

// Тестируем и замеряем
int main() {
    const int numBooks = 10000;
    auto books = generateRandomBooks(numBooks);

    RedBlackTree libraryRBTree;
    std::map<int, Book> libraryMap;

    // Вставка и замер времени
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& book : books) {
        libraryRBTree.insert(book);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> rbTreeInsertTime = end - start;

    start = std::chrono::high_resolution_clock::now();
    for (const auto& book : books) {
        libraryMap[book.id] = book;
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> mapInsertTime = end - start;

    // Поиск и замер времени
    start = std::chrono::high_resolution_clock::now();
    for (const auto& book : books) {
        libraryRBTree.search(book.id);
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> rbTreeFindTime = end - start;

    start = std::chrono::high_resolution_clock::now();
    for (const auto& book : books) {
        libraryMap.find(book.id);
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> mapFindTime = end - start;

    // Вывод результатов
    std::cout << "RB Tree Insert Time: " << rbTreeInsertTime.count() << " seconds\n";
    std::cout << "Map Insert Time: " << mapInsertTime.count() << " seconds\n";
    std::cout << "RB Tree Find Time: " << rbTreeFindTime.count() << " seconds\n";
    std::cout << "Map Find Time: " << mapFindTime.count() << " seconds\n";

    // Вывод книг в файл
    libraryRBTree.printTree("books_sorted.txt");

    return 0;
}
