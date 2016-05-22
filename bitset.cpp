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

    __MP_INDEX
    bitset::get_valid_bits_count() {
      
      __MP_INDEX i;
      __MP_INDEX j;      
      __MP_TYPE a;
      
      for (i=(__size-1);i>=0;i--){
        a = *__data[i];
        for (j=1;i<=__wl;j++){
          if ( core::get_vector_bit(__wl,a) )
            return (__wl-j+1) * (i+1);
          // Сдвигаем a на 1 влево
          a <<= 1;
        }
      }
      // Фактически значение равно нулю
      return 0;
    }


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

  // Расширяем сегмент данных, выделенный под хранение
  void 
  bitset::augment_data(bitset& _a) {
    
    // обобщаем с точностью до типа bitset
    if (__size < _a.__size)
      augment_data((__MP_INDEX)_a.size);
  }

  // Оптимизируем память, занимаемую под хранение числа
  void
  bitset::optimize_data(){
        
    __MP_INDEX bits = get_valid_bits_count();
    __MP_INDEX to_size = ( bits / __wl ) + 1;

    if (__size > to_size ) {
      // Сжимаем память под хранение
      // Выделяем новый сегмент, под хранение "связывающей" структуры
      __MP_TYPE ** data = new __MP_TYPE* [to_size];
      
      for (__MP_INDEX i = 0; i < to_size; i++)
        data[i] = __data[i];

      delete __data;

      __data = data;
      __size = to_size;      
      
    }
  };

  // Реализация побитового сдвига влево
  void
  bitset::shift_left(__MP_INDEX _n){
    
    __MP_INDEX nsi;
    __MP_INDEX nbi;

    // Основной цикл переноса битов числа при сдвиге влево
    for (__MP_INDEX si=(__size - 1); si>=0; si--){
      for (__MP_INDEX bi=__wl; bi>0; bi--) {
        // Получем рассчетные значения сдвигаемого бита
        get_bit_index_rel(si, bi, _n, VLeft, nsi, nbi);      
        // Увеличиваем число кластеров под хранение числа
        if (nsi > __size)
          augment_data(nsi);
        // выполняем get/set
        this->set( (nsi*__wl)+nbi, this->get((nsi*__wl)+nbi));
        
      }
    }

    // Обнуляем хвост
    for (__MP_INDEX si=nsi; si>=0; si--){
      for (__MP_INDEX bi= (si == nsi ? nbi : __wl); bi>0; bi--) {
        // выполняем get/set
        this->set( (si*__wl)+bi, (bit_t)0b0);        
      }
    }    

  };

  // Реализация побитового сдвига вправо
  void
  bitset::shift_right(__MP_INDEX _n) {

    __MP_INDEX nsi;
    __MP_INDEX nbi;

    // Основной цикл переноса битов числа при сдвиге влево
    for (__MP_INDEX si=0; si<__size0; si++){
      for (__MP_INDEX bi=1; bi<=__wl; bi++) {
        // Получем рассчетные значения сдвигаемого бита
        get_bit_index_rel(si, bi, _n, VRight, nsi, nbi);      
        // Увеличиваем число кластеров под хранение числа
        if (nbi == 0)
          continue;
        // выполняем get/set
        this->set( (nsi*__wl)+nbi, this->get((nsi*__wl)+nbi));
        
      }
    }

    // Обнуляем хвост
    for (__MP_INDEX si=nsi; si<__size; si++){
      for (__MP_INDEX bi= (si == nsi ? nbi : 1); bi<=__wl; bi++) {
        // выполняем get/set
        this->set( (si*__wl)+bi, (bit_t)0b0);        
      }
    }    

  };

  void
  bitset::clear(){
    
    for (__MP_INDEX i=0;i<__size;i++)
      *__data[i] = 0;
    // __size --> 1
    optimize_data();
  };


  // Полчить a*b(i)*2^i
  bitset
  bitset::get_mi(bitset& _b, __MP_INDEX _i) {
    
    bitset res;
    
    res.copy(_b);
    
    return res;
  };

  void     
  bitset::get_bit_index_rel(__MP_INDEX _si,__MP_INDEX _bi,
                      __MP_INDEX _n, VectShift _v,
                      __MP_INDEX& _nsi, __MP_INDEX& _nbi) {
    
    __MP_INDEX lmod = _n % __wl;
    __MP_INDEX ldiv = _n / __wl;

    switch (_v)
      {
      case VLeft:
        // Выполняем смещение влево
        _nsi = _si + ldiv;
        if ( _bi + lmod ) > __wl
           ++_nsi;

        _nbi = ( _bi + lmod ) % __wl;
        if (_nbi == 0)
          _nbi = __wl;  // Корректируем индекс

      case VRight:
        // Выполняем смещение вправо
        _nsi = 0;
        if ( _si >= l_div )          
          _nsi = _si - ldiv;

        if ( _bi < lmod ) {

          if (_nsi > 0) {
            --_nsi;                                
            _nbi = __wl - ( lmod - _bi);          
          }else {
            _nbi = 0;
          }

        } else {

          _nbi = _bi - lmod;

          if (_nbi == 0)
            if (_nsi > 0){
              // Корректируем индекс
              _nbi = __wl;
              --_nsi;            
            }

        }

      case default:
        // Так как неизвстен тип смещениея, возвращаем те же значения
        _nsi = _si;
        _nbi = _bi;
      }
  };



  /** Сумма */
  bitset sum(bitset& _a, bitset& _b){
        
    
    bit_t res_bit;
    __MP_TYPE c;
    __MP_TYPE b;
    __MP_TYPE a;
    __MP_INDEX index;

    // Выделяем переменную под хранение результата сложения
    bitset res;
    
    if (_a.__size > res.__size)
      res.__size = _a.__size;

    if (_b.__size > res.__size)
      res.__size = _b.__size;
    
    // Выделяем новый сегмент, под хранение "связывающей" структуры
    res.__data = new __MP_TYPE* [res.__size];
    
    // Инициализация бита переноса
    res_bit = 0b0;
    // Общий цикл суммирования
    for (index = 0;index < res.__size;index++){

      if (index < _a.__size)
        a = *_a.__data[index];
      else
        a = 0;

      if (index < _b.__size)
        b = *_b.__data[index];
      else
        b = 0;

      // Инициализация вектора переносов
      c = 0;
      // Корректируем вектор с учетом последнего переноса
      core::set_vector_bit(1,res_bit,c);      
      // Вычисляем вектор переносов
      res_bit = core::get_majority_vector(a,b,c);
      // суммируем сегмент числа
      *res.__data[index] = core::get_parity_vector(a,b,c);
        
    }
  
        
    if (res_bit == 0b1) {      
      // Требуется увеличить кластер данных на 1 и присвоить первому биту значение res_bit
      ++index;
      // Увличиваем размер памяти под хранения числа
      res.augment_data(index);
      // Инициализация сегмента числа
      if (res.__size == index)
        core::set_vector_bit(1,(bit_t) 0b1,*res.__data[index-1]);
      else
        throw std::runtime_error("не удалось расширить сегмент памяти под число");
    }
    
    // Возвращаем вычисленный результат
    return res;
  };
  
  

  /** Операция: умножение
   *  Реализуем матричный умножитель
   */
  bitset multiply(bitset& _a, bitset& _b){
    
    __MP_TYPE a;
    __MP_TYPE b;
    __MP_TYPE c;
    bit_t res_bit;
    
    // Выделяем память под хранение результата
    bitset res;
    
    
    

    return res;
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
