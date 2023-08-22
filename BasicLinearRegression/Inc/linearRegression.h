#ifndef LINEAR_REGRESSION_H_
#define LINEAR_REGRESSION_H_

#include <vector>
#include <ostream>

class LinearRegression {
public:
  LinearRegression(bool fit_intercept = true);
  void fit(const std::vector<double>& X, const std::vector<double>& y);
  double score(std::vector<double> X, std::vector<double> y);
  double predict(double x) const;
  double mean(std::vector<double>& values);
  std::vector<double> predict(std::vector<double> predicted_y) const;
  double getIntercept() const;
  double getSlope() const;
private:
  double m_slope { };
  double m_intercept { };
  bool m_fit_intercept { };
};
std::ostream& operator<<(std::ostream& str, std::vector<double> values);

#endif // !LINEAR_REGRESSION_H_
