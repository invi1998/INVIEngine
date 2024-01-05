#include "BuildPch.h"

#include "CollectClassInfo.h"


namespace CollectClassInfo
{
	constexpr char spaceString[] = " ";
	constexpr char colonString[] = ":";
	constexpr char commaString[] = ",";
	constexpr char semicolonString[] = ";";
	constexpr char leftBraceString[] = "{";
	constexpr char rightBraceString[] = "}";
	constexpr char leftBracketString[] = "[";
	constexpr char rightBracketString[] = "]";
	constexpr char leftParenthesisString[] = "(";
	constexpr char rightParenthesisString[] = ")";
	constexpr char asteriskString[] = "*";
	constexpr char ampersandString[] = "&";

	bool Collection(std::string& path, FClassAnalysis& classAnalysis)
	{
		std::vector<std::string> stringArray;
		simple_cpp_helper_file::load_file_to_strings(path, stringArray);	// 将C++文件读取到stringArray中

		for (int i = 0; i < stringArray.size(); i++)
		{
			std::string& row = stringArray[i];
			char* rowPtr = const_cast<char*>(row.c_str());

			// 定义判断是否包含子串的函数
			auto contain = [&](const char* subStr)->bool
				{
					return simple_cpp_string_algorithm::string_contain(row, subStr);
				};

			// GENERATED_BODY
			if (contain("GENERATED_BODY"))
			{
				classAnalysis.MacroLine = i + 1;
			}

			// 获取类名和父类名
			if ((contain("\tclass") || contain("class")) && contain(":") && (contain("public") || contain("protected") || contain("private")))
			{
				// class xxx : public xxx -> xxx:public xxx
				remove_string_start(rowPtr, "class ");
				remove_string_start(rowPtr, "\tclass ");

				if (contain("_API"))
				{
					// xxx_API xxx:public xx
					// L = xxx_API, R = xxx:public xx
					trim_start_inline(rowPtr);

					char R[1024] = {0};
					char L[1024] = {0};

					split(rowPtr, spaceString, L, R, false);

					// API名
					classAnalysis.APIName = L;

					row = R;	// 将R赋值给row，后面的代码继续处理row
				}

				std::vector<std::string> elementString{};
				simple_cpp_string_algorithm::parse_into_vector_array(rowPtr, elementString, colonString);		// 将rowPtr按照:分割成两个字符串，存入elementString中

				trim_start_and_end_inline(const_cast<char*>(elementString[0].c_str()));	// 去掉类名前后的空格

				// 考虑多继承的情况
				if (elementString.size() > 1)
				{
					// 多继承
					std::vector<std::string> parentString{};
					simple_cpp_string_algorithm::parse_into_vector_array(const_cast<char*>(elementString[1].c_str()), parentString, commaString);	// 将elementString[1]按照,分割成多个字符串，存入parentString中

					for (auto& parent : parentString)
					{
						char* parentPtr = const_cast<char*>(parent.c_str());
						remove_string_start(parentPtr, "public ");
						remove_string_start(parentPtr, "protected ");
						remove_string_start(parentPtr, "private ");

						trim_start_and_end_inline(parentPtr);	// 去掉父类名前后的空格

						classAnalysis.ParentClass.push_back(parent);
					}
				}
			}
		}
	}
}

