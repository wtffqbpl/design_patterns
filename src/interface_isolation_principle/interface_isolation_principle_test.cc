#include <gtest/gtest.h>
#include <iostream>

namespace {

class Document;

struct IPrinter {
  virtual void print(std::vector<Document *> docs) = 0;
};

struct IScanner {
  virtual void scan(std::vector<Document *> docs) = 0;
};

struct IFax {
  virtual void fax(std::vector<Document *> docs) = 0;
};

struct Printer : IPrinter {
  void print(std::vector<Document *> docs) override {}
};

struct Scanner : IScanner {
  void scan(std::vector<Document *> docs) override {}
};

struct IMachine : IPrinter, IScanner {
};

struct Machine : IMachine {
  IPrinter &printer;
  IScanner &scanner;

  Machine(IPrinter &printer_, IScanner &scanner_)
    : printer{printer_}
    , scanner{scanner_}
  {}

  void print(std::vector<Document *> docs) override {
    printer.print(std::move(docs));
  }

  void scan(std::vector<Document *> docs) override {
    scanner.scan(std::move(docs));
  }
};

}