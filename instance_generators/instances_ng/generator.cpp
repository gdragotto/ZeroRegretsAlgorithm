#include "dijkstra.h"
#include <armadillo>
#include <igraph.h>
#include <iostream>
using namespace std;

void saveToGephi(arma::sp_mat &G, string filename) {
  assert(G.is_square());
  ofstream out;
  out.open(filename);

  int nodes = static_cast<int>(G.n_rows);
  out << ";";
  for (int i = 0; i < nodes; i++)
	 out << to_string(i) << ((i == (nodes - 1)) ? "\n" : ";");

  for (unsigned int j = 0; j < nodes; ++j) {
	 out << to_string(j) << ";";
	 for (unsigned int k = 0; k < nodes; ++k) {
		out << to_string(static_cast<int>(G.at(j, k))) << ((k == (nodes - 1)) ? "\n" : ";");
	 }
  }

  out.close();
}

arma::sp_mat random_graph(int n, int type = 9, int colSize = -1) {

  std::random_device                 rd;
  static std::mt19937                engine(rd());
  std::uniform_int_distribution<int> weights(20, 100);
  std::uniform_int_distribution<int> nodes(0, n - 1);

  arma::sp_mat G(n, n);

  if (type != 3) {
	 igraph_t         iG;
	 igraph_adjlist_t adjlist;
	 igraph_vector_t  edges;
	 igraph_real_t    diameter;

	 bool cond = false;
	 do {
		igraph_empty(&iG, 0, true);
		switch (type) {
		case 0: {
		  std::vector<int>                   islands_candidates = {1, 2, 5, 10, 20, 50};
		  std::uniform_int_distribution<int> randomI(0, islands_candidates.size() - 1);
		  std::uniform_int_distribution<int> randomPr(1, 100);
		  int                                islands = islands_candidates.at(randomI(engine));
		  igraph_simple_interconnected_islands_game(
				&iG, islands, float(n) / float(islands), float(randomPr(engine)) / 100, 1);
		} break;

		case 1: {
		  std::uniform_int_distribution<int> randomPr(1, 100);
		  std::uniform_int_distribution<int> randomA(1, 10);
		  igraph_barabasi_game(&iG,
									  n,
									  float(randomPr(engine)) / 100,
									  1,
									  0,
									  0,
									  randomA(engine),
									  1,
									  IGRAPH_BARABASI_PSUMTREE,
									  0);
		} break;

		default: {
		  std::uniform_int_distribution<int> randomP(1, 10);
		  std::uniform_int_distribution<int> randOddEven(1, 2);
		  double p = double(randomP(engine)) / (randOddEven(engine) == 1 ? 1000 : 10000);
		  igraph_erdos_renyi_game(&iG, IGRAPH_ERDOS_RENYI_GNP, n, p, true, false);
		}
		}

		// If the diameter is low, generate another
		igraph_diameter(&iG, &diameter, nullptr, nullptr, nullptr, true, true);
		cond = (diameter < (float(n) / 20));
		if (cond)
		  igraph_empty(&iG, 0, IGRAPH_DIRECTED);
		else {
		  igraph_adjlist_init(&iG, &adjlist, IGRAPH_ALL, IGRAPH_NO_LOOPS, IGRAPH_NO_MULTIPLE);
		  cout << "Found a Graph with diameter: " << diameter << endl;
		  int numEdges = igraph_ecount(&iG);
		  igraph_vector_init(&edges, 0);
		  igraph_get_edgelist(&iG, &edges, 0);

		  for (int i = 0, j = 0; i < numEdges; i++, j += 2) {
			 G.at((long)VECTOR(edges)[j], (long)VECTOR(edges)[j + 1]) = weights(engine);
		  }
		  igraph_adjlist_clear(&adjlist);
		  igraph_vector_clear(&edges);
		}

	 } while (cond);

	 igraph_vector_destroy(&edges);
	 igraph_adjlist_destroy(&adjlist);
	 igraph_destroy(&iG);
  } else {
	 int x, y;
	 // This is a grid!
	 x = colSize, y = n / colSize;

	 std::uniform_int_distribution<int> binary(1, 10);
	 std::uniform_int_distribution<int> xRandom(0, x - 1);

	 // y is the number of columns in the grid
	 for (int j = 0; j < y; j++) {
		// x is the number of rows in the grid
		for (int i = 0; i < x; i++) {
		  // Horizontal arcs
		  if (j != (y - 1)) {
			 G.at(j * x + i, (j + 1) * x + i) = weights(engine);

			 // probability of connection
			 if (binary(engine) > 7) {
				bool cond = true;
				while (cond) {
				  int randomId = xRandom(engine);
				  // Check this node has no connections...
				  if (G.at(j * x + i, (j + 1) * x + randomId) <= 0) {
					 G.at(j * x + i, (j + 1) * x + randomId) = weights(engine);
					 cond                                    = false;
				  }
				}
			 }
		  }
		  // Vertical Arcs
		  if (i != x - 1) {
			 G.at(j * x + i, j * x + i + 1) = weights(engine);
		  }
		}
	 }

	 std::uniform_int_distribution<int> randomNode(0, n - 1);
	 // Then we have a grid graph, and we want to add 2 pair source,sink
	 int source1 = 0;
	 int source2 = x - 1;
	 int sink1   = n - 1;
	 int sink2   = n - x;
	 //@todo
	 // Reset outgoing from source

	 /*
	 G.row(source1).zeros();
	 G.row(source2).zeros();
	 // Reset incoming from sink
	 G.col(sink1).zeros();
	 G.col(sink2).zeros();


	 // Add outgoing from source to first layer
	 for (const auto &source : {source1, source2}) {
		// Connect the source to any other nodes in the first layer
		for (unsigned int i = 1; i < (x - 1); ++i)
		  G.at(source, i) = weights(engine);
	 }

	 // Add incoming from destinations
	 for (const auto &sink : {sink1, sink2}) {
		// Connect the source to any other nodes in the first layer
		for (unsigned int i = 0; i < (x - 2); ++i)
		  G.at((y - 1) * x + 1 + i, sink) = weights(engine);
	 }
	 */
  }
  // Reset auto edges
  G.diag() = arma::zeros(n);

  if (type != 3) {
	 int target1 = 0, target2 = 0;
	 for (int i = 0; i < n; ++i) {
		do {
		  target1 = nodes(engine);
		  target2 = nodes(engine);
		} while (target1 == target2 || G.at(target1, target2) > 0);
		G.at(target1, target2) = weights(engine);
	 }

	 for (int i = 0; i < n; ++i) {
		if (G.row(i).max() <= 0 || G.col(i).max() <= 0) {

		  int target = nodes(engine);
		  if (G.row(i).max() <= 0) {
			 // Add a random number of outgoing edges
			 while (target == i || G.at(i, target) > 0)
				target = nodes(engine);
			 G.at(i, target) = weights(engine);
		  }
		  target = nodes(engine);
		  if (G.col(i).max() <= 0) {
			 // Add a random number of ingoing edges
			 while (target == i || G.at(target, i) > 0)
				target = nodes(engine);
			 G.at(target, i) = weights(engine);
		  }
		}
	 }
  }

  assert(arma::max(G.diag()) < 1);
  if (type != 3) {
	 assert(!G.is_symmetric());
  }
  return G;
}

int main(int argv, char **args) {

  bool                found_st = false, found_graph = false, found_all = false;
  arma::sp_mat        G;
  arma::mat           AD, D;
  arma::vec           distances, predecessors;
  std::random_device  rd;
  static std::mt19937 engine(rd());
  int                 colSize = 5;

  vector<int> instances = {50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 1000};
  for (auto &ins : instances) {
	 for (int type = 3; type < 4; ++type) {

		string typeToString = "";
		switch (type) {
		case 0:
		  typeToString = "island";
		  break;
		case 1:
		  typeToString = "barbasi";
		  break;
		case 2:
		  typeToString = "gnp";
		  break;
		default:
		  typeToString = "grid";
		}

		int                                size = ins;
		std::uniform_int_distribution<int> randomNode(0, size - 1);
		found_st    = false;
		found_graph = false;
		found_all   = false;
		while (!found_graph || !found_st) {
		  G = random_graph(size, type, colSize);
		  // Degrees of separation
		  D.zeros(size, size);
		  // 1 for each arg, without weight
		  AD = G;
		  AD.elem(find(AD > 0)).ones();
		  double cost;
		  for (int i = 0; i < size; ++i) {
			 // Run Dijkstra from the source node i
			 dijkstra(i, -1, G, cost, distances, predecessors);
			 // If some distances are positive (e.g., there exists at least a path
			 // to another node)
			 auto found =
				  arma::conv_to<arma::vec>::from(arma::find(distances > 0 && distances < 1e10));
			 if (!found.is_empty()) {
				// Compute degrees of separation instead of distances from i
				// We use a matrix where all the edges' weights are set to 1.
				dijkstra(i, -1, arma::sp_mat(AD), cost, distances, predecessors);
				for (unsigned int j = 0; j < distances.size(); ++j) {
				  double distance = distances.at(j);
				  D.at(i, j)      = (distance > 0 && distance < 1e10) ? distance : 0;
				}
				found_graph = true;
			 }
		  }
		  if (found_graph) {

			 // Compute the best candidate source-sink based on the degrees of
			 // separation

			 arma::mat    Dtemp = D;
			 arma::sp_mat ST_Paths(size, size);
			 for (int i = 0; i < size / 5; ++i) {
				arma::uvec s = arma::ind2sub(arma::size(Dtemp), Dtemp.index_max());
				// Temporary reset. So that we can find other paths
				Dtemp.at(s(0), s(1)) = -1;
				// We know a path from s,t exits. Let's count them
				ST_Paths.at(s(0), s(1)) = ST_Paths.at(s(0), s(1)) + 1;
			 }

			 // Select the longest (s,t) paths
			 // Sum of cols (namely, paths going to the same sink)
			 arma::vec to_destination(size);
			 for (unsigned int i = 0; i < size; ++i)
				to_destination.at(i) = arma::accu(ST_Paths.col(i));

			 int minLength = 10;
			 Dtemp         = D;
			 Dtemp.elem(find(Dtemp <= minLength)).zeros();
			 std::vector<std::pair<int, int>> st_candidates;
			 arma::sp_mat                     Dtemp2(Dtemp);

			 if (type == 3) {
				st_candidates.push_back({0, size - 1});
				st_candidates.push_back({0, size - colSize});
				st_candidates.push_back({4, size - 1});
				st_candidates.push_back({4, size - colSize});
			 } else {
				for (arma::sp_mat::const_iterator e = Dtemp2.begin(); e != Dtemp2.end(); ++e) {
				  st_candidates.push_back({e.row(), e.col()});
				}
			 }

			 if (st_candidates.size() < 4 && type != 3) {
				cout << "\tNo ST candidates\n";
			 } else {
				cout << "\tGenerated " << st_candidates.size() << " ST candidates\n";
				std::uniform_int_distribution<int> randomPath(0, st_candidates.size() - 1);

				// Source1 source2 sink pairs
				auto SST_Pairs = arma::sp_mat(size, 3);
				int  sstNum    = 0;

				if (type == 3) {
				  found_all            = true;
				  SST_Pairs(sstNum, 0) = 0;
				  SST_Pairs(sstNum, 0) = colSize - 1;
				  SST_Pairs(sstNum, 0) = size - 1;
				  sstNum++;
				  SST_Pairs(sstNum, 0) = 0;
				  SST_Pairs(sstNum, 0) = colSize - 1;
				  SST_Pairs(sstNum, 0) = size - colSize;
				  sstNum++;
				  found_st = true;
				}

				while (sstNum < size && type != 3) {
				  int randomSource1 = 0, randomSource2 = 0, randomSink = 0;
				  int random = 0;

				  while ((randomSource1 == randomSource2 || randomSource2 == randomSink ||
							 randomSource1 == randomSink) ||
							(D.at(randomSource2, randomSink) <= minLength)) {
					 random        = randomPath(engine);
					 randomSource1 = st_candidates.at(random).first;
					 randomSink    = st_candidates.at(random).second;
					 randomSource2 = randomNode(engine);
				  }

				  found_st             = true;
				  SST_Pairs(sstNum, 0) = randomSource1;
				  SST_Pairs(sstNum, 1) = randomSource2;
				  SST_Pairs(sstNum, 2) = randomSink;
				  sstNum++;
				}

				auto SSTT_Pairs = arma::sp_mat(size, 6);
				int  ssttNum    = 0;
				if (found_st) {

				  if (type == 3) {
					 found_all              = true;
					 SSTT_Pairs(ssttNum, 0) = 0;
					 SSTT_Pairs(ssttNum, 1) = size - 1;
					 SSTT_Pairs(ssttNum, 2) = colSize - 1;
					 SSTT_Pairs(ssttNum, 3) = size - colSize;
					 SSTT_Pairs(ssttNum, 4) = 3;
					 SSTT_Pairs(ssttNum, 5) = size - 3;
					 ssttNum++;
				  }

				  // already found the first
				  // source sink pairs
				  while (ssttNum < size && type != 3) {
					 int randomSource1 = 0, randomSource2 = 0, randomSink1 = 0, randomSink2 = 0,
						  randomSource3 = 0, randomSink3 = 0;

					 int random1 = 0, random2 = 0, random3 = 0;

					 while (
						  (randomSource1 == randomSource2 || randomSource2 == randomSink1 ||
							randomSource1 == randomSink2 || randomSource1 == randomSink1 ||
							randomSource2 == randomSink2 || random1 == random2 ||
							randomSource3 == randomSource2 || randomSource3 == randomSource1 ||
							randomSink3 == randomSink2 || randomSink3 == randomSink1 ||
							random2 == random3 || random1 == random3) ||
						  (D.at(randomSource1, randomSink1) <= minLength ||
							D.at(randomSource2, randomSink2) <= minLength ||
							D.at(randomSource3, randomSink3) <= minLength) ||
						  // Interesting conditions
						  (D.at(randomSource1, randomSource2) >= 6 &&
							D.at(randomSource1, randomSource2) <= 0 &&
							D.at(randomSource2, randomSource1) >= 6 &&
							D.at(randomSource2, randomSource1) <= 0 &&
							D.at(randomSource3, randomSource1) >= 6 &&
							D.at(randomSource3, randomSource1) <= 0 &&
							D.at(randomSink1, randomSink2) >= 6 && D.at(randomSink1, randomSink2) <= 0 &&
							D.at(randomSink2, randomSink1) >= 6 && D.at(randomSink2, randomSink1) <= 0)

					 ) {
						random1       = randomPath(engine);
						random2       = randomPath(engine);
						random3       = randomPath(engine);
						randomSource1 = st_candidates.at(random1).first;
						randomSink1   = st_candidates.at(random1).second;
						randomSource2 = st_candidates.at(random2).first;
						randomSink2   = st_candidates.at(random2).second;
						randomSource3 = st_candidates.at(random3).first;
						randomSink3   = st_candidates.at(random3).second;
					 }

					 found_all              = true;
					 SSTT_Pairs(ssttNum, 0) = randomSource1;
					 SSTT_Pairs(ssttNum, 1) = randomSink1;
					 SSTT_Pairs(ssttNum, 2) = randomSource2;
					 SSTT_Pairs(ssttNum, 3) = randomSink2;
					 SSTT_Pairs(ssttNum, 4) = randomSource3;
					 SSTT_Pairs(ssttNum, 5) = randomSink3;
					 ssttNum++;
				  }
				}

				if (found_all) {

				  cout << "\tComputed a " << typeToString << " Graph with ST pairs: n=" << size
						 << ", max(D)=" << D.max() << endl;
				  SST_Pairs.resize(sstNum, 3);
				  SSTT_Pairs.resize(ssttNum, 6);
				  // Adjacency matrix with weights
				  G.save("instances_ng/generated/G_n_" + to_string(size) + "-" + typeToString + ".adj");
				  // Matrix with the minimum degrees of separation between (i,j)
				  D.save("instances_ng/generated/D_n_" + to_string(size) + "-" + typeToString + ".adj");
				  // Gephi adjacency matrix
				  saveToGephi(G,
								  "instances_ng/generated/Gephi_n_" + to_string(size) + "-" + typeToString +
										".csv");

				  // Source-Sink candidate pairs, (s,t,degrees of separation)
				  SST_Pairs.save("instances_ng/generated/SST_n_" + to_string(size) + "-" +
									  typeToString + ".adj");
				  SSTT_Pairs.save("instances_ng/generated/SSTT_n_" + to_string(size) + "-" +
										typeToString + ".adj");
				}
			 }
		  }
		}
	 }
  }
}
