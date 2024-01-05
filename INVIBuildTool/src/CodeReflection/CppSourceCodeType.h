#pragma once

// 代码反射

// 参数
struct FParamElement
{
	FParamElement() = default;

	std::string Name{};		// 参数名称
	bool bConst = false;			// 是否为常量
	bool bPointer = false;			// 是否为指针
	bool bReference = false;		// 是否为引用
	std::string Type{};		// 参数类型 float, int, std::string, std::vector<int>, std::vector<std::string>
};


// 变量解析
struct FVariableAnalysis : public FParamElement
{
	FVariableAnalysis()=default;

	std::string CodeType{};		// 代码类型
	bool bStatic = false;				// 是否为静态变量
};

// 函数解析
struct FFunctionAnalysis
{
	FFunctionAnalysis() = default;

	std::vector<FParamElement> Params{};		// 参数列表
	FParamElement Return{};						// 返回值

	std::string FuncName{};						// 函数名称
	bool bStatic = false;						// 是否为静态函数
	bool bVirtual = false;						// 是否为虚函数

	std::string CodeType{};						// 代码类型
};


// 类解析
struct FClassAnalysis
{
	FClassAnalysis();

	std::string ClassName{};					// 类名称
	std::vector<std::string> ParentClass{};		// 父类列表
	std::vector<FVariableAnalysis> Variables{};	// 变量列表
	std::vector<FFunctionAnalysis> Functions{};	// 函数列表

	std::string APIName;						// API名称 导出宏

	std::string CPPFileName;					// CPP文件名称

	int32_t MacroLine = 0;						// 行数（反射宏标记所在行数）

};


// 收集参数的类型枚举
enum ECollectParamType
{
	ECollectParamType_None = 0,			// 无
	ECollectParamType_Class = 1 << 0,	// 类
	ECollectParamType_Function = 1 << 1,	// 函数
	ECollectParamType_Variable = 1 << 2,	// 变量
	ECollectParamType_Macro = 1 << 3,	// 宏
	ECollectParamType_Enum = 1 << 4,	// 枚举
	ECollectParamType_Return = 1 << 5,	// 返回
	ECollectParamType_All = ECollectParamType_Class | ECollectParamType_Function | ECollectParamType_Variable,	// 所有
};
