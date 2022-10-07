#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

namespace mediator {

struct ChatRoom {
  virtual void broadcast(std::string from, std::string msg) = 0;
  virtual void message(std::string from, std::string to, std::string msg) = 0;
};

struct Person {
  std::string m_name;
  ChatRoom *m_room{nullptr};
  std::vector<std::string> m_chat_log;

  explicit Person(std::string n) : m_name(std::move(n)) {}

  void say(std::string msg) const { m_room->broadcast(m_name, std::move(msg)); }

  void pm(std::string to, std::string msg) const {
    m_room->message(m_name, std::move(to), std::move(msg));
  }

  void receive(const std::string &from, const std::string &msg) {
    std::string s{from + ": \"" + msg + "\""};
    std::cout << "[" << m_name << "'s chat session]" << s << "\n";
    m_chat_log.emplace_back(s);
  }
};

struct GoogleChat : ChatRoom {
  std::vector<Person *> m_people;

  void broadcast(std::string from, std::string msg) override {
    for (auto p : m_people)
      if (p->m_name != from)
        p->receive(from, msg);
  }

  void join(Person *p) {
    std::string join_msg = p->m_name + " joins the chat";
    broadcast("room", join_msg);
    p->m_room = this;
    m_people.push_back(p);
  }

  void message(std::string from, std::string to, std::string msg) override {
    auto target =
        std::find_if(std::begin(m_people), std::end(m_people),
                     [&](const Person *p) { return p->m_name == to; });
    if (target != std::end(m_people))
      (*target)->receive(from, msg);
  }
};

} // end of namespace mediator

TEST(mediator, basic_demo) {
  using namespace mediator;

  GoogleChat room;
  Person john{"John"};
  Person jane{"Jane"};
  room.join(&john);
  room.join(&jane);
  john.say("hi room");
  jane.say("oh, hey john");

  Person simon{"Simon"};
  room.join(&simon);
  simon.say("hi everyone!");

  jane.pm("Simon", "glad you found us, simon!");
}