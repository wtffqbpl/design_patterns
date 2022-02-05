#include <iostream>
#include <vector>
#include <memory>
#include <ostream>

class BackAccount {
private:
  int32_t m_balance{0};
  uint32_t m_current{0};

  struct Memento {
    int32_t m_balance;
    Memento(int32_t b) : m_balance(b) {}
  };
  std::vector<std::shared_ptr<const Memento>> m_changes;

public:
  BackAccount(const int32_t b) : m_balance(b) {
    m_changes.emplace_back(std::make_shared<const Memento>(m_balance));
  }

  const std::shared_ptr<const Memento> deposit(int32_t amount) {
    m_balance += amount;
    m_changes.emplace_back(std::make_shared<const Memento>(m_balance));

    return m_changes[m_current++];
  }

  void restore(const std::shared_ptr<const Memento>& m) {
    if (m) {
      m_balance = m->m_balance;
      m_changes.push_back(m);
      m_current = m_changes.size() - 1;
    }
  }

  const std::shared_ptr<const Memento> undo() {
    if (m_current > 0) {
      m_balance = m_changes[--m_current]->m_balance;
      return m_changes[m_current];
    }

    return {};
  }

  const std::shared_ptr<const Memento> redo() {
    if ((m_current + 1) < m_changes.size()) {
      m_balance = m_changes[++m_current]->m_balance;
      return m_changes[m_current];
    }

    return {};
  }

  friend std::ostream& operator<<(std::ostream& os, const BackAccount& ac) {
    return os << "balance: " << ac.m_balance;
  }
};

int main() {
  BackAccount ba{100};
  ba.deposit(50);
  ba.deposit(25);
  
  std::cout << ba << "\n";

  ba.undo();
  std::cout << "Undo 1: " << ba << "\n";
  ba.undo();
  std::cout << "Undo 2: " << ba << "\n";
  ba.redo();
  std::cout << "Undo 2: " << ba << "\n";
  return 0;
}

