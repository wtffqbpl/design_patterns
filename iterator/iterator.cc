#include <gtest/gtest.h>
/**
 * Iterator Design Pattern
 * Intent: Lets you traverse elements of a collection without exposing its
 * underlying representation (list, stack, tree, etc.).
 */

#include <iostream>
#include <utility>
#include <vector>

namespace iterator {
/**
 * C++ has its own implementation of iterator that works with a different
 * generics containers defined by the standard library.
 */

template <typename T, typename U>
class Iterator {
public:
  typedef typename std::vector<T>::iterator iter_type;
  explicit Iterator(U *p_data, bool reverse = false) : data_(p_data) {
    it_ = data_->m_data_.begin();
  }

  void First() { it_ = data_->m_data_.begin(); }
  void Next() { it_++; }
  [[nodiscard]] bool IsDone() const { return (it_ == data_->m_data_.end()); }
  iter_type Current() { return it_; }

private:
  U *data_;
  iter_type it_;
};

/**
 * Generic Collections/Containers provides one or several methods for ertrieving
 * fresh iterator instances, compatible with the collection class.
 */
template <class T>
class Container {
  friend class Iterator<T, Container>;

public:
  void Add(T a) { m_data_.push_back(a); }
  Iterator<T, Container> *CreateIterator() {
    return new Iterator<T, Container>(this);
  }

private:
  std::vector<T> m_data_;
};

class Data {
public:
  explicit Data(int a = 0) : m_data_(a) {}
  void set_data(int a) { m_data_ = a; }
  [[nodiscard]] int data() const { return m_data_; }

private:
  int m_data_;
};

/**
 * The client code may or may not know about the Concrete Iterator or Collection
 * classes, for this implementation the container is generic so you can used
 * with an int or with a custom class.
 */
void ClientCode() {
  std::cout << "________________________ Iterator with int ______________________\n";
  Container<int> cont;

  for (int i = 0; i < 10; ++i) {
    cont.Add(i);
  }

  Iterator<int, Container<int>> *it = cont.CreateIterator();

  for (it->First(); !it->IsDone(); it->Next()) {
    std::cout << *it->Current() << std::endl;
  }

  Container<Data> cont2;
  Data a(100), b(1000), c(10000);
  cont2.Add(a);
  cont2.Add(b);
  cont2.Add(c);

  std::cout << "_____________________ Iterator with custom class ____________________\n";
  Iterator<Data, Container<Data>> *it2 = cont2.CreateIterator();
  for (it2->First(); !it2->IsDone(); it2->Next()) {
    std::cout << it2->Current()->data() << std::endl;
  }

  delete it;
  delete it2;
}

} // end of namespace iterator

TEST(iterator, basic_demo) {
  using namespace iterator;

  ClientCode();
}

namespace binary_tree {

template<typename T> struct BinaryTree;

template<typename T> struct Node {
  // class members.
  T m_value = T{};
  Node<T>* m_left{nullptr};
  Node<T>* m_right{nullptr};
  BinaryTree<T>* m_tree{nullptr};
  Node<T>* m_parent{nullptr};

  // Constructors
  explicit Node(T v, Node<T> *const l = nullptr, Node<T> *const r = nullptr)
      : m_value(std::move(v)), m_left(l), m_right(r) {
    if (m_left) this->m_left->m_tree = this->m_right->m_tree = m_tree;
    if (m_right) this->m_left->m_parent = this->m_right->m_parent = this;
  }

  // Destructor
  ~Node() {
    delete m_left;
    delete m_right;
  }

  void set_tree(BinaryTree<T> *t) {
    m_tree = t;
    if (m_left) m_left->set_tree(t);
    if (m_right) m_right->set_tree(t);
  }
};

template<typename T> struct BinaryTree {
  Node<T>* m_root = nullptr;
  explicit BinaryTree(Node<T> *const r) : m_root(r) { m_root->set_tree(this); }
  ~BinaryTree() { delete m_root; }

  /* --------------------------------- Iterator Implementation --------------------- */
  template<typename U> struct PreOrderIterator {
    Node<U> *current;

    explicit PreOrderIterator(Node<U> *c) : current(c) {}
    bool operator!=(const PreOrderIterator<U>& rhs) {
      return current != rhs.current;
    }

    PreOrderIterator<U>& operator++() {
      if (current->m_right) {
        current = current->m_right;
        while (current->m_left) {
          current = current->m_left;
        }
      } else {
        Node<T> *p = current->m_parent;
        while (p && current == p->m_right) {
          current = p;
          p = p->m_parent;
        }

        current = p;
      }

      return *this;
    }

    Node<U>& operator*() { return *current; }
  };

  using iterator = PreOrderIterator<T>;

  iterator begin() {
    Node<T> *n = m_root;
    if (n) {
      while (n->m_left) {
        n = n->m_left;
      }
    }

    return iterator{n};
  }

  iterator end() { return iterator{nullptr}; }
  /* -------------------------------------------------------------------------- */
};

} // end of namespace binary_tree

TEST(iterator, binary_tree_demo) {
  using namespace binary_tree;
  //
  //             me
  //           /    \
  //        mother  father
  //        /   \
  //      m'm   m'f

  BinaryTree<std::string> family {
      new Node<std::string>{"me",
                            new Node<std::string>{"mother",
                                                  new Node<std::string>{"mother's mother"},
                                                  new Node<std::string>{"mother's father"}
                            },
                            new Node<std::string>{"father"}
      }
  };

  for_each(begin(family), end(family),
           [](auto&& n) {
             std::cout << n.m_value << '\n';
           });

  for (const auto& it : family) // works with range-based for loop as well.
    std::cout << it.m_value << std::endl;
}