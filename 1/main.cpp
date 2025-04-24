#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

class Vector3 {
protected:
    double x, y, z;

public:
    Vector3() : x(0), y(0), z(0) {} // конструктор по умолчанию

    Vector3(double x, double y, double z) : x(x), y(y), z(z) {} // конструктор с параметрами

    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {} // конструктор копирования

    Vector3& operator+=(const Vector3& other) { // перегруженный оператор +=
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3 operator+(const Vector3& t) const { // перегруженный оператор +
        return Vector3(x + t.x, y + t.y, z + t.z);
    }

    void show() const {
        cout << "{" << x << "; " << y << "; " << z << "}" << endl;
    }

    ~Vector3() {} // деструктор
};

class PlaneVector : public Vector3 {
protected:
    double A, B, C, D; // коэффициенты плоскости

public:

    PlaneVector() : Vector3(), A(0), B(0), C(1), D(0) {} // конструктор по умолчанию (плоскость z = 0)

    PlaneVector(double x, double y, double z, double A, double B, double C, double D)
        : Vector3(x, y, z), A(A), B(B), C(C), D(D) { // конструктор с параметрами
        if (!isOnPlane()) {
            // использование ostringstream для формирования сообщения
            std::ostringstream oss;
            oss << "The vector {" << x << ", " << y << ", " << z << "} does not lie on the plane "
            << A << "x + " << B << "y + " << C << "z = " << D << ".";
            throw std::invalid_argument(oss.str());
            }
    }

    PlaneVector(const PlaneVector& other)
        : Vector3(other), A(other.A), B(other.B), C(other.C), D(other.D) {} // конструктор копирования

    ~PlaneVector() {} // деструктор

    bool isOnPlane() const { // метод для проверки, лежит ли вектор на плоскости
        return A * x + B * y + C * z == D;
    }

    void show() const {
        std::cout << "Vector: ";
        Vector3::show(); // вызов метода show базового класса
        std::cout << "Plane: " << A << "x + " << B << "y + " << C << "z = " << D << "\n";
    }
};

int main() {
    Vector3 a(1, 2, 3);
    std::cout << "a = ";
    a.show();

    Vector3 b(4, 5, 6);
    std::cout << "b = ";
    b.show();

    Vector3 c = a + b;
    std::cout << "c = a + b = ";
    c.show();

    std::cout << "" << std::endl;

    try {
        PlaneVector d(1, 2, 3, 1, 1, 1, 6); // плоскость x + y + z = 6
        d.show(); // вывод вектора и уравнения плоскости

        if (d.isOnPlane()) {
            std::cout << "The vector lies on the plane.\n";
        } else {
            std::cout << "The vector does not lie on the plane.\n";
        }

        std::cout << "" << std::endl;

        PlaneVector h(0, 0, 6, 1, 1, 1, 6); // плоскость x + y + z = 6
        h.show(); // вывод вектора и уравнения плоскости

        if (h.isOnPlane()) {
            std::cout << "The vector lies on the plane.\n";
        } else {
            std::cout << "The vector does not lie on the plane.\n";
        }

        std::cout << "" << std::endl;

        // пример, который вызовет исключение
        PlaneVector f(2, 4, 6, 1, 1, 1, 6);
        f.show();

    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl; // выводим сообщение об ошибке
    }

    return 0;
}
