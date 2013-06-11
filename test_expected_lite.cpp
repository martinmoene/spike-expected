

#include "expected-lite.hpp"

int main()
{
    nonstd::expected<int> ei = 33;
}

// cl -nologo -W3 -EHsc -Ideps test-expected-lite.cpp && test-expected-lite
// g++ -Wall -Wextra -std=c++11 -o test-expected-lite.exe test-expected-lite.cpp && test-expected-lite
