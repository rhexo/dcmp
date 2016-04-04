/**
 * copyright 2015 Maxim Musolov
 */

#include <iostream>
#include <utility>
#include <string>

#include <bitset.hpp>

namespace mp {


  bitset::bitset() {
    this->free();
  };


  bitset::bitset(bitset& b){
    this->copy(b.data,b.length);    
  };

  bitset::bitset(bitset&& b){
    this->copy(b.data,b.length);    
  };

  bitset::~bitset() {
    this->free();
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
    
    // get: bit(y) of cluster(x)
    return (data[x] >> y) & 1;

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
      // bit(y) of cluster(x) -> 1
      data[x] |= 1 << y;
    } else {
      // bit(y) of cluster(x) -> 0
      data[x] &= ~(1 << y);
    }    
    
  };

  /** Получаем число битов выделенное под хранение числа */
  __MP_INDEX
  bitset::count() const {
    return (__MP_INDEX)( length * sizeof(__MP_TYPE) * byte_length );
  };
  
  /** Очищаем сегмент памяти, используемый для хранения числа
   */
  void
  bitset::free() {
    if (data != nullptr)
      delete data;
    length = 0;
  }
  
  /** Копируем представление числа из области памяти
   */
  void 
  bitset::copy(__MP_TYPE* __data,__MP_INDEX __length){

    this->free();

    if (__data != nullptr) {
      // Выделяем область памяти под хранение числа
      data = new __MP_TYPE[__length];
      // Воссоздаем из объекта по ссылке
      for (int i=0;i<__length;i++) {
        data[i] = __data[i]; 
      }
      length = __length;
    }

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

    __MP_INDEX i = 0;
    while (str[i] != '\0') {
      // Допускается синтаксис: как 1001, так и 0b1001.
      if (i==1 && str[i] == 'b')
        continue;
      if (str[i] == '1') {
        set(i,0b1);
      } else {
        if (str[i] == '0') {
          set(i,0b0);
        } else {
          throw std::runtime_error(std::string(1,str[i]) + 
                                   std::string(" не относится к двоичной системе счисления."));
        }      
      }
      ++i;
    }

  };

  // Вывод: "9" -> 0b1001
  void bitset::char_to_dec_deduce(char *str) {

  };

  bitset sum(bitset& a, bitset& b){
    
    return bitset();
  };
  
  
  namespace core {
    
    /** Раздел центральных функций по раоте с битами */

    bit_t 
    get_majority_vector(__MP_TYPE __a, __MP_TYPE __b, __MP_TYPE& __c){
      // c(x,y,z) = (x&y)|(y&z)|(x&z)

      // выполняем предвычисление вектора переносов - __c
      
    };

    __MP_TYPE 
    get_parity_vector(__MP_TYPE __a, __MP_TYPE __b, __MP_TYPE __c) {
      // s(x,y,z) = x^y^z
    };


  };
  
};
