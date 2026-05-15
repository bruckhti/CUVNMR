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
#include "GitHubUpdater.h"
#include <iostream>

std::wstring Utf8ToWide(const std::string& utf8)
{
	if (utf8.empty()) return L"";
	int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), NULL, 0);
	std::wstring wide(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), &wide[0], size);
	return wide;
}

bool IsNewVersion(const std::string& current, const std::string& latest)
{
	std::istringstream currentStream(current);
	std::istringstream latestStream(latest);
	std::string currentToken, latestToken;
	while (std::getline(currentStream, currentToken, '.') && std::getline(latestStream, latestToken, '.'))
	{
		int i_current = std::stoi(currentToken);
		int i_latest = std::stoi(latestToken);
		if (i_latest > i_current) return true;
		if (i_latest < i_current) return false;
	}
	return false;
}

bool DownloadHttps(const std::wstring& host, const std::wstring& path, const LPCWSTR& user_agent, std::vector<BYTE>& output)
{
	HINTERNET hSession = WinHttpOpen(user_agent, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (!hSession) return false;
	WinHttpSetTimeouts(hSession, 5000, 5000, 5000, 5000);
	HINTERNET hConnect = WinHttpConnect(hSession, host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
	if (!hConnect)
	{
		WinHttpCloseHandle(hSession);
		return false;
	}
	HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", path.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
	if (!hRequest)
	{
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return false;
	}
	DWORD redirectPolicy = WINHTTP_OPTION_REDIRECT_POLICY_ALWAYS;
	WinHttpSetOption(hRequest, WINHTTP_OPTION_REDIRECT_POLICY, &redirectPolicy, sizeof(redirectPolicy));
	BOOL bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	if (bResults) bResults = WinHttpReceiveResponse(hRequest, NULL);
	if (!bResults)
	{
		WinHttpCloseHandle(hRequest);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return false;
	}
	DWORD dwSize = 0;
	do
	{
		WinHttpQueryDataAvailable(hRequest, &dwSize);
		if (dwSize == 0) break;
		std::vector<BYTE> buffer(dwSize);
		DWORD dwDownloaded = 0;
		if (WinHttpReadData(hRequest, buffer.data(), dwSize, &dwDownloaded))
		{
			output.insert(output.end(), buffer.begin(), buffer.begin() + dwDownloaded);
		}
	} while (dwSize > 0);
	WinHttpCloseHandle(hRequest);
	WinHttpCloseHandle(hConnect);
	WinHttpCloseHandle(hSession);
	return true;
}

bool SaveTmpFile(const std::wstring& filename, const std::vector<BYTE>& data, std::wstring& filepath)
{
	wchar_t tempDir[MAX_PATH + 1] = { 0 };
	if (!GetTempPathW(MAX_PATH, tempDir))
	{
		wcscpy_s(tempDir, L".\\");
	}
	std::wstring PATH = std::wstring(tempDir) + filename;
	std::ofstream file(PATH, std::ios::binary);
	if (!file.is_open()) return false;
	file.write(reinterpret_cast<const char*>(data.data()), data.size());
	if (file.fail())
	{
		file.close();
		DeleteFileW(PATH.c_str());
		return false;
	}
	file.close();
	filepath = PATH;
	return true;
}

std::string GetSha(const std::wstring& filepath)
{
	HANDLE hFile = CreateFileW(filepath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return "";
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	std::string hashString;
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		if (CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
		{
			BYTE buffer[8192];
			DWORD bytesRead = 0;
			while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0)
			{
				CryptHashData(hHash, buffer, bytesRead, 0);
			}
			BYTE hash[32];
			DWORD hashLen = 32;
			if (CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0))
			{
				std::stringstream ss;
				for (DWORD i = 0; i < hashLen; i++)
				{
					ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
				}
				hashString = ss.str();
			}
			CryptDestroyHash(hHash);
		}
		CryptReleaseContext(hProv, 0);
	}
	CloseHandle(hFile);
	return hashString;
}

std::wstring DownloadAndVerify(const std::string& currentVersion, const std::wstring& info_host, const std::wstring& info_path, const std::wstring& updater_host, const std::wstring& updater_path, const std::string& expectedHash, const LPCWSTR& user_agent)
{
	std::vector<BYTE> data;
	data.clear();
	if (!DownloadHttps(updater_host, updater_path, user_agent, data)) return L"unable to download new version";
	std::wstring updater_filename = std::wstring(user_agent) + L"_update.msi";
	std::wstring updater_filepath;
	if (!SaveTmpFile(updater_filename, data, updater_filepath)) return L"failed to download updater";
	std::string actualHash = GetSha(updater_filepath);
	if (actualHash.empty()) return L"Hash calculation error";
	if (actualHash != expectedHash) return L"WARNING: update canceled due to Hash mismatch";
	HINSTANCE result = ShellExecuteW(NULL, L"open", updater_filepath.c_str(), NULL, NULL, SW_SHOWNORMAL);
	if (((INT_PTR)result) > 32) return L"";
	return L"update failed";
}

std::wstring CheckForUpdatesOnly(const std::string& currentVersion, const std::wstring& info_host, const std::wstring& info_path, const LPCWSTR& user_agent, std::wstring& updater_path, std::string& expectedHash)
{
	std::vector<BYTE> data;
	std::string latestVersion;
	if (!DownloadHttps(info_host, info_path, user_agent, data)) return L"unable to check for updates";
	std::string content(data.begin(), data.end());
	std::istringstream info_file(content);
	std::string line;
	if (std::getline(info_file, line)) latestVersion = line; else return L"unexpected update termination while reading release information (1)";
	if (std::getline(info_file, line)) updater_path = Utf8ToWide(line); else return L"unexpected update termination while reading release information (2)";
	if (std::getline(info_file, line)) expectedHash = line; else return L"unexpected update termination while reading release information (3)";
	if (!IsNewVersion(currentVersion, latestVersion)) return L"";
	return L"1";
}

