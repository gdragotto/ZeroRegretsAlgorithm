#include <algorithm>
#include <armadillo>
#include <fstream>
#include <random>

template <class T> static T Round(T a) {
  static_assert(std::is_floating_point<T>::value, "Round<T>: T must be floating point");

  return (a > 0) ? ::floor(a + static_cast<T>(0.5)) : ::ceil(a - static_cast<T>(0.5));
}

template <class T> static T round(T a, int places) {
  static_assert(std::is_floating_point<T>::value, "Round<T>: T must be floating point");

  const T shift = pow(static_cast<T>(10.0), places);

  return Round(a * shift) / shift;
}
using namespace std;

int main(int argc, char **argv) {

  string instanceFolder = "../../instance_generators/instances_quad/convex_generated/";

  std::vector<std::tuple<int, int, int, int>> instances = {{2, 1, -500, 500},
																			  {3, 1, -50, 50},
																			  {4, 1, -5, 5},
																			  {2, 2, -5, 5},
																			  {6, 1, 0, 5},
																			  {2, 3, 0, 5},
																			  {3, 2, 0, 5},
																			  //{8, 10, -3, 3},
																			  {10, 2, -5, 5},
																			  {20, 5, -10, 10}

  };
  for (int boundType : {1, 2}) {
	 for (int insType : {1, 2}) {


		for (auto &el : instances) {

		  int n  = std::get<0>(el);
		  int m  = std::get<1>(el);
		  int RQ = 5;

		  int    boundFactor = boundType == 2 ? 2 : 1;
		  double factor      = insType == 1 ? 0.1 : 0.01;
		  string iType       = insType == 1 ? "H" : "L";
		  string bType       = boundType == 1 ? "A" : "B";

		  string   instanceName = "Quadratic-" + to_string(n) + "-" + to_string(m) + "-" + bType + "-" + iType;
		  ofstream outFile;
		  outFile.open(instanceFolder + instanceName + ".txt", ios::out);


		  outFile << n << " " << m << endl;
		  outFile << std::get<2>(el) * (boundFactor) << " " << std::get<3>(el) * (boundFactor) << endl;
		  outFile.close();


		  // Generate M positive semidefinite
		  arma::mat M;
		  bool      test = true;
		  while (test) {
			 // Random matrix
			 M = arma::conv_to<arma::mat>::from(arma::randi(m * n, m * n, arma::distr_param(-RQ, +RQ)));
			 // Make it PSD
			 M    = (M * M.t());
			 test = !M.is_sympd();
		  }

		  double                                 MMax = M.max();
		  std::default_random_engine             generator;
		  std::uniform_real_distribution<double> distribution(-factor * MMax, factor * MMax);
		  std::cout << "Ranges are: " << -factor * MMax << " and " << factor * MMax << std::endl;
		  //


		  for (int i = 0; i < n; ++i) {

			 for (int j = i * m; j < (i + 1) * m; ++j) {
				for (int k = (i + 1) * m; k < M.n_cols; ++k) {
				  // std::cout << "Player "<<i<<":"<< j<<","<<k<<std::endl;
				  double vjk = round(distribution(generator), 1);
				  M.at(j, k) = M.at(j, k) + vjk;
				  M.at(k, j) = M.at(k, j) - vjk;
				}
			 }
		  }

		  M.print("M");
		  for (int i = 0; i < n; ++i) {

			 arma::mat Q = M.submat(i * m, i * m, (i + 1) * m - 1, (i + 1) * m - 1);
			 Q.print("Q");
			 Q.save(instanceFolder + instanceName + "-" + std::to_string(i) + "-Q.matrix");

			 arma::vec c = arma::conv_to<arma::vec>::from(arma::randi(m, arma::distr_param(-RQ, +RQ)));
			 c.print("c");
			 c.save(instanceFolder + instanceName + "-" + std::to_string(i) + "-c.matrix");

			 for (int o = 0; o < n; ++o) {
				if (i != o) {
				  arma::mat C = M.submat(i * m, o * m, (i + 1) * m - 1, (o + 1) * m - 1);
				  C.print("C between " + std::to_string(i) + " and " + std::to_string(o));
				  C.save(instanceFolder + instanceName + "-" + std::to_string(i) + "-C-" + std::to_string(o) + ".matrix");
				}
			 }
		  }
		}
	 }
  }

  return EXIT_SUCCESS;
}
