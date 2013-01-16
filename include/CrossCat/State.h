#ifndef GUARD_state_h
#define GUARD_state_h

#include <set>
#include <vector>
#include "View.h"

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
typedef boost::numeric::ublas::matrix<double> MatrixD;

class State {
 public:
  State(MatrixD &data,
	std::vector<int> global_row_indices, std::vector<int> global_col_indices,
	int N_GRID=31);
  //
  // getters
  int get_num_cols() const;
  int get_num_views() const;
  std::vector<int> get_view_counts() const;
  double get_crp_alpha() const;
  double get_crp_score() const;
  double get_data_score() const;
  double get_marginal_logp() const;
  //
  // mutators
  double insert_feature(int feature_idx, std::vector<double> feature_data,
			View &which_view);
  double sample_insert_feature(int feature_idx, std::vector<double> feature_data,
			       View &singleton_view);
  double remove_feature(int feature_idx, std::vector<double> feature_data,
			View** p_p_singleton_view);
  double transition_feature(int feature_idx, std::vector<double> feature_data);
  double transition_features(MatrixD &data);
  View& get_new_view();
  View& get_view(int view_idx);
  void remove_if_empty(View& which_view);
  double transition_view_i(int which_view,
			 std::map<int, std::vector<double> > row_data_map);
  double transition_views(MatrixD &data);
  double transition_crp_alpha();  
  double transition(MatrixD &data);
  //
  // calculators
  double calc_feature_view_predictive_logp(std::vector<double> col_data,
					   View v,
					   double &crp_log_delta,
					   double &data_log_delta) const;
					   
  std::vector<double> calc_feature_view_predictive_logps(std::vector<double> col_data) const;
  //
  // helpers
  double calc_crp_marginal() const;
  std::vector<double> calc_crp_marginals(std::vector<double> alphas_to_score) const;
 private:
  // parameters
  double crp_alpha;
  double crp_score;
  double data_score;
  std::vector<double> crp_alpha_grid;
  // lookups
  std::set<View*> views;
  std::map<int, View*> view_lookup;  // global_column_index to View mapping
  // sub-objects
  RandomNumberGenerator rng;
  // resources
  double draw_rand_u();
  int draw_rand_i(int max);
  // helpers
  void construct_hyper_grids(boost::numeric::ublas::matrix<double> data,
			     int N_GRID);
};

#endif // GUARD_state_h