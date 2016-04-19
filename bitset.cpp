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


  bitset::bitset(bitset& _b){
    this->copy(_b);    
  };

  bitset::bitset(bitset&& _b){
    this->copy(_b);    
  };

  bitset::~bitset() {
    this->free();
  };
    
  /** Получаем битовое число из строки */
  void 
  bitset::from_char(NumSys _q, char* _str) {
    
    // Выдираем метод вывода для приведения к внутреннему представлению
    switch (_q) 
      {
      case NumBin:
        char_to_bin_deduce(_str);
        break;
      case NumDec:
        char_to_dec_deduce(_str);
        break;
      default:
        throw std::runtime_error("Система исчисления не поддерживается.");
      }

  };

  /** Получаем число в произвольной системе исчисления в виде строки */
  char *
  bitset::to_char(NumSys _q) const {
    return (char*) "";
  };

  /** Возвращаем index бит */
  bit_t 
  bitset::get(__MP_INDEX _index) {

    /**
       Проверяем, что индекс корректен
       throw out_of_range генерируется в случае несоответствия диапазону
    */
    //check_index_valid(_index);

    // Определяем кластер бита
    __MP_INDEX x = _index / ( __wl );
    __MP_INDEX y = _index % ( __wl );

    if (x > __size)
      augment_data(x);
    
    // get: bit(y) of cluster(x)
    return (*__data[x] >> y) & 1;

  };
  
  /** Устанавливаем index бит */
  void
  bitset::set(__MP_INDEX _index, bit_t _b) {

    /**
       Проверяем, что индекс корректен
       throw out_of_range генерируется в случае несоответствия диапазону
    */
    //check_index_valid(_index);

    __MP_INDEX x = _index / ( __wl );
    __MP_INDEX y = _index % ( __wl );

    if (x > __size)
      augment_data(x);

    if (_b & 1) {
      // bit(y) of cluster(x) -> 1
      *__data[x] |= 1 << y;
    } else {
      // bit(y) of cluster(x) -> 0
      *__data[x] &= ~(1 << y);
    }    
    
  };

  /** Получаем число битов выделенное под хранение числа */
  __MP_INDEX
  bitset::count() const {
    return (__MP_INDEX)( __size * __wl );
  };
  
  /** Очищаем сегмент памяти, используемый для хранения числа
   */
  void
  bitset::free() {
    if (__data != nullptr){
      for (int i=0;i<__size;i++)
        // Удаляем выделенные под хранение сегменты памяти
        delete __data[i];
      // Удаляем выделенный под "связывание" сегмент памяти
      delete __data;
    }
    __size = 0;
  }
  
  /** Копируем представление числа из области памяти
   */
  void 
  bitset::copy(bitset& _b){
    
    int i;

    this->free();

    if (_b.__data != nullptr) {
      // Выделяем область памяти под хранение числа
      __data = new __MP_TYPE*[_b.__size];
      for(i=0;i< _b.__size;i++){
        // Выделяем память под хранение части числа, размером __MP_TYPE
        __data[i] = new __MP_TYPE;
        // Копируем по ссылке
        *__data[i] = *_b.__data[i];
      }
      // Запоминаем длину выделенных сегментов памяти
      __size = _b.__size;
    }

  };

  // /** Проверяем, что индекс является корректным и указывает 
  //     на существуеще множество битов 
  // */
  // void 
  // bitset::check_index_valid(__MP_INDEX _index){
  //   // Если index > count throw out_of_range
  //   if ((_index-1) > this->count())
  //     throw std::out_of_range(std::to_string(_index));    
  // };


  // Вывод: "1001" -> 0b1001
  void bitset::char_to_bin_deduce(char *_str) {

    __MP_INDEX i = 0;
    while (_str[i] != '\0') {
      // Допускается синтаксис: как 1001, так и 0b1001.
      if (i==1 && _str[i] == 'b')
        continue;
      if (_str[i] == '1') {
        set(i,0b1);
      } else {
        if (_str[i] == '0') {
          set(i,0b0);
        } else {
          throw std::runtime_error(std::string(1,_str[i]) + 
                                   std::string(" не относится к двоичной системе счисления."));
        }      
      }
      ++i;
    }

  };

  // Вывод: "9" -> 0b1001
  void bitset::char_to_dec_deduce(char *_str) {

  };

  // Значение в байтах
  void bitset::augment_data(__MP_INDEX _size){
    
    __MP_INDEX i;
    __MP_TYPE ** data = nullptr;
    
    // Условие выделение нового сегмента памяти
    if (__size >= _size)
      return;

    // Выделяем новый сегмент, под хранение "связывающей" структуры
    data = new __MP_TYPE* [_size];
    
    for (i=0;i< _size;i++) {
      if (i > __size) {
        // Инициализируем новые сегменты памяти
        *data[i] = 0;
      } else { 
        // Копируем существующие сегменты (копируем указатели)
        data[i] = __data[i];
      }
    }
    
    // Очищаем память выделенную под хранение структуры
    delete __data;

    // Инициализация новой структуры
    __data = data;
    __size = _size;
    
  };

  /** Сумма */
  bitset sum(bitset& a, bitset& b){
        

    return bitset();
  };
  
  
  namespace core {
    
    /** Раздел центральных функций по раоте с битами */

    bit_t 
    get_majority_vector(__MP_TYPE __a, __MP_TYPE __b, __MP_TYPE& __c){

      // c(x,y,z) = (x&y)|(y&z)|(x&z)

      // допустимые коммутации между переносами
      // SH_K - kill; i-ый бит не зависит от типа переноса i-1 бита
      // SH_P - propagate; i-ый бит зависит от типа переноса i-1 бита
      // SH_G - generate; i-ый бит генерирует перенос в i+1 
      enum shift_type {SH_K = 0, SH_P = 1, SH_G = 2 };
  
      // Коммутационная таблица по типам переносов
      // shift_type[i+1] = SH_TAB[shift_type[i-1]][shift_type[i]] - рекурентная форма
      const int SH_TAB[3][3] = { {SH_K, SH_K, SH_G},
                                 {SH_K, SH_P, SH_G},
                                 {SH_K, SH_G, SH_G} };


      // выполняем предвычисление вектора переносов - __c
      // Возврщаем бит переноса для старшего бита отдельно
      
      int i,j;
      
      // Векторный преобразователь y -> c
      int _r[3] = {0,-1,1};

      int _x[mp::__wl + 1];
      int _y[mp::__wl + 1];
      __c = 0b0;
      
      _x[0] = SH_K;

      _y[0] = SH_K;
      // Если первый бит является корректиркющим, то определяем как генерацию перестановки
      if (get_vector_bit((__MP_INDEX)(1),__c))
        _y[0] = SH_G;

      for (i=1;i<=mp::__wl;i++){
        
        j = i-1;
        // Вычисляем значения j-го бита чисел __a и __b
        bit_t aj = get_vector_bit((__MP_INDEX)(j),__a);
        bit_t bj = get_vector_bit((__MP_INDEX)(j),__b);
        // Вычисляем значение переноса для xi бита.
        // Возможные значения {SH_K,SH_P,SH_G}
        if ((aj | bj) == 0b0)  // т.е. aj = bj = 0
          _x[i] = SH_K;
        else if ((aj & bj) == 0b1)  // т.е. aj = bj = 1
          _x[i] = SH_G;
        else
          _x[i] = SH_P;

        // Вычисляем yi
        _y[i] = SH_TAB[_x[i]][_y[j]];
      }

      // Вычисление вектора перестановок и бита переноса
      for (i=0;i<mp::__wl; i++) {
        // Ошибка алгоритма
        if (_r[_y[i]] == -1)
          throw std::runtime_error("get_majority_vector сломался!");
        // Вычисляем вектор переносов
        set_vector_bit((__MP_INDEX)i,(bit_t)_r[_y[i]],__c);          
      }
      
      // Возвращаем результирующий бит переноса
      return (bit_t)_r[_y[mp::__wl]];
      
    };

    __MP_TYPE 
    get_parity_vector(__MP_TYPE __a, __MP_TYPE __b, __MP_TYPE __c) {
      // s(x,y,z) = x^y^z
      return (__MP_TYPE) __a ^ __b ^ __c;
    };

    bit_t
    get_vector_bit(__MP_INDEX __index, __MP_TYPE __c){
      return (bit_t) (__c >> __index) & 1;
    };

    void
    set_vector_bit(__MP_INDEX __index, bit_t d,  __MP_TYPE& __c){
      if (d & 1) {
        __c |= 1 << __index;  // -> 1
      } else {
        __c &= ~(1 << __index);  // -> 0
      }
    };

  };
  
};
