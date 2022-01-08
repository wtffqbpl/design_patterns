#include <iostream>

// lazy initialization.
class LzaySingleton {
private:
  static LzaySingleton* _singleton;

private:
  LzaySingleton(LzaySingleton&)=delete;
  LzaySingleton& operator=(const LzaySingleton&)=delete;

  LzaySingleton() {
    std::cout << "Constructor called." << std::endl;
  }

public:
  ~LzaySingleton() {
    std::cout << "Destructor called." << std::endl;
  }

  static LzaySingleton* getLzaySingleton() {
    if (_singleton == nullptr) {
      _singleton = new LzaySingleton;
    }

    return _singleton;
  }

  void say() const {
    std::cout << "My name is simple LzaySingleton." << std::endl;
  }
};

LzaySingleton* LzaySingleton::_singleton = nullptr;

int main()
{
  for (int i = 0; i < 3; ++i) {
    LzaySingleton* singleton = LzaySingleton::getLzaySingleton();
    std::cout << "idx = " << i << std::endl;
    singleton->say();
  }

  return 0;
}

