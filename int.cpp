/**
 * copyright 2015 Maxim Musolov
 */

#include <string>
#include <utility>
#include <iostream>
#include <int.hpp>

namespace mp {

  /** Разрядность вычислений x32 */
  Int::Int() {
  };

  /** Разрядность вычислений x32 */
  Int::Int(Int&& d) {
    // Копируем, через преобразование
    this->to_internal(d.to_string());
  };

  Int::Int(Int& d) {
    // Копируем, через преобразование
    this->to_internal(d.to_string());
  };

  /** Конструируем из последовательности символов */
  Int::Int(std::string&& s) {
    // Копируем, через преобразование
    this->to_internal(std::forward<std::string&>(s));    
  };

  /** Конструируем из встроенного 32 типа данных */
  Int::Int(std::string& s) {
    this->to_internal(s);
  };

  /** Преобразуем к строке */
  std::string 
  Int::to_string() {
    return std::move(std::string("1"));
  };
  
  /** Деструктор */
  Int::~Int() {
    // Очищаем память
    //data.value.clear();
  };

  /** Внешний тип -> внутренний тип */
  void 
  Int::to_internal(std::string& s) {
    
    // string -> ui32_t[n] 
    
  };

  void 
  Int::to_internal(std::string&& s) {   
    this->to_internal(std::forward<std::string&>(s));
  };
  
  // Функции преобразования пространства имен
  std::string 
  to_string(Int&& d) {
    return d.to_string();
  };

  std::string 
  to_string(Int& d) {
    return d.to_string();
  }

};

/** Реализация перегрузки оператора вывода в стандартный поток */
std::ostream& 
operator<<(std::ostream& cout, mp::Int& a){
  cout << a.to_string();
  return cout;
};

std::ostream& 
operator<<(std::ostream& cout, mp::Int&& a){
  return operator<<(cout,std::forward<mp::Int& >(a));;
};

/** Реализуем сумму */
mp::Int 
operator+(mp::Int&a, mp::Int&& b){
  return operator+(a,std::forward<mp::Int& >(b));
};

mp::Int 
operator+(mp::Int&&a, mp::Int&& b){
  return operator+(std::forward<mp::Int& >(a),
                   std::forward<mp::Int& >(b));
};

mp::Int 
operator+(mp::Int&&a, mp::Int& b){
  return operator+(std::forward<mp::Int& >(a),b);
};

mp::Int 
operator+(mp::Int& a, mp::Int& b){
  
  std::cout << a << " + " << b << std::endl;

  return mp::Int(std::string());
};
