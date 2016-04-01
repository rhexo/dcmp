/**
 * copyright 2015 Maxim Musolov
 */

#ifndef _MPDC_INT_HPP
#define _MPDC_INT_HPP

#include <string>
#include <iostream>

#include <bitset.hpp>

namespace mp {

  class Int {
  public:
    
    /** Конструкторы */
    Int();
    Int(Int&&);  // copy
    Int(Int&);   // copy
    Int(std::string&&);
    Int(std::string&);

    /** Преобразуем к строке */
    std::string to_string();

    /** Деструктор */
    ~Int();

  private:
    
    bitset data;

    /** Внешний тип -> внутренний тип */
    void to_internal(std::string&);
    void to_internal(std::string&&);
    
  };

  std::string to_string(Int&&);
  std::string to_string(Int&);

}

/** Перегружаем выводв стандартный поток для введенного типа данных */
std::ostream& operator<<(std::ostream&, mp::Int&);
std::ostream& operator<<(std::ostream&, mp::Int&&);

/** реализуем сумму */
mp::Int operator+(mp::Int&&, mp::Int&&);
mp::Int operator+(mp::Int&, mp::Int&&);
mp::Int operator+(mp::Int&, mp::Int&);
mp::Int operator+(mp::Int&&, mp::Int&);


#endif

