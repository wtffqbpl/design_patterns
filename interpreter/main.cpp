#include <iostream>
#include <string>

class Context {
private:
  std::string input;
  std::string output;

public:
  void SetInput(std::string i) {
    input = i;
  }

  std::string GetInput() {
    return input;
  }

  void SetOutput(std::string o) {
    output = o;
  }

  std::string GetOutput() {
    return output;
  }
};


class AbastractExpression {
public:
  virtual void Interpreter(Context *context) = 0;
  virtual ~AbastractExpression() {}
};

class TerminalExpression : public AbastractExpression {
public:
  void Interpreter(Context *context) {
    std::cout << "TerminalExpression: " << context->GetInput() << ", " <<
                                           context->GetOutput() << std::endl;
  }
};

class NontermianlExpression : public AbastractExpression {
private:
  AbastractExpression *expression;

public:
  NontermianlExpression(AbastractExpression *e) {
    expression = e;
  }

  void Interpreter(Context *context) {
    std::cout << "NontermianlExpression: " << context->GetInput() << ", " <<
      context->GetOutput() << std::endl;
    expression->Interpreter(context);
  }
};

int main() {
  Context *c = new Context();
  c->SetInput("Hello");
  c->SetOutput("World");

  AbastractExpression *exp1 = new TerminalExpression();
  AbastractExpression *exp2 = new NontermianlExpression(exp1);

  exp1->Interpreter(c);
  exp2->Interpreter(c);

  delete exp1;
  delete exp2;

  return 0;
}

