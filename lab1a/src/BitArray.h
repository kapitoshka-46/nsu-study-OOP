#ifndef BITARRAY_H
#define BITARRAY_H

#include <string>
#include <vector>
#include <algorithm>   // for any_of


//В этой задаче для простоты не требуется делать контейнер шаблонным,
//но это вполне допускается по желанию студента.
enum class operation {
    AND,
    OR,
    XOR,
};

class proxyAt;

class BitArray {
private:
    static constexpr int SIZE_ELEM = sizeof(unsigned char) * 8;
    std::vector<unsigned char> bytes;
    int size_bits {};

    int size_bytes_() const;
public:
    BitArray() = default;
    ~BitArray() = default;
    BitArray(const BitArray& b) = default;

    //Конструирует массив, хранящий заданное количество бит.
    //Первые sizeof(long) бит можно инициализровать с помощью параметра value.
    explicit BitArray(int num_bits, unsigned long value = 0);

    // Получить константую ссылку на реальный массив
    const std::vector<unsigned char> &data() const;

    //Обменивает значения двух битовых массивов.
    void swap(BitArray& other);

    BitArray& operator=(const BitArray& b);


    //Изменяет размер массива. В случае расширения, новые элементы
    //инициализируются значением value.
    void resize(int num_bits, bool value = false);

    //Очищает массив.
    void clear();
    //Добавляет новый бит в конец массива. В случае необходимости
    //происходит перераспределение памяти.
    void push_back(bool bit);


    //Битовые операции над массивами.
    //Работают только на массивах одинакового размера.
    //Обоснование реакции на параметр неверного размера входит в задачу.
    /** @throw std::invalid_argument exception if different size */
    BitArray& operator&=(const BitArray& b);
    /** @throw std::invalid_argument exception if different size */
    BitArray& operator|=(const BitArray& b);
    /** @throw std::invalid_argument exception if different size */
    BitArray& operator^=(const BitArray& b);

    //Битовый сдвиг с заполнением нулями.
    BitArray& operator<<=(int n);
    BitArray& operator>>=(int n);
    BitArray operator<<(int n) const;
    BitArray operator>>(int n) const;


    bool get(int i) const;

    //Устанавливает бит с индексом n в значение val.
    /** @throw std::invalid_argument exception if different size */
    BitArray& set(int n, bool val = true);
    //Заполняет массив истиной.
    BitArray& set();

    //Устанавливает бит с индексом n в значение false.
    /** @throw std::invalid_argument exception if different size */
    BitArray& reset(int n);
    //Заполняет массив ложью.
    BitArray& reset();

    //true, если массив содержит истинный бит.
    bool any() const;
    //true, если все биты массива ложны.
    bool none() const;
    //Битовая инверсия
    BitArray operator~() const;



    //Подсчитывает количество единичных бит.
    int count() const;


    //Возвращает значение бита по индексу i.
    /** @throw std::out_of_range if i < 0 or >= size */
    proxyAt operator[](int i);
    bool operator[](int i) const;

    int size() const;
    bool empty() const;

    //Возвращает строковое представление массива.
    std::string to_string() const;


    // ------------- My own functions ------------- //
    static bool is_equal(const BitArray & a, const BitArray & b);
    static bool is_not_equal(const BitArray & a, const BitArray & b);
    /** @throw std::invalid_argument exception if different size */
    static BitArray do_bit_operation(const BitArray & a, const BitArray & b, operation op);


};  // --------------------END OF CLASS-----------------------//

class proxyAt {
    BitArray& arr;
    int index;
public:
    proxyAt& operator=(bool bit);
    operator bool() const;
    proxyAt(BitArray& arr, int index);
};


bool operator==(const BitArray & a, const BitArray & b);
bool operator!=(const BitArray & a, const BitArray & b);

/** @throw std::invalid_argument exception if different size */
BitArray operator&(const BitArray& b1, const BitArray& b2);
/** @throw std::invalid_argument exception if different size */
BitArray operator|(const BitArray& b1, const BitArray& b2);
/** @throw std::invalid_argument exception if different size */
BitArray operator^(const BitArray& b1, const BitArray& b2);

std::ostream& operator<<(std::ostream & lhs, const BitArray & b1);


#endif //BITARRAY_H
