#include <iostream>
#include <string>

// interface for decorator and object.
class Pancake {
public:
  virtual std::string getDescription() {
    return description;
  }
  virtual double cost() = 0;

public:
  std::string description = "Basic Pancake";
};

// 装饰器的基类
class CondimentDecorator : public Pancake {
public:
  std::string getDescription() override {
    return Pancake::getDescription();
  }
};

// 肉饼
class MeatPancake : public Pancake {
public:
  MeatPancake() {
    description = "MeatPancake";
  }
  double cost() override {
    return 6;
  }
};

// 鸡蛋饼
class EggPancake : public Pancake {
public:
  EggPancake() {
    description = "EggPancake";
  }
  double cost() override {
    return 5;
  }
};

// 额外加鸡蛋
class Egg : public CondimentDecorator {
public:
  Egg(Pancake* d) : base(d) {}
  std::string getDescription() override {
    return base->getDescription() + ", Egg";
  }
  double cost() override {
    return base->cost() + 1.5;
  }

private:
  Pancake* base;
};

// 额外加土豆
class Potato : public CondimentDecorator {
public:
  Potato(Pancake* d) : base(d) {}
  std::string getDescription() override {
    return base->getDescription() + ", Potato";
  }
  double cost() override {
    return base->cost() + 1;
  }

private:
  Pancake* base;
};

// 额外加培根
class Bacon : public CondimentDecorator {
public:
  Bacon(Pancake* d) : base(d) {}
  std::string getDescription() override {
    return base->getDescription() + ", Bacon";
  }
  double cost() override {
    return base->cost() + 2;
  }

private:
  Pancake* base;
};

int main()
{
  Pancake* pan = new EggPancake();
  // 增加额外的东西
  pan = (Pancake*)(new Bacon(new Potato(pan)));

  std::cout << pan->getDescription() << ", $: " << pan->cost() << std::endl;

  return 0;
}


