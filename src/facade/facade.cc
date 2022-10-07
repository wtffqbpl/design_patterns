#include <gtest/gtest.h>
#include <iostream>

namespace facade {
// 抽象控件类，提供接口
class Control {
public:
  virtual ~Control() = default;
  virtual void start() = 0;
  virtual void shutdown() = 0;
};

// 子控件，主机
class Host : public Control {
public:
  void start() override { std::cout << "Host start" << std::endl; }
  void shutdown() override { std::cout << "Host shutdown" << std::endl; }
};

// 子控件，显示器
class LCDDisplay : public Control {
public:
  void start() override { std::cout << "LCD Display start" << std::endl; }
  void shutdown() override { std::cout << "LCD Display shutdown" << std::endl; }
};

// 子控件，外部设备
class Peripheral : public Control {
public:
  void start() override { std::cout << "Peripheral start" << std::endl; }
  void shutdown() override { std::cout << "Peripheral shutdown" << std::endl; }
};

class Computer {
public:
  void start() {
    host_.start();
    display_.start();
    peripheral_->start();
    std::cout << "Computer start" << std::endl;
  }

  void shutdown() {
    host_.shutdown();
    display_.shutdown();
    peripheral_->shutdown();
    std::cout << "Computer shutdown" << std::endl;
  }

private:
  Host host_{};
  LCDDisplay display_{};
  std::shared_ptr<Peripheral> peripheral_ = std::make_shared<Peripheral>();
};

} // end of namespace facade

TEST(facade, basic_demo) {
  facade::Computer computer;

  computer.start();

  std::cout << "======= Client do something else !=======" << std::endl;

  computer.shutdown();
}