#pragma once

#include <iostream>
#include <cstring>
#include <iomanip>
#include <shlwapi.h>
#include <shlobj.h>

namespace Utils
{
	static std::string OpenFile()
	{
		std::string filename(MAX_PATH, '\0');
		OPENFILENAME ofn = { };

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = "MotorM4X MDL Archive File (*.mdl)\0*.mdl\0MotorM4X DTF Archive File (*.dtf)\0*.dtf\0All Files (*.*)\0*.*\0";
		ofn.lpstrFile = &filename[0];
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Select a File";
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

		if (!GetOpenFileName(&ofn))
			filename = "";

		return filename;
	}

	static std::string OpenDirectory()
	{
		BROWSEINFO bi{0};
		TCHAR szDisplayName[MAX_PATH];

		bi.hwndOwner = NULL;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = szDisplayName;
		bi.lpszTitle = "Select Extract Folder";
		bi.ulFlags = BIF_RETURNONLYFSDIRS;
		bi.lParam = NULL;
		bi.iImage = 0;

		LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
		TCHAR szPathName[256];

		if (NULL != pidl)
		{
			BOOL bRet = SHGetPathFromIDList(pidl, szPathName);

			if (bRet)
				return &szPathName[0];
		}

		return "";
	}


	static std::string GetFileNameWithExt(const std::string& str)
	{
		auto found = str.find_last_of("/\\");
		const auto strt = found < str.size() ? str.substr(found + 1, -1) : str;
		
		return strt;
	}

	static std::string GetFileName(const std::string& str)
	{
		std::string strt = GetFileNameWithExt(str);
		
		size_t lastdot = strt.find_last_of(".");
		if (lastdot == std::string::npos)
			return strt;
		return strt.substr(0, lastdot);
	}

	std::string GetFileExtention(const std::string& str)
	{
		const auto lastDot = str.find_last_of(".");
		if (lastDot == std::string::npos)
			return str;
		return str.substr(lastDot, str.length());
	}

	std::string GetFilePath(const std::string& str)
	{
		const auto found = str.find_last_of("/\\");
		return found != std::string::npos ? (str.substr(0, found) + "\\") : "";
	}
}