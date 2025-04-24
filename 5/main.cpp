#include <iostream>
#include <iterator>
#include <stdexcept>

using namespace std;

template <typename T>
class List {
protected:
    struct Node { // структура, представляющая узел списка
                  // каждый узел содержит данные типа T и указатель на следующий узел
        T data;
        Node* pNext;
        Node(): pNext(nullptr){};
        Node(const T &l): data(l), pNext(nullptr){};
    };

private:
    Node* head;
    int size;

public:
    List() : head(nullptr), size(0) {} // конструктор по умолчанию

    ~List(){ // деструктор
        while (head){
            pop_front();
        }
    };

    void push_front(const T &l){ // метод добавляет новый узел с данными l в начало списка
        Node* node = new Node(l);
        node -> pNext = head;
        head = node;
        ++size;
    };

    void pop_front(){ // метод удаляет первый узел списка, если он существует
        if (head){
            Node* NewHead = head -> pNext;
            delete head;
            head = NewHead;
            --size;
        }
    }

    void print(){ // метод выводит все элементы списка
        Node* p = head;
        while (p != nullptr){
            cout << p -> data << " ";
            p = p -> pNext;
        }
        cout << endl;
    }

    // итератор для обхода списка в прямом порядке
    class ForwardIterator: public iterator<forward_iterator_tag, T> {
    protected:
        Node* position; // указатель на текущую позицию итератора

        ForwardIterator(Node* p){ // конструктор, принимающий указатель на узел
            position = p;
        }

        friend class List;

    public:
        ForwardIterator(){ // конструктор по умолчанию
            position = nullptr;
        }

        ForwardIterator(const ForwardIterator &iterator){ // конструктор копирования
            position = iterator.position;
        }

        ForwardIterator& operator=(const ForwardIterator &iterator){ // оператор присваивания
            position = iterator.position;
            return *this;
        }

        ForwardIterator& operator++() {
        // оператор инкремента предназначен для перемещения итератора на следующий узел в списке
            if (position) {
                position = position->pNext;
            }
            return *this;
        }

        T& operator*() {
        // оператор разыменования, возвращающий данные текущего узла
        // позволяет изменять данные, на которые указывает итератор
            if (!position) {
                throw std::runtime_error("Dereferencing null iterator");
            }
            return position->data;
        }

        const T& operator*() const {
        // константная версия оператора разыменования
        // не позволяет изменять данные, на которые указывает итератор
            if (!position) {
                throw std::runtime_error("Dereferencing null iterator");
            }
            return position->data;
        }

        friend bool operator==(const ForwardIterator &it1, const ForwardIterator &it2){
            return it1.position == it2.position;
        }

        friend bool operator!=(const ForwardIterator &it1, const ForwardIterator &it2){
            return it1.position != it2.position;
        }
    };

    // итератор для обхода списка в обратном порядке
    class ReverseIterator: public ForwardIterator {
    protected:
        Node *headPosition;

        ReverseIterator(Node *p, Node *hp) : ForwardIterator(p) {
        // конструктор, принимающий указатель на текущий узел и указатель на голову списка
            headPosition = hp;
        }

         friend class List;

    public:
        ReverseIterator() : ForwardIterator() {}; // конструктор по умолчанию

        ReverseIterator& operator++() {
        // оператор инкремента предназначен для перемещения итератора на предыдущий узел в списке
            if (this->position == nullptr) {
                this->position = headPosition;
                while (this->position && this->position->pNext) {
                    this->position = this->position->pNext;
                }
            } else {
                Node* p = headPosition;
                while (p && p->pNext != this->position) {
                    p = p->pNext;
                }
                this->position = p;
                }
            return *this;
        }

        T& operator*() { // оператор разыменования, возвращающий данные текущего узла
            if (!this->position) {
                throw std::runtime_error("Dereferencing null iterator");
            }
            return this->position->data;
        }

        const T& operator*() const { // константная версия оператора разыменования
            if (!this->position) {
                throw std::runtime_error("Dereferencing null iterator");
            }
            return this->position->data;
        }
    };

    // методы fBegin() и fEnd() предоставляют итераторы для обхода списка в прямом порядке
    ForwardIterator fBegin() const { // метод возвращает итератор, указывающий на первый узел списка
        return ForwardIterator(head);
    }

    ForwardIterator fEnd() const { // метод возвращает итератор, который указывает на nullptr
        return ForwardIterator(nullptr);
    }

    // методы rBegin() и rEnd() предоставляют итераторы для обхода списка в обратном порядке
    ReverseIterator rBegin() const { // метод возвращает итератор, который указывает на последний узел списка
        if (head == nullptr) {
            return ReverseIterator(nullptr, head);
        }
        Node* lastNode = head;
        while (lastNode->pNext != nullptr) {
            lastNode = lastNode->pNext;
        }
        return ReverseIterator(lastNode, head);
    }

    ReverseIterator rEnd() const { // метод возвращает итератор, который указывает на nullptr
        return ReverseIterator(nullptr, head);
    }
};

int main() {
    List<int> l = List<int> ();
    l.push_front(5);
    l.push_front(4);
    l.push_front(3);
    l.push_front(2);
    l.push_front(1);
    l.print();
    std::cout << "" << std::endl;
    for (auto i = l.fBegin(); i != l.fEnd(); ++i){
        cout << *(i) << endl;
    }
    std::cout << "" << std::endl;
    for (auto i = l.rBegin(); i != l.rEnd(); ++i){
        cout << *(i) << endl;
    }
    return 0;
}
