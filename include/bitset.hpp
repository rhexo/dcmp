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

    __MP_TYPE *data;
    __MP_INDEX length;

    bit_t get(__MP_INDEX);
    void set(__MP_INDEX, bit_t);
    __MP_TYPE count() const;
    
    void from_char(NumSys,char*);
    char* to_char(NumSys) const;
    
  private: 
    void check_index_valid(__MP_INDEX);
    // Вывод: "1001" -> 0b1001
    void char_to_bin_deduce(char *);
    // Вывод: "9" -> 0b1001
    void char_to_dec_deduce(char *);
  };

};

#endif
