using namespace std;
#include "NABasicObject.h"

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
    cout<<"**********case test_delete_first start***********"<<endl;

    int * ptr1 = new (NULLHEAP) int(2);
    int * ptr2 = new (NULLHEAP) int[10];
    test * ptr3 = new (NULLHEAP) test(0);
    /*
      delete ptr; 调用的就是 void operator delete(void * ptr); 而与new 匹配的delete 不是自己调用的
      ，而是在new申请,成功却在构造函数时候出错，new operator自己根据operator new 来寻找 
      对应的operator delete 来调用，稍后介绍。
    */
    delete ptr1;
    cout<<"----------case test_delete_first result---------"<<endl;

    memory_scan(&gGlobalMemory);
    delete [] ptr2;
    delete ptr3;
    cout<<"**********case test_delete_first end***********"<<endl<<endl;
}

void test_delete_median()
{
    cout<<"**********case test_delete_median start***********"<<endl;
    int * ptr1 = new (NULLHEAP) int(2);
    int * ptr2 = new (NULLHEAP) int[10];
    test * ptr3 = new (NULLHEAP) test(0);
    /*
      delete ptr; 调用的就是 void operator delete(void * ptr); 而与new 匹配的delete 不是自己调用的
      ，而是在new申请,成功却在构造函数时候出错，new operator自己根据operator new 来寻找 
      对应的operator delete 来调用，稍后介绍。
    */
    delete [] ptr2;
    cout<<"----------case test_delete_median result---------"<<endl;

    memory_scan(&gGlobalMemory);    
    delete ptr1;
    delete ptr3;
    cout<<"**********case test_delete_median end***********"<<endl<<endl;
}

void test_delete_end()
{
    cout<<"**********case test_delete_end start***********"<<endl;
    int * ptr1 = new (NULLHEAP) int(2);
    int * ptr2 = new (NULLHEAP) int[10];
    test * ptr3 = new (NULLHEAP) test(0);
    /*
      delete ptr; 调用的就是 void operator delete(void * ptr); 而与new 匹配的delete 不是自己调用的
      ，而是在new申请,成功却在构造函数时候出错，new operator自己根据operator new 来寻找 
      对应的operator delete 来调用，稍后介绍。
    */
    delete ptr3;
    cout<<"----------case test_delete_end result---------"<<endl;

    memory_scan(&gGlobalMemory);    
    delete ptr1;
    delete [] ptr2;
    cout<<"**********case test_delete_end end***********"<<endl<<endl;
}

void test_delete_all()
{
    cout<<"**********case test_delete_all start***********"<<endl;
    int * ptr1 = new (NULLHEAP) int(2);
    int * ptr2 = new (NULLHEAP) int[10];
    test * ptr3 = new (NULLHEAP) test(0);
    /*
      delete ptr; 调用的就是 void operator delete(void * ptr); 而与new 匹配的delete 不是自己调用的
      ，而是在new申请,成功却在构造函数时候出错，new operator自己根据operator new 来寻找 
      对应的operator delete 来调用，稍后介绍。
    */
    delete ptr1;
    delete [] ptr2;
    delete ptr3;
    cout<<"----------case test_delete_all result---------"<<endl;

    memory_scan(&gGlobalMemory);
    cout<<"**********case test_delete_all end***********"<<endl<<endl;
}

void test_delete_nothrow()
{
    cout<<"**********case test_delete_nothrow start***********"<<endl;
    int * ptr1 = new (nothrow) int(2);
    cout<<"----------case test_delete_nothrow result---------"<<endl;

    memory_scan(&gGlobalMemory);
    delete ptr1;
    cout<<"**********case test_delete_nothrow end***********"<<endl<<endl;
}

void test_delete_new()
{
    cout<<"**********case test_delete_new start***********"<<endl;
    int * ptr1 = new int(2);
    int * ptr2 = new int[10];
    cout<<"----------case test_delete_nothrow result---------"<<endl;

    memory_scan(&gGlobalMemory);
    delete ptr1;
    delete [] ptr2;
    cout<<"**********case test_delete_new end***********"<<endl<<endl;
}

void test_two_parameters_new()
{
    cout<<"**********case test_two_parameters_new start***********"<<endl;
    int * ptr1 = new (NULLHEAP, false)int(2);
    int * ptr2 = new (NULLHEAP, false)int[2];
    cout<<"----------case test_two_parameters_new result---------"<<endl;

    memory_scan(&gGlobalMemory);
    delete ptr1;
    delete ptr2;
    cout<<"**********case test_two_parameters_new end***********"<<endl<<endl;
}

int main()
{
    test_delete_first();
    test_delete_median();
    test_delete_end();
    test_delete_all();
    test_delete_nothrow();
    test_delete_new();
    test_two_parameters_new();

    return 0;
}
