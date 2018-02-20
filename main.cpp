using namespace std;
#include "overwritemem.h"

class test
{
public:
    test(int a) {a_ = a;}

private:
    int a_;
    int b_;
};

void test_delete_first()
{
    int * ptr1 = new int(2);
    int * ptr2 = new int[10];
    test * ptr3 = new test(0);
    /*
      delete ptr; 调用的就是 void operator delete(void * ptr); 而与new 匹配的delete 不是自己调用的
      ，而是在new申请,成功却在构造函数时候出错，new operator自己根据operator new 来寻找 
      对应的operator delete 来调用，稍后介绍。
    */
    delete ptr1;
    cout<<"**********case test_delete_first***********"<<endl;

    memory_scan(&gGlobalMemory);
    delete ptr2;
    delete ptr3;
}

void test_delete_median()
{
    int * ptr1 = new int(2);
    int * ptr2 = new int[10];
    test * ptr3 = new test(0);
    /*
      delete ptr; 调用的就是 void operator delete(void * ptr); 而与new 匹配的delete 不是自己调用的
      ，而是在new申请,成功却在构造函数时候出错，new operator自己根据operator new 来寻找 
      对应的operator delete 来调用，稍后介绍。
    */
    delete [] ptr2;
    cout<<"**********case test_delete_median***********"<<endl;

    memory_scan(&gGlobalMemory);    
    delete ptr1;
    delete ptr3;
}

void test_delete_end()
{
    int * ptr1 = new int(2);
    int * ptr2 = new int[10];
    test * ptr3 = new test(0);
    /*
      delete ptr; 调用的就是 void operator delete(void * ptr); 而与new 匹配的delete 不是自己调用的
      ，而是在new申请,成功却在构造函数时候出错，new operator自己根据operator new 来寻找 
      对应的operator delete 来调用，稍后介绍。
    */
    delete ptr3;
    cout<<"**********case test_delete_end***********"<<endl;

    memory_scan(&gGlobalMemory);    
    delete ptr1;
    delete ptr2;
}

void test_delete_all()
{
    int * ptr1 = new int(2);
    int * ptr2 = new int[10];
    test * ptr3 = new test(0);
    /*
      delete ptr; 调用的就是 void operator delete(void * ptr); 而与new 匹配的delete 不是自己调用的
      ，而是在new申请,成功却在构造函数时候出错，new operator自己根据operator new 来寻找 
      对应的operator delete 来调用，稍后介绍。
    */
    delete ptr1;
    delete ptr2;
    delete ptr3;
    cout<<"**********case test_delete_all***********"<<endl;

    memory_scan(&gGlobalMemory);    
}


int main()
{
    test_delete_first();
    test_delete_median();
    test_delete_end();
    test_delete_all();

    return 0;
}
