#include <iostream>
using namespace std;

// Namespace
// To avoid name conflicts, we place declarations inside a namespace.
// To access them, use the :: operator (e.g., MY_SPACE::Function).
// 'using namespace' disables the need for :: by bringing everything into scope.
// 'using namespace::item' disables namespace requirement only for that specific item.

namespace MY_SPACE
{
    void IntChange(int* _Data)
    {
        *_Data = 200;
    }

    void IntChange(int& _Data)
    {
        _Data = 400;
    }

    namespace SUB_SPACE
    {
        void IntPtrChange(int*& _Data)
        {
            _Data = (int*)10;
        }

        void IntPtrChange(int** _Data)
        {
            *_Data = (int*)4;
        }
    }

    int g_Int = 0; // Global variable inside namespace
}

// Bring only one function from nested namespace into global scope
using MY_SPACE::SUB_SPACE::IntPtrChange;

void IntChange(int* _Data)
{
    *_Data = 200;
}

int main()
{
    int* p = nullptr;
    IntChange(p);

    int a = 10;
    MY_SPACE::IntChange(&a);
    MY_SPACE::IntChange(a);

    int* pInt = nullptr;
    IntPtrChange(pInt);
    IntPtrChange(&pInt);

    MY_SPACE::g_Int = 100;

    cout << endl;
    return 0;
}

