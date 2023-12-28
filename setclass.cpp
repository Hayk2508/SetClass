#include <iostream>
#include <cassert>
template<typename T>
class Set {
    struct Node {
        T value;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
        int height;
        int balanceFactor;
        // Functions can be added if necessary
    };

    Node* root{};
    size_t size_{};

public:
    class iterator {
        Node* current;
    public:
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        friend class Set;
        iterator() :current(nullptr) {}
        iterator(Node* node) :current(node) {}
        reference operator*() const {
            return current->value;
        }
        pointer operator->() const {
            return current;
        }
        iterator operator++() {
            if (current->right != nullptr) {
                current = current->right;
                while (current->left != nullptr) {
                    current = current->left;
                }
            }
            else {
                while (current->parent != nullptr && current == current->parent->right) {
                    current = current->parent;
                }
                current = current->parent;
            }
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            if (current->right != nullptr) {
                current = current->right;
                while (current->left != nullptr) {
                    current = current->left;
                }
            }
            else {
                while (current->parent != nullptr && current == current->parent->right) {
                    current = current->parent;
                }
                current = current->parent;
            }
            return tmp;
        }
        friend bool operator==(iterator lhs, iterator rhs) {
            return lhs.current == rhs.current;
        }
        friend bool operator!=(iterator lhs, iterator rhs) {
            return lhs.current != rhs.current;
        }
    };

    iterator begin() {
        Node* tmp = root;
        if (tmp == nullptr) {
            return iterator(nullptr);
        }
        while (tmp->left != nullptr) {
            tmp = tmp->left;
        }
        return iterator(tmp);
    }

    iterator end() {
        return (iterator)nullptr;
    }

    iterator find(const T& value) {
        iterator it(root);
        iterator it_(begin());
        while (it_ != nullptr) {
            if (*it_ == value) {
                return it_;
            }
            ++it_;
        }
        return end();
    }

    void erase(iterator pos) {
        if (pos.current == nullptr) {
            std::cout << "Iterator is pointing to nullptr!" << std::endl;
            return;
        }
        Node* node_to_remove = pos.current;
        if (node_to_remove->left == nullptr && node_to_remove->right == nullptr) {
            if (node_to_remove->parent != nullptr) {
                if (node_to_remove->parent->left == node_to_remove) {
                    node_to_remove->parent->left = nullptr;
                }
                else {
                    node_to_remove->parent->right = nullptr;
                }
            }
            else {
                root = nullptr;
            }
            delete node_to_remove;
        }

        else if (node_to_remove->left == nullptr || node_to_remove->right == nullptr) {
            Node* child;

            if ((node_to_remove->left != nullptr)) {
                child = node_to_remove->left;
            }
            else {
                child = node_to_remove->right;
            }
            if (node_to_remove->parent != nullptr) {
                if (node_to_remove->parent->left == node_to_remove) {
                    node_to_remove->parent->left = child;
                }
                else {
                    node_to_remove->parent->right = child;
                }
                child->parent = node_to_remove->parent;
            }
            else {
                root = child;
                root->parent = nullptr;
            }

            delete node_to_remove;
        }

        else {
            Node* successor = node_to_remove->right;
            while (successor->left != nullptr) {
                successor = successor->left;
            }
            node_to_remove->value = successor->value;
            if (successor->right != nullptr) {
                successor->right->parent = successor->parent;
            }
            if (successor->parent->left == successor) {
                successor->parent->left = nullptr;
            }
            else {
                successor->parent->right = nullptr;
            }

            delete successor;
        }
        --size_;
        updateHeightAndBalance(pos.current->parent);
    }
    int getHeight(Node* node) {
        if (node != nullptr) {
            return node->height;
        }
        return 0;
    }
    void updateHeightAndBalance(Node* node) {
        Node* tmp = node;
        while (tmp != nullptr) {
            tmp->height = 1 + std::max(getHeight(tmp->left), getHeight(tmp->right));
            tmp->balanceFactor = getHeight(tmp->left) - getHeight(tmp->right);
            tmp = tmp->parent;
        }
    }


    void insert(const T& value)
    {
        Node* newN = new Node;
        newN->value = value;
        newN->height = 1;
        newN->balanceFactor = 0;

        if (root == nullptr) {
            root = newN;
            ++size_;
            updateHeightAndBalance(newN);
            return;
        }

        Node* n = root;
        while (true) {
            if (value < n->value) {
                if (n->left != nullptr) {
                    n = n->left;
                }
                else {
                    n->left = newN;
                    n->left->value = value;
                    n->left->parent = n;
                    ++size_;
                    updateHeightAndBalance(newN);
                    return;
                }
            }
            else if (value > n->value) {
                if (n->right != nullptr)
                    n = n->right;
                else {
                    n->right = newN;
                    n->right->value = value;
                    n->right->parent = n;
                    ++size_;
                    updateHeightAndBalance(newN);
                    return;
                }
            }
            else {
                return;
            }
        }
    }

    void print_inorder() {
        Node* current = root;
        while (current != nullptr) {
            if (current->left == nullptr) {
                std::cout << current->value << " ";
                current = current->right;
            }
            else {
                Node* predecessor = current->left;

                while (predecessor->right != nullptr && predecessor->right != current) {
                    predecessor = predecessor->right;
                }

                if (predecessor->right == nullptr) {
                    predecessor->right = current;
                    current = current->left;
                }
                else {
                    predecessor->right = nullptr;
                    std::cout << current->value << " ";
                    current = current->right;
                }
            }
        }

        std::cout << std::endl;
    }
};


int main() {
    Set<int> st;
    st.insert(1);
    st.insert(3);
    st.insert(5);
    st.insert(2);
    st.insert(4);

    auto it = st.find(3);
    st.erase(it);

    for (auto x : st) {
        std::cout << x << ' ';
    } // Output: 1 2 4 5

    assert(st.find(3) == st.end());
}