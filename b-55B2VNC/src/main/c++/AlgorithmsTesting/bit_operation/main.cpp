#include <iostream>
#include <cstdint>
#include <cmath>

int64_t convertDoubleToInt64(double number) {

    int64_t integerPart = static_cast<int64_t>(number); // Целая часть
    double fractionalPart = number - integerPart;       // Дробная часть

    // Проверяем, что целая часть помещается в 16 бит
    if (integerPart < 0 || integerPart >= (1LL << 16)) {
        throw std::overflow_error("Целая часть числа не помещается в 16 бит");
    }
    // Сдвигаем целую часть влево на 48 бит
    int64_t result = integerPart << 48;

    // Преобразуем дробную часть в 48 бит
    int64_t fractionalBits = static_cast<int64_t>(fractionalPart * (1LL << 48));

    // Добавляем дробную часть к результату
    result |= fractionalBits;

    return result;
}

int main() {
    try {
        double number = 7.6; 
        int64_t result = convertDoubleToInt64(number);

        std::cout << "Число: " << number << std::endl;
        std::cout << "Результат в int64: " << result << std::endl;

        std::cout << "Результат в двоичной системе: ";
        for (int i = 63; i >= 0; --i) {
            std::cout << ((result >> i) & 1);
        }
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}