#ifndef FILESYSTEM_HELPERS
#define FILESYSTEM_HELPERS

class FileSystemHelpers 
{
public:
	static std::string GetAbsoluteDir(const std::string & absoluteDir, const std::string & relativeDir)
	{
		///Maybe add regex path validations, throw exceptions?
		std::string dir = relativePart,
		absoluteDir = absolutePart,
		backStr = std::string("..") + DSP();

		FileSystemHelpers::RemoveFromEnd(dir, DSP());
		FileSystemHelpers::RemoveFromEnd(absoluteDir, DSP());

		uint32_t len = dir.size();

		boost::replace_all(dir, backStr, "");

		uint32_t removed = (len - dir.size()) / backStr.length();
		std::string::size_type index = std::string::npos;

		for (uint32_t i = 0; i < removed; i++)
		{
			uint32_t found = absoluteDir.find_last_of(DSP(), index - 1);
			if (found < index)
				index = found;
		}

		if (index < std::string::npos)
			absoluteDir.erase(absoluteDir.begin() + index, absoluteDir.end());

		FileSystemHelpers::RemoveFromEnd(absoluteDir, DSP());
		return absoluteDir;
	}

	static void RemoveFromEnd(std::string & stringToSearch, const std::string & toRemove)
	{
		if (&stringToSearch[stringToSearch.length() - toRemove.length()] == toRemove) {
			stringToSearch.erase((stringToSearch.length() - toRemove.length()), toRemove.length());
		}
	}
};

#endif