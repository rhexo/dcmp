/**
 * copyright 2015 Maxim Musolov
 */

#include <iostream>
#include <cstdlib>
#include <mpdc.hpp>
#include <memory>
#include <bitset>

using namespace mp;

int main(void){
  
  char i1 = 0b1010;
  char i2 = 0b11101;
  std::bitset<1000000> v;

  std::cout << std::bitset<8>(i2 % i1) << std::endl;
  v |= 1;
  v <<= 2;
  std::cout << v << std::endl;
  

  Int a("112");
  std::cout << a + Int("100") << std::endl;
  std::cout << Int("201") << std::endl;
  
  return (EXIT_SUCCESS);
};

