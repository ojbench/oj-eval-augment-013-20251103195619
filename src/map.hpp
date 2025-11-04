/**
* implement a container like std::map
*/
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
   class Key,
   class T,
   class Compare = std::less <Key>
   > class map {
  public:
   typedef pair<const Key, T> value_type;

  private:
   enum Color { RED, BLACK };

   struct Node {
       value_type *data;
       Node *left;
       Node *right;
       Node *parent;
       Color color;

       Node(const value_type &val, Node *p = nullptr)
           : data(new value_type(val)), left(nullptr), right(nullptr),
             parent(p), color(RED) {}

       ~Node() {
           if (data) delete data;
       }
   };

   Node *root;
   Node *header; // sentinel node, header->left points to leftmost, header->right points to rightmost
   size_t node_count;
   Compare comp;

   void rotateLeft(Node *x) {
       Node *y = x->right;
       x->right = y->left;
       if (y->left != nullptr) {
           y->left->parent = x;
       }
       y->parent = x->parent;
       if (x->parent == nullptr) {
           root = y;
       } else if (x == x->parent->left) {
           x->parent->left = y;
       } else {
           x->parent->right = y;
       }
       y->left = x;
       x->parent = y;
   }

   void rotateRight(Node *x) {
       Node *y = x->left;
       x->left = y->right;
       if (y->right != nullptr) {
           y->right->parent = x;
       }
       y->parent = x->parent;
       if (x->parent == nullptr) {
           root = y;
       } else if (x == x->parent->right) {
           x->parent->right = y;
       } else {
           x->parent->left = y;
       }
       y->right = x;
       x->parent = y;
   }

   void insertFixup(Node *z) {
       while (z->parent != nullptr && z->parent->color == RED) {
           if (z->parent == z->parent->parent->left) {
               Node *y = z->parent->parent->right;
               if (y != nullptr && y->color == RED) {
                   z->parent->color = BLACK;
                   y->color = BLACK;
                   z->parent->parent->color = RED;
                   z = z->parent->parent;
               } else {
                   if (z == z->parent->right) {
                       z = z->parent;
                       rotateLeft(z);
                   }
                   z->parent->color = BLACK;
                   z->parent->parent->color = RED;
                   rotateRight(z->parent->parent);
               }
           } else {
               Node *y = z->parent->parent->left;
               if (y != nullptr && y->color == RED) {
                   z->parent->color = BLACK;
                   y->color = BLACK;
                   z->parent->parent->color = RED;
                   z = z->parent->parent;
               } else {
                   if (z == z->parent->left) {
                       z = z->parent;
                       rotateRight(z);
                   }
                   z->parent->color = BLACK;
                   z->parent->parent->color = RED;
                   rotateLeft(z->parent->parent);
               }
           }
       }
       root->color = BLACK;
   }

   void transplant(Node *u, Node *v) {
       if (u->parent == nullptr) {
           root = v;
       } else if (u == u->parent->left) {
           u->parent->left = v;
       } else {
           u->parent->right = v;
       }
       if (v != nullptr) {
           v->parent = u->parent;
       }
   }

   Node* minimum(Node *x) const {
       while (x->left != nullptr) {
           x = x->left;
       }
       return x;
   }

   Node* maximum(Node *x) const {
       while (x->right != nullptr) {
           x = x->right;
       }
       return x;
   }

   Node* successor(Node *x) const {
       if (x->right != nullptr) {
           return minimum(x->right);
       }
       Node *y = x->parent;
       while (y != nullptr && x == y->right) {
           x = y;
           y = y->parent;
       }
       return y;
   }

   Node* predecessor(Node *x) const {
       if (x->left != nullptr) {
           return maximum(x->left);
       }
       Node *y = x->parent;
       while (y != nullptr && x == y->left) {
           x = y;
           y = y->parent;
       }
       return y;
   }

   void eraseFixup(Node *x, Node *x_parent) {
       while (x != root && (x == nullptr || x->color == BLACK)) {
           if (x == x_parent->left) {
               Node *w = x_parent->right;
               if (w->color == RED) {
                   w->color = BLACK;
                   x_parent->color = RED;
                   rotateLeft(x_parent);
                   w = x_parent->right;
               }
               if ((w->left == nullptr || w->left->color == BLACK) &&
                   (w->right == nullptr || w->right->color == BLACK)) {
                   w->color = RED;
                   x = x_parent;
                   x_parent = x->parent;
               } else {
                   if (w->right == nullptr || w->right->color == BLACK) {
                       if (w->left != nullptr) w->left->color = BLACK;
                       w->color = RED;
                       rotateRight(w);
                       w = x_parent->right;
                   }
                   w->color = x_parent->color;
                   x_parent->color = BLACK;
                   if (w->right != nullptr) w->right->color = BLACK;
                   rotateLeft(x_parent);
                   x = root;
                   break;
               }
           } else {
               Node *w = x_parent->left;
               if (w->color == RED) {
                   w->color = BLACK;
                   x_parent->color = RED;
                   rotateRight(x_parent);
                   w = x_parent->left;
               }
               if ((w->right == nullptr || w->right->color == BLACK) &&
                   (w->left == nullptr || w->left->color == BLACK)) {
                   w->color = RED;
                   x = x_parent;
                   x_parent = x->parent;
               } else {
                   if (w->left == nullptr || w->left->color == BLACK) {
                       if (w->right != nullptr) w->right->color = BLACK;
                       w->color = RED;
                       rotateLeft(w);
                       w = x_parent->left;
                   }
                   w->color = x_parent->color;
                   x_parent->color = BLACK;
                   if (w->left != nullptr) w->left->color = BLACK;
                   rotateRight(x_parent);
                   x = root;
                   break;
               }
           }
       }
       if (x != nullptr) x->color = BLACK;
   }

   void clearTree(Node *node) {
       if (node == nullptr) return;
       clearTree(node->left);
       clearTree(node->right);
       delete node;
   }

   Node* copyTree(Node *node, Node *parent) {
       if (node == nullptr) return nullptr;
       Node *newNode = new Node(*(node->data), parent);
       newNode->color = node->color;
       newNode->left = copyTree(node->left, newNode);
       newNode->right = copyTree(node->right, newNode);
       return newNode;
   }

   Node* findNode(const Key &key) const {
       Node *current = root;
       while (current != nullptr) {
           if (comp(key, current->data->first)) {
               current = current->left;
           } else if (comp(current->data->first, key)) {
               current = current->right;
           } else {
               return current;
           }
       }
       return nullptr;
   }

  public:
   class const_iterator;
   class iterator {
      private:
       Node *node;
       const map *container;

       friend class map;
       friend class const_iterator;

       iterator(Node *n, const map *c) : node(n), container(c) {}

      public:
       iterator() : node(nullptr), container(nullptr) {}

       iterator(const iterator &other) : node(other.node), container(other.container) {}

       iterator operator++(int) {
           iterator temp = *this;
           ++(*this);
           return temp;
       }

       iterator &operator++() {
           if (node == nullptr) {
               throw invalid_iterator();
           }
           if (node->right != nullptr) {
               node = container->minimum(node->right);
           } else {
               Node *y = node->parent;
               while (y != nullptr && node == y->right) {
                   node = y;
                   y = y->parent;
               }
               node = y;
           }
           return *this;
       }

       iterator operator--(int) {
           iterator temp = *this;
           --(*this);
           return temp;
       }

       iterator &operator--() {
           if (node == nullptr) {
               if (container == nullptr || container->root == nullptr) {
                   throw invalid_iterator();
               }
               node = container->maximum(container->root);
           } else {
               if (node->left != nullptr) {
                   node = container->maximum(node->left);
               } else {
                   Node *y = node->parent;
                   while (y != nullptr && node == y->left) {
                       node = y;
                       y = y->parent;
                   }
                   if (y == nullptr) {
                       throw invalid_iterator();
                   }
                   node = y;
               }
           }
           return *this;
       }

       value_type &operator*() const {
           if (node == nullptr) {
               throw invalid_iterator();
           }
           return *(node->data);
       }

       bool operator==(const iterator &rhs) const {
           return node == rhs.node && container == rhs.container;
       }

       bool operator==(const const_iterator &rhs) const {
           return node == rhs.node && container == rhs.container;
       }

       bool operator!=(const iterator &rhs) const {
           return !(*this == rhs);
       }

       bool operator!=(const const_iterator &rhs) const {
           return !(*this == rhs);
       }

       value_type *operator->() const noexcept {
           return node->data;
       }
   };

   class const_iterator {
      private:
       Node *node;
       const map *container;

       friend class map;
       friend class iterator;

       const_iterator(Node *n, const map *c) : node(n), container(c) {}

      public:
       const_iterator() : node(nullptr), container(nullptr) {}

       const_iterator(const const_iterator &other) : node(other.node), container(other.container) {}

       const_iterator(const iterator &other) : node(other.node), container(other.container) {}

       const_iterator operator++(int) {
           const_iterator temp = *this;
           ++(*this);
           return temp;
       }

       const_iterator &operator++() {
           if (node == nullptr) {
               throw invalid_iterator();
           }
           if (node->right != nullptr) {
               node = container->minimum(node->right);
           } else {
               Node *y = node->parent;
               while (y != nullptr && node == y->right) {
                   node = y;
                   y = y->parent;
               }
               node = y;
           }
           return *this;
       }

       const_iterator operator--(int) {
           const_iterator temp = *this;
           --(*this);
           return temp;
       }

       const_iterator &operator--() {
           if (node == nullptr) {
               if (container == nullptr || container->root == nullptr) {
                   throw invalid_iterator();
               }
               node = container->maximum(container->root);
           } else {
               if (node->left != nullptr) {
                   node = container->maximum(node->left);
               } else {
                   Node *y = node->parent;
                   while (y != nullptr && node == y->left) {
                       node = y;
                       y = y->parent;
                   }
                   if (y == nullptr) {
                       throw invalid_iterator();
                   }
                   node = y;
               }
           }
           return *this;
       }

       const value_type &operator*() const {
           if (node == nullptr) {
               throw invalid_iterator();
           }
           return *(node->data);
       }

       bool operator==(const iterator &rhs) const {
           return node == rhs.node && container == rhs.container;
       }

       bool operator==(const const_iterator &rhs) const {
           return node == rhs.node && container == rhs.container;
       }

       bool operator!=(const iterator &rhs) const {
           return !(*this == rhs);
       }

       bool operator!=(const const_iterator &rhs) const {
           return !(*this == rhs);
       }

       const value_type *operator->() const noexcept {
           return node->data;
       }
   };

   map() : root(nullptr), header(nullptr), node_count(0) {}

   map(const map &other) : root(nullptr), header(nullptr), node_count(0) {
       if (other.root != nullptr) {
           root = copyTree(other.root, nullptr);
           node_count = other.node_count;
       }
   }

   map &operator=(const map &other) {
       if (this != &other) {
           clear();
           if (other.root != nullptr) {
               root = copyTree(other.root, nullptr);
               node_count = other.node_count;
           }
       }
       return *this;
   }

   ~map() {
       clear();
   }

   T &at(const Key &key) {
       Node *node = findNode(key);
       if (node == nullptr) {
           throw index_out_of_bound();
       }
       return node->data->second;
   }

   const T &at(const Key &key) const {
       Node *node = findNode(key);
       if (node == nullptr) {
           throw index_out_of_bound();
       }
       return node->data->second;
   }

   T &operator[](const Key &key) {
       Node *node = findNode(key);
       if (node == nullptr) {
           value_type val(key, T());
           auto result = insert(val);
           return result.first.node->data->second;
       }
       return node->data->second;
   }

   const T &operator[](const Key &key) const {
       Node *node = findNode(key);
       if (node == nullptr) {
           throw index_out_of_bound();
       }
       return node->data->second;
   }

   iterator begin() {
       if (root == nullptr) {
           return iterator(nullptr, this);
       }
       return iterator(minimum(root), this);
   }

   const_iterator cbegin() const {
       if (root == nullptr) {
           return const_iterator(nullptr, this);
       }
       return const_iterator(minimum(root), this);
   }

   iterator end() {
       return iterator(nullptr, this);
   }

   const_iterator cend() const {
       return const_iterator(nullptr, this);
   }

   bool empty() const {
       return node_count == 0;
   }

   size_t size() const {
       return node_count;
   }

   void clear() {
       clearTree(root);
       root = nullptr;
       node_count = 0;
   }

   pair<iterator, bool> insert(const value_type &value) {
       Node *y = nullptr;
       Node *x = root;

       while (x != nullptr) {
           y = x;
           if (comp(value.first, x->data->first)) {
               x = x->left;
           } else if (comp(x->data->first, value.first)) {
               x = x->right;
           } else {
               return pair<iterator, bool>(iterator(x, this), false);
           }
       }

       Node *z = new Node(value, y);

       if (y == nullptr) {
           root = z;
       } else if (comp(value.first, y->data->first)) {
           y->left = z;
       } else {
           y->right = z;
       }

       node_count++;
       insertFixup(z);

       return pair<iterator, bool>(iterator(z, this), true);
   }

   void erase(iterator pos) {
       if (pos.node == nullptr || pos.container != this) {
           throw invalid_iterator();
       }

       Node *z = pos.node;
       Node *y = z;
       Node *x;
       Node *x_parent;
       Color y_original_color = y->color;

       if (z->left == nullptr) {
           x = z->right;
           x_parent = z->parent;
           transplant(z, z->right);
       } else if (z->right == nullptr) {
           x = z->left;
           x_parent = z->parent;
           transplant(z, z->left);
       } else {
           y = minimum(z->right);
           y_original_color = y->color;
           x = y->right;

           if (y->parent == z) {
               x_parent = y;
           } else {
               x_parent = y->parent;
               transplant(y, y->right);
               y->right = z->right;
               y->right->parent = y;
           }

           transplant(z, y);
           y->left = z->left;
           y->left->parent = y;
           y->color = z->color;
       }

       delete z;
       node_count--;

       if (y_original_color == BLACK) {
           eraseFixup(x, x_parent);
       }
   }

   size_t count(const Key &key) const {
       return findNode(key) != nullptr ? 1 : 0;
   }

   iterator find(const Key &key) {
       Node *node = findNode(key);
       if (node == nullptr) {
           return end();
       }
       return iterator(node, this);
   }

   const_iterator find(const Key &key) const {
       Node *node = findNode(key);
       if (node == nullptr) {
           return cend();
       }
       return const_iterator(node, this);
   }
};

}

#endif