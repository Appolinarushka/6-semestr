// variadic templates

#include <iostream>
#include <utility>

// объявление шаблона структуры tuple, который может принимать переменное количество типов
template <typename... Types> struct tuple;

template<> struct tuple<>{}; // пустая специализация tuple<> представляет собой пустой кортеж

// рекурсивная шаблонная структура tuple
// первый тип T хранится непосредственно как переменная-член
// остальные типы обрабатываются через наследование
template<typename T, typename... Types>
struct tuple<T, Types...>: public tuple<Types...>{
    T value;

    // конструктор по умолчанию
    tuple() : tuple<Types...>(), value(T()) {}

    // конструктор с параметрами
    tuple(const T& val, const Types&... vals) : tuple<Types...>(vals...), value(val) {}
};

// функция print
void print(){
    std::cout << std::endl; // выводит пустую строку
}

template<typename T, typename... Types>
void print(const T& a, const Types&... b){
    std::cout << a << ' ' <<  std::endl;
    print(b...); // выводит переменное количество аргументов
}

// шаблон element
// используется для получения типа элемента по индексу из tuple
// если индекс равен 0, то мы возвращаем тип T и сам тип кортежа
// если индекс больше 0, мы рекурсивно обращаемся к следующему элементу кортежа, уменьшая индекс на 1
template<size_t index, typename Ttuple> struct element;

template<typename T, typename... Types>
struct element<0, tuple<T, Types...>> {
    using Type_t = T;
    using TupleType_t = tuple<T, Types...>;
};

template<size_t index, typename T, typename... Types>
struct element<index, tuple<T, Types...>>
        : public element<index - 1, tuple<Types...>>{};

// функция get
// позволяет получить ссылку на элемент кортежа по индексу
// она использует element для определения типа элемента и возвращает его
template<size_t index, typename... Types>
typename element<index, tuple<Types...>>::Type_t&
get(tuple<Types...>& a){
    using TupleType_t = typename element<index, tuple<Types...>>::TupleType_t;
    return static_cast<TupleType_t&>(a).value;
}

// функция MakeTuple
// создаёт экземпляр tuple и заполняет его значениями, переданными в качестве аргументов
template<typename... Types>
tuple<Types...> MakeTuple(const Types&... a){
    return tuple<Types...>(a...); // используем конструктор tuple
}

// специальная версия MakeTuple для ссылочных типов
template<typename... Types>
tuple<Types&...> MakeTuple(Types&... a){
    return tuple<Types&...>(a...); // используем конструктор tuple
}

// функция tie
template<typename... Types>
tuple<Types&...> tie(Types&... args) {
    return MakeTuple(args...);
}

int main() {
    tuple<int, double> myTuple(123, 3.14); // использование конструктора с параметрами
    auto a = get<0>(myTuple);
    auto b = get<1>(myTuple);
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << std::endl;
    auto tuple = MakeTuple(21.01, 2004); // использование функции MakeTuple
    auto c = get<0>(tuple);
    auto d = get<1>(tuple);
    std::cout << c << std::endl;
    std::cout << d << std::endl;
    std::cout << std::endl;
    // пример использования функции tie
    int x = 1;
    double y = 2.5;
    auto myTie = tie(x, y);
    std::cout << "Before modification:" << std::endl;
    std::cout << get<0>(myTie) << std::endl;
    std::cout << get<1>(myTie) << std::endl;
    get<0>(myTie) = 3;
    get<1>(myTie) = 7.5;
    std::cout << std::endl;
    std::cout << "After modification:" << std::endl;
    std::cout << x << std::endl;
    std::cout << y << std::endl;
    return 0;
}
