#include "RB_tree.h"
#include "Tree_tester.h"
#include <iostream>
#include <string>

int main() {

    RB_tree tree;
    std::string input;

    std::getline(std::cin, input);

    TreeTester<RB_tree>::processAndPrintResults(tree, input);
    
    return 0;
}
