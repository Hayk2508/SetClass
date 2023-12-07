#include <iostream>
#include <cassert>
template<typename T>
class Set {
  struct Node {
    T value;
    Node *left = nullptr;
    Node *right = nullptr;
    Node *parent = nullptr;

    // Functions can be added if necessary ()
  };

  Node *root{};
  size_t size_{};

public:
    class iterator {
        Node *current;
    public:
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        iterator():current(nullptr){}
        iterator(Node *node):current(node){}
        reference operator*() const {
            return current->value;
        }
        pointer operator->() const{
            return current;
        }
        iterator operator++() {
            if (current->right != nullptr) {
                current = current->right;
                while (current->left != nullptr) {
                    current = current->left;
                }
            } else {
                while (current->parent != nullptr && current == current->parent->right) {
                    current = current->parent;
                }
                current = current->parent;
            }
            return *this;
        }

        iterator operator++(int){
            iterator tmp = *this;
            if (current->right != nullptr) {
                current = current->right;
                while (current->left != nullptr) {
                    current = current->left;
                }
            } else {
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
        while(tmp->left != nullptr){
            tmp = tmp->left;
        }
        return iterator(tmp);
    }

    iterator end(){
        Node* tmp = root;
        if (tmp == nullptr) {
            return nullptr;
        }
        while(tmp->right != nullptr){
            tmp = tmp->right;
        }
        return (iterator)tmp;
    }

    iterator find(const T &value) {
        iterator it(root);
        iterator it_(begin());
        while(it_!=nullptr){
            if(*it_ == value){
                return it_;
            }
            ++it_;
        }
        return end();
    }


    void insert(const T &value)
    {
        Node* newN = new Node;
        newN->value = value;
        if(root == nullptr){
            root = newN;
            return;
        }
        Node* n = root;
        while (true) {
            if  (value < n->value) {
                if (n->left != nullptr){
                    n = n->left;

                }

                else {
                    n->left = new Node;
                    n->left->value = value;
                    n->left->parent = n;
                            return;
                }
            }
            else if (value > n->value) {
                if ( n->right != nullptr )
                    n = n->right;
                else {
                    n->right = new Node;
                    n->right->value = value;
                    n->right->parent = n;
                    return;
                }
            }
            else{
                return;
            }
        }
    }
  void print_inorder() {
     Node *current = root;

     while (current != nullptr) {
         if (current->left == nullptr) {
             std::cout << current->value << " ";
             current = current->right;
         } else {
             Node *predecessor = current->left;

             while (predecessor->right != nullptr && predecessor->right != current) {
                 predecessor = predecessor->right;
             }

             if (predecessor->right == nullptr) {
                 predecessor->right = current;
                 current = current->left;
             } else {
                 predecessor->right = nullptr;
                 std::cout << current->value << " ";
                 current = current->right;
             }
         }
     }

     std::cout << std::endl;

    }
};


int main(){

        Set<int> st;
        st.insert(10);
        st.insert(-10);
        st.insert(20);
        st.insert(10);
        st.insert(0);

        for (auto x : st) {
            std::cout << x << ' ';
        } // Output: -10 0 10 20
        std::cout << std::endl;

        auto it = st.find(10);
        std::cout << *it << std::endl; // Output: 10
    assert(st.find(-20) == st.end());
}