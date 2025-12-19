#include<iostream>
int main()
{
  int* i = new int;
  *i = 10;
  std::cout << "MEMORY ALLOCATED " <<std::endl;
  return 0;
}