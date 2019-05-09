#ifndef _TWAPI_W01_UTIL_H_
#define _TWAPI_W01_UTIL_H_

#include <ShlDisp.h>

class TW_File
{
protected:
	HANDLE mh_file;

public:
	TW_File();
	~TW_File();

	char IsOpenFile();
	HANDLE GetHandle();
	__int64 GetFileSize();

	int OpenFile(const wchar_t *ap_path, unsigned char a_is_read = 1, unsigned char a_is_write = 0);

	void WriteStream(const void *ap_data, unsigned int a_data_size);
	int ReadStream(void *ap_data, unsigned int a_max_size);

	void CloseFile();
};

class TW_TextFile
{
protected:
	FILE *mp_file;
	wchar_t m_internal_buffer[2048];
public:
	TW_TextFile();
	~TW_TextFile();

	FILE *GetPointer();
	__int64 GetFileSize();

	int OpenFile(const wchar_t *ap_path, const wchar_t *ap_open_flag);
	void CloseFile();

	int WriteFormat(const wchar_t *ap_format, ...);
	void WriteString(const wchar_t *ap_string, int a_length = -1);
	void WriteString(TW_String *ap_string);

	int ReadString(wchar_t *ap_string, int a_max_length, int a_remove_line_peed = 1);
	int ReadString(TW_String *ap_string, int a_remove_line_peed = 1);
};

struct FileItemInfo {
	// 0th bit : 0 -> file, 1 -> directory, 
	// 7th bit(0x80, 0x7F) : 1 -> ��ó�� ����, 0 -> ó�� �Ϸ�� ����, 6th bit(0x40, 0xBF) : 1 -> Ȯ���� ���� �ɸ�, 0 -> Ȯ���� ���� �Ȱɸ�
	UINT8 state;
	UINT8 depth_count;
	UINT8 ext_memory_size;     // .�� �� Ȯ������ ���̰� �޸� ũ��� ����Ǿ� �ִ�. "tipsware_name.txt" �̶�� 6�� ����Ǿ� ���� ("." �� "\0"�� ���� ����)
	UINT8 ext_start_pos;
	UINT16 item_name_length;   // \0�� ������ ũ�Ⱑ ����� "test" �̸� 5�� ����Ǿ� ����
	UINT16 item_full_length;   // \0�� ������ ũ�Ⱑ ����� 

	wchar_t *p_item_name;
	node_ext *p_parent;
};

typedef void(*T_DELETE_FILE_DATA)(void *ap_object);

class TDirectoryManager
{
private:
	UINT8 m_use_ext_info;
	INT32 m_base_path_length;  // m_base_path ���ڿ��� ������ ����ִ� 'c:\test' �̸� 7�� ����Ǿ� �ִ�.

	TW_ListExt m_item_list;
	wchar_t m_base_path[MAX_PATH];

	void GetFilesFromDirectory(wchar_t *a_path, UINT16 a_length, INT32 a_depth, node_ext *ap_parent);

public:
	TDirectoryManager(INT8 a_use_ext_info = 1);
	~TDirectoryManager();

	// ad_data �� ��θ� �����մϴ�. ap_make_path�� MAX_PATH ũ���̰ų� ap_data->item_full_length + 1 ũ�⿩�� �մϴ�.
	void GetFullPath(FileItemInfo *ap_data, wchar_t *ap_make_path);
	// ad_data �� ��θ� �����մϴ�. ������ m_base_path�� ������� �ʰ� ap_dest_path �� ����ؼ� �����մϴ�.
	// ap_make_path�� MAX_PATH ũ���̰ų� ap_data->item_full_length + a_dest_path_length - m_base_path_length + 1 ũ�⿩�� �մϴ�.
	INT32 GetFullPath(wchar_t *ap_dest_path, INT32 a_dest_path_length, FileItemInfo *ap_data, wchar_t *ap_make_path);

	// ������ ��θ� �߰��մϴ�. ap_item_path ���� ���� �Ǵ� ���� ��� ���������� ������ ���, ���� \�� ���ԵǸ� �ʵȴ�
	int AddItem(wchar_t *ap_item_path, INT8 a_is_directory);
	// AddItem �Լ��� ������ ������ �˻��Ͽ� ������ ������ ����Ʈ�� �߰��մϴ�.
	void ScanDirectory();
	void ResetContent();

	// ap_dest_path�� TDirectoryManager�� ������ �ִ� �����۵��� �����Ѵ�. ( ������ ���� ��� �����Ѵ� )
	// CopyItemToDestPath ���޵Ǵ� dest ��δ� MAX_PATH ũ��� �Ҵ�� �޸��̿��� �Ѵ�. �׸��� ����� ������ \�� ���ԵǾ� �־�� �Ѵ�
	// a_dest_path_length ���� wcslen(ap_dest_path)�� ���� ����ִ�
	// a_use_ext_filter �� ����Ϸ��� CheckFileExt �Լ��� ���� ȣ���ؼ� ���� ����� ������ �Ѵ�
	void CopyItemToDestPath(wchar_t *ap_dest_path, INT32 a_dest_path_length, INT8 a_use_ext_filter = 0);
	// ���޵� ���� Ȯ���� ��ϰ� ��ġ�ϴ� ������ ��� �ִ��� üũ�ϴ� �Լ�
	int CheckFileExt(INT32 a_ext_count, wchar_t **ap_ext_list, UINT8 *ap_ext_memory_size);
	// state �� ����� ���� Ȯ���� ���� ����� �ʱ�ȭ�Ѵ�
	void ResetFileExtFilter();
	TW_ListExt *GetItemList();
	wchar_t *GetBasePath();
	INT32 GetBasePathLength();
};



// �����ڵ� ������ �����Ѵ�.
int TWAPI_CreateUnicodeFile(const wchar_t *ap_file_path, char a_check_exist_file = 1);
// ����ڰ� ������ ������ ũ�⸦ ��´�. ���� ������ ������ 0xFFFFFFFF ���� ��ȯ�Ѵ�.
unsigned int TWAPI_GetFileSize(wchar_t *ap_file_path);
// ������ ����ִ� Zip ������ �����.
void TWAPI_CreateZipFile(const wchar_t *ap_file_path, char a_is_overwrite = 0);
// ������ Zip ���Ͽ� ������ �߰��Ѵ�.
void TWAPI_AddFileToZip(wchar_t *ap_zip_path, wchar_t *ap_file_path);

// Zip ������ ���� ������ �����Ѵ�. (RST_TW_Zip.DLL, DSH_TW_Zip.DLL�� ���)
void TWAPI_CreateZipEngine();
// �ڽ��� ������ ���� ����� ������ Zip ������ ���� ������ �����.
void TWAPI_AddFileToZipEngine(wchar_t *ap_zip_path, int a_file_count, char *(*afp_get_file_name)(int a_index, void *ap_data), void *ap_data);
// Zip ������ ���� �����ߴ� ������ �����Ѵ�.
void TWAPI_DestroyZipEngine();

// API �Լ� ���� �߻��� ���� ������ ���ڿ��� ���� �� ���
void TWAPI_GetLastErrorMessage(TW_String *ap_string);

// ������ ��θ� �����Ѵ�. ���� ���, 'c:\temp\tips\example'�̶�� ��θ� ������
// C ����̺꿡 temp ������ �����ϰ� temp ������ tips ������ �����ϰ� tips ������ example ������ �����մϴ�.
// ���� �ش� ������ �̹� �����Ѵٸ� �ش� ������ ������ ���õ˴ϴ�.
void TWAPI_CreateDirectory(wchar_t *ap_path, int a_path_length);
// ������ ��ΰ� �����ϴ��� üũ�Ѵ�. �����ϸ� 1, �������� ������ 0�� ��ȯ�Ѵ�.
int TWAPI_IsExistPath(const wchar_t *ap_path);
// "c:\temp\test.txt" ��� �����ϸ� ".txt" �� . �κ��� ����Ű�� �ּҸ� ��ȯ�Ѵ�. 
// ������ ��ο� . �� ���ٸ� NULL�� ��ȯ�Ѵ�.
const wchar_t *TWAPI_GetExtName(const wchar_t *ap_file_name);
const char *TWAPI_GetExtNameA(const char *ap_file_name);
// ���ϸ��� ���Ե� ��ο��� ��θ� �����Ͽ� ���ο� �޸𸮸� �Ҵ��Ͽ� �������ִ� �Լ�
// "c:\temp\test.txt" ��� �����ϸ� ���ο� �޸𸮸� �����ϰ� �ش� �޸𸮿� "c:\temp\"�� �������ش�. 
// �����ϸ� NULL �� ��ȯ�Ѵ�. �׸��� new �����ڷ� �����Ҵ�� �޸� �̱� ������ ��ȯ�� �ּҴ� �ݵ�� �����ؾ� �Ѵ�.
// ap_create_path_length �� ������ �ѱ�� ������ ���ڿ��� ���̸� �־��ش�. "c:\temp\" ��� 8�� ����ȴ�.
// ���丮�θ� ������ ��ο��� ������ ���丮�� ������ ��θ��� ���ϰ� �ʹٸ� �ش� ����� ���� \ �� �����ϰ� ������ �ȴ�.
// ���� ���, "c:\temp\tips"��� ��θ� ������ tips�� ������ "c:\temp\"��� ��θ� ����� �ݴϴ�.
wchar_t *TWAPI_GetPathExceptFileName(const wchar_t *ap_path_name, int *ap_create_path_length);
char *TWAPI_GetPathExceptFileNameA(const char *ap_path_name, int *ap_create_path_length);
// ���ϸ��� ���Ե� ��ο��� ���ϸ��� �����ϴ� �ּҸ� ã�� �Լ� 
// "c:\temp\test.txt" ��� �����ϸ� "test.txt" �̸��� ������ġ�� �ش��ּҸ� ��ȯ�Ѵ�.
// ���丮�θ� ������ ��ο��� ������ ���丮�� �̸��� ���ϰ� �ʹٸ� �ش� ����� ���� \�� �����ϰ� ������ �ȴ�.
// ���� ���, "c:\temp\tips"��� ��θ� ������ "tips"�� ���� �ּҸ� ��ȯ�Ѵ�.
const wchar_t *TWAPI_GetFileNameFromPath(const wchar_t *ap_path_name);
const char *TWAPI_GetFileNameFromPathA(const char *ap_path_name);
// ������ ��ο� �ִ� ��� ���� �� ������ �����Ѵ�.
// ap_path �� ���޵Ǵ� ��δ� MAX_PATH ũ�⸦ ���� �޸��̿��� �ϸ� \ �� ������ �Ѵ�. 
// a_path_length ���� wcslen(ap_path) ���� ���� �Ѵ� 
int TWAPI_RemoveFiles(wchar_t *ap_path, unsigned int a_path_length);

// ���� �۾� ��θ� �������� FindFirstFile �Լ����� ����� ��� �˻� ���ڿ��� �����Ѵ�.
UINT32 TWAPI_MakeFindPath(wchar_t *ap_path, INT32 a_max_len, wchar_t *ap_filter = (wchar_t *)L"*.*", INT32 a_filter_len = 3);

// �� �Լ��� FindFirstFile���� ����� �˻� ��θ� �����մϴ�. ���� ���, ap_path�� 'c:\tips\work\*.*' ������ 
// ��ΰ� ����Ǿ� �ְ� ap_dir_name�� '..'�̶�� �����Ǹ� 'c:\tips\*.*'�� �������ִ� �Լ��Դϴ�.
// �׸��� ���ڿ� ���� ���ڿ� ���̴� ��� '\0'�� �������� �ʽ��ϴ�. ���� ap_path�� ���丮�� �߰����� ��
// ������ ������ �ʵ��� a_dir_len + 1��ŭ�� ���� ������ �־�� �մϴ�.
INT32 TWAPI_UpdateFilePath(wchar_t *ap_path, INT32 *ap_path_len, wchar_t *ap_dir_name, INT32 a_dir_len, 
							wchar_t *ap_filter = (wchar_t *)L"*.*", INT32 a_filter_len = 3);

#endif