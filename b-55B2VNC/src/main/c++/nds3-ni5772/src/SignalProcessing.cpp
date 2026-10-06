
#include "SignalProcessing.h"

double count_average(std::vector <uint64_t> & data_buffer, size_t size) {

  double average = 0.0;
  if (!data_buffer.empty()) {

    auto startIt = data_buffer.begin();
    auto endIt = startIt + size;

    double sum = std::accumulate(startIt, endIt, 0.0);
    average = sum / size;

    data_buffer.clear();
  } else {

  }
  return average;
}

int64_t convertDoubleToInt64(double number) {
  //
  int64_t integerPart = static_cast<int64_t>(number);  //
  double fractionalPart = number - integerPart;        //
  // Check is integer part fit to 16 bit
  if (integerPart < 0 || integerPart >= (1LL << 16)) {
    throw std::overflow_error("Error of conversion");
  }
  // Move integer part to the left by 48 bit
  int64_t result = integerPart << 48;
  // Transform fractional part into 48 bit
  int64_t fractionalBits = static_cast<int64_t>(fractionalPart * (1LL << 48));
  result |= fractionalBits;

  return result;
}