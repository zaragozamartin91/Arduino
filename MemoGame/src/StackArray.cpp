#include "StackArray.h"



template<int SIZE, typename TYPE>
unsigned int mz::StackArray<SIZE,TYPE>::size() const {
    return SIZE;
}
