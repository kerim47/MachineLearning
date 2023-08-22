#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <random>
#include "../Inc/linearRegression.h"
#include "../Inc/csv.h"

int getColumnIndex(const CSVIterator& loop, const std::string& columnName) {
    for (size_t i = 0; i < (*loop).size(); ++i) {
        if ((*loop)[i] == columnName) {
            return static_cast<int>(i);
        }
    }
    throw std::runtime_error("Column not found: " + columnName);
}


template<typename T>
struct Data {
    std::vector<T> x_train;
    std::vector<T> x_test;
    std::vector<T> y_train;
    std::vector<T> y_test;
};

template<typename T>
Data<T> train_test_split(std::vector<T>& X, 
                         std::vector<T>& y, 
                         double test_ratio = 0.4, 
                         unsigned int seed = 0,
                         bool shuffle = true) {
    Data<T> result;

    size_t size { X.size() };
    if(size != y.size())
        throw std::runtime_error("X and y not equal length!");

    if(X.empty() || y.empty())
        return Data<T>();

    size_t test_size {};

    if (test_ratio <= 0.0 || test_ratio >= 1.0)
        throw std::invalid_argument("test_ratio should be between 0 and 1 (exclusive).");

    test_size = static_cast<size_t>(size * test_ratio);

    std::vector<size_t> indices(size);
    std::iota(indices.begin(), indices.end(), 0);

    std::mt19937 gen(seed);
    if(shuffle) {
        std::shuffle(indices.begin(), indices.end(), gen);
        std::shuffle(X.begin(), X.end(), gen);
        std::shuffle(y.begin(), y.end(), gen);
    }

    for (size_t i = 0; i < size; ++i) {
        if (i < test_size) {
            result.x_test.push_back(X[indices[i]]);
            result.y_test.push_back(y[indices[i]]);
        } else {
            result.x_train.push_back(X[indices[i]]);
            result.y_train.push_back(y[indices[i]]);
        }
    }

    return result;
}

template<typename T>
std::ostream& operator<<(std::ostream& str, std::vector<T> values) {
  str << "[ ";
  for(size_t i = 0; i< values.size(); i++) {
    if(i != values.size() - 1) 
      str << values[i] << ", ";
    else 
      str << values[i];
  }

  str << " ]";
  return str;
}


bool isExtensionFile(const std::string_view file, std::string_view extension = "csv") {
  size_t position { file.find_last_of(".") };
  if(position != std::string::npos) {
    std::string_view ext = file.substr(position + 1);
    return extension == ext;
  }
  return false;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <csv_filename>" << std::endl;
    return EXIT_FAILURE;
  }
  std::string file_name { argv[1] };

  if(!isExtensionFile(file_name)) {
    std::cerr << "Error: Specified file is not a CSV file." << std::endl;
    return 1;
  }

  std::vector<double> X {};
  std::vector<double> y {};
  double total_bill_value;
  double tip_value;

  std::string total_bill_str { "total_bill" };
  std::string tip_str { "tip" };

  std::ifstream file { file_name };
  if (!file) {
    std::cerr << "Error: File \"" << file_name << "\" not found." << std::endl;
    return 1;
  }
  CSVRange* range { new CSVRange{file} };


  CSVIterator loop = range->begin();
  int index_total_bill = getColumnIndex(loop, total_bill_str);
  int index_tip = getColumnIndex(loop, tip_str);

  for (++loop; loop != CSVIterator(); ++loop) {
    
    std::istringstream total_bill_ss{ std::string { (*loop)[index_total_bill] }};
    total_bill_ss >> total_bill_value;
    
    std::istringstream tip_ss{ std::string { (*loop)[index_tip] } };
    tip_ss >> tip_value;
    
    X.push_back(total_bill_value);
    y.push_back(tip_value);
  }

  LinearRegression* model = new LinearRegression();

  double test_ratio = 0.4;
  unsigned int seed = 42;

  Data<double> result = train_test_split(X, y, test_ratio, seed);
  model->fit(result.x_train, result.y_train);

  std::cout << "x_test  = " << result.x_test  << std::endl;
  std::cout << "y_test  = " << result.y_test  << std::endl;
  std::cout << "x_train = " << result.x_train << std::endl;
  std::cout << "y_train = " << result.y_train << std::endl;

  std::vector<double> y_pred = model->predict(result.x_test);

  std::cout << "Eğim (Slope)              : " << model->getSlope() << std::endl;
  std::cout << "Kesme Terimi (Intercept)  : " << model->getIntercept() << std::endl;
  std::cout << "R-kare (R-squared) Değeri : " << model->score(result.y_test, y_pred) << std::endl;
  std::cout << "x_test Tahminleri         : " << y_pred << std::endl;

  delete model;
  delete range;

  return 0;
}
