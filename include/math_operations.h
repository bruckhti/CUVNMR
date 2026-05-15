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
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

double determinant(std::vector<double> square_matrix, bool initialize, int j);
bool random_bool();
int random_natural_number(int max_value);
