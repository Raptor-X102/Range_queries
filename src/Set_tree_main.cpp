#include "Set_tree.h"
#include "Tree_tester.h"
#include <iostream>
#include <string>

int main() {

    Set_tree tree;
    std::string input;

    std::getline(std::cin, input);

    TreeTester<Set_tree>::processAndPrintResults(tree, input);
    
    return 0;
}
