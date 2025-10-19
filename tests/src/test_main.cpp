#include "Tree_tester.h"
#include "RB_tree.h"
#include "Set_tree.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> test_cases = {
        "k 10 k 20 q 8 31 q 6 9 k 30 k 40 q 15 40",
        "k 5 k 3 k 7 k 1 k 4 k 6 k 8 q 2 7",
        "k 1 k 2 k 3 k 4 k 5 q 0 6 q 1 4 q 2 2",
        "k 10 k 20 k 30 q 5 35 q 15 25 q 25 35",
        "q 1 2 k 1 k 2 k 3 q 0 3"
    };

    std::cout << "=== TREE COMPARISON TESTS ===" << std::endl;
    
    for (size_t i = 0; i < test_cases.size(); ++i) {
        std::cout << "\nTest case " << i + 1 << ":\n";
        compareTreesWithTiming<Set_tree, RB_tree>(test_cases[i], "Set_tree", "RB_tree");
    }

    std::string large_test;
    for (int i = 0; i < 1000; ++i) {
        large_test += "k " + std::to_string(i * 2) + " ";
    }
    large_test += "q 100 500 q 200 800 q 50 950";
    
    std::cout << "Large test (1000 insertions + 3 queries):\n";
    compareTreesWithTiming<Set_tree, RB_tree>(large_test, "Set_tree", "RB_tree");

    return 0;
}
