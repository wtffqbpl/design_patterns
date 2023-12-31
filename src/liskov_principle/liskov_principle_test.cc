#include <gtest/gtest.h>

namespace {

class Rectangle {
protected:
  int width, height;

public:
  explicit Rectangle(const int width_, const int height_)
      : width{width_}, height{height_}
  {}

  [[nodiscard]] int get_width() const { return width; }
  virtual void set_width(const int width_) { this->width = width_; }

  [[nodiscard]] int get_height() const { return height; }
  virtual void set_height(const int height_) { this->height = height_; }

  [[nodiscard]] int area() const { return this->width * this->height; }
};

#if 0
// Square class不应该存在，因为Square不应该同时存在set_width 和 set_height api.
class Square : public Rectangle {
public:
  explicit Square(int size)
      : Rectangle(size, size)
  {}

  void set_width(const int width_) override {
    this->width = this->height = width_;
  }

  void set_height(const int height_) override {
    this->height = this->width = height_;
  }
};
#endif

// More better:
struct RectangleFactory {
  static Rectangle create_rectangle(int w, int h) { return Rectangle{w, h}; }
  static Rectangle crate_square(int size) {return Rectangle{size, size}; }
};

// And provide a function for square checking.
// bool Rectangle::is_square() const { return height == width; }

}