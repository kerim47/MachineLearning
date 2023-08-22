#include <vector>
#include <cmath>
#include "../Inc/linearRegression.h"

LinearRegression::LinearRegression(bool fit_intercept) : m_fit_intercept(fit_intercept) {}

void LinearRegression::fit(const std::vector<double>& X, const std::vector<double>& y) {
  size_t size = X.size();
  if (size != y.size())
     throw std::runtime_error("Error: X and y have different sizes.");

  if(X.empty())
    throw std::runtime_error("X veya y vector degerleri boş olamaz.");

  double sum_x = 0.0;
  double sum_y = 0.0;
  double sum_xy = 0.0;
  double sum_x_squared = 0.0;

  for (size_t i = 0; i < size; ++i) {
    sum_x += X[i];
    sum_y += y[i];
    sum_xy += X[i] * y[i];
    sum_x_squared += X[i] * X[i];
  }

  double n = static_cast<double>(size);
  double slope{};
  if(m_fit_intercept)
     slope =  (n * sum_xy - sum_x * sum_y) / (n * sum_x_squared - sum_x * sum_x); 

  double intercept { (sum_y - slope * sum_x) / n };

  m_slope = slope;
  m_intercept = intercept;
}

double LinearRegression::score(std::vector<double> y, std::vector<double> y_pred) {
  size_t size = y.size();
  if (size != y_pred.size())
    throw std::runtime_error("Error: y and y_pred have different sizes.");

  if(y.empty() || y_pred.empty())
    throw std::runtime_error("y or y_pred vector values cannot be empty.");

  double squaredDiffLine = 0.0;
  double squaredMeanY = 0.0;
  double mean_y_pred = mean(y_pred);

  for (size_t i = 0; i < size; ++i) {
    squaredDiffLine += std::pow(y[i] - y_pred[i], 2);
    squaredMeanY += std::pow(y[i] - mean_y_pred, 2);
  }
  
  if (squaredMeanY == 0.0) {
    throw std::runtime_error("Mean of y_pred cannot be equal to mean of y.");
  }

  return 1 - (squaredDiffLine / squaredMeanY);
}

double LinearRegression::mean(std::vector<double>& values) {
  size_t size = values.size(); // Boyutu bir değişkende sakla
  if (size > 0) {
    double sum_values = 0.0;
    for (size_t i = 0; i < size; i++)
      sum_values += values[i];
    return sum_values / static_cast<double>(size);
  }
  return 0.0;
}

double LinearRegression::predict(double x) const {
  return m_slope * x + m_intercept;
}
std::vector<double> LinearRegression::predict(std::vector<double> predicted_y) const {
  std::vector<double> result;
  for(size_t i=0; i< predicted_y.size(); i++) {
    result.push_back(predict(predicted_y[i]));
  }
  return result;
}
double LinearRegression::getIntercept() const {
  return m_intercept;
}
double LinearRegression::getSlope() const {
  return m_slope;
}

std::ostream& operator<<(std::ostream& str, std::vector<double> values) {
  int size = values.size();
  str << "[ ";
  for (size_t i=0; i< size; i++)
    str << values[i] << ((i != size - 1) ? ", " : "" );
  str << " ]";
  return str;
}

