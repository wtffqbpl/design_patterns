#include <gtest/gtest.h>
#include <iostream>

namespace prototype {
// interface for prototype pattern.
class Prototype {
public:
  Prototype() = default;
  virtual Prototype *clone() = 0;
  virtual ~Prototype() = default;
};

class ConcretePrototypeA : public Prototype {
public:
  ConcretePrototypeA() : member_(0) { std::cout << "Default Constructor\n"; }
  ConcretePrototypeA(const ConcretePrototypeA &rhs) : member_(rhs.member_) {
    std::cout << "Copy Constructor\n";
  }

  ConcretePrototypeA *clone() override {
    std::cout << "copy of self.\n";
    return new ConcretePrototypeA(*this);
  }

  ~ConcretePrototypeA() override = default;

private:
  int member_;
};

} // end of namespace prototype

TEST(prototype, basic_demo) {
  using namespace prototype;

  // generate first obj.
  auto *conPro = new ConcretePrototypeA();

  // copy self.
  ConcretePrototypeA *conPro1 = conPro->clone();

  // copy self.
  ConcretePrototypeA *conPro2 = conPro->clone();

  delete conPro;
  conPro = nullptr;

  delete conPro1;
  conPro1 = nullptr;

  delete conPro2;
  conPro2 = nullptr;
}