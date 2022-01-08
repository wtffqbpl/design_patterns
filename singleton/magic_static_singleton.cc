#include <iostream>

class MagicStaticSingleton {
public:
  ~MagicStaticSingleton() {
    std::cout << "Destructor called." << std::endl;
  }

  MagicStaticSingleton(const MagicStaticSingleton&)=delete;
  MagicStaticSingleton operator=(const MagicStaticSingleton&)=delete;

  static MagicStaticSingleton& getInstance() {
    static MagicStaticSingleton instance;
    return instance;
  }

  void say() const {
    std::cout << "this is a MagicStaticSingleton." << std::endl;
  }

private:
  MagicStaticSingleton() {
    std::cout << "Constructor called." << std::endl;
  }
};

int main()
{
  for (int i = 0; i < 3; ++i) {
    MagicStaticSingleton& inst = MagicStaticSingleton::getInstance();
    std::cout << "index = " << i << std::endl;
    inst.say();
  }
  return 0;
}
