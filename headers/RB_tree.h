#pragma once

#include <utility> 
#include <memory>
#include <cassert>

class RB_tree {

    class Node;
    class iterator;
    

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
        
        Node* inserted_node = find_insert_position_(key);
        if (!inserted_node) return; // key already exists
        
        perform_insert_(inserted_node, key);
        Rebalance_(inserted_node);
    }

    int distance(int left_key, int right_key) {

        if (left_key > right_key) 
            return 0;

        auto left_bound_node = upper_bound_(left_key);
        if (!left_bound_node)
            return 0;

        auto right_bound_node = upper_bound_(right_key);
        
        int distance = 0;
        auto current = left_bound_node;
        
        while (current && current != right_bound_node) {

            ++distance;
            ++current;
        }

        return distance;
    }

private:
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

    class iterator {
    private:
        Node* current_;
        RB_tree* tree_;

    public:
        explicit iterator(Node* node = nullptr, RB_tree* tree = nullptr) 
            : current_(node), tree_(tree) {}
        iterator(const iterator& other) = default;
        iterator& operator=(const iterator& other) = default;

        iterator& operator++() {

            if (current_ && tree_) 
                current_ = tree_->get_next_node_(current_);
            
            return *this;
        }

        iterator operator++(int) {

            iterator temp = *this;
            ++(*this);
            return temp;
        }

        int& operator*() const {
            return current_->key_;
        }
        
        Node* operator->() const {
            return current_;
        }
        
        bool operator==(const iterator& other) const {
            return current_ == other.current_;
        }
        
        bool operator!=(const iterator& other) const {
            return current_ != other.current_;
        }
        
        explicit operator bool() const {
            return current_ != nullptr;
        }
    };


    iterator begin() {
        return iterator(min_node_(root_.get()), this);
    }

    iterator end() {
        return iterator(nullptr, this);
    }

    iterator find(int key) {

        Node* current = root_.get();
        while (current) {
            if (key == current->key_) return iterator(current);
            current = key < current->key_ ? current->left_.get() : current->right_.get();
        }
        return end();
    }

    Node* find_insert_position_(int key) {

        Node* curr_node = root_.get();
        Node* parent = nullptr;

        while (curr_node) {

            parent = curr_node;

            if (key > curr_node->key_)
                curr_node = curr_node->right_.get();

            else if (key < curr_node->key_)
                curr_node = curr_node->left_.get();

            else
                return nullptr;
        }
        return parent;
    }

    void perform_insert_(Node* parent, int key) {

        auto new_node = std::make_unique<Node>(key);
        new_node->parent_ = parent;

        if (key > parent->key_)
            parent->right_ = std::move(new_node);
        else
            parent->left_ = std::move(new_node);
    }

    void Rebalance_(Node* node) {

        if (!node || !node->parent_) {

            dye_root_black_();
            return;
        }

        while (node != root_.get() && node->parent_->colour_ == Node::Colour::Red) {

            handle_red_parent_case_(node);
        }
          
        dye_root_black_();
    }

    void dye_root_black_() {

        if (root_)
            root_->colour_ = Node::Colour::Black;
    }

    void handle_red_parent_case_(Node*& node) {

        auto parent = node->parent_;
        auto grandparent = parent->parent_;

        if (!grandparent) return;

        if (grandparent->left_.get() == parent)
            handle_left_parent_case_(node, parent, grandparent);

        else
            handle_right_parent_case_(node, parent, grandparent);
    }

    void handle_left_parent_case_(Node*& node, Node* parent, Node* grandparent) {

        auto uncle = grandparent->right_.get();

        if (uncle && uncle->colour_ == Node::Colour::Red) {

            handle_red_uncle_case_(parent, uncle, grandparent);
            node = grandparent;
        } 

        else {

            handle_black_uncle_left_case_(node, parent, grandparent);
        }
    }

    void handle_right_parent_case_(Node*& node, Node* parent, Node* grandparent) {

        auto uncle = grandparent->left_.get();

        if (uncle && uncle->colour_ == Node::Colour::Red) {

            handle_red_uncle_case_(parent, uncle, grandparent);
            node = grandparent;
        } 

        else {

            handle_black_uncle_right_case_(node, parent, grandparent);
        }
    }

    void handle_red_uncle_case_(Node* parent, Node* uncle, Node* grandparent) {

        parent->colour_ = Node::Colour::Black;
        uncle->colour_ = Node::Colour::Black;
        grandparent->colour_ = Node::Colour::Red;
    }

    void handle_black_uncle_left_case_(Node*& node, Node* parent, Node* grandparent) {

        if (node == parent->right_.get()) {

            node = parent;
            rotate_left_(node);
            parent = node->parent_;
        }

        parent->colour_ = Node::Colour::Black;
        grandparent->colour_ = Node::Colour::Red;
        rotate_right_(grandparent);
    }

    void handle_black_uncle_right_case_(Node*& node, Node* parent, Node* grandparent) {

        if (node == parent->left_.get()) {

            node = parent;
            rotate_right_(node);
            parent = node->parent_;
        }

        parent->colour_ = Node::Colour::Black;
        grandparent->colour_ = Node::Colour::Red;
        rotate_left_(grandparent);
    }

    void rotate_left_(Node* node) {

        if (!node || !node->right_) 
            return;
        
        auto& owner_of_node = unique_ptr_of_(node);
        auto right_child = std::move(node->right_);
        
        if (!right_child) return;
        
        Node* right_child_raw = right_child.get();
        auto left_grandchild = std::move(right_child->left_);

        node->right_ = std::move(left_grandchild);
        if (node->right_) 
            node->right_->parent_ = node;

        right_child->parent_ = node->parent_;
        right_child->left_ = std::move(owner_of_node);
        
        if (right_child->left_)
            right_child->left_->parent_ = right_child_raw;

        if (!node->parent_)
            root_ = std::move(right_child);

        else
            owner_of_node = std::move(right_child);
    } 

    void rotate_right_(Node* node) {

        if (!node || !node->left_) 
            return;

        auto& owner_of_node = unique_ptr_of_(node);
        auto left_child = std::move(node->left_);
        
        if (!left_child) return;
        
        Node* left_child_raw = left_child.get();
        auto right_grandchild = std::move(left_child->right_);

        node->left_ = std::move(right_grandchild);
        if (node->left_)
            node->left_->parent_ = node;

        left_child->parent_ = node->parent_;

        left_child->right_ = std::move(owner_of_node);
        
        if (left_child->right_)
            left_child->right_->parent_ = left_child_raw;

        if (!node->parent_)
            root_ = std::move(left_child);

        else
            owner_of_node = std::move(left_child);
    }
    
    std::unique_ptr<Node>& unique_ptr_of_(Node* node) {

        if (!node || !node->parent_)
            return root_;
        
        Node* parent = node->parent_;
        
        if (parent->left_ && parent->left_.get() == node)
            return parent->left_;
        
        if (parent->right_ && parent->right_.get() == node)
            return parent->right_;
        
        return root_;
    }

    iterator lower_bound_(int key) {

        Node* result = nullptr;
        Node* current = root_.get();
        
        while (current) {

            if (current->key_ >= key) {

                result = current;
                current = current->left_.get();
            } 

            else {

                current = current->right_.get();
            }
        }

        return iterator(result, this);
    }

    iterator upper_bound_(int key) {

        Node* result = nullptr;
        Node* current = root_.get();
        
        while (current) {

            if (current->key_ > key) {

                result = current;
                current = current->left_.get();
            } 

            else {

                current = current->right_.get();
            }
        }

        return iterator(result, this);
    }
    
    Node* get_next_node_(Node* node) {

        if (!node) return nullptr;

        if (node->right_) {

            Node* min_node = min_node_(node->right_.get());
            return min_node ? min_node : nullptr;
        }
        
        Node* current = node;
        Node* parent = node->parent_;
        
        while (parent && current == parent->right_.get()) {

            current = parent;
            parent = parent->parent_;
        }

        return parent;
    }

    Node* min_node_(Node* node) {

        if (!node) return nullptr;

        while (node->left_) {
            
            node = node->left_.get();
            if (!node) break; 
        }

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
