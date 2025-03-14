#include <iostream>
#include <map>
#include <set>
using namespace std;
int main() {
    std::map<std::string, std::string> parents = {
        {"Alexei", "Peter_I"},
        {"Anna", "Peter_I"},
        {"Elizabeth", "Peter_I"},
        {"Peter_II", "Alexei"},
        {"Peter_III", "Anna"},
        {"Paul_I", "Peter_III"},
        {"Alexander_I", "Paul_I"},
        {"Nicholaus_I", "Paul_I"}
    };
    std:map<std::string, int> heights;
    for (const auto& pair : parents) {
        std::string child = pair.first;
        std::string parent = pair.second;
        if (heights.find(child) == heights.end()) {
            int height = 0;
            std::string cur = child;
            while (parents.find(cur) != parents.end()) {
                cur = parents[cur];
                height += 1;
            }
            heights[child] = height;

        }
    }
    for (const auto& pair : parents) {
        std::string parent = pair.second;
        if (heights.find(parent) == heights.end()) {
            heights[parent] = 0;
        }
    }
    std::set<std::string> names;
    for (const auto& pair : heights) {
        names.insert(pair.first);
    }
    for (const auto& name : names) {
        std::cout << name << " " << heights[name] << std::endl;
    }
    return 0;
}