#include <gtest/gtest.h>
#include <iostream>
#include <list>
#include <string>
#include <utility>

namespace observer {
class Observer;

class Subject {
public:
  virtual ~Subject() = default;
  virtual void registerObsvr(Observer *obsvr) = 0;
  virtual void removeObsvr(Observer *obsvr) = 0;
  virtual void notifyObsvrs(const std::string &msg) = 0;
};

class Observer {
public:
  virtual ~Observer() = default;
  virtual void Update(const std::string &msg) = 0;
  virtual std::string getName() const = 0;
};

class QQGroup : public Subject {
public:
  QQGroup() { _observers = new std::list<Observer *>(); }
  void registerObsvr(Observer *obsvr) override;
  void removeObsvr(Observer *obsvr) override;
  void notifyObsvrs(const std::string &msg) override;

private:
  std::list<Observer *> *_observers;
};

void QQGroup::registerObsvr(Observer *obsvr) { _observers->push_back(obsvr); }

void QQGroup::removeObsvr(Observer *obsvr) {
  if (!_observers->empty())
    _observers->remove(obsvr);
}

void QQGroup::notifyObsvrs(const std::string &msg) {
  std::cout << "Group msg: " << msg << std::endl;
  auto iter = _observers->begin();

  for (; iter != _observers->end(); ++iter) {
    (*iter)->Update(msg);
  }
}

class RoomMate : public Observer {
private:
  std::string name_;
  std::string action_;
  std::string now_;

public:
  explicit RoomMate(std::string name, std::string now, std::string action)
    : name_(std::move(name)), action_(std::move(action)), now_(std::move(now)) {}

  void Update(const std::string &msg) override;
  [[nodiscard]] std::string getName() const override { return name_; };
};

void RoomMate::Update(const std::string &msg) {
  std::cout << "This is " << name_ << std::endl;
  if (msg == "checked")
    std::cout << "action: " << action_ << std::endl << std::endl;
  else
    std::cout << "Go on: " << now_ << std::endl << std::endl;
}

} // end of namespace observer

TEST(observer, basic_demo) {
  using namespace observer;

  auto *B = new RoomMate("B",
      "sleeping",
      "get dressed and run to classroom");
  auto *C = new RoomMate("C",
      "playing games",
      "pay the fee and run to classroom");
  auto *D = new RoomMate("D",
      "shopping with girl friend",
      "go back to school and be worried about girl friend's agnry");

  auto *qqgroup = new QQGroup();
  qqgroup->registerObsvr(B);
  qqgroup->registerObsvr(C);
  qqgroup->registerObsvr(D);

  qqgroup->notifyObsvrs("not checked");
  qqgroup->notifyObsvrs("checked");
}

namespace observer_new {
/**
 * Observer Design Pattern
 * Intent: Lets you define a subscription mechanism to notify multiple objects
 * about any events that happen to the object they're observing.
 *
 * Note that there's a lot of different terms with similar meaning associated
 * with this pattern. Just remember that the Subject is also called the
 * Publisher and the Observer is often called the Subscriber and vice versa.
 * Also the verbs "observe", "listen" or "track" ususally mean the same thing.
 */

class IObserver {
public:
  virtual ~IObserver() = default;
  virtual void Update(const std::string &message_from_subject) = 0;
};

// TODO: 利用模版来实现接口
class ISubject {
public:
  virtual ~ISubject() = default;
  virtual void Attach(IObserver *observer) = 0;
  virtual void Detach(IObserver *observer) = 0;
  virtual void Notify() = 0;
};

/**
 * The Subject owns some important state and notifies observers when
 * the state changes.
 */

class Subject : public ISubject {
public:
  ~Subject() override {
    std::cout << "Goodbye, I was the Subject.\n";
  }

  /**
   * The subscription management methods.
   */
  void Attach(IObserver *observer) override {
    list_observer_.push_back(observer);
  }

  void Detach(IObserver *observer) override {
    list_observer_.remove(observer);
  }

  void Notify() override {
    auto iterator = list_observer_.begin();
    HowManyObserver();

    while (iterator != list_observer_.end()) {
      (*iterator)->Update(message_);
      ++iterator;
    }
  }

  void CreateMessage(std::string message = "Empty") {
    this->message_ = std::move(message);
    Notify();
  }

  void HowManyObserver() {
    std::cout << "There are " << list_observer_.size() << " observers in the list.\n";
  }

  /**
   * Usually, the subscription logic is only a fraction of what a Subject can
   * really do. Subjects commonly hold some important business logic, that
   * triggers a notification method whenever something important is about to
   * happen (or after it).
   */
  void SomeBusinessLogic() {
    this->message_ = "Change message message";
    Notify();
    std::cout << "I'm about to do some thing important.\n";
  }
private:
  std::list<IObserver *> list_observer_;
  std::string message_;
};

class Observer : public IObserver {
public:
  explicit Observer(Subject &subject) : subject_(subject) {
    this->subject_.Attach(this);
    std::cout << "Hi, I'm the Observer \"" << ++Observer::static_number_ << "\".\n";
    this->number_ = Observer::static_number_;
  }

  ~Observer() override {
    std::cout << "Goodbye, I was the Observer \"" << this->number_ << "\".\n";
  }

  void Update(const std::string &message_from_subject) override {
    message_from_subject_ = message_from_subject;
    PrintInfo();
  }

  void RemoveMeFromTheList() {
    subject_.Detach(this);
    std::cout << "Observer \"" << this->number_ << "\" removed from the list.\n";
  }

  void PrintInfo() {
    std::cout << "Observer \"" << this->number_ << "\": a new message is available --> "
              << this->message_from_subject_ << "\n";
  }

private:
  std::string message_from_subject_;
  Subject &subject_;
  static int static_number_;
  int number_;
};

int Observer::static_number_ = 0;

void ClientCode() {
  auto *subject = new Subject;
  auto *observer1 = new Observer{*subject};
  auto *observer2 = new Observer{*subject};
  auto *observer3 = new Observer{*subject};
  auto *observer4 = new Observer{*subject};
  auto *observer5 = new Observer{*subject};

  subject->CreateMessage("Hello World! :D");
  observer3->RemoveMeFromTheList();

  subject->CreateMessage("The weather is hot today! :p");
  observer4->RemoveMeFromTheList();

  subject->CreateMessage("My new car is great! ;)");
  observer5->RemoveMeFromTheList();

  observer4->RemoveMeFromTheList();
  observer1->RemoveMeFromTheList();

  delete observer5;
  delete observer4;
  delete observer3;
  delete observer2;
  delete observer1;
  delete subject;
}

} // end of namespace observer_new

TEST(observer, new_demo) {
  using namespace observer_new;
  ClientCode();
}
