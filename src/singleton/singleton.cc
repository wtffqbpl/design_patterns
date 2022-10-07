#include <gtest/gtest.h>
#include <iostream>

namespace singleton {

class MagicStaticSingleton {
public:
  MagicStaticSingleton(const MagicStaticSingleton &) = delete;
  MagicStaticSingleton operator=(const MagicStaticSingleton &) = delete;

  static MagicStaticSingleton &getInstance() {
    static MagicStaticSingleton instance;
    return instance;
  }

  void say() const {
    std::cout << "this is a MagicStaticSingleton." << std::endl;
  }

  ~MagicStaticSingleton() { std::cout << "Destructor called.\n"; }

private:
  MagicStaticSingleton() { std::cout << "Constructor called.\n"; }
};

} // end of namespace singleton

TEST(singleton, basic_demo) {
  using namespace singleton;

  for (unsigned i = 0; i < 3; ++i) {
    MagicStaticSingleton& inst = MagicStaticSingleton::getInstance();
    std::cout << "index = " << i << std::endl;
    inst.say();
  }
}

namespace lazy_singleton {
// lazy initialization.
class LazySingleton {
private:
  inline static LazySingleton* singleton_ = nullptr;

public:
  LazySingleton(LazySingleton&)=delete;
  LazySingleton& operator=(const LazySingleton&) = delete;

  LazySingleton() {
    std::cout << "Constructor called.\n";
  }

public:
  ~LazySingleton() { std::cout << "Destructor called.\n"; }

  static LazySingleton* getLzaySingleton() {
    if (singleton_ == nullptr) {
      singleton_ = new LazySingleton;
    }

    return singleton_;
  }

  void say() const {
    std::cout << "My name is simple LazySingleton.\n";
  }
};

} // end of namespace lazy_singleton

TEST(singleton, lazy_singleton_demo) {
  using namespace lazy_singleton;

  for (unsigned i = 0; i < 3; ++i) {
    LazySingleton* singleton = LazySingleton::getLzaySingleton();
    std::cout << "idx = " << i << std::endl;
    singleton->say();
  }
}