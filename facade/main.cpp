#include <iostream>

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
  void start() override {
    std::cout << "Host start" << std::endl;
  }

  void shutdown() override {
    std::cout << "Host shutdown" << std::endl;
  }
};

// 子控件，显示器
class LCDDisplay : public Control {
public:
  void start() override {
    std::cout << "LCD Display start" << std::endl;
  }

  void shutdown() override {
    std::cout << "LCD Display shutdown" << std::endl;
  }
};

// 子控件，外部设备
class Peripheral : public Control {
public:
  void start() override {
    std::cout << "Peripheral start" << std::endl;
  }

  void shutdown() override {
    std::cout << "Peripheral shutdown" << std::endl;
  }
};

class Computer {
public:
  void start() {
    m_host.start();
    m_display.start();
    m_Peripheral->start();
    std::cout << "Computer start" << std::endl;
  }

  void shutdown() {
    m_host.shutdown();
    m_display.shutdown();
    m_Peripheral->shutdown();
    std::cout << "Computer shutdown" << std::endl;
  }

private:
  Host m_host;
  LCDDisplay m_display;
  std::shared_ptr<Peripheral> m_Peripheral= std::make_shared<Peripheral>();
};

int main()
{
  Computer computer;

  computer.start();

  std::cout << "======= Client do something else !=======" << std::endl;

  computer.shutdown();

  return 0;
}
