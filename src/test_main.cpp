#include "RB_tree.h"
#include <iostream>

int main() {
    RB_tree tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(5);
    tree.insert(15);
    
    std::cout << tree.distance(6, 25) << std::endl; // должно быть 3 (10, 15, 20)
    return 0;
}
