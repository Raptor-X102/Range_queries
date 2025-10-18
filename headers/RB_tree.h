#pragma once

#include <utility> 

class RB_tree {
    
    class Node {

    public:
        enum class Colour { Black, Red };

        int key_;
        Node* left_ = nullptr;
        Node* right_ = nullptr;
        Node* parent_ = nullptr;
        Colour colour_ = Colour::Red;

        explicit Node(int key) : key_(key) {}
    };

    Node* root_ = nullptr;
    

public:

    RB_tree() = default;

    RB_tree(const RB_tree& rhs) : root_(copy_tree_(rhs.root_)) {}
    
    RB_tree& operator=(const RB_tree& rhs) {

        if (this != &rhs) {

            RB_tree temp(rhs);
            swap_(temp);  
        }

        return *this;
    }

    RB_tree(RB_tree&& other) noexcept : root_(other.root_) {
        other.root_ = nullptr;
    }

    RB_tree& operator=(RB_tree&& other) noexcept {

        if (this != &other) {

            destroy_(root_);           
            root_ = other.root_;
            other.root_ = nullptr;
        }

        return *this;
    }

    ~RB_tree() {

        destroy_(root_);
    }

    void insert(int key) {

        if (!root_) {

            root_ = new Node(key);
            root_->colour_ = Node::Colour::Black; 
            return;
        }
        
        Node* curr_node = root_;
        Node* parent = nullptr;
        
        while (curr_node) {

            parent = curr_node;
            
            if (key > curr_node->key_) 
                curr_node = curr_node->right_;

            else if (key < curr_node->key_) 
                curr_node = curr_node->left_;
            
            else
                return;
        }
        
        auto new_node = new Node(key);
        new_node->parent_ = parent;
        
        if (key > parent->key_)
            parent->right_ = new_node;

        else
            parent->left_ = new_node;
        
        
        Rebalance_(new_node);
    }

    int distance(int left_key, int right_key) {

        auto left_bound_node = upper_bound_(left_key);
        if (!left_bound_node)
            return 0;

        auto right_bound_node = upper_bound_(right_key);
        int distance = 0;
        while (left_bound_node != right_bound_node) {

            distance++;
            left_bound_node = get_next_node_(left_bound_node);
        }

        return distance;
    }

private:
    void Rebalance_(Node* node) {

        if (!node || !node->parent_) {
            if (root_) root_->colour_ = Node::Colour::Black;
            return;
        }

        while (node != root_ && node->parent_->colour_ == Node::Colour::Red) {

            auto parent = node->parent_;
            auto grandparent = parent->parent_;
            if (grandparent->left_ == parent) {

                auto uncle = grandparent->right_;
                if (uncle && uncle->colour_ == Node::Colour::Red) {

                    parent->colour_ = Node::Colour::Black;
                    uncle->colour_ = Node::Colour::Black;
                    grandparent->colour_ = Node::Colour::Red;
                    node = grandparent;
                }

                else {
                    
                    if (node == parent->right_) {

                        node = parent;
                        rotate_left_(node);
                        parent = node->parent_;
                    }
                    
                    parent->colour_ = Node::Colour::Black;
                    grandparent->colour_ = Node::Colour::Red;
                    rotate_right_(grandparent);
                }
            }

            else {

                auto uncle = grandparent->left_;

                if (uncle && uncle->colour_ == Node::Colour::Red) {

                    parent->colour_ = Node::Colour::Black;
                    uncle->colour_ = Node::Colour::Black;
                    grandparent->colour_ = Node::Colour::Red;
                    node = grandparent;
                }

                else {

                    if (node == parent->left_) {
                        
                        node = parent;
                        rotate_right_(node);
                        parent = node->parent_;
                    }

                    parent->colour_ = Node::Colour::Black;
                    grandparent->colour_ = Node::Colour::Red;
                    rotate_left_(grandparent);
                }
            }
        }

        root_->colour_ = Node::Colour::Black;
    }

    void rotate_left_(Node* node) {

       auto right_child = node->right_;  
       auto left_grandchild = right_child->left_; 
       node->right_ = left_grandchild;
       if (left_grandchild)
           left_grandchild->parent_ = node;

       right_child->parent_ = node->parent_;

       if (!node->parent_)
           root_ = right_child;

       else {

            if (node == node->parent_->left_)
                node->parent_->left_ = right_child;

            else
                node->parent_->right_ = right_child;
       }

       right_child->left_ = node;
       node->parent_ = right_child;
    }

    void rotate_right_(Node* node) {

       auto left_child = node->left_;  
       auto right_grandchild = left_child->right_; 
       node->left_ = right_grandchild;
       if (right_grandchild)
           right_grandchild->parent_ = node;

       left_child->parent_ = node->parent_;

       if (!node->parent_)
           root_ = left_child;

       else {

            if (node == node->parent_->left_)
                node->parent_->left_ = left_child;

            else
                node->parent_->right_ = left_child;
       }

       left_child->right_ = node;
       node->parent_ = left_child;
    }

    Node* lower_bound_(int key) {

        Node* lower_bound_node = nullptr;
        auto curr_node = root_;
        while (curr_node) {

            if (curr_node->key_ == key)
                return curr_node;

            if (curr_node->key_ > key) {
                
                lower_bound_node = curr_node;
                curr_node = curr_node->left_;
            }

            else {
                
                curr_node = curr_node->right_;
            }
        }

        return lower_bound_node;
    }

    Node* upper_bound_(int key) {

        Node* upper_bound_node = nullptr;
        auto curr_node = root_;
        while (curr_node) {

            if (curr_node->key_ > key) {
                
                upper_bound_node = curr_node;
                curr_node = curr_node->left_;
            }

            else {
                
                curr_node = curr_node->right_;
            }
        }

        return upper_bound_node;
    }
    
    Node* get_next_node_(Node* node) {

        if (node->right_) {
            return min_node_(node->right_);
        }
        
        Node* parent = node->parent_;
        while (parent && node == parent->right_) {

            node = parent;
            parent = parent->parent_;
        }

        return parent;
    }

    Node* min_node_(Node* node) {

        while (node->left_) 
            node = node->left_;

        return node;
    }

    Node* copy_tree_(Node* node, Node* parent = nullptr) {
        
        if (!node)
            return nullptr;

        Node* new_node = new Node(node->key_);
        new_node->colour_ = node->colour_;
        new_node->parent_ = parent;
        new_node->left_ = copy_tree_(node->left_, new_node);
        new_node->right_ = copy_tree_(node->right_, new_node);

        return new_node;
    }
    
    void swap_(RB_tree& other) noexcept {

        std::swap(root_, other.root_);
    }

    void destroy_(Node* node) {
        
        if (!node) 
            return;

        destroy_(node->left_);
        destroy_(node->right_);
        delete node;
    }
};
