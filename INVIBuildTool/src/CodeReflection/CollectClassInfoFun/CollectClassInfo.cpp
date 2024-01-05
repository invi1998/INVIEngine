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

	constexpr char constString[] = "const";
	constexpr char staticString[] = "static";
	constexpr char virtualString[] = "virtual";
	constexpr char overrideString[] = "override";
	constexpr char finalString[] = "final";
	constexpr char inlineString[] = "inline";
	constexpr char explicitString[] = "explicit";
	constexpr char friendString[] = "friend";
	constexpr char mutableString[] = "mutable";
	constexpr char typenameString[] = "typename";
	constexpr char templateString[] = "template";

	constexpr char codeTypeString[] = "CodeType";
	constexpr char groupNumberString[] = "Group";

	bool GetCodeTypeByFunc(char* row_ptr, FFunctionAnalysis& function_analysis)
	{
		char L[1024] = {0};
		char R[1024] = {0};

		// UFUNCTION(Meta = (BlueprintInternalUseOnly = "true", DisplayName = "Get All Actors Of Class", Keywords = "GetAllActorsOfClass"), BlueprintPure, Category = "Utilities|Object", meta = (BlueprintInternalUseOnly = "true", DisplayName = "Get All Actors Of Class", Keywords = "GetAllActorsOfClass"))

		split(row_ptr, codeTypeString, L, R, false);	// 将row_ptr按照 "CodeType" 分割成两个字符串，存入L和R中

		std::vector<std::string> elementString{};
		simple_cpp_string_algorithm::parse_into_vector_array(L, elementString, commaString);		// 将L按照,分割成多个字符串，存入elementString中

		if (elementString[0].find("Event"))	// Event类型的函数，作为程序入口在蓝图中执行
		{
			function_analysis.CodeType = "Event";
			return true;
		}
		else if (elementString[0].find("Describe"))
		{
			function_analysis.CodeType = "Describe";
			return true;
		}
		return false;

	}

	bool Collection(std::string& path, FClassAnalysis& classAnalysis)
	{
		std::vector<std::string> stringArray;
		simple_cpp_helper_file::load_file_to_strings(path, stringArray);	// 将C++文件读取到stringArray中

		for (int i = 0; i <= stringArray.size(); i++)
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

			// 获取标记的成员函数
			if (contain("UFUNCTION"))
			{
				FFunctionAnalysis functionAnalysis{};
				if (GetCodeTypeByFunc(rowPtr, functionAnalysis))
				{
					row = stringArray[i+1];	// 指向下一行的内容,即函数声明

					// 判断是否是静态函数
					if (contain(staticString))
					{
						functionAnalysis.bStatic = true;
					}


					classAnalysis.Functions.push_back(functionAnalysis);
				}
			}
		}
	}
}

