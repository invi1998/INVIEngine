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

		split(row_ptr, codeTypeString, L, R, false);	// ��row_ptr���� "CodeType" �ָ�������ַ���������L��R��

		std::vector<std::string> elementString{};
		simple_cpp_string_algorithm::parse_into_vector_array(L, elementString, commaString);		// ��L����,�ָ�ɶ���ַ���������elementString��

		if (elementString[0].find("Event"))	// Event���͵ĺ�������Ϊ�����������ͼ��ִ��
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
		simple_cpp_helper_file::load_file_to_strings(path, stringArray);	// ��C++�ļ���ȡ��stringArray��

		for (int i = 0; i <= stringArray.size(); i++)
		{
			std::string& row = stringArray[i];
			char* rowPtr = const_cast<char*>(row.c_str());

			// �����ж��Ƿ�����Ӵ��ĺ���
			auto contain = [&](const char* subStr)->bool
				{
					return simple_cpp_string_algorithm::string_contain(row, subStr);
				};

			// GENERATED_BODY
			if (contain("GENERATED_BODY"))
			{
				classAnalysis.MacroLine = i + 1;
			}

			// ��ȡ�����͸�����
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

					// API��
					classAnalysis.APIName = L;

					row = R;	// ��R��ֵ��row������Ĵ����������row
				}

				std::vector<std::string> elementString{};
				simple_cpp_string_algorithm::parse_into_vector_array(rowPtr, elementString, colonString);		// ��rowPtr����:�ָ�������ַ���������elementString��

				trim_start_and_end_inline(const_cast<char*>(elementString[0].c_str()));	// ȥ������ǰ��Ŀո�

				// ���Ƕ�̳е����
				if (elementString.size() > 1)
				{
					// ��̳�
					std::vector<std::string> parentString{};
					simple_cpp_string_algorithm::parse_into_vector_array(const_cast<char*>(elementString[1].c_str()), parentString, commaString);	// ��elementString[1]����,�ָ�ɶ���ַ���������parentString��

					for (auto& parent : parentString)
					{
						char* parentPtr = const_cast<char*>(parent.c_str());
						remove_string_start(parentPtr, "public ");
						remove_string_start(parentPtr, "protected ");
						remove_string_start(parentPtr, "private ");

						trim_start_and_end_inline(parentPtr);	// ȥ��������ǰ��Ŀո�

						classAnalysis.ParentClass.push_back(parent);
					}
				}
			}

			// ��ȡ��ǵĳ�Ա����
			if (contain("UFUNCTION"))
			{
				FFunctionAnalysis functionAnalysis{};
				if (GetCodeTypeByFunc(rowPtr, functionAnalysis))
				{
					row = stringArray[i+1];	// ָ����һ�е�����,����������

					// �ж��Ƿ��Ǿ�̬����
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

