#include <iostream>

template <typename T>
class SmartPointer {
private:
    struct ControlBlock {
        T* ptr;
        int count;
        int size;
        bool isArray; // true если массив, false если одиночный элемент
        ControlBlock(T* p, int s, bool isArr) : ptr(p), count(1), size(s), isArray(isArr) {}
    };
    ControlBlock* controlBlock;

public:
    // конструктор для одиночного элемента
    SmartPointer(T* p = nullptr) {
        if (p) {
            controlBlock = new ControlBlock(p, 1, false);
        } else {
            controlBlock = nullptr;
        }
    }

    // конструктор для массива
    SmartPointer(T* p, int size) {
        if (p && size > 0) {
            controlBlock = new ControlBlock(p, size, true);
        } else {
            controlBlock = nullptr;
        }
    }

    // конструктор копирования
    SmartPointer(const SmartPointer& p) {
        controlBlock = p.controlBlock;
        if (controlBlock) {
            controlBlock->count++;
        }
    }

    // move-конструктор
    SmartPointer(SmartPointer&& p) noexcept : controlBlock(p.controlBlock) {
        p.controlBlock = nullptr;
    }

    // деструктор
    ~SmartPointer() {
        if (controlBlock && --controlBlock->count == 0) {
            if (controlBlock->isArray) {
                delete[] controlBlock->ptr;
            } else {
                delete controlBlock->ptr;
            }
            delete controlBlock;
        }
    }

    // перегруженные операторы присваивания
    // этот оператор присваивания позволяет присваивать один объект SmartPointer другому
    SmartPointer& operator=(const SmartPointer& p) {
        if (this != &p) {
            if (controlBlock && --controlBlock->count == 0) {
                if (controlBlock->isArray) {
                    delete[] controlBlock->ptr;
                } else {
                    delete controlBlock->ptr;
                }
                delete controlBlock;
            }
            controlBlock = p.controlBlock;
            if (controlBlock) {
                controlBlock->count++;
            }
        }
        return *this;
    }

    // move-оператор присваивания
    SmartPointer& operator=(SmartPointer&& p) noexcept {
        if (this != &p) {
            if (controlBlock && --controlBlock->count == 0) {
                if (controlBlock->isArray) {
                    delete[] controlBlock->ptr;
                } else {
                    delete controlBlock->ptr;
                }
                delete controlBlock;
            }
            controlBlock = p.controlBlock;
            p.controlBlock = nullptr;
        }
        return *this;
    }

    // этот оператор присваивания позволяет присваивать указатель на объект типа T объекту SmartPointer
    SmartPointer& operator=(T* p) {
        if (controlBlock && --controlBlock->count == 0) {
            if (controlBlock->isArray) {
                delete[] controlBlock->ptr;
            } else {
                delete controlBlock->ptr;
            }
            delete controlBlock;
        }
        if (p) {
            controlBlock = new ControlBlock(p, 1, false);
        } else {
            controlBlock = nullptr;
        }
        return *this;
    }

    // оператор разыменования
    // возвращает ссылку на объект, на который указывает ptr
    T& operator*() {
        return *(controlBlock->ptr);
    }

    const T& operator*() const {
        return *(controlBlock->ptr);
    }

    // оператор доступа к членам
    // позволяет получить доступ к членам объекта, на который указывает ptr
    T* operator->() {
        return controlBlock->ptr;
    }

    const T* operator->() const {
        return controlBlock->ptr;
    }

    // функция для получения текущего значения счётчика
    int getCount() const {
        return controlBlock ? controlBlock->count : 0;
    }

    // метод, проверяющий, чем управляет текущий объект SmartPointer
    bool isArrayType() const {
        return controlBlock ? controlBlock->isArray : false;
    }

    // функция для получения размера массива
    int getSize() const {
        return controlBlock && controlBlock->isArray ? controlBlock->size : 1;
    }

    // индексация для доступа к элементам массива
    T& operator[](int index) {
        return controlBlock->ptr[index];
    }

    const T& operator[](int index) const {
        return controlBlock->ptr[index];
    }
};

int main() {
    SmartPointer<int> el1(new int(100)); // создаём SmartPointer на объект int
    std::cout << "Value: " << *el1 << std::endl; // используем оператор разыменования
    std::cout << "Reference count after creating el1: " << el1.getCount() << std::endl;

    SmartPointer<int> el2 = el1; // используем конструктор копирования
    // создаётся новый SmartPointer, который указывает на тот же объект, что и el1
    // счётчик ссылок увеличивается до 2
    std::cout << "Reference count after copying el2: " << el1.getCount() << std::endl;

    {
        SmartPointer<int> el3; // создаём ещё один SmartPointer
        el3 = el1; // используем оператор присваивания
        std::cout << "Reference count after assigning el3: " << el1.getCount() << std::endl;
    } // el3 выходит из области видимости здесь

    std::cout << "Reference count after el3 goes out of scope: " << el1.getCount() << std::endl;

    std::cout << std::endl;

    int* arr = new int[5]{10, 20, 30, 40, 50}; // создаём массив из 5 целых чисел
    SmartPointer<int> ar1(arr, 5); // создаём SmartPointer на массив int

    std::cout << "Array elements: ";
    for (int i = 0; i < ar1.getSize(); ++i) {
        std::cout << ar1[i] << " "; // используем оператор индексации
    }
    std::cout << std::endl;

    std::cout << "Reference count after creating ar1: " << ar1.getCount() << std::endl;

    SmartPointer<int> ar2 = ar1; // используем конструктор копирования
    std::cout << "Reference count after copying ar2: " << ar1.getCount() << std::endl;

    {
        SmartPointer<int> ar3; // создаём ещё один SmartPointer
        ar3 = ar1; // используем оператор присваивания
        std::cout << "Reference count after assigning ar3: " << ar1.getCount() << std::endl;
    } // ar3 выходит из области видимости здесь

    std::cout << "Reference count after ar3 goes out of scope: " << ar1.getCount() << std::endl;

    return 0;
}
