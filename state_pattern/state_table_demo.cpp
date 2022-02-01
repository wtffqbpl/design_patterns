#include <iostream>
#include <string>
#include <ostream>
#include <unordered_map>
#include <vector>

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


int main() {
  std::unordered_map<State, std::vector<std::pair<Trigger, State>>> transition_table;

  transition_table[State::OffHook] = {
    {Trigger::CallDialed, State::Connecting},
    {Trigger::StopUsingPhone, State::OnHook}
  };

  transition_table[State::Connecting] = {
    {Trigger::HungUp, State::OffHook},
    {Trigger::CallConnected, State::Connected}
  };

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

  for (;;) {
    std::cout << "The phone is currently " << currentState << std::endl;
    std::cout << "Select a trigger:\n";

    uint32_t i = 0;
    for (auto item : transition_table[currentState]) {
      std::cout << i++ << ". " << item.first << "\n";
    }

    uint32_t input;
    std::cin >> input;

    currentState = transition_table[currentState][input].second;

    if (currentState == exitState) break;
  }

  std::cout << "We are done using the phone." << std::endl;

  return 0;
}

