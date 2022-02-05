#include <iostream>
#include <vector>

struct BankAccount {
  int32_t m_banance;

  void deposit(int32_t amount) {
    m_banance += amount;
  }

  void withdraw(int32_t amount) {
    m_banance -= amount;
  }
};

struct Command {
  virtual void execute() = 0;
};

struct BanckAccountCommand : Command {
  enum class Action : bool {
    deposit,
    withdraw
  };

  BankAccount & m_ac;
  Action m_action;
  int32_t m_amount;
 
  BanckAccountCommand(BankAccount &ac, Action a, int32_t amnt) :
    m_ac(ac), m_action(a), m_amount(amnt) {}

  void execute() {
    (m_action == Action::deposit) ?
      m_ac.deposit(m_amount) : m_ac.withdraw(m_amount);
  }

  void undo() {
    (m_action == Action::deposit) ?
      m_ac.withdraw(m_amount) : m_ac.deposit(m_amount);
  }
};

struct CompositeBankAccountCommand : std::vector<BanckAccountCommand>, Command {
  CompositeBankAccountCommand(const std::initializer_list<value_type>& items) :
    std::vector<BanckAccountCommand>(items) {}

  void execute() {
    for (auto &cmd : *this) {
      cmd.execute();
    }
  }

    void undo() {
      for (auto &cmd : *this)
        cmd.undo();
    }
};

int main() {

  BankAccount ba1{1000};
  BankAccount ba2{1000};

  CompositeBankAccountCommand commands {
    BanckAccountCommand{ba1, BanckAccountCommand::Action::withdraw, 200},
    BanckAccountCommand{ba2, BanckAccountCommand::Action::deposit, 200}
  };

  commands.execute();
  commands.undo();

  std::cout << ba1.m_banance << std::endl;
  std::cout << ba2.m_banance << std::endl;

  return 0;
}