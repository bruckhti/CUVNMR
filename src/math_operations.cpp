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
#include "math_operations.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

double determinant(std::vector<double> square_matrix, bool initialize, int j)
{
	if (square_matrix.size() == 1 && initialize == false)
	{
		return 1.0;
	}
	else if (square_matrix.size() == 1 && initialize == true)
	{
		return square_matrix[0];
	}
	int entries = square_matrix.size();
	double root = sqrt(entries);
	if (root != (int)root)
	{
		return 100.0;
	}
	std::vector<double> smaller_square_matrix;
	if (!initialize)
	{
		for (int index_i = 1; index_i < root; index_i++)
		{
			for (int index_j = 0; index_j < root; index_j++)
			{
				if (index_j != j)
				{
					smaller_square_matrix.push_back(square_matrix[(index_i * root) + index_j]);
				}
			}
		}
	}
	double det = 0.0;
	if (initialize)
	{
		for (int use_j = 0; use_j < root; use_j++)
		{
			det = det + pow(-1.0, use_j) * square_matrix[use_j] * determinant(square_matrix, false, use_j);
		}
	}
	else
	{
		int redundant_entries = smaller_square_matrix.size();
		double redundant_root = sqrt(redundant_entries);
		for (int use_j = 0; use_j < redundant_root; use_j++)
		{
			det = det + pow(-1.0, use_j) * smaller_square_matrix[use_j] * determinant(smaller_square_matrix, false, use_j);
		}
	}
	return det;
}

bool random_bool()
{
	int random_integer = rand() % 2;
	if (random_integer == 0)
		return true;
	else
		return false;
}

int random_natural_number(int max_value)
{
	int random_integer = rand() % (max_value + 1);
	return random_integer;
}