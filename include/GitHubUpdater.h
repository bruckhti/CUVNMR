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
#ifndef GITHUBUPDATER_HPP
#define GITHUBUPDATER_HPP

#include <windows.h>
#include <winhttp.h>
#include <shellapi.h>
#include <wincrypt.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>

#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "crypt32.lib")

std::wstring Utf8ToWide(const std::string& utf8);
bool IsNewVersion(const std::string& current, const std::string& latest);
bool DownloadHttps(const std::wstring& host, const std::wstring& path, const LPCWSTR& user_agent, std::vector<BYTE>& output);
bool SaveTmpFile(const std::wstring& filename, const std::vector<BYTE>& data, std::wstring& filepath);
std::string GetSha(const std::wstring& filepath);
std::wstring DownloadAndVerify(const std::string& currentVersion, const std::wstring& info_host, const std::wstring& info_path, const std::wstring& updater_host, const std::wstring& updater_path, const std::string& expectedHash, const LPCWSTR& user_agent);
std::wstring CheckForUpdatesOnly(const std::string& currentVersion, const std::wstring& info_host, const std::wstring& info_path, const LPCWSTR& user_agent, std::wstring& updater_path, std::string& expectedHash);

#endif