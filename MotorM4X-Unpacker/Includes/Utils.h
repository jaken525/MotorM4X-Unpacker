#pragma once

#include <iostream>
#include <cstring>
#include <iomanip>
#include <shlwapi.h>
#include <shlobj.h>
#include <algorithm>

namespace Utils
{
	static bool IsDigits(const std::string& line) 
	{
		return !line.empty() && std::all_of(line.begin(), line.end(), ::isdigit);
	}

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

	static std::string OpenDirectory(const std::string& message = "Select Extract Folder")
	{
		BROWSEINFO bi{0};
		TCHAR szDisplayName[MAX_PATH];

		bi.hwndOwner = NULL;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = szDisplayName;
		bi.lpszTitle = message.c_str();
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
}