#include <gtest/gtest.h>
#include <NTGS/Core/Reflection.h>
#include <memory>
#include <sstream>

using namespace NTGS;

struct TestObject : public Object {
    int mData1 = 123;
    std::string mData2 = "this is a message";
    std::map<std::string, std::string> mData3 = { {"a", "A"}, {"b", "B"} };
    float mData4[3] = { 1.0, 2.0, 3.0 };

    void Func1(double f, char& c) {
        c = 'x';
    }

    int& Func2() {
        return mData1;
    }

    const std::string& Func3()  {
        return mData2;
    }
};

TEST(Reflection, General_Test) {
    TestObject cTestObject;
    cTestObject.AddProperty("mData1", &TestObject::mData1);
    cTestObject.AddProperty("mData2", &TestObject::mData2);
    cTestObject.AddProperty("mData4", &TestObject::mData4);
    cTestObject.AddMethod("Func1", &TestObject::Func1);
    cTestObject.AddMethod("Func2", &TestObject::Func2);
    cTestObject.AddMethod("Func3", &TestObject::Func3);

    Object& pObject = cTestObject;

    EXPECT_EQ(pObject.Get<int>("mData1"), 123);
    EXPECT_EQ(pObject.Get<std::string>("mData2"), cTestObject.mData2);

    char c = 0;
    pObject.Invoke("Func1", 20.12, c);
    EXPECT_EQ(c, 'x');

    //pObject.Invoke<int&>("Func2") += 10;
    //EXPECT_EQ(pObject.Get<int>("mData1"), 123 + 10);

    std::shared_ptr<Object> cBasePtr = std::make_shared<TestObject>();
    std::shared_ptr<TestObject> cPtr = std::dynamic_pointer_cast<TestObject>(cBasePtr);
    EXPECT_EQ(cPtr->Func2(), 123);
}