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

	FParamElement&& CollectionVariableType(char* str, ECollectParamType collect_param)
	{
		FParamElement param_element{};

		param_element.Name = "Return";

		if (simple_cpp_string_algorithm::string_contain(str, constString))
		{
			param_element.bConst = true;
			remove_string_start(str, constString);
		}
		if (simple_cpp_string_algorithm::string_contain(str, asteriskString))
		{
			param_element.bPointer = true;
			remove_string_start(str, asteriskString);
		}
		if (simple_cpp_string_algorithm::string_contain(str, ampersandString))
		{
			trim_start_and_end_inline(str);
			param_element.bReference = true;
			remove_string_start(str, ampersandString);
		}

		trim_start_and_end_inline(str);
		param_element.Type = collect_param;

		return std::move(param_element);
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

						char L[1024] = {0};
						char R[1024] = {0};

						split(rowPtr, spaceString, L, R, false);	// 将rowPtr按照 " " 分割成两个字符串，存入L和R中)

						row = L;	// 将R赋值给row，后面的代码继续处理row
					}
					if (contain(virtualString))	// 判断是否是虚函数
					{
						functionAnalysis.bVirtual = true;

						char L[1024] = {0};
						char R[1024] = {0};

						split(rowPtr, spaceString, L, R, false);	// 将rowPtr按照 " " 分割成两个字符串，存入L和R中)

						row = L;	// 将R赋值给row，后面的代码继续处理row
					}

					// 确定返回类型
					char Temp[1024] = {0};

					{
						char L[1024] = {0};
						char R[1024] = {0};
						trim_start_inline(rowPtr);
						split(rowPtr, spaceString, R, Temp, false);	// 将rowPtr按照 " " 分割成两个字符串，存入R和Temp中)



						functionAnalysis.Return = CollectionVariableType(R, ECollectParamType::ECollectParamType_Return);

						//void Hello1(UObject *Context, int32 &A,float b,bool C
						remove_char_end(Temp, '}');
						remove_char_end(Temp, '{');
						trim_end_inline(Temp);
						remove_char_end(Temp, ';');
						remove_char_end(Temp, ')');
						//Tmp = Hello1(UObject *Context, int32 &A,float b,bool C

						char RStr[1024] = { 0 };
						char LStr[1024] = { 0 };

						split(Temp, leftParenthesisString, LStr, RStr, false);	// 将Temp按照 "(" 分割成两个字符串，存入LStr和RStr中)

						// 函数名
						functionAnalysis.FuncName = RStr;

						// 解析参数列表
						std::vector<std::string> paramList{};
						simple_cpp_string_algorithm::parse_into_vector_array(LStr, paramList, commaString);	// 将LStr按照,分割成多个字符串，存入paramList中

						// 收集变量
						for (std::string& ele : paramList)
						{
							char* elePtr = const_cast<char*>(ele.c_str());

							// 移除空格
							trim_start_and_end_inline(elePtr);

							char L[1024] = {0};
							char R[1024] = {0};

							FParamElement param_element{};

							if (simple_cpp_string_algorithm::string_contain(elePtr, asteriskString))
							{
								param_element.bPointer = true;
								split(elePtr, asteriskString, R, L, false);	// 将elePtr按照 "*" 分割成两个字符串，存入L和R中)
							}
							else if (simple_cpp_string_algorithm::string_contain(elePtr, ampersandString))
							{
								param_element.bReference = true;
								split(elePtr, ampersandString, R, L, false);	// 将elePtr按照 "&" 分割成两个字符串，存入L和R中)
							}
							else
							{
								split(elePtr, spaceString, R, L, false);
							}

							if (c_str_contain(L, constString))
							{
								param_element.bConst = true;
								remove_string_start(L, constString);
							}

							trim_start_and_end_inline(R);	// 去掉变量名前后的空格
							trim_start_and_end_inline(L);	// 去掉变量名前后的空格

							// 变量名
							param_element.Name = L;

							// 变量类型
							param_element.Type = R;

							functionAnalysis.Params.push_back(param_element);
						}

					}

					classAnalysis.Functions.push_back(functionAnalysis);
				}
			}
		}


	}
}

