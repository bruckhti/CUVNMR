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

#include "pch.h"
#include "NelderMeadSimplex.h"
#include "math_operations.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void NelderMeadSimplex::InitializeBreak(std::string breaktype)
{
	m_breaktype = breaktype;
}

NelderMeadSimplex::NelderMeadSimplex(double (*function)(std::vector<double>), CMainTab2* pTab2)
{
	function_pointer = function;
	set_break_function(break_function_placeholder);
	C_trust_region_delta_manip = 0.0;
	a_g_b_s.push_back(1.0);
	a_g_b_s.push_back(1.0);
	a_g_b_s.push_back(0.5);
	a_g_b_s.push_back(0.5);
	C_convergence_value = 0.001;
	C_max_iterations = 500;
	C_convergence_parameter = 0.01;
	C_conv_type = 0;
	convergence_difference = 0.0;
	m_Tab2 = pTab2;
}

void NelderMeadSimplex::set_optimization_parameter(double alpha, double gamma, double beta, double sigma)
{
	if (alpha > 0.0)
	{
		a_g_b_s[0] = alpha;
	}
	if (gamma > 0.0)
	{
		a_g_b_s[1] = gamma;
	}
	if (beta > 0.0 && beta < 1.0)
	{
		a_g_b_s[2] = beta;
	}
	if (sigma > 0.0 && sigma < 1.0)
	{
		a_g_b_s[3] = sigma;
	}
}

NelderMeadSimplex::~NelderMeadSimplex()
{
}

void NelderMeadSimplex::NULLappend_dvector(std::vector<double>& vector, int num_of_elements)
{
	for (int i = 0; i < num_of_elements; i++)
	{
		vector.push_back(0.0);
	}
}

void NelderMeadSimplex::save_variables(std::vector<double> parameter_set, double trust_region_delta, bool use_relative_values)
{
	C_number_of_parameters = parameter_set.size();
	C_trust_region_delta = trust_region_delta;
	C_use_relative_values = use_relative_values;
	C_parameter_set.clear();
	for (int i = 0; i < parameter_set.size(); i++)
	{
		C_parameter_set.push_back(parameter_set[i]);
	}
}

void NelderMeadSimplex::set_trust_region(std::vector<double>& trust_region)
{
	for (int o = 0; o < 3; o++)
	{
		for (int i = 0; i < C_number_of_parameters; i++)
		{
			if (C_use_relative_values == true)
			{
				C_trust_region_delta_manip = C_trust_region_delta * std::abs(C_parameter_set[i]);
			}
			else
			{
				C_trust_region_delta_manip = C_trust_region_delta;
			}
			if (o == 0)
			{
				trust_region[(o * C_number_of_parameters) + i] = C_parameter_set[i] - C_trust_region_delta_manip;
			}
			if (o == 1)
			{
				trust_region[(o * C_number_of_parameters) + i] = C_parameter_set[i];
			}
			else
			{
				trust_region[(o * C_number_of_parameters) + i] = C_parameter_set[i] + C_trust_region_delta_manip;
			}
		}
	}
}

void NelderMeadSimplex::copy_vector(std::vector<double>& paste_vector, std::vector<double> copy_vector)
{
	paste_vector.clear();
	for (int i = 0; i < copy_vector.size(); i++)
	{
		paste_vector.push_back(copy_vector[i]);
	}
}

void NelderMeadSimplex::calculate_guess_solutions()
{
	std::vector<double> temp_parameters;
	NULLappend_dvector(temp_parameters, C_number_of_parameters);
	for (int l = 0; l <= C_number_of_parameters; l++)
	{
		for (int k = 0; k < C_number_of_parameters; k++)
		{
			temp_parameters[k] = guess_matrix[(l * C_number_of_parameters) + k];
		}
		function_values_vector.push_back(fxn_function(temp_parameters));
	}
}

bool NelderMeadSimplex::copy_matrix_column(std::vector<double>& paste_matrix, std::vector<double> copy_matrix, int paste_column, int copy_column, int number_of_rows)
{
	if (paste_matrix.size() != copy_matrix.size())
	{
		return false;
	}
	for (int i = 0; i < number_of_rows; i++)
	{
		paste_matrix[(paste_column * number_of_rows) + i] = copy_matrix[(copy_column * number_of_rows) + i];
	}
	return true;
}

void NelderMeadSimplex::sort_results(int best_index, int middle_index, int worst_index)
{
	std::vector<double> temp_function_values_vector;
	NULLappend_dvector(temp_function_values_vector, function_values_vector.size());
	std::vector<double> temp_guess_matrix;
	NULLappend_dvector(temp_guess_matrix, guess_matrix.size());
	int write_index;
	write_index = 1;
	temp_function_values_vector[0] = function_values_vector[best_index];
	temp_function_values_vector[temp_function_values_vector.size() - 1] = function_values_vector[worst_index];
	copy_matrix_column(temp_guess_matrix, guess_matrix, 0, best_index, C_number_of_parameters);
	copy_matrix_column(temp_guess_matrix, guess_matrix, temp_function_values_vector.size() - 1, worst_index, C_number_of_parameters);
	if (temp_function_values_vector.size() > 2)
	{
		temp_function_values_vector[temp_function_values_vector.size() - 2] = function_values_vector[middle_index];
		copy_matrix_column(temp_guess_matrix, guess_matrix, temp_function_values_vector.size() - 2, middle_index, C_number_of_parameters);
	}
	for (int i = 0; i < temp_function_values_vector.size(); i++)
	{
		if (i != best_index && i != middle_index && i != worst_index)
		{
			temp_function_values_vector[write_index] = function_values_vector[i];
			copy_matrix_column(temp_guess_matrix, guess_matrix, write_index, i, C_number_of_parameters);
			write_index++;
		}
	}
	copy_vector(function_values_vector, temp_function_values_vector);
	copy_vector(guess_matrix, temp_guess_matrix);
}

double NelderMeadSimplex::check_results()
{
	bool last_check = true;
	std::vector<double> temp_function_values_vector;
	NULLappend_dvector(temp_function_values_vector, function_values_vector.size());
	std::vector<double> temp_guess_matrix;
	NULLappend_dvector(temp_guess_matrix, guess_matrix.size());
	std::vector<double> value_assessment(6);
	value_assessment[0] = function_values_vector[0];
	value_assessment[1] = function_values_vector[0];
	value_assessment[2] = function_values_vector[0];
	value_assessment[3] = 0;
	value_assessment[4] = 0;
	value_assessment[5] = 0;
	for (int i = 0; i < function_values_vector.size(); i++)
	{
		if (function_values_vector[i] < value_assessment[0])
		{
			value_assessment[0] = function_values_vector[i];
			value_assessment[3] = i;
		}
	}
	for (int i = 0; i < function_values_vector.size(); i++)
	{
		if (function_values_vector[i] > value_assessment[2])
		{
			value_assessment[2] = function_values_vector[i];
			value_assessment[5] = i;
		}
	}
	if (value_assessment[3] == value_assessment[5])
	{
		value_assessment[5] = 1;
	}
	if (function_values_vector.size() > 2)
	{
		for (int i = 0; i < function_values_vector.size(); i++)
		{
			if (function_values_vector[i] <= value_assessment[2] && function_values_vector[i] > value_assessment[0] && function_values_vector[i] >= value_assessment[1])
			{
				if (i != value_assessment[5])
				{
					value_assessment[1] = function_values_vector[i];
					value_assessment[4] = i;
					last_check = false;
				}
			}
		}
	}
	for (int i = 0; i < function_values_vector.size(); i++)
	{
		if (i == value_assessment[3] || value_assessment[5] == i)
		{
		}
		else if (last_check == true)
		{
			value_assessment[1] = function_values_vector[i];
			value_assessment[4] = i;
			last_check = false;
		}
	}
	sort_results((int)value_assessment[3], (int)value_assessment[4], (int)value_assessment[5]);
	return value_assessment[0];
}

void NelderMeadSimplex::create_guess_matrix()
{
	guess_matrix.clear();
	pointvector_matrix.clear();
	copy_vector(guess_matrix, C_parameter_set);
	NULLappend_dvector(guess_matrix, pow(C_number_of_parameters, 2));
	NULLappend_dvector(pointvector_matrix, pow(C_number_of_parameters, 2));
	bool lin_dep = true;
	do
	{
		for (int i = 1; i <= C_number_of_parameters; i++)
		{
			for (int j = 0; j < C_number_of_parameters; j++)
			{
				guess_matrix[(i * C_number_of_parameters) + j] = trust_region[(C_number_of_parameters * random_natural_number(2)) + j];
			}
		}
		for (int i = 0; i < C_number_of_parameters; i++)
		{
			for (int j = 0; j < C_number_of_parameters; j++)
			{
				pointvector_matrix[(i * C_number_of_parameters) + j] = guess_matrix[((i + 1) * C_number_of_parameters) + j] - guess_matrix[j];
			}
		}
		if (pointvector_matrix.size() == 1)
		{
			lin_dep = false;
		}
		else if (determinant(pointvector_matrix, true, 0) != 0)
		{
			lin_dep = false;
		}
		else
		{
			lin_dep = true;
		}
	} while (lin_dep);
}

bool NelderMeadSimplex::reflexion()
{
	x_r.clear();
	mean_point_vector.clear();
	NULLappend_dvector(mean_point_vector, C_number_of_parameters);
	NULLappend_dvector(x_r, C_number_of_parameters);
	int N = C_number_of_parameters;
	for (int y = 0; y < N; y++)
	{
		for (int i = 0; i < N; i++)
		{
			mean_point_vector[y] = mean_point_vector[y] + guess_matrix[(i * C_number_of_parameters) + y];
		}
		mean_point_vector[y] = (1.0 / N) * mean_point_vector[y];
	}
	for (int y = 0; y < N; y++)
	{
		x_r[y] = mean_point_vector[y] + a_g_b_s[0] * (mean_point_vector[y] - guess_matrix[(N * C_number_of_parameters) + y]);
	}
	y_r = 0;
	y_r = fxn_function(x_r);
	if (y_r < function_values_vector[0])
	{
		return true;
	}
	else
	{
		return false;
	}
}

void NelderMeadSimplex::expansion()
{
	x_e.clear();
	NULLappend_dvector(x_e, C_number_of_parameters);
	int N = C_number_of_parameters;
	for (int y = 0; y < N; y++)
	{
		x_e[y] = x_r[y] + a_g_b_s[1] * (x_r[y] - mean_point_vector[y]);
	}
	y_e = 0;
	y_e = fxn_function(x_e);
	if (y_e < y_r)
	{
		replace_matrix_column_by_vector(guess_matrix, x_e, C_number_of_parameters, C_number_of_parameters);
		function_values_vector[C_number_of_parameters] = y_e;
	}
	else
	{
		replace_matrix_column_by_vector(guess_matrix, x_r, C_number_of_parameters, C_number_of_parameters);
		function_values_vector[C_number_of_parameters] = y_r;
	}
}

bool NelderMeadSimplex::replace_matrix_column_by_vector(std::vector<double>& paste_matrix, std::vector<double> copy_vector, int paste_column, int number_of_rows)
{
	if (copy_vector.size() != number_of_rows)
	{
		return false;
	}
	for (int i = 0; i < number_of_rows; i++)
	{
		paste_matrix[(paste_column * number_of_rows) + i] = copy_vector[i];
	}
	return true;
}

bool NelderMeadSimplex::better_xi()
{
	int i = function_values_vector.size() - 2;
	if (y_r < function_values_vector[i])
	{
		replace_matrix_column_by_vector(guess_matrix, x_r, i, C_number_of_parameters);
		function_values_vector[i] = y_r;
		return true;
	}
	else
	{
		return false;
	}
}

bool NelderMeadSimplex::contraction()
{
	x_con.clear();
	NULLappend_dvector(x_con, C_number_of_parameters);
	int N = C_number_of_parameters;
	if (y_r < function_values_vector[N])
	{
		for (int i = 0; i < C_number_of_parameters; i++)
		{
			x_con[i] = x_r[i] + a_g_b_s[2] * (mean_point_vector[i] - x_r[i]);
		}
	}
	else
	{
		for (int i = 0; i < C_number_of_parameters; i++)
		{
			x_con[i] = guess_matrix[(C_number_of_parameters * C_number_of_parameters) + i] + a_g_b_s[2] * (mean_point_vector[i] - guess_matrix[(C_number_of_parameters * C_number_of_parameters) + i]);
		}
	}
	y_con = 0;
	y_con = fxn_function(x_con);
	if (y_con < function_values_vector[C_number_of_parameters])
	{
		replace_matrix_column_by_vector(guess_matrix, x_con, C_number_of_parameters, C_number_of_parameters);
		function_values_vector[C_number_of_parameters] = y_con;
		return true;
	}
	else
	{
		return false;
	}
}

void NelderMeadSimplex::compression()
{
	std::vector<double> temp_xi;
	double y_temp_xi;
	NULLappend_dvector(temp_xi, C_number_of_parameters);
	for (int n = 1; n <= C_number_of_parameters; n++)
	{
		for (int i = 0; i < C_number_of_parameters; i++)
		{
			temp_xi[i] = guess_matrix[(n * C_number_of_parameters) + i] + a_g_b_s[3] * (guess_matrix[(0 * C_number_of_parameters) + i] - guess_matrix[(n * C_number_of_parameters) + i]);
		}
		y_temp_xi = 0;
		y_temp_xi = fxn_function(temp_xi);
		replace_matrix_column_by_vector(guess_matrix, temp_xi, n, C_number_of_parameters);
		function_values_vector[n] = y_temp_xi;
	}
}

bool NelderMeadSimplex::downhillsimplex(std::vector<double> parameter_set, double trust_region_delta, bool use_relative_values)
{
	counter = 0;
	result_list.clear();
	save_variables(parameter_set, trust_region_delta, use_relative_values);
	NULLappend_dvector(trust_region, (C_number_of_parameters * 3));
	set_trust_region(trust_region);
	create_guess_matrix();
	calculate_guess_solutions();
	result_list.push_back(check_results());
	convergence = false;
	set_new_convergence_essentials(true);
	do
	{
		if (reflexion())
		{
			expansion();
		}
		else
		{
			if (!better_xi())
			{
				if (!contraction())
				{
					compression();
				}
			}
		}
		result_list.push_back(check_results());
		set_new_convergence_essentials(false);
		convergence = check_for_convergence();
		counter++;
		algorithm_break();
	} while (counter < C_max_iterations && convergence == false);
	if (counter == C_max_iterations)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool NelderMeadSimplex::check_for_convergence()
{
	if (C_conv_type == 0)
	{
		convergence_difference = 0.0;
		convergence_difference = last_optimum - new_optimum;
		last_optimum = new_optimum;
		if (convergence_difference == 0.0)
		{
			return false;
		}
		else if (convergence_difference < C_convergence_value && convergence_difference != 0.0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		convergence_difference_parameters.clear();
		NULLappend_dvector(convergence_difference_parameters, C_number_of_parameters);
		bool converged;
		bool parameter_changed;
		parameter_changed = false;
		converged = true;
		for (int i = 0; i < C_number_of_parameters; i++)
		{
			convergence_difference_parameters[i] = last_optimum_parameters[i] - new_optimum_parameters[i];
			last_optimum_parameters[i] = new_optimum_parameters[i];
			if (converged == true)
			{
				if (convergence_difference_parameters[i] == 0.0)
				{
					//
				}
				else if (convergence_difference_parameters[i] < C_convergence_parameter && convergence_difference_parameters[i] > (C_convergence_parameter * (-1.0)))
				{
					parameter_changed = true;
				}
				else
				{
					converged = false;
					parameter_changed = true;
				}
			}
		}

		if (parameter_changed == true && converged == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

double NelderMeadSimplex::fxn_function(std::vector<double> parameters)
{
	double return_value;
	return_value = (*function_pointer)(parameters);
	return return_value;
}

void NelderMeadSimplex::set_convergence_criteria(int max_iterations, double convergence_value, double convergence_parameter)
{
	if (max_iterations > 1000000 || max_iterations < 1)
	{
		//
	}
	else
	{
		C_max_iterations = max_iterations;
	}
	if (convergence_value > 1.0 || convergence_value < 0.0000000001)
	{
		//
	}
	else
	{
		C_convergence_value = convergence_value;
	}
	if (convergence_parameter > 1.0 || convergence_parameter < 0.0000000001)
	{
		//
	}
	else
	{
		C_convergence_parameter = convergence_parameter;
	}
}

double NelderMeadSimplex::get_optimized_parameter(int index)
{
	if (index >= 0 && index < C_number_of_parameters)
	{
		return guess_matrix[index];
	}
	return 0.0;
}

int NelderMeadSimplex::get_number_of_iterations()
{
	return counter;
}

double NelderMeadSimplex::get_convergence()
{
	return convergence_difference;
}

double NelderMeadSimplex::get_convergence(int index)
{
	if (convergence_difference_parameters.empty())
	{
		return 0.0;
	}
	else
	{
		if (index < 0 || index >= convergence_difference_parameters.size())
		{
			return 0.0;
		}
		else
		{
			return convergence_difference_parameters[index];
		}
	}
}

double NelderMeadSimplex::get_result_value()
{
	return function_values_vector[0];
}

void NelderMeadSimplex::fconv_type(int conv_type)
{
	if (conv_type > 1 || conv_type < 0)
	{
		C_conv_type = 0;
	}
	else
	{
		C_conv_type = conv_type;
	}
}

void NelderMeadSimplex::set_new_convergence_essentials(bool initialize)
{
	if (initialize)
	{
		last_optimum = function_values_vector[0];
		last_optimum_parameters.clear();
		for (int i = 0; i < C_number_of_parameters; i++)
		{
			last_optimum_parameters.push_back(guess_matrix[i]);
		}
	}
	else
	{
		new_optimum = function_values_vector[0];
		new_optimum_parameters.clear();
		for (int i = 0; i < C_number_of_parameters; i++)
		{
			new_optimum_parameters.push_back(guess_matrix[i]);
		}
	}
}

void NelderMeadSimplex::algorithm_break()
{
	(*break_function_pointer)();

	if (m_breaktype == "ShiftFit")
	{
		std::vector<double> tmp_params(C_number_of_parameters, 0.0);
		for (size_t i = 0; i < C_number_of_parameters; i++)
		{
			tmp_params[i] = guess_matrix[i];
		}
		m_Tab2->setFittedChi(m_Tab2->GetChiBySym(tmp_params), true);
		m_Tab2->UpdateWindow();
	}
}

void NelderMeadSimplex::set_break_function(void (*break_function)())
{
	break_function_pointer = break_function;
}

void break_function_placeholder()
{
	// keep empty
}