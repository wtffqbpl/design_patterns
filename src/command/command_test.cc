#include <gtest/gtest.h>
#include <iostream>
#include <utility>
#include <vector>

struct walk {
  void operator()() {
    std::cout << "walk" << std::endl;
  }
};

struct jog {
  void operator()() {
    std::cout << "jog" << std::endl;
  }
};

struct run {
  void operator()() {
    std::cout << "run" << std::endl;
  }
};

struct macroCommand : std::vector<std::function<void()>> {
  void operator()() {
    for (auto &&f : *this)
      f();
  }
};

void doIt(const std::function<void()>& f) {
  f();
}

TEST(command, basic_test) {
  std::stringstream oss;
  testing::internal::CaptureStdout();

  doIt(walk{});
  doIt(jog{});
  doIt(run{});

  macroCommand cardio_workout;
  cardio_workout.push_back(walk{});
  cardio_workout.push_back(jog{});
  cardio_workout.push_back(run{});
  cardio_workout();

  oss << "walk\n"
         "jog\n"
         "run\n"
         "walk\n"
         "jog\n"
         "run\n";

  std::string act_output = testing::internal::GetCapturedStdout();

#ifndef NDEBUG
  std::cout << "Expected output:\n"
            << oss.str() << '\n'
            << "Actual output:\n"
            << act_output << '\n';
#endif

  EXPECT_TRUE(oss.str() == act_output);
}

/**
 * The Command interface declares a method for executing a command.
 */
class Command {
public:
  virtual ~Command() {}
  virtual void Execute() const = 0;
};

/**
 * Some commands implement simple operations on their own.
 */
class SimpleCommand : public Command {
private:
  std::string pay_load_;

public:
  explicit SimpleCommand(std::string pay_load) : pay_load_(std::move(pay_load)) {}

  void Execute() const override {
    std::cout << "SimpleCommand: See, I can do simple things like print (" <<
              this->pay_load_ << ")\n";
  }
};

/**
 * The Receiver classes contain some important business logic. They know how to
 * perform all kinds of operations, associated with carrying out a request. In
 * fact, any class may serve as a Receiver.
 */
class Receiver {
public:
  void DoSomething(const std::string &a) {
    std::cout << "Receiver: Working on (" << a << ")\n";
  }

  void DoSomethingElse(const std::string &b) {
    std::cout << "Receiver: Also working on (" << b << ")\n";
  }
};

/**
 * However, some commands can delegate more complex operations to other objects,
 * called 'receivers'.
 */
class ComplexCommand : public Command {
  /**
   * @var Receiver
   */
private:
  Receiver *receiver_;
  /**
   * Context data, required for launching the receiver's methods.
   */
  std::string a_;
  std::string b_;

  /**
   * Complex commands can accept one or several receiver objects along with any
   * context data via the constructor.
   */
public:
  ComplexCommand(Receiver *receiver, std::string a, std::string b) :
      receiver_(receiver), a_(std::move(a)), b_(std::move(b)) {}
  /**
   * Commands can delegate to any methods of a receiver.
   */
  void Execute() const override {
    std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";
    this->receiver_->DoSomething(this->a_);
    this->receiver_->DoSomethingElse(this->b_);
  }
};

/**
 * The Invoker is associated with one or serveral commands. It sends a request to
 * the command.
 */
class Invoker {
  /**
   * @var Command
   */
private:
  Command *on_start_;
  /**
   * @var Command
   */

  Command *on_finish_;
  /**
   * Initialize commands;
   */

public:
  ~Invoker() {
    delete on_start_;
    delete on_finish_;
  }

  void SetOnStart(Command *command) {
    this->on_start_ = command;
  }

  void SetOnFinish(Command *command) {
    this->on_finish_ = command;
  }

  /**
   * The Invoker does not depend on concrete command or receiver classes. The
   * Invoker passes a request to a receiver indirectly, by executing a command.
   */
  void DoSomethingImportant() {
    std::cout << "Invoker: Does anybody want something done before I begin?\n";
    if (this->on_start_)
      this->on_start_->Execute();

    std::cout << "Invoker: ...doing something really important...\n";
    std::cout << "Invoker: Does anybody want something done after I finish?\n";

    if (this->on_finish_)
      this->on_finish_->Execute();
  }
};

/**
 * the client code parameterize an invoker with any commands.
 */
TEST(command, test1) {
  auto *invoker = new Invoker;
  invoker->SetOnStart(new SimpleCommand{"Say Hi!"});
  auto *receiver = new Receiver;
  invoker->SetOnFinish(new ComplexCommand{receiver, "Send email", "Save report"});
  invoker->DoSomethingImportant();

  delete invoker;
  delete receiver;
}

namespace demo2 {
struct BankAccount {
  int32_t balance_;

  void deposit(int32_t amount) { balance_ += amount; }

  void withdraw(int32_t amount) { balance_ -= amount; }
};

struct Command {
  virtual void execute() = 0;
};

struct BankAccountCommand : Command {
  enum class Action : bool { deposit, withdraw };

  BankAccount &m_ac;
  Action m_action;
  int32_t m_amount;

  BankAccountCommand(BankAccount &ac, Action a, int32_t amnt)
      : m_ac(ac), m_action(a), m_amount(amnt) {}

  void execute() override {
    (m_action == Action::deposit) ? m_ac.deposit(m_amount)
                                  : m_ac.withdraw(m_amount);
  }

  void undo() {
    (m_action == Action::deposit) ? m_ac.withdraw(m_amount)
                                  : m_ac.deposit(m_amount);
  }
};

struct CompositeBankAccountCommand : std::vector<BankAccountCommand>, Command {
  CompositeBankAccountCommand(const std::initializer_list<value_type> &items)
      : std::vector<BankAccountCommand>(items) {}

  void execute() override {
    for (auto &cmd : *this) {
      cmd.execute();
    }
  }

  void undo() {
    for (auto &cmd : *this)
      cmd.undo();
  }
};

} // end of namespace demo2

TEST(command, demo2){
  demo2::BankAccount ba1{1000};
  demo2::BankAccount ba2{1000};

  demo2::CompositeBankAccountCommand command_list {
      demo2::BankAccountCommand{ba1,
                                demo2::BankAccountCommand::Action::withdraw,
                                200},
      demo2::BankAccountCommand{ba2,
                                demo2::BankAccountCommand::Action::deposit,
                                200}
  };

  command_list.execute();
  command_list.undo();

  std::cout << ba1.balance_ << std::endl;
  std::cout << ba2.balance_ << std::endl;
}
