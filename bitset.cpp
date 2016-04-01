/**
 * copyright 2015 Maxim Musolov
 */

#include <iostream>
#include <utility>
#include <string>

#include <bitset.hpp>

namespace mp {


  bitset::bitset() {
    data = nullptr;
    length = 0;
  };

  bitset::~bitset() {
    if (data != nullptr)
      delete data;
    length = 0;
  };

  /** Получаем битовое число из строки */
  void 
  bitset::from_char(NumSys q, char* str) {
    
    // Выдираем метод вывода для приведения к внутреннему представлению
    switch (q) 
      {
      case NumBin:
        char_to_bin_deduce(str);
        break;
      case NumDec:
        char_to_dec_deduce(str);
        break;
      default:
        throw std::runtime_error("Система исчисления не поддерживается.");
      }

  };

  /** Получаем число в произвольной системе исчисления в виде строки */
  char *
  bitset::to_char(NumSys q) const {
    return (char*) "";
  };

  /** Возвращаем index бит */
  bit_t 
  bitset::get(__MP_INDEX index) {

    /**
       Проверяем, что индекс корректен
       throw out_of_range генерируется в случае несоответствия диапазону
    */
    check_index_valid(index);

    // Определяем кластер бита
    __MP_INDEX x = index / ( sizeof(__MP_TYPE) * byte_length );
    __MP_INDEX y = index % ( sizeof(__MP_TYPE) * byte_length );

    return (data[x] >> (y-1)) & 1;

  };
  
  /** Устанавливаем index бит */
  void
  bitset::set(__MP_INDEX index, bit_t b) {

    /**
       Проверяем, что индекс корректен
       throw out_of_range генерируется в случае несоответствия диапазону
    */
    check_index_valid(index);

    __MP_INDEX x = index / ( sizeof(__MP_TYPE) * byte_length );
    __MP_INDEX y = index % ( sizeof(__MP_TYPE) * byte_length );

    if (b & 1) {
      // y-1 -> 1
      data[x] |= 1 << (y-1);
    } else {
      // y-1 -> 0
      data[x] &= ~(1 << (y-1));
    }    
    
  };

  /** Получаем число битов выделенное под хранение числа */
  __MP_INDEX
  bitset::count() const {
    return (__MP_INDEX)( length * sizeof(__MP_TYPE) * byte_length );
  };

  /** Проверяем, что индекс является корректным и указывает 
      на существуеще множество битов 
  */
  void 
  bitset::check_index_valid(__MP_INDEX index){
    // Если index > count throw out_of_range
    if ((index-1) > this->count())
      throw std::out_of_range(std::to_string(index));    
  };


  // Вывод: "1001" -> 0b1001
  void bitset::char_to_bin_deduce(char *str) {

  };

  // Вывод: "9" -> 0b1001
  void bitset::char_to_dec_deduce(char *str) {

  };

};
