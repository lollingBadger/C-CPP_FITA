#include <iostream>
using namespace std;

int main() {
    int i = 0;
    while(i < 10000) {
        int* p = new int[1000000];
        std::cout << "Allocated a million ints for "<< ++i << "Times" << std::endl; // allocate 1 million ints
        delete p;
    }
    return 0;
}
