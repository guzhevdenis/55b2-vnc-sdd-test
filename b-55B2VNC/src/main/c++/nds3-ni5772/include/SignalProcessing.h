#include <vector>
#include <unistd.h>
#include <cmath>
#include <numeric>
#include <cstdint>
#include <stdexcept>

double count_average(std::vector <uint64_t> & data_buffer, size_t size);
int64_t convertDoubleToInt64(double number);