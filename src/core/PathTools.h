#pragma once

#ifdef _WIN32

#include "boost/filesystem/path.hpp"
#include "boost/algorithm/string/replace.hpp"
#include "boost/filesystem/operations.hpp"

using Path = boost::filesystem::path;

static std::string _WorkaroundVisualStudio(const Path & path)
{
	auto pathString = boost::filesystem::canonical(path).string();
	boost::replace_all(pathString, "/", "\\");
	return pathString;
}

static Path _MakePosix(const Path& path)
{
	std::wstring pathWstr = std::wstring(path.c_str());
	std::string pathStr = std::string(pathWstr.begin(), pathWstr.end());
	boost::replace_all(pathStr, "\\", "/");
	return Path(pathStr);
}

#endif

#ifdef _WIN32
#define MakePosix(path) _MakePosix(path)
#define WorkaroundVisualStudio(path) _WorkaroundVisualStudio(path)
#else
#define MakePosix(path) path
#define WorkaroundVisualStudio(path) path
#endif
