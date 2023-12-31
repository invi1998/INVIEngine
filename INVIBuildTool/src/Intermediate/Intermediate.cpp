#include "BuildPch.h"
#include "Intermediate.h"


namespace Intermdiate
{
	// 生成.h文件
	void GenerateH(std::vector<std::string>& AnalysisRaw,
		const FClassAnalysis& ClassAnalysis,
		std::vector<std::string>& StaticRegistration)
	{
		//头定义
		{
			AnalysisRaw.push_back("#pragma once");
			AnalysisRaw.push_back("");
			AnalysisRaw.push_back("#include \"ObjectScript.h\"");
			AnalysisRaw.push_back("");

			std::string MClassName = " Z_BT_" + ClassAnalysis.ClassName;

			/*#define  Z_BT_USimpleCodeLibrary \*/
			AnalysisRaw.push_back(std::string("#define ") + MClassName + ((ClassAnalysis.Functions.size() > 0) ? " \\" : ""));

			//类名
			std::string ClearClassName = ClassAnalysis.ClassName;
			{
				char* ClearClassNamePtr = const_cast<char*>(ClearClassName.c_str());

				trim_start_and_end_inline(ClearClassNamePtr);

				//移除头部C开头或者G开头
				remove_char_start(ClearClassNamePtr, 'C');
				remove_char_start(ClearClassNamePtr, 'G');

				//ClearClassName = SimpleCodeLibrary
			}

			if (ClassAnalysis.Functions.size() > 0)
			{
				for (const FFunctionAnalysis& Function : ClassAnalysis.Functions)
				{
					if (Function.CodeType == "Describe")
					{
						//VMC_Hello1
						std::string VMString = ("VMC_" + Function.FuncName);
						/*BT_VM(VMC_Hello1) \*/
						AnalysisRaw.push_back("BT_VM(" + VMString + ") \\");
						/*{ \*/
						AnalysisRaw.push_back("{ \\");

						if (Function.Params.size() > 0)
						{
							std::string VariableAdd;
							for (const FParamElement& Variable : Function.Params)
							{
								std::string StackString = " Stack.GetParmAddr(); \\";

								//Z_Context_Name
								std::string VariableName = " Z_" + Variable.Name + "_Name";

								//添加获取具体值的语句
								/*	UObject* Z_Context_Name = (UObject*) Stack.GetParmAddr(); \*/
								AnalysisRaw.push_back(std::string("\t") +
									Variable.Type +
									(Variable.bPointer ? "*" : "") +
									VariableName +
									" = " +
									std::string((Variable.bPointer ? "" : "*")) +
									"(" +
									Variable.Type +
									"*)" +
									StackString);

								//取得变量的名字
								// , Z_Context_Name, Z_A_Name, Z_b_Name, Z_C_Name
								VariableAdd += ("," + VariableName);
							}

							char* VariableAddPtr = const_cast<char*>(VariableAdd.c_str());
							remove_char_start(VariableAddPtr, ',');

							//赋值
							if (Function.bStatic)
							{
								if (Function.Return.Type == "void")
								{
									/*USimpleCodeLibrary::Hello1( Z_Context_Name, Z_A_Name, Z_b_Name, Z_C_Name); \*/
									AnalysisRaw.push_back(std::string("\t") + ClassAnalysis.ClassName +
										"::" +
										Function.FuncName +
										"(" +
										VariableAdd +
										"); \\");
								}
								else
								{
									/**(std::string*)RefData = USimpleCodeLibrary::Hello3( Z_Context_Name); \*/
									AnalysisRaw.push_back(std::string("\t") + (Function.Return.bPointer ? "" : "*") +
										std::string("(" +
											Function.Return.Type +
											"*)") +
										"RefData = " +
										ClassAnalysis.ClassName +
										"::" +
										Function.FuncName +
										"(" +
										VariableAdd +
										"); \\");
								}
							}
						}
						AnalysisRaw.push_back("} \\");

						/*	FFuntionManage::SetNativeFuncPtr(FBTFuntionID(("SimpleCodeLibrary"),("Hello1"), USimpleCodeLibrary::VMC_Hello1));*/
						StaticRegistration.push_back(std::string("\t") + "FFuntionManage::SetNativeFuncPtr(FBTFuntionID((\"" +
							ClearClassName + "\"),(\"" + Function.FuncName + "\"), " +
							ClassAnalysis.ClassName + "::" + VMString + "));");
					}
					//else if (Function.CodeType == "Event")
					//{

					//}
				}

				if (simple_cpp_string_algorithm::index_valid(AnalysisRaw.size(), AnalysisRaw.size() - 1))
				{
					remove_char_end(
						const_cast<char*>(AnalysisRaw[AnalysisRaw.size() - 1].c_str()),
						'\\');
				}

				AnalysisRaw.push_back((""));

				/*#define USimpleCodeLibrary_12_GENERATED_BODY_BT \*/

				char BuffLine1[1024] = { 0 };
				AnalysisRaw.push_back(("#define ") + ClassAnalysis.ClassName + ("_") + _itoa(ClassAnalysis.MacroLine, BuffLine1, 10) + ("_GENERATED_BODY_BT") + (" \\"));

				//Z_BT_USimpleCodeLibrary
				AnalysisRaw.push_back(MClassName);
				AnalysisRaw.push_back((""));

				AnalysisRaw.push_back(("#define ") + std::string("CURRENT_FILE_ID_BT ") + ClassAnalysis.ClassName);

				char BuffLine2[1024] = { 0 };
				AnalysisRaw.push_back(("#define ") + std::string("NewLine ") + _itoa(ClassAnalysis.MacroLine, BuffLine2, 10));
			}
		}
	}

	// 生成.cpp文件
	void GenerateCPP(std::vector<std::string>& AnalysisRaw,
		const FClassAnalysis& ClassAnalysis,
		std::vector<std::string>& StaticRegistration)
	{
		//头定义
		{
			AnalysisRaw.push_back("/*===========================================================================");
			AnalysisRaw.push_back("	Generated code exported from BT UBT.");
			AnalysisRaw.push_back("===========================================================================*/");

			AnalysisRaw.push_back("");
			AnalysisRaw.push_back("#ifdef _MSC_VER");
			AnalysisRaw.push_back("#pragma warning (push)");
			AnalysisRaw.push_back("#pragma warning (disable : 4883)");
			AnalysisRaw.push_back("#endif");
			AnalysisRaw.push_back("PRAGMA_DISABLE_DEPRECATION_WARNINGS");
			AnalysisRaw.push_back("");

			//代码区
			{
				if (ClassAnalysis.Functions.size() > 0)
				{
					for (const FFunctionAnalysis& Function : ClassAnalysis.Functions)
					{
						if (Function.CodeType == "Event")
						{
							//Name_EventEntryA
							std::string FunctionName = ("Name_") +
								Function.FuncName;

							//为后面声明做准备
							std::vector<std::string> ParamStr;
							auto GetParmString = [&Function](std::vector<std::string>& ParamArray)->std::string
								{
									if (Function.Params.size() == 0)
									{
										return "";
									}
									else
									{
										std::string ParamString;
										for (auto& Param : Function.Params)
										{
											//int32 A 
											std::string NewParam = (Param.bConst ? ("const") : ("")) +
												Param.Type +
												(Param.bPointer ? ("* ") : (Param.bReference ? ("& ") : (" "))) +
												Param.Name;

											//int32 A,int32 B.int32 c
											ParamString += (",") + NewParam;

											ParamArray.push_back(NewParam);
										}
										remove_char_end(const_cast<char*>(ParamString.c_str()), ',');

										return ParamString;
									}
								};

							//static FName Name_EventEntryA = FName(("EventEntryA")); 
							AnalysisRaw.push_back(
								("static FName ") +
								FunctionName +
								(" = FName((\"") +
								Function.FuncName +
								("\")); "));

							//添加结构实现
							//void USimpleCodeLibrary::EventEntryB(int32 A)
							AnalysisRaw.push_back(Function.Return.Type +
								(Function.Return.bPointer ? ("*") : (Function.Return.bReference ? ("&") : (" "))) +
								ClassAnalysis.ClassName + ("::") + Function.FuncName +
								("(") + GetParmString(ParamStr) + (")"));

							AnalysisRaw.push_back(("{"));

							//添加类结构
							//FParm_EventEntryB
							std::string StructName = ("Parm_") + Function.FuncName;
							if (Function.Params.size() > 0)
							{
								//struct FParm_EventEntryB
								AnalysisRaw.push_back(std::string(("\t")) + ("struct F") + StructName);
								{
									AnalysisRaw.push_back(std::string(("\t")) + ("{"));//{
									for (auto& Param : ParamStr)
									{
										AnalysisRaw.push_back(std::string(("\t")) + std::string(("\t")) + Param + (";"));
									}
									AnalysisRaw.push_back(std::string(("\t")) + ("};"));//};
								}
								//声明我们的结构
								//FParm_EventEntryB Parm_EventEntryB;
								AnalysisRaw.push_back(std::string(("\t")) + ("F") + StructName + (" ") + StructName + (";"));

								//赋值
								{
									//Parm_EventEntryB.A = A;
									for (auto& Param : Function.Params)
									{
										AnalysisRaw.push_back(std::string(("\t")) + StructName + (".") + Param.Name + (" = ") + Param.Name + (";"));
									}
								}
							}

							//	ExecutionScript(FindScriptFuntion(Name_EventEntryB),&Parm_EventEntryB);
							AnalysisRaw.push_back(std::string(("\t")) + ("ExecutionScript(FindScriptFuntion(") +
								FunctionName + ("),") + (Function.Params.size() == 0 ? ("NULL") : (("&") + StructName)) + (");"));

							AnalysisRaw.push_back(("} "));
						}
					}
				}

				AnalysisRaw.push_back((""));
				AnalysisRaw.push_back(("/* As an executable function pointer for global registration"));
				AnalysisRaw.push_back(("we skip the UE4 UFund local function registration because there is no bytecode in it.*/"));

				//Register_USimpleCodeLibrary()
				std::string Register_Func = ("Register_") + ClassAnalysis.ClassName + ("()");
				//uint8 Register_USimpleCodeLibrary()
				AnalysisRaw.push_back(("uint8 ") + Register_Func);
				AnalysisRaw.push_back(("{"));
				{
					//合并两个vector
					AnalysisRaw.insert(AnalysisRaw.end(), StaticRegistration.begin(), StaticRegistration.end());
					AnalysisRaw.push_back((""));
					AnalysisRaw.push_back(("\treturn 0;"));
				}
				AnalysisRaw.push_back(("}"));

				//激活静态收集
				//static uint8 USimpleCodeLibrary_Index = Register_USimpleCodeLibrary();
				AnalysisRaw.push_back(("static uint8 ") + ClassAnalysis.ClassName + ("_Index = ") + Register_Func + (";"));
			}

			AnalysisRaw.push_back("");

			AnalysisRaw.push_back("PRAGMA_ENABLE_DEPRECATION_WARNINGS");
			AnalysisRaw.push_back("#ifdef _MSC_VER");
			AnalysisRaw.push_back("#pragma warning (pop)");
			AnalysisRaw.push_back("#endif");
		}
	}


	bool Builder(const FClassAnalysis& ClassAnalysis, std::vector<std::string>& OutAnalysisRawH,
		std::vector<std::string>& OutAnalysisRawCpp)
	{

		std::vector<std::string> StaticRegistration;	// 静态注册

		// 生成发射数据
		GenerateH(OutAnalysisRawH, ClassAnalysis, StaticRegistration);
		GenerateCPP(OutAnalysisRawCpp, ClassAnalysis, StaticRegistration);

		return OutAnalysisRawH.size() && OutAnalysisRawCpp.size();
	}
}

