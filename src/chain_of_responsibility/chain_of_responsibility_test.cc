#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

/**
 * 下面这个职责链模式的示例，是GoF的书上的例子，即如果当前可以处理，则不继续处理
 * 如果当前可以处理，则停止处理。
 * 大部分场景下，会用到职责链模式的另外一种变形，即会将所有的指责类都遍历一遍,
 * 不会因为当前处理了就停止了。
 */
/**
 * The Handler interface declares a method for builing the chian of handlers.
 * It also declares a method for executing a request.
 */
class Handler {
public:
  virtual Handler *SetNext(Handler *handler) = 0;
  virtual std::string Handle(std::string request) = 0;
};

/**
 * The default chaining behavior can be implemented inside a base handler class.
 */
class AbstractHandler : public Handler {
  /**
   * @var Handler
   */
private:
  Handler *next_handler_;

public:
  AbstractHandler() : next_handler_(nullptr) { }
  Handler *SetNext(Handler *handler) override {
    this->next_handler_ = handler;
    // Returning a handler from here will let us link handlers in a convenient
    // way like this:
    // $monkey->setNext($squirrel)->setNext($dog);
    return handler;
  }

  std::string Handle(std::string request) override {
    if (this->next_handler_) {
      return this->next_handler_->Handle(request);
    }

    return {};
  }
};

/**
 * All Concrete Handlers either handle a request or pass it to the next handler
 * in the chain.
 */
class MonkeyHandler : public AbstractHandler {
public:
  std::string Handle(std::string request) override {
    if (request == "Banana") {
      return "Monkey: I'll eat the " + request + ".\n";
    } else {
      return AbstractHandler::Handle(request);
    }
  }
};

class SquirrelHandler : public AbstractHandler {
public:
  std::string Handle(std::string request) override {
    if (request == "Nut") {
      return "Squirrel: I'll eat the " + request + ".\n";
    } else {
      return AbstractHandler::Handle(request);
    }
  }
};

class DogHandler : public AbstractHandler {
public:
  std::string Handle(std::string request) override {
    if (request == "MeatBall") {
      return "Dog: I'll eat the " + request + ".\n";
    } else {
      return AbstractHandler::Handle(request);
    }
  }
};

/**
 * The client code is usually suited to work with a single handler. In most
 * cases, it is not even aware that the handler is part of a chain.
 */
void ClientCode(Handler &handler) {
  std::vector<std::string> food = {"Nut", "Banana", "Cup of coffee"};

  for (const std::string &f : food) {
    std:: cout << "Client: Who wants a " << f << "?\n";
    const std::string result = handler.Handle(f);

    if (!result.empty()) {
      std::cout << " " << result;
    } else {
      std::cout << " " << f << " was left untouched.\n";
    }
  }
}

/**
 * @brief The other part of the client code constructs the actual chain.
 */
TEST(chain_of_responsibility, basic_test){
  auto *monkey = new MonkeyHandler{};
  auto *squirrel = new SquirrelHandler{};
  auto *dog = new DogHandler{};
  monkey->SetNext(squirrel)->SetNext(dog);

  /**
   * The client should be able to send a request to any handler, not just the
   * first one in the chain.
   */
  std::cout << "Chain: Monkey > Squirrel > Dog\n\n";
  ClientCode(*monkey);
  std::cout << "\n";
  std::cout << "Subchain: Squirrel > Dog\n\n";
  ClientCode(*squirrel);

  delete monkey;
  delete squirrel;
  delete dog;
}
