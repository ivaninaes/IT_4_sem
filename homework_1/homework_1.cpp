// // #1
// #include <iostream>
// #include <string>
// template <typename Cont>
//using namespace std;
// void Print(const Cont& cont, const std::string& sep) {
//     bool first = true;
//     for (const auto& e : cont) {
//         if (!first) {
//             std::cout << sep;
//         }
//         std::cout << e;
//         first = false;
//     }
//     std::cout << "/n";
// }
// // #2
// #include <iostream>
// #include <list>
// #include <vector>
// #include <algorithm>
//
// int main() {
//     std::list<int> inp = {1, -3, 5, -2, 4, 9, -7, 3, -119}; // задали некий список
//     std::vector<int> pos;
//     std::vector<int> neg;
// // разделяем его на положительные и отрицательные числа
//     for (std::list<int>::iterator it = inp.begin(); it != inp.end(); ++it) {
//         if (*it < 0) {
//             neg.push_back(*it);
//         } else if (*it > 0) {
//             pos.push_back(*it);
//         }
//     }
// // сортируем
//     std::sort(pos.begin(), pos.end());
//     std::sort(neg.begin(), neg.end());
// // вывод
//     for (std::vector<int>::iterator it = pos.begin(); it != pos.end(); ++it) {
//         std::cout << *it << " ";
//     }
//     std::cout << std::endl;
//
//     for (std::vector<int>::iterator it = neg.begin(); it != neg.end(); ++it) {
//         std::cout << *it << " ";
//     }
//     std::cout << std::endl;
//
//     return 0;
// }
// // #3
// #include <iostream>
// using namespace std;
// #include <sstream>
// #include <unordered_map>
// #include <algorithm>
// #include <map>
// #include <string>
// #include <cctype>
//
// std::string low(const std::string& str) {
//     std::string result;
//     for (char c : str) {
//         result += std::tolower(c);
//     }
//     return result;
// }
// int main() {
//     std::string inp = "This is a random text with several random words. That is because the program should pass a test!";
//     std::istringstream iss(inp);
//     std::unordered_map<std::string, int> cnt;
//     std::string word;
//     while (iss >> word) {
//         std::string clean;
//         for (char c : word) {
//             if (!std::ispunct(c)) {
//                 clean += c;
//             }
//         }
//         clean = low(clean);
//         ++cnt[clean];
//     }
//     std::map<std::string, int> sorted_cnt;
//     for (auto it = cnt.begin(); it != cnt.end(); ++it) {
//         sorted_cnt[it->first] = it->second;
//     }
//     std::string most_common_word;
//     int max_count = -1;
//     for (auto it = sorted_cnt.begin(); it != sorted_cnt.end(); ++it) {
//         if (it->second > max_count) {
//             most_common_word = it->first;
//             max_count = it->second;
//         }
//     }
//     std::cout << most_common_word << std::endl;
//     return 0;
// }
// // #4
// #include <iostream>
// #include <stack>
// #include <string>
// bool isok(const std::string& s) {
//     std::stack<char> stack;
//     for (char c : s) {
//         if (c == '(' || c == '{' || c == '[') {
//             stack.push(c);
//         }
//         else {
//             if (stack.empty()) {
//                 return false;
//             }
//             char top = stack.top();
//             stack.pop();
//             if ((c == ')' && top != '(') ||
//                 (c == ']' && top != '[') ||
//                 (c == '}' && top != '{')) {
//                 return false;
//             }
//         }
//     }
//     return stack.empty();
// }
// int main() {
//     std::string s;
//     std::cin >> s;
//     if (isok(s)) {
//         std::cout << "ok" << std::endl;
//     }
//     else {
//         std::cout << "not ok" << std::endl;
//     }
//     return 0;
// }
