#include <Eigen/Dense>
#include <iostream>

using Eigen::MatrixXd;
using Eigen::VectorXd;

int main() {
  MatrixXd m(2, 2);
  m(0, 0) = 1;
  m(1, 0) = 3;
  m(0, 1) = 2;
  m(1, 1) = m(0, 0) + m(0, 1);
  std::cout << m << std::endl;
  VectorXd v(4);
  v(0) = 1;
  v(1) = 2;
  v(2) = 3;
  v(3) = 0;
  std::cout << v << std::endl;
}
