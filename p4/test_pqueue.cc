#include <gtest/gtest.h>

#include <functional>

#include "pqueue.h"

TEST(PQueue, less) {
  PQueue<int> pq;

  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);

  // more tests added
  EXPECT_EQ(pq.Top(), 2);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 23);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 34);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 42);
  pq.Pop();
  EXPECT_THROW(pq.Top(), std::exception);
  EXPECT_EQ(pq.Size(), 0);
}

TEST(PQueue, great) {
  PQueue<int, std::greater<int>> pq;

  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);

  // more tests added
  // custom class and pointer follow same logic
  EXPECT_EQ(pq.Top(), 42);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 34);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 23);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 2);
  pq.Pop();
  EXPECT_THROW(pq.Top(), std::exception);
  EXPECT_EQ(pq.Size(), 0);
}

class MyClass {
 public:
  explicit MyClass(int n) : n_(n) {}
  bool operator < (const MyClass &mc) const { return n_ < mc.n_; }
  bool operator () (const MyClass* mcp_1, const MyClass* mcp_2) const {
    return *mcp_1 < *mcp_2;
  }
  int n() { return n_; }
 private:
  int n_;
};

struct MyClassPtrComp {
  bool operator () (const MyClass* mcp_1, const MyClass* mcp_2) const {
    return *mcp_1 < *mcp_2;
  }
};

TEST(PQueue, custom_class) {
  std::vector<MyClass> vec{MyClass(42), MyClass(23), MyClass(2), MyClass(34)};

  PQueue<MyClass> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top().n(), vec[2].n());
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top().n(), vec[1].n());
}

// #if 0
TEST(PQueue, custom_class_pointer) {
  std::vector<MyClass*> vec{new MyClass(42), new MyClass(23),
                            new MyClass(2), new MyClass(34)};

  PQueue<MyClass*, MyClassPtrComp> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top(), vec[2]);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), vec[1]);
}
// #endif

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
