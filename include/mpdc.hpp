#ifndef _MPDC_HPP
#define _MPDC_HPP

#include <iostream>
#include <cstdint>
#include <string>

namespace mp {
  
  typedef typename std::uint64_t ui64_t;
  typedef typename std::uint32_t ui32_t;

  static constexpr int bitarity = 64;

  template <typename T = int>
  struct data_t {
    std::bitset<bitarity> *value = nullptr;
    T n;
  };

  /** разрядность вычислений x32 */
  class Int {
  public:
    
    /** Конструкторы */
    Int();
    Int(Int&&);  // copy
    Int(Int&);  // copy
    Int(std::string&&);
    Int(std::string&
);

    /** Преобразуем к строке */
    std::string to_string();

    /** Деструктор */
    ~Int();

  private:
    
    data_t<> data;

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
template <typename T>
mp::Int operator+(T&&, T&&);
mp::Int operator+(mp::Int&, mp::Int&&);
mp::Int operator+(mp::Int&, mp::Int&);
mp::Int operator+(mp::Int&&, mp::Int&);

#endif
