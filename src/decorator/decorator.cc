#include <gtest/gtest.h>
#include <iostream>
#include <string>

// interface for decorator and object.
class Pancake {
protected:
  Pancake() = default;
  explicit Pancake(std::string desc) : description(std::move(desc)) {}

public:
  Pancake(const Pancake &pan) : description(pan.getDescription()) {}
  Pancake &operator=(const Pancake &pan) {
    this->description = pan.getDescription();
    return *this;
  }

  [[nodiscard]] virtual std::string getDescription() const { return description; }
  [[nodiscard]] virtual double cost() const = 0;

public:
  std::string description{"Basic Pancake"};
};

// 装饰器的基类
class CondimentDecorator : public Pancake {
public:
  [[nodiscard]] std::string getDescription() const override {
    return Pancake::getDescription ();
  }
};

// 肉饼
class MeatPancake : public Pancake {
public:
  MeatPancake() : Pancake("MeatPancake") {}
  [[nodiscard]] double cost() const override { return 6; }
};

// 鸡蛋饼
class EggPancake : public Pancake {
public:
  EggPancake() : Pancake("EggPancake") {}
  [[nodiscard]] double cost() const override { return 5; }
};

// 额外加鸡蛋
class Egg : public CondimentDecorator {
public:
  explicit Egg(Pancake *d) : base(*d) {}
  [[nodiscard]] std::string getDescription() const override {
    return base.getDescription() + ", Egg";
  }
  [[nodiscard]] double cost() const override { return base.cost() + 1.5; }

private:
  Pancake &base;
};

// 额外加土豆
class Potato : public CondimentDecorator {
public:
  explicit Potato(Pancake *d) : base(*d) {}
  [[nodiscard]] std::string getDescription() const override {
    return base.getDescription() + ", Potato";
  }
  [[nodiscard]] double cost() const override { return base.cost() + 1; }

private:
  Pancake &base;
};

// 额外加培根
class Bacon : public CondimentDecorator {
public:
  explicit Bacon(Pancake *d) : base(*d) {}
  [[nodiscard]] std::string getDescription() const override {
    return base.getDescription() + ", Bacon";
  }
  [[nodiscard]] double cost() const override { return base.cost() + 2; }

private:
  Pancake &base;
};

TEST(decorator, basic_test){
  Pancake* pan = new EggPancake();
  // 增加额外的东西
  pan = (Pancake*)(new Bacon(new Potato(pan)));

  std::cout << pan->getDescription() << ", $: " << pan->cost() << std::endl;
}