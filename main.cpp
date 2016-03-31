/**
 * copyright 2015 Maxim Musolov
 */

#include <iostream>
#include <cstdlib>
#include <mpdc.hpp>
#include <memory>
#include <bitset>

using namespace mp;

static constexpr int N = 8;

int main(void){
  
  // std::bitset<8> b10 = 0b1010;
  // std::bitset<8> b = 0b11101;

  // std::cout << "b: " << b.to_string() << ", b10: " << b10.to_string() << std::endl;
  // std::cout << "b&b10: " << (b & b10).to_string() << std::endl;
  // std::cout << "b|b10: " << (b | b10).to_string() << std::endl;
  // std::cout << "~b,~b10: " << (~b).to_string() << ", " << (~b10).to_string()  << std::endl;
  // std::cout << "b^b10: " << (b ^ b10).to_string() << std::endl;  
  

  unsigned int w = 0;
  unsigned long ww = 0;
  unsigned long long  www = 0;

  std::cout << sizeof(w) << std::endl;
  std::cout << sizeof(ww) << std::endl;
  std::cout << sizeof(www) << std::endl;

  try {
    
    std::bitset<N> x(0b11111111);
    std::bitset<N> y(0b00000001);
    std::bitset<N> c(0);
    std::bitset<N> s(0);
    
    // При N=8 получим исключение

    /** Алгоритм вычисления суммы */

    for (int i=0;i<N;i++){
      s[i]=x[i]^y[i]^c[i];
      if ((i+1)==N) {
        if (((x[i]&y[i])|(y[i]&c[i])|(x[i]&c[i])) & std::bitset<1>(1)[0]){
          throw std::runtime_error("Переполнение регистра");
        }
      } else {
        c[i+1]=(x[i]&y[i])|(y[i]&c[i])|(x[i]&c[i]);
      }          
    }
      
    
    std::cout << "s=" << s.to_ullong() << std::endl;
    std::cout << "bit(s): " << s.to_string() << std::endl;


  } catch (std::runtime_error &e) {
    std::cout << "ex: " << e.what() << std::endl;
  }

  // Int a("112");
  // std::cout << a + Int("100") << std::endl;
  // std::cout << Int("201") << std::endl;
  
  return (EXIT_SUCCESS);
};

