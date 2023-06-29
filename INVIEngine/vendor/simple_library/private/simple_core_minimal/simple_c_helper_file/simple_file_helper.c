// Copyright (C) RenZhai.2022.All Rights Reserved.
#include "../../../public/simple_core_minimal/simple_c_helper_file/simple_file_helper.h"
#include "../../../public/simple_core_minimal/simple_c_core/simple_c_array/simple_c_array_string.h"

//���ڼ��ShellExecute�ķ���ֵ��Ϣ
bool check_ShellExecute_ret(int ret)
{
	if (ret == 0)
	{
		// �ڴ治��
		assert(0, "open_url_w=>insufficient memory.");
	}
	else if (ret == 2)
	{
		// �ļ�������
		assert(0, "open_url_w=>File name error.");
	}
	else if (ret == 3)
	{
		// ·��������
		assert(0, "open_url_w=>Path name error.");
	}
	else if (ret == 11)
	{
		// EXE �ļ���Ч
		assert(0, "open_url_w=>Invalid .exe file.");
	}
	else if (ret == 26)
	{
		// ������������
		assert(0, "open_url_w=>A sharing error occurred.");
	}
	else if (ret == 27)
	{
		// �ļ�������ȫ����Ч
		assert(0, "open_url_w=>incomplete or invalid file name.");
	}
	else if (ret == 28)
	{
		// ��ʱ
		assert(0, "open_url_w=>timeout.");
	}
	else if (ret == 29)
	{
		// DDE ����ʧ��
		assert(0, "open_url_w=> DDE transaction failed.");
	}
	else if (ret == 30)
	{
		// ���ڴ������� DDE �����������ɸ� DDE ����
		assert(0, "open_url_w=> is processing another DDE transaction and cannot complete the DDE transaction.");
	}
	else if (ret == 31)
	{
		// û���������Ӧ�ó���
		assert(0, "open_url_w=>no associated application.");
	}

	return ret <= 32;
}

void init_def_c_paths(def_c_paths *c_paths)
{
	c_paths->index = 0;
	memset(c_paths->paths,0,sizeof(c_paths->paths) - 1);
}

void init_def_c_paths_w(def_c_paths_w* c_paths)
{
	c_paths->index = 0;
	memset(c_paths->paths, 0, sizeof(c_paths->paths) - 1);
}

int copy_file(char *Src, char *Dest)
{
	//��ǰ�Ļ��� ����1MB��С����������ͻ������ �������std C�γ����������չ
	char Buf[1024 * 1024] = { 0 };
	int FileSize = 0;
	FILE *FpSrc = NULL;
	FILE *FpDest = NULL;

	if ((FpSrc = fopen(Src, "rb")) != NULL)
	{
		if ((FpDest = fopen(Dest, "wb")) != NULL)
		{
			while ((FileSize = fread(Buf, 1, 512, FpSrc)) > 0)
			{
				fwrite(Buf, FileSize, sizeof(char), FpDest);
				memset(Buf, 0, sizeof(Buf));
			}

			fclose(FpSrc);
			fclose(FpDest);

			return 0;
		}
	}

	return -1;
}
void find_files(char const *in_path, def_c_paths *str, bool b_recursion)
{
#ifdef  _WIN64
	struct _finddatai64_t finddata;
#else
#ifdef _WIN32    
	struct _finddata_t finddata;
#endif 
#endif 

#ifdef  _WIN64
	intptr_t hfile = 0;
#else
#ifdef _WIN32    
	long hfile = 0;
#endif 
#endif 
	
	char tmp_path[1024] = { 0 };
	strcpy(tmp_path, in_path);
	strcat(tmp_path, "\\*");
	if ((hfile = 
#ifdef _WIN64
		_findfirst64
#else
#ifdef WIN32	
		_findfirst
#endif // _WIN64
#endif // _WIN32
		(tmp_path, &finddata)) != -1)
	{
		do
		{
			if (finddata.attrib & _A_SUBDIR)
			{
				if (b_recursion)
				{
					if (strcmp(finddata.name, ".") == 0 ||
						strcmp(finddata.name, "..") == 0)
					{
						continue;
					}

					char new_path[1024] = { 0 };
					strcpy(new_path, in_path);
					strcat(new_path, "\\");
					strcat(new_path, finddata.name);

					find_files(new_path, str, b_recursion);
				}
			}
			else
			{
				strcpy(str->paths[str->index], in_path);
				strcat(str->paths[str->index], "\\");
				strcat(str->paths[str->index++], finddata.name);
			}

		} while (
#ifdef _WIN64
		_findnext64
#else
#ifdef _WIN32
			
		_findnext
#endif
#endif
			(hfile, &finddata) == 0);
		_findclose(hfile);
	}
}

bool is_file_exists(char const* filename)
{
	FILE* file = fopen(filename, "r");
	if(file)
	{
		fclose(file);
		return true;
	}
	return false;
}

bool create_file(char const *filename)
{
	FILE *f = NULL;
	if ((f = fopen(filename,"w+")) != NULL)
	{
		fclose(f);

		return true;
	}

	return false;
}

bool create_file_directory(char const *in_path)
{
	simple_c_string c_file;
	if (strstr(in_path, "\\"))
	{
		dismantling_string(in_path, "\\", &c_file);
	}
	else if (strstr(in_path, "/"))
	{
		dismantling_string(in_path, "/", &c_file);
	}

	char path[260] = { 0 };
	for (int i = 0;i < c_file.size;i++)
	{
		char *value = get_string(i,&c_file);
		strcat(value, "\\");
		strcat(path, value);
		if (_access(path,0) == -1)
		{
			_mkdir(path);
		}
	}

	destroy_string(&c_file);

	return _access(path, 0) == 0;
}

bool open_url(const char* url)
{
	//���ַ�תΪխ�ַ�
	wchar_t path[1024] = { 0 };
	char_to_wchar_t(path,1024, url);

	return open_url_w(path);
}

bool open_url_by_param(const char* url, const char* param)
{
	//���ַ�תΪխ�ַ�
	wchar_t path[1024] = { 0 };
	char_to_wchar_t(path, 1024, url);

	wchar_t my_param[1024] = { 0 };
	char_to_wchar_t(my_param, 1024, param);
	return open_url_by_param_w(path, my_param);
}

bool open_by_operation(const char* in_operation, const char* url, const char* param)
{
	wchar_t my_operation[1024] = { 0 };
	char_to_wchar_t(my_operation, 1024, in_operation);

	//���ַ�תΪխ�ַ�
	wchar_t path[1024] = { 0 };
	char_to_wchar_t(path, 1024, url);

	wchar_t my_param[1024] = { 0 };
	char_to_wchar_t(my_param, 1024, param);

	return open_by_operation_w(my_operation,path, my_param);
}

bool open_explore(const char* url)
{
	//���ַ�תΪխ�ַ�
	wchar_t path[1024] = { 0 };
	char_to_wchar_t(path, 1024, url);

	return open_explore_w(path);
}

bool get_file_buf(const char *path, char *buf)
{
	FILE *f = NULL;
	if ((f = fopen(path, "r")) != NULL)
	{
		char buf_tmp[2048] = { 0 };
		int file_size = 0;
		while ((file_size = fread(buf_tmp, 1,1024, f)) > 0)
		{
			strcat(buf, buf_tmp);
			memset(buf_tmp, 0, sizeof(buf_tmp));
		}

		fclose(f);

		return buf[0] != '\0';
	}

	return false;
}

bool save_file_buff(const char* path, char* buf)
{
	FILE* f = NULL;
	if ((f = fopen(path, "w")) != NULL)
	{
		fprintf(f, "%s", buf);
		fclose(f);

		return true;
	}

	return false;
}

bool add_file_buf(const char *path, char *buf)
{
	FILE *f = NULL;
	if ((f = fopen(path, "a+")) != NULL)
	{
		fprintf(f, "%s", buf);
		fclose(f);

		return true;
	}

	return false;
}

bool add_new_file_buf(const char *path, char *buf)
{
	FILE *f = NULL;
	if ((f = fopen(path, "w+")) != NULL)
	{
		fprintf(f, "%s", buf);
		fclose(f);

		return true;
	}

	return false;
}

bool add_new_file_buf_w(const wchar_t* path, char* buf)
{
	FILE* f = NULL;
	if ((f = _wfopen(path, L"w+")) != NULL)
	{
		fprintf(f, "%s", buf);
		fclose(f);

		return true;
	}

	return false;
}

bool get_file_buf_w(const wchar_t* path, char* buf)
{
	FILE* f = NULL;
	if ((f = _wpopen(path, L"r")) != NULL)
	{
		char buf_tmp[2048] = { 0 };
		int file_size = 0;
		while ((file_size = fread(buf_tmp, 1, 1024, f)) > 0)
		{
			strcat(buf, buf_tmp);
			memset(buf_tmp, 0, sizeof(buf_tmp));
		}

		fclose(f);

		return buf[0] != '\0';
	}

	return false;
}

bool save_data_to_disk_w(const wchar_t* path, char* buf, int buf_size)
{
	FILE* f = NULL;
	if ((f = _wfopen(path, L"w+")) != NULL)
	{
		fwrite(buf, buf_size, 1, f);
		fclose(f);
		return true;
	}

	return false;
}

bool load_data_from_disk_w(const wchar_t* path, char* buf)
{
	FILE* f = NULL;
	if ((f = _wfopen(path, L"rb")) != NULL)
	{
		//���ļ�ָ���ƶ����ļ�β��;
		fseek(f, 0, SEEK_END);

		int l = 0;
		//�����ص�ǰ�ļ���λ�ã����ص�λΪ�ֽ� ����0 �����ļ���Ч
		if ((l = ftell(f)) > 0)
		{
			//���ļ�ָ���Ƶ��ļ�ͷ��
			rewind(f);
			//���ļ�����������
			fread(buf, sizeof(unsigned char), l, f);
			//�ļ�������Ҫ0��β
			//buf[l] = '\0';
		}
		fclose(f);

		return true;
	}

	return false;
}

bool is_file_exists_w(const wchar_t* filename)
{
	FILE* f = NULL;
	if ((f = _wfopen(filename, L"r")) != NULL)
	{
		fclose(f);

		return true;
	}

	return false;
}

bool open_url_w(const wchar_t* url)
{
	return open_by_operation_w(L"open", url, NULL);;
}

bool open_url_by_param_w(const wchar_t* url, const wchar_t* param)
{
	return open_by_operation_w(L"open", url, param);;
}

bool open_by_operation_w(const wchar_t* in_operation, const wchar_t* url, const wchar_t* param)
{
	return check_ShellExecute_ret(ShellExecute(NULL,
		in_operation,
		url,
		param,
		NULL,
		SW_SHOWNORMAL));
}

bool open_explore_w(const wchar_t* url)
{
	return open_by_operation_w(L"explore", url,NULL);;
}

unsigned int get_file_size_by_filename_w(const wchar_t* filename)
{
	unsigned int file_size = 0;

	FILE* f = NULL;
	if ((f = _wfopen(filename, L"r")) != NULL)
	{
		file_size = get_file_size(f);

		fclose(f);
	}

	return file_size;
}

bool load_data_from_disk(const char* path, char* buf)
{
	FILE* f = NULL;
	if ((f = fopen(path, "rb")) != NULL)
	{
		//���ļ�ָ���ƶ����ļ�β��;
		fseek(f, 0, SEEK_END);

		int l = 0;
		//�����ص�ǰ�ļ���λ�ã����ص�λΪ�ֽ� ����0 �����ļ���Ч
		if ((l = ftell(f)) > 0)
		{
			//���ļ�ָ���Ƶ��ļ�ͷ��
			rewind(f);
			//���ļ�����������
			fread(buf, 1, l, f);
			//�ļ�������Ҫ0��β
			//buf[l] = '\0';
		}
		fclose(f);

		return true;
	}

	return false;
}

unsigned int get_file_size_by_filename(const char *filename)
{
	unsigned int file_size = 0;

	FILE *f = NULL;
	if ((f = fopen(filename,"r")) != NULL)
	{
		file_size = get_file_size(f);

		fclose(f);
	}

	return file_size;
}

//asdoiajoi ajs aoisjd oaisjd oiasjdoi asodao ijaosijd oaisdja index
unsigned int get_file_size(FILE *file_handle)
{
	unsigned int file_size = 0;

	unsigned int current_read_postion = ftell(file_handle);
	fseek(file_handle, 0, SEEK_END);
	file_size = ftell(file_handle);
	fseek(file_handle, current_read_postion, SEEK_SET);

	return file_size;
}

bool save_data_to_disk(const char* path, char* buf, int buf_size)
{
	FILE* f = NULL;
	if ((f = fopen(path, L"wb")) != NULL)
	{
		fwrite(buf, buf_size, 1, f);
		fclose(f);
		return true;
	}

	return false;
}

size_t wchar_t_to_char(
	_out_pram(char*) dst_char,
	size_t char_size,
	_in_pram(wchar_t const*) _Src)
{
	size_t wchar_t_size = wcslen(_Src);

	size_t wchar_t_to_char_count = 0;
	printf("\nwchar_t to char:[%s];\n", strerror(wcstombs_s(
		&wchar_t_to_char_count, 
		dst_char, char_size, 
		_Src,
		wchar_t_size)));

	return wchar_t_to_char_count;
}

size_t char_to_wchar_t(
	_out_pram(wchar_t*) dst_wchar_t,
	size_t wchar_t_size, 
	_in_pram(char const*) _Src)
{
	size_t char_size = strlen(_Src);
	
	size_t char_to_wchar_t_count = 0;
	printf("\nchar to wchar_t:[%s];\n", strerror(mbstowcs_s(
		&char_to_wchar_t_count, 
		dst_wchar_t,
		wchar_t_size, 
		_Src,
		char_size)));
	
	return char_to_wchar_t_count;
}