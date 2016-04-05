/**
 * copyright 2015 Maxim Musolov
 */

#ifndef _MPDC_BITSET_HPP
#define _MPDC_BITSET_HPP


namespace mp {
  
  typedef unsigned long long ui64_t;
  typedef unsigned long ui32_t;

  typedef bool bit_t;

#define __MP_TYPE ui32_t
#define __MP_INDEX ui32_t


  static const __MP_INDEX __wl = 8 * sizeof(__MP_TYPE);

  //static constexpr int bitarity = 64;
  

  /**
     bits operations 
     setting bit(x): number |= 1 << x;
     clearing bit(x): number &= ~(1 << x);
     toggling bit(x): number ^= 1 << x;
     getting bit(x): bit = (number >> x) & 1
  */

  enum NumSys {NumBin, NumDec};

  /** Класс представления числа в виде последовательности битов */
  class bitset {

    friend class bitset;
    friend bitset sum(bitset&, bitset&);

  public:

    typedef __MP_TYPE type;
    
    bitset();
    ~bitset();
    // copy
    bitset(bitset&);
    bitset(bitset&&);


    bit_t 
    get(__MP_INDEX);

    void 
    set(__MP_INDEX, bit_t);

    __MP_TYPE 
    count() const;
    
    void 
    from_char(NumSys,char*);

    char* 
    to_char(NumSys) const;        

  private: 

    __MP_TYPE **__data;
    __MP_INDEX __size;
    
    //Очищаем память выделенную под хранение числа
    void 
    free();

    // Копируем из области памяти
    void 
    copy(bitset&);
    
    // // Проверяем, что индекс, определенный как параметр, соответствует всем 
    // // ограничениям, чтобы называться таковым
    // void 
    // check_index_valid(__MP_INDEX);

    // Вывод: "1001" -> 0b1001
    void 
    char_to_bin_deduce(char *);

    // Вывод: "9" -> 0b1001
    void 
    char_to_dec_deduce(char *);

    // Расширяем сегмент данных, выделенный под хранение
    void 
    augment_data(__MP_INDEX _size);

  };


  /** Операция: Сложение
   *  Реализуем по схеме сложения с предвычислением переносов
   */
  bitset 
  sum(bitset&, bitset&);
  
  namespace core {

    /** Область центральных функция для реализации операций над типом bitset */

    /** Вычисляем вектор переносов для типа __MP_TYPE 
     *  В качестве результата возвращаем бит переноса 
     *  (length * sizeof( __MP_TYPE )) + 1, т.е. ||__MP_TYPE||+1 бит.
     */
    bit_t 
    get_majority_vector(__MP_TYPE __a, __MP_TYPE __b, __MP_TYPE& __c);

    /** Вычисляем четность (parity).
     *  Результатом является вектор битов по основанию __MP_TYPE.
     *  Если в прошлом (на i-1 шаге) был вычислен бит переноса, он должен быть учтен в 
     *  векторе __c.
     */
    __MP_TYPE 
    get_parity_vector(__MP_TYPE __a, __MP_TYPE __b, __MP_TYPE __c);

    /** Вычисляем значение бита на базе типа __MP_TYPE
     */
    bit_t
    get_vector_bit(__MP_INDEX __index, __MP_TYPE __c);

    /** Вычисляем новое значение бита на базе типа __MP_TYPE
     */
    void
    set_vector_bit(__MP_INDEX __index, bit_t d,  __MP_TYPE& __c);

    
  }
};

#endif
