#include "BuildPch.h"

#include "CollectClassInfo.h"


namespace CollectClassInfo
{
	bool Collection(std::string& path, FClassAnalysis& classAnalysis)
	{
		std::vector<std::string> stringArray;
		simple_cpp_helper_file::load_file_to_strings(path, stringArray);	// 将C++文件读取到stringArray中
	}
}

