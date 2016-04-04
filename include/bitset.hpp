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


  static const int byte_length = 8;

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
  struct bitset {
  public:

    typedef __MP_TYPE type;
    
    bitset();
    ~bitset();
    // copy
    bitset(bitset&);
    bitset(bitset&&);

    __MP_TYPE *data;
    __MP_INDEX length;

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
    
    //Очищаем память выделенную под хранение числа
    void 
    free();
    // Копируем из области памяти
    void 
    copy(__MP_TYPE*,__MP_INDEX);
    
    // Проверяем, что индекс, определенный как параметр, соответствует всем 
    // ограничениям, чтобы называться таковым
    void 
    check_index_valid(__MP_INDEX);
    // Вывод: "1001" -> 0b1001
    void 
    char_to_bin_deduce(char *);
    // Вывод: "9" -> 0b1001
    void 
    char_to_dec_deduce(char *);
  };


  /** Операция: Сложение
   *  Реализуем по схеме сложения с предвычислением переносов
   */
  bitset 
  sum(bitset&, bitset&);
  
  namespace core {

    /** Область центральных функция для реализации операций над типом bitset */

    // допустимые коммутации между переносами
    // SH_K - kill; i-ый бит не зависит от типа переноса i-1 бита
    // SH_P - propagate; i-ый бит зависит от типа переноса i-1 бита
    // SH_G - generate; i-ый бит генерирует перенос в i+1 
    enum shift_type {SH_K = 0, SH_P = 1, SH_G = -1 };
  
    // Коммутационная таблица по типам переносов
    // shift_type[i+1] = SH_TAB[shift_type[i-1]][shift_type[i]] - рекурентная форма
    const int SH_TAB[3][3] = { {SH_K, SH_K, SH_G},
                               {SH_K, SH_P, SH_G},
                               {SH_K, SH_G, SH_G} };

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
    
  }
};

#endif
