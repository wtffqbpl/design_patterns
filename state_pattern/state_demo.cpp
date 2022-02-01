#include <iostream>
#include <ostream>
#include <memory>

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
/* --------------------------------------------------------------------------------- */

/* ---------------------------------------- States ---------------------------------- */
struct State {
  virtual std::unique_ptr<State> on_event(Event event) = 0;

  virtual ~State() {}
};

struct Idle : State {
  std::unique_ptr<State> on_event(Event event);
};

struct Connecting : State {
  std::unique_ptr<State> on_event(Event event);
private:
  uint32_t                 m_trial = 0;
  static constexpr uint8_t m_max_trial = 3;
};

struct Connected : State {
  std::unique_ptr<State> on_event(Event event);
};
/* --------------------------------------------------------------------------------- */

/*********************************** Transitions *************************************/
std::unique_ptr<State> Idle::on_event(Event event) {
  std::cout << "Idle -> " << event << std::endl;
  if (event == Event::connect) return std::make_unique<Connecting>();
  return nullptr;
}

std::unique_ptr<State> Connecting::on_event(Event event) {
  std::cout << "Connecting -> " << event << std::endl;
  switch (event) {
    default: break;
    case Event::connected: return std::make_unique<Connected>();
    case Event::timeout: return ++m_trial < m_max_trial ? nullptr : std::make_unique<Idle>();
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
  void establish_conntection(Events... e) {(dispatch(e), ...);}
};

int main() {
  Bluetooth bl;
  bl.establish_conntection(Event::connect, Event::timeout, Event::connected, Event::disconnect);

  return 0;
}




