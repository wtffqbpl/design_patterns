#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>

namespace memento {

class BackAccount {
private:
  int32_t balance_{0};
  uint32_t current{0};

  struct Memento {
    int32_t balance;
    explicit Memento(int32_t b) : balance(b) {}
  };
  std::vector<std::shared_ptr<const Memento>> changes_;

public:
  explicit BackAccount(const int32_t b) : balance_(b) {
    changes_.emplace_back(std::make_shared<const Memento>(balance_));
  }

  std::shared_ptr<const Memento> deposit(int32_t amount) {
    balance_ += amount;
    changes_.emplace_back(std::make_shared<const Memento>(balance_));

    return changes_[current++];
  }

  void restore(const std::shared_ptr<const Memento> &m) {
    if (m) {
      balance_ = m->balance;
      changes_.push_back(m);
      current = changes_.size() - 1;
    }
  }

  std::shared_ptr<const Memento> undo() {
    if (current > 0) {
      balance_ = changes_[--current]->balance;
      return changes_[current];
    }

    return {};
  }

  std::shared_ptr<const Memento> redo() {
    if ((current + 1) < changes_.size()) {
      balance_ = changes_[++current]->balance;
      return changes_[current];
    }

    return {};
  }

  friend std::ostream &operator<<(std::ostream &os, const BackAccount &ac) {
    return os << "balance: " << ac.balance_;
  }
};

} // end of namespace memento

TEST(memento, basic_demo) {
  using namespace memento;

  BackAccount ba{100};
  ba.deposit(50);
  ba.deposit(25);
  
  std::cout << ba << "\n";

  ba.undo();
  std::cout << "Undo 1: " << ba << '\n';
  ba.undo();
  std::cout << "Undo 2: " << ba << '\n';
  ba.redo();
  std::cout << "Undo 2: " << ba << '\n';
}

