#include <iostream>

// interface for prototype pattern.
class Prototype {
public:
  Prototype() {}
  virtual ~Prototype() {}

  virtual Prototype* clone() = 0;
};

class ConcretePrototypeA : public Prototype {
public:
  ConcretePrototypeA() : member(0) {
    std::cout << "Constructor" << std::endl;
  }
  ~ConcretePrototypeA() {}

  ConcretePrototypeA(const ConcretePrototypeA &rhs) {
    member = rhs.member;
  }

  virtual ConcretePrototypeA* clone() {
    std::cout << "copy of self." << std::endl;
    return new ConcretePrototypeA(*this);
  }

private:
  int member;
};

int main()
{
  // generate first obj.
  ConcretePrototypeA *conPro = new ConcretePrototypeA();

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

  return 0;
}

