#include <gtest/gtest.h>
#include <iostream>
#include <typeinfo>
#include <unordered_map>

/**
 * The base State class declares methos that all Concrete State should
 * implement and also provides a backreference to the Context object, associated
 * with the State. This backreference can be used by States to transition the
 * Context to another State.
 */

namespace state_pattern {
class Context;

class State {
  /**
   * @var Context
   */
protected:
  Context *context_{nullptr};

public:
  virtual ~State() = default;

  void set_context(Context *context) { this->context_ = context; }

  virtual void Handle1() = 0;
  virtual void Handle2() = 0;
};

/**
 * The Context defines the interface of interest to clients. It also maintains a
 * reference to an instance of a State subclass, which represents the current
 * state of the Context.
 */
class Context {
  /**
   * @var State A reference to the current state of the Context.
   */
private:
  State *state_;

public:
  explicit Context(State *state) : state_(nullptr) {
    this->TransitionTo(state);
  }
  ~Context() { delete state_; }

  /**
   * The Context allows changing the State object at runtime.
   */
  void TransitionTo(State *state) {
    std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";

    delete this->state_;
    this->state_ = state;
    this->state_->set_context(this);
  }

  /**
   * The Context delegates part of its behavior to the current State object.
   */
  void Request1() { this->state_->Handle1(); }
  void Request2() { this->state_->Handle2(); }
};

/**
 * Concrete States implement various behaviors, associated with a state of the
 * Context.
 */
class ConcreteStateA : public State {
public:
  void Handle1() override;
  void Handle2() override { std::cout << "ConcreteStateA handles request2.\n"; }
};

class ConcreteStateB : public State {
public:
  void Handle1() override { std::cout << "ConcreteStateB handles request1.\n"; }

  void Handle2() override {
    std::cout << "ConcreteStateB handles request2.\n";
    std::cout << "ConcreteStateB wants to change the state of the context.\n";
    this->context_->TransitionTo(new ConcreteStateA);
  }
};

void ConcreteStateA::Handle1() {
  std::cout << "ConcreteStateA handles request1.\n";
  std::cout << "ConcreteStateA wants to change the state of the context.\n";

  this->context_->TransitionTo(new ConcreteStateB);
}

/**
 * The Client code.
 */
void ClientCode() {
  auto *context = new Context(new ConcreteStateA);
  context->Request1();
  context->Request2();
  delete context;
}

} // end of namespace state_pattern

TEST(state, basic_demo) {
  using namespace state_pattern;
  ClientCode();
}

namespace state_pattern_new {
/**
 * The State Design Pattern solves two main problems:
 * 1. An object should change its behaviour when its internal state changes.
 * 2. State-specific behaviour should define independently(i.e. in class). So adding
 *    new states should not affect the behaviour of existing states.
 */

/* ---------------------------------------- Events ---------------------------------- */
enum Event {
  connect,
  connected,
  disconnect,
  timeout
};

inline std::ostream &operator<<(std::ostream &os, const Event &event) {
  switch (event) {
  case Event::connect: os << "connect"; break;
  case Event::connected: os << "connected"; break;
  case Event::disconnect: os << "disconnect"; break;
  case Event::timeout: os << "timeout"; break;
  }
  return os;
}
/* -------------------------------------------------------------------------- */

/* ---------------------------------------- States -------------------------- */
struct State {
  virtual std::unique_ptr<State> on_event(Event event) = 0;

  virtual ~State() = default;
};

struct Idle : State {
  std::unique_ptr<State> on_event(Event event) override;
};

struct Connecting : State {
  std::unique_ptr<State> on_event(Event event) override;
private:
  uint32_t                 m_trial = 0;
  static constexpr uint8_t m_max_trial = 3;
};

struct Connected : State {
  std::unique_ptr<State> on_event(Event event) override;
};
/* -------------------------------------------------------------------------- */

/*********************************** Transitions ******************************/
std::unique_ptr<State> Idle::on_event(Event event) {
  std::cout << "Idle -> " << event << '\n';
  if (event == Event::connect) return std::make_unique<Connecting>();
  return nullptr;
}

std::unique_ptr<State> Connecting::on_event(Event event) {
  std::cout << "Connecting -> " << event << '\n';
  switch (event) {
  default: break;
  case Event::connected: return std::make_unique<Connected>();
  case Event::timeout: return ++m_trial < m_max_trial
               ? nullptr : std::make_unique<Idle>();
  }
  return nullptr;
}

std::unique_ptr<State> Connected::on_event(Event event) {
  std::cout << "Connected -> " << event << std::endl;
  if (event == Event::disconnect)
    return std::make_unique<Idle>();

  return nullptr;
}
/* --------------------------------------------------------------------------------- */

struct Bluetooth {
  std::unique_ptr<State> m_curr_state = std::make_unique<Idle>();

  void dispatch(Event event) {
    auto new_state = m_curr_state->on_event(event);
    if (new_state)
      m_curr_state = std::move(new_state);
  }

  template <typename ... Events>
  void establish_connection(Events... e) { (dispatch(e), ...); }
};

} // end of namespace state_pattern_new

TEST(state, state_pattern_new_demo) {
  using namespace state_pattern_new;

  Bluetooth bl;
  bl.establish_connection(Event::connect, Event::timeout,
                           Event::connected, Event::disconnect);
}

namespace state_table {

enum class State {
  OffHook,
  Connecting,
  Connected,
  OnHold,
  OnHook
};

inline std::ostream& operator<<(std::ostream &os, State &state) {
  switch(state) {
  case State::OffHook: os << "off the hook"; break;
  case State::Connecting: os << "connecting"; break;
  case State::Connected: os << "connected"; break;
  case State::OnHold: os << "on hold"; break;
  case State::OnHook: os << "on the hook"; break;
  }

  return os;
}

enum class Trigger {
  CallDialed,
  HungUp,
  CallConnected,
  PlacedOnHold,
  TakenOffHold,
  LeftMessage,
  StopUsingPhone
};

inline std::ostream& operator<<(std::ostream &os, Trigger &trigger) {
  switch(trigger) {
  case Trigger::CallDialed: os << "call dialed"; break;
  case Trigger::HungUp: os << "hung up"; break;
  case Trigger::CallConnected: os << "call connected"; break;
  case Trigger::PlacedOnHold: os << "placed on hold"; break;
  case Trigger::TakenOffHold: os << "taken off hold"; break;
  case Trigger::LeftMessage: os << "left message"; break;
  case Trigger::StopUsingPhone: os << "putting phone on hook"; break;
  }

  return os;
}

} // end of namespace state_table

TEST(state, state_table_demo) {
  using namespace state_table;

  std::unordered_map<State, std::vector<std::pair<Trigger, State>>> transition_table;

  transition_table[State::OffHook] = {
      {Trigger::CallDialed, State::Connecting},
      {Trigger::StopUsingPhone, State::OnHook} };

  transition_table[State::Connecting] = {
      {Trigger::HungUp, State::OffHook},
      {Trigger::CallConnected, State::Connected} };

  transition_table[State::Connected] = {
      {Trigger::LeftMessage, State::OffHook},
      {Trigger::HungUp, State::OffHook},
      {Trigger::PlacedOnHold, State::OnHold}
  };

  transition_table[State::OnHold] = {
      {Trigger::TakenOffHold, State::Connected},
      {Trigger::HungUp, State::OffHook}
  };

  State currentState{State::OffHook};
  State exitState{State::OnHook};

  std::vector<Trigger> states{Trigger::CallDialed, Trigger::StopUsingPhone};

  for (auto state : states) {
    std::cout << "The phone is currently " << currentState << '\n';
    std::cout << "Select a trigger:\n";

    uint32_t i = 0;
    for (auto &item : transition_table[currentState]) {
      std::cout << i++ << ". " << item.first << '\n';
    }

    currentState =
        transition_table[currentState][static_cast<int32_t>(state)].second;
    if (currentState == exitState) break;
  }

  std::cout << "We are done using the phone.\n";
}
