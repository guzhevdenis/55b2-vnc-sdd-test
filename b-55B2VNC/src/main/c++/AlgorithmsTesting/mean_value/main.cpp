#include <iostream>
#include <cstdint>
#include <cmath>
#include <vector>
#include <numeric>



std::int64_t mean_val_calc(std::vector<std::uint64_t> data_buffer2){
        // Если накоплено больше 1000 элементов, вычисляем среднее и добавляем в вектор countRateFluctuations
        auto startIt = data_buffer2.begin();
        auto endIt = startIt + 2;
        double sum = std::accumulate(startIt, endIt, 0.0);
        double MSVAverage = sum / 2;
		std::cout << "Sum: " << sum << std::endl;
		std::cout << "Average: " << MSVAverage << std::endl;
		std::cout << "Converted uint64_t: " << data_buffer2.size() << std::endl;
		std::cout << "Converted uint64_t: " << data_buffer2.front() << std::endl;
		std::cout << "Converted uint64_t: " << data_buffer2.back() << std::endl;
        // Удаляем обработанные элементы
        data_buffer2.clear();
}
    

int main() {
    std::vector<std::uint64_t> data_buffer2 {1, 2,3,4,5};
    mean_val_calc(data_buffer2);
    mean_val_calc(data_buffer2);
    return 0;
}