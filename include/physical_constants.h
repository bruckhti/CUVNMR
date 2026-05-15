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
#include <string>
#include <vector>

extern double N_avo;
extern double mu0;
extern double g_e;
extern double muB;
extern double k_B;
extern double h;
extern double hquer;
extern double PI;
extern std::vector<double> gamma;
extern double a0;
extern double Aprefac;
void initialize_physical_constants();
double get_gyromagnetic_ratio(std::string isotope, int index, bool rad);
std::vector<std::vector<double>> MatrixMultiplication(std::vector<std::vector<double>> left, std::vector<std::vector<double>> right);
std::vector<std::vector<double>> transpose(std::vector<std::vector<double>> matrix);
long double determinant(std::vector<std::vector<double>> matrix, bool first_call);
std::vector<std::vector<double>> eigenvectors(std::vector<std::vector<double>> matrix, std::vector<double>& eigenvalues, std::string& warning);
std::vector<std::vector<double>> inverse_Matrix(std::vector<std::vector<double>> original_matrix);