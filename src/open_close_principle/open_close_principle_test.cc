#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

namespace {

enum class Color { Red, Green, Blue };
enum class Size { Small, Medium, Large };

struct Product {
  std::string name;
  Color color;
  Size size;
};

template <typename T> struct Specification {
  virtual bool is_satisfied(T *item) const = 0;
};

template <typename T> struct Filter {
  [[nodiscard]] virtual std::vector<T*>
      filter(std::vector<T*> items, const Specification<T> &spec) const = 0;
};

struct ProductFilter : Filter<Product> {
  [[nodiscard]] std::vector<Product*> filter(
      std::vector<Product*> items,
      const Specification<Product> &spec) const override
  {
    std::vector<Product*> result;
    for (auto &p : items)
      if (spec.is_satisfied(p))
        result.push_back(p);
    return result;
  }
};

struct ColorSpecification : Specification<Product> {
  Color color;

  explicit ColorSpecification(const Color color_) : color{color_} {}

  bool is_satisfied(Product *item) const override {
    return item->color == color;
  }
};

struct SizeSpecification : Specification<Product> {
  Size size;

  explicit SizeSpecification(const Size size_) : size{size_} {}

  bool is_satisfied(Product *item) const override {
    return item->size == size;
  }
};

template <typename T> struct AndSpecification : public Specification<T> {
  const Specification<T> &first;
  const Specification<T> &second;

  AndSpecification(const Specification<T> &first_, const Specification<T> &second_)
    : first(first_), second(second_)
  {}

  bool is_satisfied(T *item) const override {
    return first.is_satisfied(item) && second.is_satisfied(item);
  }
};

template <typename T>
AndSpecification<T> operator&&(const Specification<T> &first,
                               const Specification<T> &second) {
  return {first, second};
}

void test1() {
  Product apple{"Apple", Color::Green, Size::Small};
  Product tree{"Tree", Color::Green, Size::Large};
  Product house{"House", Color::Blue, Size::Large};

  std::vector<Product*> all{&apple, &tree, &house};

  ProductFilter pf;
  ColorSpecification green{Color::Green};

  auto green_things = pf.filter(all, green);
  for (auto &x : green_things)
    std::cout << x->name << " is green\n";
}

void test2() {
  Product apple{"Apple", Color::Green, Size::Small};
  Product tree{"Tree", Color::Green, Size::Large};
  Product house{"House", Color::Blue, Size::Large};

  std::vector<Product*> all{&apple, &tree, &house};

  ProductFilter pf;
  ColorSpecification green{Color::Green};
  SizeSpecification large{Size::Large};

  AndSpecification<Product> green_and_large{large, green};

  auto big_green_things = pf.filter(all, green_and_large);
  for (auto &x : big_green_things)
    std::cout << x->name << " is large and green\n";
}

void test3() {
  Product apple{"Apple", Color::Green, Size::Small};
  Product tree{"Tree", Color::Green, Size::Large};
  Product house{"House", Color::Blue, Size::Large};

  std::vector<Product*> all{&apple, &tree, &house};

  ProductFilter pf;
  ColorSpecification green{Color::Green};
  SizeSpecification large{Size::Large};

  auto big_green_things = pf.filter(all, green && large);
  for (auto &x : big_green_things)
    std::cout << x->name << " is large and green\n";
}

}

TEST(open_close_principle_test, test1) {
  std::stringstream oss;

  testing::internal::CaptureStdout();
  test1();

  oss << "Apple is green\n"
         "Tree is green\n";

  auto act_output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(oss.str(), act_output);
}

TEST(open_close_principle_test, test2) {
  std::stringstream oss;
  testing::internal::CaptureStdout();

  test2();
  oss << "Tree is large and green\n";

  auto act_output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(oss.str(), act_output);
}

TEST(open_close_principle_test, test3) {
  std::stringstream oss;
  testing::internal::CaptureStdout();

  test3();
  oss <<"Tree is large and green\n";

  auto act_output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(oss.str(), act_output);
}