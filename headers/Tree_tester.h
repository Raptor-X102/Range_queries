#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include "Timer.h"

std::string truncateInput(const std::string& input, size_t max_length = 50) {

    if (input.length() <= max_length)
        return input;
    
    return input.substr(0, max_length) + "...";
}

template<typename Tree>
class TreeTester {
public:
    static std::vector<int> processCommands(Tree& tree, const std::string& input) {

        std::stringstream ss(input);
        char command;
        std::vector<int> results;
        
        while (ss >> command) {

            if (command == 'k') {

                int key;
                ss >> key;
                tree.insert(key);
            } 

            else if (command == 'q') {

                int left, right;
                ss >> left >> right;
                results.push_back(tree.distance(left, right));
            }
        }

        return results;
    }

    static void processAndPrintResults(Tree& tree, const std::string& input) {

        auto results = processCommands(tree, input);
        
        for (size_t i = 0; i < results.size(); ++i) {

            std::cout << results[i];
            if (i != results.size() - 1)
                std::cout << " ";
            
        }

        std::cout << std::endl;
    }
};

template<typename Tree1, typename Tree2>
void compareTreesWithTiming(const std::string& input, const std::string& name1, const std::string& name2) {

    std::cout << "Comparing " << name1 << " vs " << name2 << ":" << std::endl;
    std::cout << "Input: " << truncateInput(input) << std::endl;
    
    Tree1 tree1;
    Tree2 tree2;
    
    std::vector<int> results1, results2;
    
    try {
        {
            std::cerr << name1 << " - ";
            Timer timer;
            results1 = TreeTester<Tree1>::processCommands(tree1, input);
        }
        
        {
            std::cerr << name2 << " - ";
            Timer timer;
            results2 = TreeTester<Tree2>::processCommands(tree2, input);
        }
    } 

    catch (const std::exception& e) {

        std::cerr << "Exception during tree operations: " << e.what() << std::endl;
        return;
    }
    
    std::vector<std::pair<int, int>> queries;
    std::stringstream ss(input);
    char command;
    
    while (ss >> command) {

        if (command == 'q') {

            int left, right;
            if (ss >> left >> right) {

                queries.push_back({left, right});
            }

            else {

                std::cerr << "Warning: Failed to parse query bounds\n";
                ss.clear();
            }
        } 

        else if (command == 'k') {

            int key;
            ss >> key; 
        }
    }
    
    size_t min_results = std::min(results1.size(), results2.size());
    size_t min_queries = std::min(min_results, queries.size());
    
    bool all_match = true;
    for (size_t i = 0; i < min_queries; ++i) {

        std::cout << "Query [" << queries[i].first << ", " << queries[i].second << "]: ";
        std::cout << name1 << " = " << results1[i];
        std::cout << ", " << name2 << " = " << results2[i];
        
        if (results1[i] == results2[i]) {

            std::cout << " ✓ MATCH" << std::endl;
        } 

        else {

            std::cout << " ✗ MISMATCH" << std::endl;
            all_match = false;
        }
    }
    
    if (results1.size() != results2.size()) {

        std::cout << "WARNING: Different number of results: " 
                  << name1 << "=" << results1.size() << ", "
                  << name2 << "=" << results2.size() << std::endl;
        all_match = false;
    }
    
    if (queries.size() != min_results) {

        std::cout << "WARNING: Number of queries (" << queries.size() 
                  << ") doesn't match number of results (" << min_results << ")" << std::endl;
        all_match = false;
    }
    
    std::cout << "Overall: " << (all_match ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}
