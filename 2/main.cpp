#include <iostream>
#include <stdexcept>
#include <initializer_list>

template <typename T>
class deque { // double-ended queue (двусторонняя очередь)
private:
    T* data; // указатель на массив, который будет хранить элементы
    int size; // общий размер массива
    int begin; // индекс начала очереди
    int end; // индекс конца очереди
    const int add = 30; // количество элементов, на которое увеличивается размер массива при необходимости

public:
    deque() : data(new T[30]), size(30), begin(15), end(15) { // конструктор по умолчанию
    }

    // конструктор с использованием initializer_list
    deque(std::initializer_list<T> init) : data(new T[init.size() + 30]), size(init.size() + 30), begin(15), end(15) {
        for (const auto& item : init) {
            data[end++] = item; // добавляем элементы в конец
        }
    }

    deque(const deque<T>& d) { // конструктор копирования
        size = d.size;
        begin = d.begin;
        end = d.end;
        data = new T[size];
        for (int i = begin; i < end; i++) {
            data[i] = d.data[i];
        }
    }

    deque(deque<T> &&d) noexcept : data(d.data), size(d.size), begin(d.begin), end(d.end) { // move-конструктор
        d.data = nullptr; // обнуляем указатель в перемещаемом объекте
        d.size = 0; // обнуляем размер
        d.begin = 0; // обнуляем начальный индекс
        d.end = 0; // обнуляем конечный индекс
    }

    ~deque() {
        delete[] data; // деструктор, освобождающий память
    }

    deque<T>& operator=(const deque<T>& val) { // оператор присваивания
        if (this != &val) { // проверка на самоприсваивание
            delete[] data; // освобождаем старый массив
            size = val.size;
            begin = val.begin;
            end = val.end;
            data = new T[size];
            for (int i = begin; i < end; i++) {
                data[i] = val.data[i];
            }
        }
        return *this;
    }

    deque<T>& operator=(deque<T>&& val) noexcept { // move-оператор присваивания
        if (this != &val) { // проверка на самоприсваивание
            delete[] data; // освобождаем старый массив
            data = val.data; // переносим указатель
            size = val.size; // переносим размер
            begin = val.begin; // переносим начальный индекс
            end = val.end; // переносим конечный индекс

            // обнуляем перемещаемый объект
            val.data = nullptr;
            val.size = 0;
            val.begin = 0;
            val.end = 0;
        }
        return *this;
    }

    T operator[](int index) { // оператор доступа по индексу
        return data[begin + index]; // возвращает элемент по индексу
    }

    // методы добавления элементов
    void push_back(T val) {
        if (end < size) {
            data[end++] = val; // добавление элемента в конец
        }
        else {
            size += 30; // увеличение размера массива
            T *temp = new T[size]; // создание нового массива
            for (int i = begin; i < end; i++) {
                temp[i] = data[i]; // копирование старых данных
            }
            temp[end++] = val; // добавление нового элемента
            delete[] data; // освобождение старого массива
            data = temp; // перенаправление указателя
        }
    }

    void push_front(T val){
        if (begin > 0){
            data[--begin] = val; // добавление элемента в начало
        }
        else{
            size += add; // увеличение размера массива
            begin += add / 2; // сдвиг начала
            end += add / 2; // сдвиг конца
            T *temp  = new T[size]; // создание нового массива
            for (int i = begin; i < end; i++){
                temp[i] = data[i - 1]; // копирование данных
            }
            temp[--begin] = val; // добавление нового элемента
            delete[] data; // освобождение старого массива
            data = temp; // перенаправление указателя
        }
    }

    // методы удаления элементов
    void pop_back() { // удаление элемента из конца очереди
    if (end > begin) { // проверка, есть ли элементы в очереди, которые можно удалить
        --end;
        data[end] = T();
        }
    }

    void pop_front() { // удаление элемента из начала очереди
    if (begin < end) { // проверка, есть ли элементы в очереди, которые можно удалить
        data[begin] = T();
        ++begin;
        }
    }

    // в обоих методах мы не удаляем элементы физически из массива,
    // а просто изменяем индексы, которые указывают на начало и конец очереди

    int  Size (){
        return end - begin; // возвращает количество элементов в очереди
    }

    void print (){
        if (this -> begin == this -> end){
            std::cout << "deque is empty" << std::endl;
            return;
        }
        for (int i = this -> begin; i < this -> end; i++){
            std::cout << this -> data[i] << ' ' << std::endl; // выводит элементы очереди
        }
    }
};

int main() {
    deque<int> d{1, 2, 3}; // создание первого объекта deque с инициализацией
    d.print(); // печать текущего состояния первого deque
    std::cout << std::endl;
    deque<int> e; // создание второго объекта deque без инициализации
    e.push_back(10); // добавление элемента 12 в конец второго deque
    for (int i = 9; i > 0; i--){
        e.push_back(i); // добавление i в конец второго deque
        e.push_front(i); // добавление i в начало второго deque
    }
    e.print(); // печать текущего состояния второго deque
}
