#include <stdio.h>
#include <iostream>
using namespace std;
#define f(a,b) a##b
#define g(a)   #a
#define h(a) g(a)
int main()
{
        char a = 'a';
        cout<<g(a)<<endl; // a
        cout<<g(g(a))<<endl; // g(a)
        printf("%s\n", h(f(1,2)));   // 12
        printf("%s\n", g(f(1,2))); // f(1,2)
        printf("%s\n", g(h(f(1,2)))); // h(f(1,2))
        printf("%s\n", h(g(f(1,2)))); // "f(1,2)"
        printf("%s\n", h(h(f(1,2)))); // "12"
        return 0;
}