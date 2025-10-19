#pragma once

#include <utility> 
#include <memory>

class RB_tree {
    
    class Node {

    public:
        enum class Colour { Black, Red };

        int key_;
        std::unique_ptr<Node> left_;
        std::unique_ptr<Node> right_;
        Node* parent_ = nullptr;
        Colour colour_ = Colour::Red;

        explicit Node(int key) : key_(key) {}
    };

    std::unique_ptr<Node> root_;
    

public:

    RB_tree() = default;

    RB_tree(const RB_tree& rhs) : root_(copy_tree_(rhs.root_.get())) {}
    
    RB_tree& operator=(const RB_tree& rhs) {

        if (this != &rhs) {

            RB_tree temp(rhs);
            swap_(temp);  
        }

        return *this;
    }

    RB_tree(RB_tree&& rhs) = default;

    RB_tree& operator=(RB_tree&& rhs) noexcept {

        if (this != &rhs) 
            root_ = std::move(rhs.root_); 
        
        return *this;
    }

    ~RB_tree() = default;

    void insert(int key) {

        if (!root_) {

            root_ = std::make_unique<Node>(key);
            root_->colour_ = Node::Colour::Black; 
            return;
        }
        
        Node* curr_node = root_.get();
        Node* parent = nullptr;
        
        while (curr_node) {

            parent = curr_node;
            
            if (key > curr_node->key_) 
                curr_node = curr_node->right_.get();

            else if (key < curr_node->key_) 
                curr_node = curr_node->left_.get();
            
            else
                return;
        }
        
        auto new_node = std::make_unique<Node>(key);
        new_node->parent_ = parent;
        
        Node* inserted = new_node.get();

        if (key > parent->key_)
            parent->right_ = std::move(new_node);

        else
            parent->left_ = std::move(new_node);
        
        
        Rebalance_(inserted);
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

        while (node != root_.get() && node->parent_->colour_ == Node::Colour::Red) {

            auto parent = node->parent_;
            auto grandparent = parent->parent_;
            if (grandparent->left_.get() == parent) {

                auto uncle = grandparent->right_.get();
                if (uncle && uncle->colour_ == Node::Colour::Red) {

                    parent->colour_ = Node::Colour::Black;
                    uncle->colour_ = Node::Colour::Black;
                    grandparent->colour_ = Node::Colour::Red;
                    node = grandparent;
                }

                else {
                    
                    if (node == parent->right_.get()) {

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

                auto uncle = grandparent->left_.get();

                if (uncle && uncle->colour_ == Node::Colour::Red) {

                    parent->colour_ = Node::Colour::Black;
                    uncle->colour_ = Node::Colour::Black;
                    grandparent->colour_ = Node::Colour::Red;
                    node = grandparent;
                }

                else {

                    if (node == parent->left_.get()) {
                        
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

        if (!node || !node->right_) 
            return;
        
        auto& owner_of_node = unique_ptr_of_(node);
        auto right_child = std::move(node->right_);
        Node* right_child_raw = right_child.get();
        auto left_grandchild = std::move(right_child->left_);

        node->right_ = std::move(left_grandchild);
        if (node->right_) 
            node->right_->parent_ = node;

        right_child->parent_ = node->parent_;
        right_child->left_ = std::move(owner_of_node);
        right_child->left_->parent_ = right_child_raw;

        if (!node->parent_)
            root_ = std::move(right_child);

        else
            owner_of_node = std::move(right_child);
    } 

    void rotate_right_(Node* node) {

        if (!node || !node->right_) 
            return;

        auto& owner_of_node = unique_ptr_of_(node);
        auto left_child = std::move(node->left_);
        Node* left_child_raw = left_child.get();
        auto right_grandchild = std::move(left_child->right_);

        node->left_ = std::move(right_grandchild);
        if (node->left_)
            node->left_->parent_ = node;

        left_child->parent_ = node->parent_;

        left_child->right_ = std::move(owner_of_node);
        left_child->right_->parent_ = left_child_raw;

        if (!node->parent_)
            root_ = std::move(left_child);
        else
            owner_of_node = std::move(left_child);
    }
    std::unique_ptr<Node>& unique_ptr_of_(Node* node) {

        if (!node || !node->parent_)
            return root_;
        
        if (node->parent_->left_.get() == node)
            return node->parent_->left_;

        else
            return node->parent_->right_;
    }

    Node* lower_bound_(int key) {

        Node* lower_bound_node = nullptr;
        auto curr_node = root_.get();
        while (curr_node) {

            if (curr_node->key_ == key)
                return curr_node;

            if (curr_node->key_ > key) {
                
                lower_bound_node = curr_node;
                curr_node = curr_node->left_.get();
            }

            else {
                
                curr_node = curr_node->right_.get();
            }
        }

        return lower_bound_node;
    }

    Node* upper_bound_(int key) {

        Node* upper_bound_node = nullptr;
        auto curr_node = root_.get();
        while (curr_node) {

            if (curr_node->key_ > key) {
                
                upper_bound_node = curr_node;
                curr_node = curr_node->left_.get();
            }

            else {
                
                curr_node = curr_node->right_.get();
            }
        }

        return upper_bound_node;
    }
    
    Node* get_next_node_(Node* node) {

        if (node->right_) {
            return min_node_(node->right_.get());
        }
        
        Node* parent = node->parent_;
        while (parent && node == parent->right_.get()) {

            node = parent;
            parent = parent->parent_;
        }

        return parent;
    }

    Node* min_node_(Node* node) {

        while (node->left_) 
            node = node->left_.get();

        return node;
    }

    std::unique_ptr<Node> copy_tree_(const Node* node, Node* parent = nullptr) {
        
        if (!node)
            return nullptr;

        auto new_node = std::make_unique<Node>(node->key_);
        new_node->colour_ = node->colour_;
        new_node->parent_ = parent;
        new_node->left_ = copy_tree_(node->left_.get(), new_node.get());
        new_node->right_ = copy_tree_(node->right_.get(), new_node.get());

        return new_node;
    }
    
    void swap_(RB_tree& rhs) noexcept {

        std::swap(root_, rhs.root_);
    }

};
