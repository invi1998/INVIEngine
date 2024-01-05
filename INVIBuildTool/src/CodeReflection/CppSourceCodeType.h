#pragma once

// ���뷴��

// ����
struct FParamElement
{
	FParamElement() = default;

	std::string Name{};		// ��������
	bool bConst = false;			// �Ƿ�Ϊ����
	bool bPointer = false;			// �Ƿ�Ϊָ��
	bool bReference = false;		// �Ƿ�Ϊ����
	std::string Type{};		// �������� float, int, std::string, std::vector<int>, std::vector<std::string>
};


// ��������
struct FVariableAnalysis : public FParamElement
{
	FVariableAnalysis()=default;

	std::string CodeType{};		// ��������
	bool bStatic = false;				// �Ƿ�Ϊ��̬����
};

// ��������
struct FFunctionAnalysis
{
	FFunctionAnalysis() = default;

	std::vector<FParamElement> Params{};		// �����б�
	FParamElement Return{};						// ����ֵ

	std::string FuncName{};						// ��������
	bool bStatic = false;						// �Ƿ�Ϊ��̬����
	bool bVirtual = false;						// �Ƿ�Ϊ�麯��

	std::string CodeType{};						// ��������
};


// �����
struct FClassAnalysis
{
	FClassAnalysis();

	std::string ClassName{};					// ������
	std::vector<std::string> ParentClass{};		// �����б�
	std::vector<FVariableAnalysis> Variables{};	// �����б�
	std::vector<FFunctionAnalysis> Functions{};	// �����б�

	std::string APIName;						// API���� ������

	std::string CPPFileName;					// CPP�ļ�����

	int32_t MacroLine = 0;						// �����������������������

};


// �ռ�����������ö��
enum ECollectParamType
{
	ECollectParamType_None = 0,			// ��
	ECollectParamType_Class = 1 << 0,	// ��
	ECollectParamType_Function = 1 << 1,	// ����
	ECollectParamType_Variable = 1 << 2,	// ����
	ECollectParamType_Macro = 1 << 3,	// ��
	ECollectParamType_Enum = 1 << 4,	// ö��
	ECollectParamType_Return = 1 << 5,	// ����
	ECollectParamType_All = ECollectParamType_Class | ECollectParamType_Function | ECollectParamType_Variable,	// ����
};
