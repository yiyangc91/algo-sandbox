#pragma once

#include <memory>
#include <numeric>
#include <variant>
#include <string>
#include <sstream>

// Solutions are required to output a tree that represents the operations that
// were taken with the numbers.

enum class Operation : char {
   Add = '+',
   Subtract = '-',
   Divide = '/',
   Multiply = '*',
};

class Node {
   int numerator;
   unsigned int denominator;

   virtual Node* cloneImpl() const {
      return new Node(numerator, denominator);
   }

   static std::pair<int, unsigned int> calculate(Node* left, Node* right, Operation op) {
      switch (op) {
         case Operation::Add: {
            int numer = left->numerator*right->denominator
               + right->numerator*left->denominator;
            unsigned int denom = left->denominator * right->denominator;
            unsigned int divisor = std::gcd(numer, denom);

            if (!divisor) return {0,0};
            return { numer/(int)divisor, denom/divisor };
         }
         case Operation::Subtract: {
            int numer = left->numerator*right->denominator
               - right->numerator*left->denominator;
            unsigned int denom = left->denominator * right->denominator;
            unsigned int divisor = std::gcd(numer, denom);
            if (!divisor) return {0,0};

            return { numer/(int)divisor, denom/divisor };
         }
         case Operation::Divide: {
            int n;
            unsigned int n2;
            if (right->numerator < 0) {
               n = -left->numerator;
               n2 = -right->numerator;
            } else {
               n = left->numerator;
               n2 = right->numerator;
            }
            int numer = n * right->denominator;
            unsigned int denom = left->denominator * n2;
            unsigned int divisor = std::gcd(numer, denom);

            if (!divisor) return {0,0};
            return { numer/(int)divisor, denom/divisor };
         }
         case Operation::Multiply: {
            int numer = left->numerator * right->numerator;
            unsigned int denom = left->denominator * right->denominator;
            unsigned int divisor = std::gcd(numer, denom);
            if (!divisor) return {0,0};

            return { numer/(int)divisor, denom/divisor };
         }
         default:
            abort();
      }
   }

public:
   Node(int numerator, unsigned int denominator)
      : numerator(numerator), denominator(denominator) {}
   Node(std::pair<int, unsigned int> p)
      : numerator(p.first), denominator(p.second) {}
   Node(Node* left, Node* right, Operation op)
      : Node(calculate(left, right, op)) {}
   virtual ~Node() = default;

   // Copy constructor and assignment operators
   Node(const Node&) = default;
   Node& operator=(const Node&) = default;

   // prevent slicing
   template <typename T,
            std::enable_if_t<!std::is_same_v<T, Node>, int> = 0>
   Node(const T&) = delete;
   template <typename T,
            std::enable_if_t<!std::is_same_v<T, Node>, int> = 0>
   Node& operator=(const T&) = delete;

   std::unique_ptr<Node> clone() const {
      return std::unique_ptr<Node>(cloneImpl());
   }
   
   // This copies out the whole subtree, flattening out
   // any OpRaws into actual ownership of the nodes.

   // hack:
   // comparisons defined here are for sorting nodes within
   // the node list.
   // thus for those intents and purposes two nodes are
   // equivalent if they are the same value (numerator
   // and denominators are normalized via gcd division)

   bool operator<(const Node& o) const {
      // totally arbitrary order
      if (this->denominator < o.denominator) return true;
      return this->numerator < o.numerator;
   }

   bool operator==(const Node& o) const {
      return this->denominator == o.denominator &&
         this->numerator == o.numerator;
   }

   virtual std::string string() const {
      std::stringstream ss {};
      ss << numerator;
      if (denominator != 1) {
         ss << '/';
         ss << denominator;
      }
      return ss.str();
   }

   int getNumerator() const { return numerator; }
   unsigned int getDenominator() const { return denominator; }
};

class Op : public Node {
   std::unique_ptr<Node> left;
   std::unique_ptr<Node> right;
   Operation op;

   virtual Node* cloneImpl() const override {
      return new Op(left->clone(), right->clone(), op);
   }

public:
   Operation getOperation() const { return op; }

   Op(std::unique_ptr<Node>&& left,
         std::unique_ptr<Node>&& right,
         Operation op):
      left(std::move(left)), right(std::move(right)), op(op),
      Node(left.get(), right.get(), op) {}
   virtual ~Op() override = default;

   // Can't copy unique_ptrs anyway.
   Op(const Op&) = delete;
   Op& operator=(const Op&) = delete;

   virtual std::string string() const override {
      std::stringstream ss {};
      ss << '(' << left->string();
      ss << static_cast<std::underlying_type_t<Operation>>(op);
      ss << right->string() << ')';
      return ss.str();
   }
};

// Op that does not own the nodes it points to.
class OpRaw : public Node {
   Node* left;
   Node* right;
   Operation op;

   virtual Node* cloneImpl() const override {
      return new Op(left->clone(), right->clone(), op);
   }

public:
   OpRaw(Node* left, Node* right, Operation op):
      left(left), right(right), op(op),
      Node(left, right, op) {}
   virtual ~OpRaw() override = default;
   OpRaw(const OpRaw&) = default;
   OpRaw& operator=(const OpRaw&) = default;

   Operation getOperation() const { return op; }

   virtual std::string string() const {
      std::stringstream ss {};
      ss << '(' << left->string();
      ss << static_cast<std::underlying_type_t<Operation>>(op);
      ss << right->string() << ')';
      return ss.str();
   }
};

class NodeList {
};
