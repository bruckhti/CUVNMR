/*
* Copyright (c) 2026 Tim Bruckhoff / Affiliation: Ruprecht-Karls-University Heidelberg, Germany
*
* This source code is licensed under the EUPL v1.2.
* See the LICENSE file in the repository root directory for full details.
*
* CITATION:
* If you use this software for your research, please cite it as
* described in the CITATION.cff file located in the root directory.
*
* SPDX-License-Identifier: EUPL-1.2
*/

#pragma once
#include <vector>
#include "CMainTab2.h"
#include "NelderMeadSimplex.h"

class CMainTab2;
void break_function_placeholder();

class NelderMeadSimplex
{
public:
	NelderMeadSimplex(double (*function)(std::vector<double>), CMainTab2* p_Tab2);
	~NelderMeadSimplex();
	void InitializeBreak(std::string breaktype);
	bool downhillsimplex(std::vector<double> parameter_set, double trust_region_delta, bool use_relative_values);
	void set_optimization_parameter(double alpha, double gamma, double beta, double sigma);
	void set_convergence_criteria(int max_iterations, double convergence_value, double convergence_parameter);
	void fconv_type(int conv_type);
	void set_break_function(void (*break_function)());
	double get_optimized_parameter(int index);
	int get_number_of_iterations();
	double get_convergence();
	double get_convergence(int index);
	double get_result_value();
private:
	CMainTab2* m_Tab2;
	std::string m_breaktype;
	double (*function_pointer)(std::vector<double>);
	void (*break_function_pointer)();
	std::vector<double> trust_region;
	std::vector<double> guess_matrix;
	std::vector<double> pointvector_matrix;
	std::vector<double> function_values_vector;
	double C_trust_region_delta_manip;
	int C_number_of_parameters;
	std::vector<double> C_parameter_set;
	double C_trust_region_delta;
	bool C_use_relative_values;
	std::vector<double> x_r;
	double y_r;
	std::vector<double> x_e;
	double y_e;
	std::vector<double> x_con;
	double y_con;
	std::vector<double> mean_point_vector;
	std::vector<double> result_list;
	std::vector<double> a_g_b_s;
	int C_max_iterations;
	double C_convergence_value;
	double C_convergence_parameter;
	int C_conv_type;
	double last_optimum;
	std::vector<double> last_optimum_parameters;
	double new_optimum;
	std::vector<double> new_optimum_parameters;
	bool convergence;
	double convergence_difference;
	std::vector<double> convergence_difference_parameters;
	int counter;
	double fxn_function(std::vector<double> parameters);
	void NULLappend_dvector(std::vector<double>& vector, int num_of_elements);
	void save_variables(std::vector<double> parameter_set, double trust_region_delta, bool use_relative_values);
	void set_trust_region(std::vector<double>& trust_region);
	void copy_vector(std::vector<double>& paste_vector, std::vector<double> copy_vector);
	bool copy_matrix_column(std::vector<double>& paste_matrix, std::vector<double> copy_matrix, int paste_column, int copy_column, int number_of_rows);
	bool replace_matrix_column_by_vector(std::vector<double>& paste_matrix, std::vector<double> copy_vector, int paste_column, int number_of_rows);
	void create_guess_matrix();
	void calculate_guess_solutions();
	double check_results();
	void sort_results(int best_index, int middle_index, int worst_index);
	bool reflexion();
	void expansion();
	bool better_xi();
	bool contraction();
	void compression();
	bool check_for_convergence();
	void set_new_convergence_essentials(bool initialize);
	void algorithm_break();
};