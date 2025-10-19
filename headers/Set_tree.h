#pragma once

#include <set>
#include <string>
#include <vector>
#include <iostream>

class Set_tree {
private:
    std::set<int> tree_;

public:
    void insert(int key) {

        tree_.insert(key);
    }

    int distance(int left_key, int right_key) {

        auto start = tree_.upper_bound(left_key);  
        auto end = tree_.upper_bound(right_key);   
        
        return std::distance(start, end);
    }

    void printKeys() const {

        for (int key : tree_) 
            std::cout << key << " ";
        
        std::cout << std::endl;
    }
};

