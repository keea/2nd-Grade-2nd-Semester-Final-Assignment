#ifndef _TWAPI_W01_H_
#define _TWAPI_W01_H_

/*
  �� ���̺귯���� Win32�� Direct2D�� ����ؼ� ���α׷��� �ϴ� 
  �����ڵ��� ���� ���ϰ� ���α׷��� ������ �� �ֵ��� ���� ���̺귯�� �Դϴ�.
  ������ ���� ������� �������̽� ���̺귯���� �����ϰ� �������
  �ȵ���̵�� ���̺귯���� TWAPI_A01�� Ŭ������ �Լ��� ����ϰ� �����Ͽ�
  Win32 ���α׷��� �ȵ���̵� �۰��� �ҽ� ������ ���ϰ� �� �� �ֵ��� �߽��ϴ�.
  �� ���̺귯���� ���� �������� �߰��ϰų� �������� �ʿ��ϴٸ� �Ʒ��� �� ���Ϸ�
  �ǰ� �����ֽðų� ��α׿� ��۷� �����ּ���.
  �̸��� : tipsware@naver.com
  ��α� : http://blog.naver.com/tipsware
  
  ���̺귯�� ���� : 2017�� 11�� 2�� ( �輺�� )
  ���̺귯�� ���� ������Ʈ : 2017�� 12�� 14�� ( �輺�� )

  �׸��� �� ���̺귯���� ����ϴ� ������� ���ϰ� ����� �� �ֵ��� 
  �ʿ��� ���̺귯���� �߰��ϴ� ��ó���� �ڵ带 ����ϴ�.

  #pragma comment(lib, "D2D1.lib")
  #pragma comment(lib, "D3D11.lib")
  #pragma comment(lib, "DXGI.lib")
  #pragma comment(lib, "dxguid.lib")
  #pragma comment(lib, "DWRITE.lib")
  #pragma comment(lib, "Msimg32.lib")
  #pragma comment(lib, "WS2_32.lib")

  #ifdef _DEBUG
      #pragma comment(lib, "DSH_TWAPI_W01.lib")
  #else
      #pragma comment(lib, "RST_TWAPI_W01.lib")
  #endif
*/

#include <Windows.h>
#include <Windowsx.h>
#include <stdio.h>

#include <wrl.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DXGI1_2.h>
#include <d2d1effectauthor.h>
#include <dwrite.h>

using namespace Microsoft::WRL;

// Direct2D�� ����ϱ� ���� ��� ���ϰ� ���̺귯�� ������ ���Խ�Ų��.
#include <d2d1.h>
#include <Wincodec.h>     // IWICImagingFactory�� ����ϱ� ���ؼ� �߰�

// Direct2D���� ���� ����� ���� �����̽��� ������ �� �ֵ��� �����Ѵ�.
using namespace D2D1;

#define TW_SetIRect(x, l, t, r, b){ x.left = l; x.top = t; x.right = r; x.bottom = b; }
#define TW_SetFRect(x, l, t, r, b){ x.left = (float)(l); x.top = (float)(t); x.right = (float)(r); x.bottom = (float)(b); }
#define TW_PosInRect(pos, l, t, r, b) (l <= pos.x && t <= pos.y && r >= pos.x && b >= pos.y) 
#define TW_PtInRect(pos, rect) (rect.left <= pos.x && rect.top <= pos.y && rect.right >= pos.x && rect.bottom >= pos.y) 

#define TW_SetFPos(pt, x_pos, y_pos){ pt.x = (FLOAT)x_pos; pt.y = (FLOAT)y_pos; }
#define TW_SetFSize(size, x_pos, y_pos){ size.width = (FLOAT)x_pos; size.height = (FLOAT)y_pos; }

#define TW_COLOR(color) (color/256.0f)
#define TW_RGB(color, red, green, blue) { color.r = (FLOAT)(red/255.0); color.g = (FLOAT)(green/255.0); color.b = (FLOAT)(blue/255.0); color.a = 1.0f; }
#define TW_FRGB(color, red, green, blue) { color.r = (FLOAT)red; color.g = (FLOAT)green; color.b = (FLOAT)blue; color.a = 1.0f; }

#define TW_Release(x) { if(x != NULL){ x->Release(); x = NULL; }}

#define T_INVALID_INDEX        0xFFFFFFFF

#define TW_IMAGE_BMP           0
#define TW_IMAGE_PNG           1
#define TW_IMAGE_JPEG          2

#define ALIGNMENT_LEFT        0
#define ALIGNMENT_CENTER      1
#define ALIGNMENT_RIGHT       2

template<class Interface>
inline void TW_IRelease(Interface **ap_interface_object)
{
	if (*ap_interface_object != NULL) {
		(*ap_interface_object)->Release();
		(*ap_interface_object) = NULL;
	}
}

// Direct2D�� �����ϴ� ���� ��ü�� �����ϴ� Factory ��ü
extern ID2D1Factory *gp_factory;
// Windows 8.1 �̻��� �ü������ ����ϱ� ���� Factory ��ü
extern ID2D1Factory1 *gp_factory_ext;
// WIC(Windows Imaging Component)���� ��ü�� �����ϱ� ���� Factory ��ü
extern IWICImagingFactory *gp_wic_factory;

// TW_String Ŭ������ ���ڿ��� ���ϰ� ������ �� �ֵ��� ���� Ŭ�����Դϴ�. �� Ŭ������ ���ڿ� ó���� ���� ������ �ϱ� 
// ���ؼ� ���ڿ��� ���̸� ���� ����ϴ� ��찡 ������ �� �� ����ϴ� ���ڿ��� ���̴� ���� ���ڿ��� ���̸� �ǹ��մϴ�. 
// ���� ���, �� ��ü�� L"abc" ���ڿ��� ������ ������ ���ڿ��� ���̰� ����� m_length���� 3�� ���� �˴ϴ�. �׸��� ���ڿ�
// a_length�� ����Ҷ��� �� ��Ģ�� �����ϰ� ����˴ϴ�.
class TW_String
{
protected:
	wchar_t *mp_string;   // �� ��ü�� �����ϴ� ���ڿ��� �ּ�
	int m_length;         // �� ��ü�� �����ϴ� ���ڿ��� ���� ("abc"�̸� 3�� ����)

public:
	TW_String();  // �⺻ ������
	TW_String(const char *ap_string);
	TW_String(const wchar_t *ap_string);
	TW_String(const TW_String &ar_string);  // ���� ������

	virtual ~TW_String();

	void InitialObject(const wchar_t *ap_string);  // ��ü �ʱ�ȭ �Լ�
	inline int GetLength() { return m_length; }   // ���ڿ��� ���̸� �˷��ִ� �Լ�
	inline const wchar_t *GetString() { return mp_string; } // ���ڿ��� ������ �˷��ִ� �Լ�
	void RemoveString();                           // �� ��ü�� �����ϴ� ���ڿ��� �����Ѵ�.

	// ���ڿ��� Ư�� ���ڿ��� ���ԵǾ� �ִٸ� �� ��ġ�� ã�´�.
	const wchar_t *FindString(const wchar_t *ap_string);
	// ���ڿ��� Ư�� ���ڰ� ���ԵǾ� �ִٸ� �� ��ġ�� ã�´�.
	const wchar_t *FindChar(wchar_t a_char);
	// ������ ��ġ�� �ִ� ���ڸ� ��´�.
	wchar_t GetAt(UINT32 a_index);
	// Ư�� ��ġ�� ���ڸ� �����Ѵ�.
	void SetAt(UINT32 a_index, wchar_t a_char);
	// ���ڿ��� ���� ������ ���ڸ� �߰��Ѵ�.
	void AddChar(wchar_t a_char);
	// ���޵� ���ڿ��� �����ؼ� �����Ѵ�.
	void SetString(const wchar_t *ap_string);
	// ���޵� ���ڿ��� �ּҸ� �״�� ����Ѵ�. (���縦 �����ʰ� ������ �״�� �����)
	void AttachString(wchar_t *ap_string, int a_length);
	// ����� ���ڿ��� �ٸ� ���ڿ��� �߰��ϴ� ��쿡 ����Ѵ�.
	// ���� ���, "abc"�� ����Ǿ� �ִµ� AddString("def", 3); �̶�� ����ϸ� "abcdef"�� �˴ϴ�.
	void AddString(const wchar_t *ap_string, int a_length);
	// ���ڿ����� ���� �������� �ִ� ���ڸ� �����Ѵ�. ��ȯ ���� ���� ������ �����̴�.
	INT32 DeleteLastChar();
	// ���� ���ڿ��� ����ִ��� Ȯ���ϴ� �Լ�
	int IsEmpty();

	// ���޵� �ƽ�Ű ���ڿ��� �����ڵ�� �����Ͽ� Ŭ���� ���ο� �����Ѵ�. ��ȯ���� ��ȯ�� ������ �����̴� (wcslen ���� ��ġ�Ѵ� )
	int AsciiToUnicode(char *ap_src_string);
	// ���޵� �ƽ�Ű ���ڿ��� �����ڵ�� �����Ͽ� ap_dst_string�� ��ȯ�Ѵ�. ��ȯ���� ��ȯ�� ������ �����̴� (wcslen ���� ��ġ�Ѵ� )
	// ap_dest_string �� NULL�� ����ϸ� ��ȯ�������� ���ڿ� ���̸� �˾Ƴ��� �ִ�.
	static int AsciiToUnicode(char *ap_src_string, wchar_t *ap_dest_string);
	// Ŭ������ �����ϴ� ���ڿ��� UTF8 �������� �����Ͽ� ap_dest_string�� ��ȯ�Ѵ�. ��ȯ���� UTF8 ���ڿ��� ���� �޸� ũ���̴�.
	// ap_dest_string �� NULL�� ����ϸ� ��ȯ�������� ���ڿ� ���̸� �˾Ƴ��� �ִ�.
	int UnicodeToUTF8(char *ap_dest_string);
	// ap_src_string ���ڿ��� UTF8 �������� �����Ͽ� ap_dest_string�� ��ȯ�Ѵ�. ��ȯ���� UTF8 ���ڿ��� ���� �޸� ũ���̴�.
	// ap_dest_string �� NULL�� ����ϸ� ��ȯ�������� ���ڿ� ���̸� �˾Ƴ��� �ִ�.
	static int UnicodeToUTF8(wchar_t *ap_src_string, char *ap_dest_string);
	// Ŭ������ �����ϴ� ���ڿ��� �ƽ�Ű�ڵ�� �����Ͽ� ap_dst_string�� ��ȯ�Ѵ�. ��ȯ���� ��ȯ�� ������ �����̴� (strlen ���� ��ġ�Ѵ� )
	// ap_dest_string �� NULL�� ����ϸ� ��ȯ�������� ���ڿ� ���̸� �˾Ƴ��� �ִ�.
	int UnicodeToAscii(char *ap_dest_string);
	// ���޵� �����ڵ� ���ڿ��� �ƽ�Ű�ڵ�� �����Ͽ� ap_dst_string�� ��ȯ�Ѵ�. ��ȯ���� ��ȯ�� ������ �����̴� (strlen ���� ��ġ�Ѵ� )
	// ap_dest_string �� NULL�� ����ϸ� ��ȯ�������� ���ڿ� ���̸� �˾Ƴ��� �ִ�.
	static int UnicodeToAscii(wchar_t *ap_src_string, char *ap_dest_string);

	// printf�� ���� ������ ����ϰ� ���� �� ����ϴ� �Լ�. �� �Լ��� ����� �ش��ϴ� ���ڿ��� ȭ�鿡 ��µ��� �ʰ�
	// �� ��ü�� ���ڿ�(mp_string)�� ����˴ϴ�.
	int Format(const wchar_t *ap_format, ...);

	// ���� ������ �����ε� str = L"test";
	void operator=(const TW_String& ar_string);
	// ���ڿ� Ȯ��(append)�� ���� ������ �����ε�
	void operator+=(const TW_String& ar_str);
	// ���ڿ� Ȯ��(append)�� ���� ������ �����ε�
	void operator+=(const wchar_t a_char);

	// ���ڿ��� ���ϱ� ���� ������ �����ε�
	friend TW_String operator+(const TW_String& ar_str1, const TW_String& ar_str2)
	{
		TW_String str = ar_str1;
		str.AddString(ar_str2.mp_string, ar_str2.m_length);
		return str;
	}
	// ���ڿ� �񱳸� ���� ������ �����ε� (������ Ȯ���ϱ� ���� ������)
	friend char operator==(const TW_String& ar_str1, const TW_String& ar_str2)
	{
		if (ar_str1.m_length != ar_str2.m_length) return 0;
		return memcmp(ar_str1.mp_string, ar_str2.mp_string, ar_str1.m_length << 1) == 0;
	}
	// ���ڿ� �񱳸� ���� ������ �����ε� (�ٸ��� Ȯ���ϱ� ���� ������)
	friend char operator!=(const TW_String& ar_str1, const TW_String& ar_str2)
	{
		if (ar_str1.m_length != ar_str2.m_length) return 1;
		return memcmp(ar_str1.mp_string, ar_str2.mp_string, ar_str1.m_length << 1) != 0;
	}

	friend char operator!=(const TW_String& ar_str1, const wchar_t *ap_str2)
	{
		if (ar_str1.m_length != wcslen(ap_str2)) return 1;
		return memcmp(ar_str1.mp_string, ap_str2, ar_str1.m_length << 1) != 0;
	}

	operator LPCWSTR() const
	{
		return mp_string;
	}

	operator LPWSTR()
	{
		return mp_string;
	}
};


// TW_StringA Ŭ������ �ƽ�Ű ������ ���ڿ��� ���ϰ� ������ �� �ֵ��� ���� Ŭ�����Դϴ�. �� Ŭ������ ���ڿ� ó���� ���� ������ �ϱ� 
// ���ؼ� ���ڿ��� ���̸� ���� ����ϴ� ��찡 ������ �� �� ����ϴ� ���ڿ��� ���̴� ���� ���ڿ��� ���̸� �ǹ��մϴ�. 
// ���� ���, �� ��ü�� "abc" ���ڿ��� ������ ������ ���ڿ��� ���̰� ����� m_length���� 3�� ���� �˴ϴ�. �׸��� ���ڿ�
// a_length�� ����Ҷ��� �� ��Ģ�� �����ϰ� ����˴ϴ�.
class TW_StringA
{
protected:
	char *mp_string;   // �� ��ü�� �����ϴ� ���ڿ��� �ּ�
	int m_length;      // �� ��ü�� �����ϴ� ���ڿ��� ���� ("abc"�̸� 3�� ����)

public:
	TW_StringA();  // �⺻ ������
	TW_StringA(const char *ap_string);
	TW_StringA(const wchar_t *ap_string);
	TW_StringA(const TW_StringA &ar_string);  // ���� ������

	virtual ~TW_StringA();

	void InitialObject(const char *ap_string);  // ��ü �ʱ�ȭ �Լ�
	inline int GetLength() { return m_length; }   // ���ڿ��� ���̸� �˷��ִ� �Լ�
	inline const char *GetString() { return mp_string; } // ���ڿ��� ������ �˷��ִ� �Լ�
	void RemoveString();                           // �� ��ü�� �����ϴ� ���ڿ��� �����Ѵ�.

												   // ���ڿ��� Ư�� ���ڿ��� ���ԵǾ� �ִٸ� �� ��ġ�� ã�´�.
	const char *FindString(const char *ap_string);
	// ���ڿ��� Ư�� ���ڰ� ���ԵǾ� �ִٸ� �� ��ġ�� ã�´�.
	const char *FindChar(char a_char);
	// ������ ��ġ�� �ִ� ���ڸ� ��´�.
	char GetAt(UINT32 a_index);
	// Ư�� ��ġ�� ���ڸ� �����Ѵ�.
	void SetAt(UINT32 a_index, char a_char);
	// ���ڿ��� ���� ������ ���ڸ� �߰��Ѵ�.
	void AddChar(char a_char);
	// ���޵� ���ڿ��� �����ؼ� �����Ѵ�.
	void SetString(const char *ap_string);
	// ���޵� ���ڿ��� �ּҸ� �״�� ����Ѵ�. (���縦 �����ʰ� ������ �״�� �����)
	void AttachString(char *ap_string, int a_length);
	// ����� ���ڿ��� �ٸ� ���ڿ��� �߰��ϴ� ��쿡 ����Ѵ�.
	// ���� ���, "abc"�� ����Ǿ� �ִµ� AddString("def", 3); �̶�� ����ϸ� "abcdef"�� �˴ϴ�.
	void AddString(const char *ap_string, int a_length);
	// ���ڿ����� ���� �������� �ִ� ���ڸ� �����Ѵ�. ��ȯ ���� ���� ������ �����̴�.
	INT32 DeleteLastChar();
	// ���� ���ڿ��� ����ִ��� Ȯ���ϴ� �Լ�
	int IsEmpty();

	// Ŭ������ �����ϴ� ���ڿ��� �����ڵ� �������� �����Ͽ� ap_dest_string �� ��ȯ�Ѵ�. ��ȯ���� ��ȯ�� ������ �����̴�.
	int AsciiToUnicode(wchar_t *ap_dest_string);
	// ���޵� �ƽ�Ű ���ڿ��� �����ڵ�� �����Ͽ� ap_dst_string�� ��ȯ�Ѵ�. ��ȯ���� ��ȯ�� ������ �����̴� (wcslen ���� ��ġ�Ѵ� )
	// ap_dest_string �� NULL�� ����ϸ� ��ȯ�������� ���ڿ� ���̸� �˾Ƴ��� �ִ�.
	static int AsciiToUnicode(char *ap_src_string, wchar_t *ap_dest_string);
	// ap_src_string ���ڿ��� UTF8 �������� �����Ͽ� ap_dest_string�� ��ȯ�Ѵ�. ��ȯ���� UTF8 ���ڿ��� ���� �޸� ũ���̴�.
	// ap_dest_string �� NULL�� ����ϸ� ��ȯ�������� ���ڿ� ���̸� �˾Ƴ��� �ִ�.
	static int UnicodeToUTF8(wchar_t *ap_src_string, char *ap_dest_string);
	// ���޵� �����ڵ� ���ڿ��� �ƽ�Ű�ڵ�� �����Ͽ� Ŭ���� ���ο� �����Ѵ�.��ȯ���� ��ȯ�� ������ �����̴� (strlen ���� ��ġ�Ѵ� )
	int UnicodeToAscii(wchar_t *ap_src_string);
	// ���޵� �����ڵ� ���ڿ��� �ƽ�Ű�ڵ�� �����Ͽ� ap_dst_string�� ��ȯ�Ѵ�. ��ȯ���� ��ȯ�� ������ �����̴� (strlen ���� ��ġ�Ѵ� )
	// ap_dest_string �� NULL�� ����ϸ� ��ȯ�������� ���ڿ� ���̸� �˾Ƴ��� �ִ�.
	static int UnicodeToAscii(wchar_t *ap_src_string, char *ap_dest_string);

	// printf�� ���� ������ ����ϰ� ���� �� ����ϴ� �Լ�. �� �Լ��� ����� �ش��ϴ� ���ڿ��� ȭ�鿡 ��µ��� �ʰ�
	// �� ��ü�� ���ڿ�(mp_string)�� ����˴ϴ�.
	int Format(const char *ap_format, ...);

	// ���� ������ �����ε� str = L"test";
	void operator=(const TW_StringA& ar_string);
	// ���ڿ� Ȯ��(append)�� ���� ������ �����ε�
	void operator+=(const TW_StringA& ar_str);
	// ���ڿ� Ȯ��(append)�� ���� ������ �����ε�
	void operator+=(char a_char);

	// ���ڿ��� ���ϱ� ���� ������ �����ε�
	friend TW_StringA operator+(const TW_StringA& ar_str1, const TW_StringA& ar_str2)
	{
		TW_StringA str = ar_str1;
		str.AddString(ar_str2.mp_string, ar_str2.m_length);
		return str;
	}
	// ���ڿ� �񱳸� ���� ������ �����ε� (������ Ȯ���ϱ� ���� ������)
	friend char operator==(const TW_StringA& ar_str1, const TW_StringA& ar_str2)
	{
		if (ar_str1.m_length != ar_str2.m_length) return 0;
		return memcmp(ar_str1.mp_string, ar_str2.mp_string, ar_str1.m_length) == 0;
	}
	// ���ڿ� �񱳸� ���� ������ �����ε� (�ٸ��� Ȯ���ϱ� ���� ������)
	friend char operator!=(const TW_StringA& ar_str1, const TW_StringA& ar_str2)
	{
		if (ar_str1.m_length != ar_str2.m_length) return 1;
		return memcmp(ar_str1.mp_string, ar_str2.mp_string, ar_str1.m_length) != 0;
	}

	friend char operator!=(const TW_StringA& ar_str1, const char *ap_str2)
	{
		if (ar_str1.m_length != strlen(ap_str2)) return 1;
		return memcmp(ar_str1.mp_string, ap_str2, ar_str1.m_length) != 0;
	}

	operator LPCSTR() const
	{
		return mp_string;
	}

	operator LPSTR()
	{
		return mp_string;
	}
};

// ���� ����Ʈ�� �����ϰ� �� ����� ����
struct node_ext {
	// ����� ���� �����͸� �����ϱ� ���� ����� ������
	void *p_object;
	// ����� ���� ��带 ����Ű�� ������
	node_ext *p_next;
};

class TW_StockListExt;

class TW_ListExt
{
private:
	// ���� ����Ʈ�� ó���� ���� ����Ű�� ������.
	node_ext *mp_head, *mp_tail;
	// ���� ����Ʈ�� ��ϵ� ����� ������ ������ �ִ� ����.
	int m_node_ext_count;

	// ����ڰ� ����ϴ� �޸𸮸� �����Ҷ� TW_ListExt Ŭ������ ���� ȣ��� �Լ��� �ּҸ� ������ ������
	void(*mp_delete_user_data)(void *ap_object);

public:
	// ��ü ������. ����ڰ� ������ ����� ���� �����͸� ��Ȯ�ϰ� �����ϱ� ���ؼ� �� ��忡 �����
	// �����͸� �����Ҷ� ȣ���� �Լ��� ������ �������� �Ѱ� �޴´�.
	// ( Example ) 
	//
	//    void DeleteUserData(void *parm_object)
	//    {
	//        UserData *p = (UserData *)parm_object;  // �ڽ��� �Ҵ��� ������ ������ ��ȯ�Ѵ�.
	//
	//        delete[] p->p_name; // �ش� ��ü�� ����ü���� �߰������� �Ҵ��� �޸𸮰� �ִٸ� �����Ѵ�.
	//        delete p; // �ش� ��忡 �Ҵ�� �޸𸮸� �����Ѵ�.
	//    }
	//
	//    TW_ListExt my_list(DeleteUserData);
	//
	// ���� NULL�� ���ڷ� �ѱ�� ���, TW_ListExt Ŭ������ �ڽ��� �ı��ɶ� ��常 �����ϰ� ��忡
	// ����� �����ʹ� �״�� �д�. ������ �����ϴ� �������� ������ ���Ḯ��Ʈ�� �����Ҷ��� ������
	// �������� �ʵ��� NULL �� �Ѱܼ� ����ϸ� �ȴ�.
	TW_ListExt(void(*afp_delete_user_data)(void *ap_object) = NULL);
	// ��ü �ı���
	~TW_ListExt();

	// ����ڰ� ����� �޸𸮸� �ı��Ҷ� ȣ���� �Լ��� ����
	void ChangeDeleteUserData(void(*afp_delete_user_data)(void *ap_object));
	// ���� ����Ʈ�� �ʱ�ȭ�ϴ� �Լ�
	void InitListObject();

	// [ ���ο� ��带 �߰��ϴ� �Լ��� ]

	// AddObject, AddObjectByIndex �Լ��� ����ڰ� ������ �޸𸮸� �״�� ��忡 �����ؼ� ����ϱ� ������ 
	// �Ʒ��� ���� �ΰ��� ���·� ����ϴ°� ������ �߻��ϴ� �����ؾ� �մϴ�.
	// ����Ʈ�� ���� ���ο� ��带 �߰��ϴ� �Լ�. 
	//
	//  1. ���� ������ �ּҸ� �Ѱܼ��� �ȵ˴ϴ�.
	//
	//    UserData data;
	//    m_my_list.AddObject(&data);
	//    (����) data ������ ���������̱� ������ �Լ��� ����ÿ� �޸𸮿��� ������� �˴ϴ�. ������ �� �����
	//           m_my_list�� ���� ���ϱ� ������ �ش� �ּҸ� �״�� ����ϰ� �Ǿ� ������ �߻��ϰ� �˴ϴ�.
	//
	//  2. �����Ҵ�� �ּҶ�� �ص� TW_ListExt�� �Ѱ��� �ּҴ� TW_ListExt�� �����ϱ� ���� �����ϸ� ������ �߻��մϴ�.
	//
	//    UserData *p_data = new UserData;
	//    m_my_list.AddObject(p_data);
	//    delete p_data;  <-- (����) �̷��� ���� �����ع����� m_my_list �� ���޵� �ּҰ� ������ �޸𸮸� ����Ű�� �Ǿ�
	//                                ������ �߻��մϴ�.
	//

	// ����Ʈ�� ���� ���ο� ��带 �߰��ϴ� �Լ�. 
	void AddObject(void *parm_object);
	// ���ϴ� ��ġ�� ��带 �߰��ϴ� �Լ�, 0 �� ����ϸ� ����Ʈ�� ���� ù ��ġ�� ��尡 �߰��ȴ�.
	// �߰��� ��ġ�� �߸� �����ϸ� ��� �߰��� �����ϰ� -1 ���� ��ȯ�Ѵ�.
	char AddObjectByIndex(int parm_index, void *parm_object);
	node_ext *AddObjectAfterThisNode(node_ext *parm_this_node, void *parm_p_object);

	// [ ��带 �����ϴ� �Լ��� ]

	// DeleteNode, DeleteAllObject, DeleteNodeByIndex �Լ��� �ι�° ���ڷ� ���� parm_delete_object_flag ������
	// ��忡 ����� ����� �����͸� �����Ұ����� ���θ� �����ϳ���. 
	// ( 0 -> ��常 ����, 1 -> ���� ����� ������ ��� ����)

	// ����Ʈ�� ��ϵ� ���� �߿��� parm_node�� ������ ��带 ã�Ƽ� �ش� ��带 �����Ѵ�.
	char DeleteNode(node_ext *parm_node, char parm_delete_object_flag = 1);
	// ����Ʈ�� ��ϵ� ���� �߿��� parm_object�� ������ ������ ������ �ִ� ��带 ã�Ƽ� �ش� ��带 �����Ѵ�.
	char DeleteNode(void *parm_object, char parm_delete_object_flag = 1);
	// ����Ʈ�� ��ϵ� ��� ��带 �����Ѵ�.
	void DeleteAllNode(char parm_delete_object_flag = 1);
	// ����ڰ� ������ ��ġ�� �ִ� ��带 ã�Ƽ� �ش� ��带 �����Ѵ�.
	char DeleteNodeByIndex(int parm_index, char parm_delete_object_flag = 1);

	// [ ��带 �˻��ϴ� �Լ��� ]

	// ���Ḯ��Ʈ���� ������ ��ġ�� �ִ� ����� �ּҸ� ��´�.
	node_ext *GetNodeByIndex(int parm_index);
	// ���Ḯ��Ʈ���� ������ ��ġ�� �ִ� ��尡 ����Ű�� �����͸� ��´�.
	void *GetObjectByIndex(int parm_index);
	// ���Ḯ��Ʈ�� ��ϵ� �����߿��� parm_object�� ��õ� �ּҿ� ������ �ּҸ� ���� �����Ͱ� �ִ���
	// ã�Ƽ� �� ��ġ�� ��ȯ�Ѵ�. ( -1 �̸� �˻��� �����ߴٴ� ���̴�. )
	int FindNodeIndexByObject(void *parm_object);

	// ���� ó�� ��ġ�� �ִ� ����� �ּҸ� ��´�.
	node_ext *GetHeadNode();
	// ������ ����� ������ �ִ� ��带 ��´�.
	node_ext *GetNextNode(node_ext *parm_current_node);
	// ���� ������ ��ġ�� �ִ� ����� �ּҸ� ��´�.
	node_ext *GetTailNode();

	void CloneTipsListExt(TW_ListExt *parm_src_list, char parm_delete_src_list = 1);
	void CloneTipsListExt(TW_StockListExt *parm_src_list);

	// ���Ḯ��Ʈ�� ��ϵ� ����� ������ ��´�.
	int GetNodeCount();
};

class TW_StockListExt
{
private:
	// ���� ����Ʈ�� ó���� ���� ����Ű�� ������.
	node_ext *mp_head, *mp_tail;
	// ���� ����Ʈ�� ��ϵ� ����� ������ ������ �ִ� ����.
	int m_node_ext_count;

	// �����ؼ� �̸� �Ҵ��� ����� �ּ�, �Ҵ� ������ �ּ��� ��ġ
	node_ext *mp_fixed_node_memory, *mp_alloc_node_pos;
	// �̸� �Ҵ��� ����� �ִ� ����
	unsigned int m_max_fixed_node_count;

	// ����ڰ� ����ϴ� �޸𸮸� �����Ҷ� TW_StockListExt Ŭ������ ���� ȣ��� �Լ��� �ּҸ� ������ ������
	void(*mp_delete_user_data)(void *parm_object);

public:
	// �� ��ü�� ����� ����� �ִ� ������ ��� ������ ����� �Լ��� �����Ѵ�.
	// ( parm_delete_user_data �� ���ؼ��� TW_ListExt::TW_ListExt �� ���� )
	TW_StockListExt(unsigned int parm_max_fixed_node_count, void(*parm_delete_user_data)(void *parm_object) = NULL);
	// ��ü �ı���
	~TW_StockListExt();

	// ����ڰ� ����� �޸𸮸� �ı��Ҷ� ȣ���� �Լ��� ����
	void ChangeDeleteUserData(void(*parm_delete_user_data)(void *parm_object));
	// ���� ����Ʈ�� �ʱ�ȭ�ϴ� �Լ�
	void InitListObject();

	// ����Ʈ�� ���� ���ο� ��带 �߰��ϴ� �Լ�. ( TW_ListExt::AddObject ���ǻ��� ���� )
	void AddObject(void *parm_object);
	// ���ϴ� ��ġ�� ��带 �߰��ϴ� �Լ�, 0 �� ����ϸ� ����Ʈ�� ���� ù ��ġ�� ��尡 �߰��ȴ�.
	// �߰��� ��ġ�� �߸� �����ϸ� ��� �߰��� �����ϰ� -1 ���� ��ȯ�Ѵ�.
	char AddObjectByIndex(int parm_index, void *parm_object);
	node_ext *AddObjectAfterThisNode(node_ext *parm_this_node, void *parm_p_object);

	// [ ��带 �����ϴ� �Լ��� ]

	// DeleteNode, DeleteAllObject, DeleteNodeByIndex �Լ��� �ι�° ���ڷ� ���� parm_delete_object_flag ������
	// ��忡 ����� ����� �����͸� �����Ұ����� ���θ� �����ϳ���. 
	// ( 0 -> ��常 ����, 1 -> ���� ����� ������ ��� ����)

	// ����Ʈ�� ��ϵ� ���� �߿��� parm_node�� ������ ��带 ã�Ƽ� �ش� ��带 �����Ѵ�.
	char DeleteNode(node_ext *parm_node, char parm_delete_object_flag = 1);
	// ����Ʈ�� ��ϵ� ��� ��带 �����Ѵ�.
	void DeleteAllNode(char parm_delete_object_flag = 1);
	// ����ڰ� ������ ��ġ�� �ִ� ��带 ã�Ƽ� �ش� ��带 �����Ѵ�.
	char DeleteNodeByIndex(int parm_index, char parm_delete_object_flag = 1);

	// [ ��带 �˻��ϴ� �Լ��� ]

	// ���Ḯ��Ʈ���� ������ ��ġ�� �ִ� ����� �ּҸ� ��´�.
	node_ext *GetNodeByIndex(int parm_index);
	// ���Ḯ��Ʈ���� ������ ��ġ�� �ִ� ��尡 ����Ű�� �����͸� ��´�.
	void *GetObjectByIndex(int parm_index);
	// ���Ḯ��Ʈ�� ��ϵ� �����߿��� parm_object�� ��õ� �ּҿ� ������ �ּҸ� ���� �����Ͱ� �ִ���
	// ã�Ƽ� �� ��ġ�� ��ȯ�Ѵ�. ( -1 �̸� �˻��� �����ߴٴ� ���̴�. )
	int FindNodeIndexByObject(void *parm_object);

	// ���� ó�� ��ġ�� �ִ� ����� �ּҸ� ��´�.
	node_ext *GetHeadNode();
	// ������ ����� ������ �ִ� ��带 ��´�.
	node_ext *GetNextNode(node_ext *parm_current_node);
	// ���� ������ ��ġ�� �ִ� ����� �ּҸ� ��´�.
	node_ext *GetTailNode();

	// ���Ḯ��Ʈ�� ��ϵ� ����� ������ ��´�.
	int GetNodeCount();
};

class TW_Font_D2D
{
protected:
	IDWriteTextFormat * mp_text_format;
	TW_String m_font_name;
	FLOAT m_font_size;

public:
	TW_Font_D2D();
	virtual ~TW_Font_D2D();

	INT32 CreateFont(const wchar_t *ap_name, FLOAT a_size, DWRITE_FONT_WEIGHT a_weight = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE  a_style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH  a_stretch = DWRITE_FONT_STRETCH_NORMAL, UINT8 is_korean = 1);
	void DestroyFont();

	void SetAlignment(UINT8 a_type = ALIGNMENT_CENTER);
	IDWriteTextFormat *GetFormat();
	D2D1_SIZE_F GetTextSize(const wchar_t *ap_text);
};

#define MAX_STOCK_SOLID_BRUSH          100

class TW_StockSolidBrushManager
{
private:
	ID2D1SolidColorBrush * mp_brush[MAX_STOCK_SOLID_BRUSH];
	D2D1_COLOR_F m_brush_color[MAX_STOCK_SOLID_BRUSH];
	int m_brush_count;

public:
	TW_StockSolidBrushManager();
	~TW_StockSolidBrushManager();

	ID2D1SolidColorBrush *GetMyBrush(ID2D1RenderTarget *ap_render_target, D2D1_COLOR_F *ap_color);
	void DeleteAllBrush();
};

#define MAX_TOUCH_POINT     15
#define MAX_CONTROL_COUNT   64

struct TouchData
{
	UINT8 flag;
	UINT8 user_data_id;   // p_user_data ����ϴ� ��ü�� �����ϱ� ���� ����ϴ� ID
	POINT start_pos;
	POINT temp_pos;
	UINT32 start_time; // sec
	UINT32 start_tick; // m-sec
	UINT32 id;
	UINT8 *p_user_data;
};

class TW_Wnd;

#define TC_NOTIFY_MESSAGE_ID       m_control_index + 1000
#define TIMER_COUNT_PER_CONTROL    10
#define BASE_TIMER_ID_FOR_CONTROL  1000

#define TCS_FOCUS               0x00000001
#define TCS_EXTERNAL_FONT       0x00000002
#define TCS_PUSH                0x00000004
#define TCS_VISIBLE             0x00000008

class TW_Ctrl
{
protected:
	unsigned int m_state; // 0 bit -> Focus State(0x01), 1 bit -> use external font(0x02), 2 bit -> use Push State(0x04), 3  bit -> visible(0x08)
	TW_StockSolidBrushManager *mp_sb_man;
	TW_Wnd *mp_parent_wnd;
	TW_Font_D2D *mp_font;
	ID2D1BitmapRenderTarget *mp_target;
	D2D1_RECT_F m_ctrl_client_rect, m_ctrl_rect;
	int m_x, m_y, m_width, m_height;
	unsigned int m_control_index;
	UINT32 m_control_id;

public:
	TW_Ctrl(TW_Wnd *ap_parent_wnd, int a_x, int a_y, int a_width, int a_height);
	TW_Ctrl(TW_Wnd *ap_parent_wnd, int a_guide_ctrl_id);
	virtual ~TW_Ctrl();

	void InitObject(TW_Wnd *ap_parent_wnd, int a_x, int a_y, int a_width, int a_height);
	void SetControlIndex(unsigned int a_index);
	void SetTimer(unsigned int a_timer_id, unsigned int a_elapse);
	void KillTimer(unsigned int a_timer_id);
	virtual void CreateRenderTarget();
	void DestroyRenderTarget();
	virtual void ReloadResourceForRenderTarget();
	void SetVisible(char a_visible_flag = 1);

	inline int GetX() { return m_x; }
	inline int GetY() { return m_y; }
	inline int GetWidth() { return m_width; }
	inline int GetHeight() { return m_height; }
	inline ID2D1RenderTarget *GetRenderTarget() { return mp_target; }
	inline TW_Wnd *GetParent() { return mp_parent_wnd; }
	inline unsigned int GetVisible()
	{
		return m_state & TCS_VISIBLE;
	}

	inline char PtInRect(int a_x, int a_y)
	{
		return (a_x >= m_x && a_y >= m_y && a_x <= (m_x + m_width) && a_y <= (m_y + m_height));
	}

	inline char PtInClient(int a_x, int a_y)
	{
		return (a_x >= 0 && a_y >= 0 && a_x <= m_width && a_y <= m_height);
	}

	inline void ConvertClientPos(POINT *ap_pos)
	{
		ap_pos->x = ap_pos->x - m_x;
		ap_pos->y = ap_pos->y - m_y;
	}

	inline unsigned int GetControlIndex()
	{
		return m_control_index;
	}

	inline unsigned int GetID()
	{
		return m_control_id;
	}

	inline unsigned int GetControlState()
	{
		return m_state;
	}

	inline TW_Font_D2D *GetFont()
	{
		return mp_font;
	}

	// NULL �̸� gp_default_font ���
	void SetFont(TW_Font_D2D *ap_font = NULL, UINT8 a_auto_delete_flag = 0, UINT8 a_redraw_flag = 1);
	void SetFont(const wchar_t *ap_font_name, FLOAT a_size, UINT8 a_alignment_flag = ALIGNMENT_CENTER, UINT8 a_redraw_flag = 1);

	virtual void Draw(ID2D1RenderTarget *ap_target);
	void RedrawControl(char a_remake_flag = 1);
	virtual void SetPosControl(int a_x, int a_y, int a_width, int a_height, char a_remake_flag = 1);
	virtual void SetFocus(char a_make_draw_flag = 1);
	virtual void KillFocus(char a_make_draw_flag = 1);

	virtual int OnTouchAction_NewTouch(int a_touch_count, TouchData *ap_data, POINT a_pos);
	virtual int OnTouchAction_Release(int a_touch_count, TouchData *ap_data, POINT a_pos);
	virtual int OnTouchAction_Drag(int a_touch_count, TouchData *ap_data, POINT a_pos);

	inline int GetFocus()
	{
		return m_state & TCS_FOCUS;
	}

	void SetCapture();
	void ReleaseCapture();

	virtual void OnLButtonDown(int nFlags, POINT a_pos);
	virtual void OnLButtonUp(int nFlags, POINT a_pos);
	virtual void OnLButtonDblClk(int nFlags, POINT a_pos);
	virtual void OnRButtonDown(int nFlags, POINT a_pos);
	virtual void OnRButtonUp(int nFlags, POINT a_pos);
	virtual void OnMouseMove(int nFlags, POINT a_pos);
	virtual void OnTimer(unsigned int a_timer_id);
	virtual int OnMouseWheel(unsigned short nFlags, short zDelta, POINT a_pos);
	virtual int OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual int OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual void MakeDraw() { }
};

class TW_Button : public TW_Ctrl
{
protected:
	D2D1_COLOR_F m_border_color[2], m_text_color[2], m_bk_color[2];
	TW_String m_text;

public:
	TW_Button(TW_Wnd *ap_parent_wnd, const wchar_t *ap_text, int a_x, int a_y, int a_width, int a_height, unsigned char a_make_draw_flag = 1);
	TW_Button(TW_Wnd *ap_parent_wnd, const wchar_t *ap_text, int a_guide_ctrl_id, unsigned char a_make_draw_flag = 1);

	virtual ~TW_Button();

	void InitObject(TW_Wnd *ap_parent_wnd, const wchar_t *ap_text, unsigned char a_make_draw_flag);
	virtual void MakeDraw();
	virtual void OnLButtonDown(int nFlags, POINT a_pos);
	virtual void OnLButtonUp(int nFlags, POINT a_pos);
	virtual void OnMouseMove(int nFlags, POINT a_pos);

	void SetTitleString(const wchar_t *ap_title_string);
	TW_String *GetTitleString();

	void SetTextColor(D2D1_COLOR_F a_normal_color, D2D1_COLOR_F a_focusd_color);
	void SetBorderColor(D2D1_COLOR_F a_normal_color, D2D1_COLOR_F a_focusd_color);
	void SetBkColor(D2D1_COLOR_F a_normal_color, D2D1_COLOR_F a_focusd_color);
};

class TW_FrameText : public TW_Ctrl
{
protected:
	D2D1_COLOR_F m_border_color, m_text_color, m_bk_color;
	TW_String m_text;

public:
	TW_FrameText(TW_Wnd *ap_parent_wnd, const wchar_t *ap_text, int a_x, int a_y, int a_width, int a_height, UINT8 a_make_draw_flag = 1);
	TW_FrameText(TW_Wnd *ap_parent_wnd, const wchar_t *ap_text, int a_guide_ctrl_id, UINT8 a_make_draw_flag = 1);

	virtual ~TW_FrameText();

	void InitObject(TW_Wnd *ap_parent_wnd, const wchar_t *ap_text, UINT8 a_make_draw_flag);
	virtual void MakeDraw();

	void SetTitleString(const wchar_t *ap_title_string, UINT8 a_redraw_flag = 1);
	TW_String *GetTitleString();

	void SetIntValue(INT32 a_value, UINT8 a_redraw_flag = 1);
	INT32 GetIntValue();

	void SetDoubleValue(double a_value, UINT8 a_count = 2, UINT8 a_auto_remove = 1, UINT8 a_redraw_flag = 1);
	double GetDoubleValue();

	void SetTextColor(D2D1_COLOR_F a_color);
	void SetBorderColor(D2D1_COLOR_F a_color);
	void SetBkColor(D2D1_COLOR_F a_color);
};

class TW_SimpleScrollBar
{
protected:
	// ���콺 �̵��� ���� ��ǥ�� ����ϴ� ����
	int m_prev_pos;
	// Thumb Ŭ�� ��ġ�� ����ϴ� ����
	int m_thumb_click_pos;
	// ��ũ�� ��ǥ �󿡼� ���� ��ġ
	int m_cur_pos;

	// ������ �ּ�/�ִ� ���� �󿡼� ���� ��ġ
	int m_scroll_pos;
	// ������ �ּ�/�ִ� ��ġ
	int m_min_pos, m_max_pos;
	int m_display_range;

	// Thumb ����
	int m_thumb_length;
	// Wheel ���� �̵� ����
	int m_move_range_by_wheel;
	int m_x, m_y, m_width, m_height;

	char m_thumb_click_flag;
	RECT m_rect;

	D2D1_COLOR_F m_thumb_color[2];
	TW_StockSolidBrushManager *mp_sb_man;

public:
	TW_SimpleScrollBar(TW_StockSolidBrushManager *ap_sb_man, int a_x, int a_y, int a_width, int a_height);
	virtual ~TW_SimpleScrollBar();

	virtual void DrawBar(ID2D1RenderTarget *ap_target);
	int CheckInPos(POINT a_pos);

	void SetRange(int a_min_pos, int a_max_pos, int a_display_range);
	inline int GetMax()
	{
		return m_max_pos;
	}

	inline int GetMin()
	{
		return m_min_pos;
	}

	void SetScrollPos(int a_scroll_pos);
	int GetScrollPos();

	void SetDisplayRange(int a_display_range);
	int GetDisplayRange();

	void SetPos(int a_x, int a_y);
	void SetSize(int a_width, int a_height);
	inline int GetWidth()
	{
		return m_width;
	}

	inline int GetHeight()
	{
		return m_height;
	}

	void SetThumbColor(D2D1_COLOR_F m_normal_thumb_color, D2D1_COLOR_F m_focus_thumb_color);
	void SetMoveRangeByWheel(int a_move_range);

	int OnClickDown(POINT a_pos);
	int OnClickUp(POINT a_pos);
	int OnMove(POINT a_pos);
	void OnWheel(short int a_z_delta);
};

#define TLBE_LBTN_SELCHANGE      29501
#define TLBE_LBTN_DBLCLK         29502
#define TLBE_RBTN_UP             29503

class TW_ListBox;

typedef void(*TW_LISTBOX_DISPLAY_ITEM)(TW_ListBox *ap_list_box, void *ap_data, INT32 a_index, RECT a_rect, ID2D1RenderTarget *ap_render_target, TW_StockSolidBrushManager *ap_sb_man, TW_Font_D2D *ap_font, UINT8 a_selected_flag);
typedef void(*TW_LISTBOX_BK_DISPLAY_TYPE)(TW_ListBox *ap_list_box, ID2D1RenderTarget *ap_render_target, TW_StockSolidBrushManager *ap_sb_man, INT32 a_display_width, INT32 a_display_height);

class TW_ListBox : public TW_Ctrl
{
protected:
	// ��Ʈ�� �ڵ����� ���� ( �ܺο��� ���������� ����ϴ� ��Ʈ�� ChangeFont �� �����Ҷ��� 0 ���� ���� )
	UINT8 m_auto_delete_font_flag;
	// ���콺 Ŭ�� ���� ( 0 -> Ŭ�� �ʵ�, 1 -> ���� ��ư Ŭ��, 2 -> ������ ��ư Ŭ�� )
	UINT8 m_mouse_click_flag;
	// ����Ʈ�ڽ��� ��, ����, ���õ� �׸��� ��ġ, �� �׸�� ����, ȭ�鿡 ��°����� �׸��� �ִ� ����
	INT32 m_prev_cursor_index, m_cursor_index, m_item_height, m_max_display_item_count;

	// ����Ʈ �ڽ��� �߰��� �׸���� ������ ���� ����Ʈ ��ü
	TW_ListExt m_data_list;
	TW_SimpleScrollBar *mp_simple_bar;

	// ������ �ٸ� ����Ʈ�ڽ����� ����ϴ� CWnd *parm_base_wnd ���ڴ� parm_this->GetParent() �� ��ü �����ؼ� ������
	TW_LISTBOX_DISPLAY_ITEM mfp_display_item;
	TW_LISTBOX_BK_DISPLAY_TYPE mfp_bk_display_type;

public:
	TW_ListBox(TW_Wnd *ap_parent_wnd, INT32 a_guide_ctrl_id, void(*ap_delete_user_data)(void *ap_object), INT32 a_scroll_bar_width = 10);
	TW_ListBox(TW_Wnd *ap_parent_wnd, INT32 a_x, INT32 a_y, INT32 a_width, INT32 a_height, void(*ap_delete_user_data)(void *ap_object), INT32 a_scroll_bar_width = 10);
	virtual ~TW_ListBox();

	void InitObject(void(*ap_delete_user_data)(void *ap_object), INT32 a_scroll_bar_width);

	int AddData(void *ap_data);
	int InsertData(INT32 a_index, void *ap_data);
	void DeleteData(INT32 a_index, UINT8 a_delete_object_flag = 1);
	void ResetContent(UINT8 a_delete_object_flag = 1);

	void SetCurSel(INT32 a_index);
	int GetCurSel();

	void SetItemHeight(INT32 a_height);
	void SetLastDataSel();

	TW_ListExt *GetDataList();
	INT32 GetCount();

	void ChangeItemDisplay(TW_LISTBOX_DISPLAY_ITEM afp_display_item);
	void ChangeBkDisplay(TW_LISTBOX_BK_DISPLAY_TYPE afp_bk_display_type);

	void ChangeDeleteUserData(void(*ap_delete_user_data)(void *ap_object));
	void UpdateScrollState();

	void MakeDraw();

	virtual void OnLButtonDown(int nFlags, POINT a_pos);
	virtual void OnLButtonUp(int nFlags, POINT a_pos);
	virtual void OnLButtonDblClk(int nFlags, POINT a_pos);
	virtual void OnMouseMove(int nFlags, POINT a_pos);
	virtual int OnMouseWheel(unsigned short nFlags, short zDelta, POINT a_pos);

	virtual int OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual int OnTouchAction_NewTouch(int a_touch_count, TouchData *ap_data, POINT a_pos);
	virtual int OnTouchAction_Release(int a_touch_count, TouchData *ap_data, POINT a_pos);
	virtual int OnTouchAction_Drag(int a_touch_count, TouchData *ap_data, POINT a_pos);

	virtual char OnUpArrowKey(char a_notify_flag = 1);
	virtual char OnDownArrowKey(char a_notify_flag = 1);

	virtual void SetPosControl(INT32 a_x, INT32 a_y, INT32 a_width, INT32 a_height, UINT8 a_remake_flag = 1);
};

class TW_CtrlWnd
{
protected:
	char m_attached_flag;  // ��Ʈ�� ���� ��������
	HWND mh_wnd;           // ��Ʈ���� ������ �ڵ� ��
	COLORREF m_fill_color, m_text_color; // ��Ʈ���� ������ ���ڻ�
	COLORREF m_in_border_color, m_out_border_color; // ��Ʈ���� �׵θ� ��
	UINT m_control_id;  // ��Ʈ���� ID ��
	HFONT mh_font;  // ��Ʈ�ѿ� ����� �۲� �ڵ� ���� ������ ����

public:
	TW_CtrlWnd();           // ��ü ������
	virtual ~TW_CtrlWnd();  // ��ü �ı���

	inline HWND GetHandle() { return mh_wnd; }
	// �ܺο��� ������� ��Ʈ���� �� Ŭ������ �����ϴ� ��쿡 ���
	void Attach(HWND ah_attach_wnd);  
	// ��Ʈ���� ������ ������ �� ����ϴ� �Լ�
	void ChangeCtrlColor(COLORREF a_fill_color, COLORREF a_out_border_color, 
						 COLORREF a_in_border_color, COLORREF a_text_color);
	// ��Ʈ���� Owner Draw �Ӽ��� True�� ��쿡 ó���ϴ� �Լ�
	virtual void OnDrawItem(DRAWITEMSTRUCT *ap_dis);
	virtual INT32 OnCtrlColor(HDC ah_dc, HWND ah_wnd);

	// ��Ʈ���� �ٽ� �׸� �ʿ䰡 ������ ȣ���ϴ� �Լ�
	void Invalidate();
	// ��Ʈ�ѿ� �۲��� �����Ѵ�. a_auto_delete�� 1�� �Ѱ��ָ� �� �ڵ� ����
	// mh_font�� �����ߴٰ� ��Ʈ���� ���ŵɶ� �۲õ� ���� �����մϴ�.
	void SetFont(HFONT ah_font, UINT32 a_auto_delete = 1);
};

class TW_ButtonWnd : public TW_CtrlWnd
{
public:
	TW_ButtonWnd();   // ��ü ������
	~TW_ButtonWnd();  // ��ü �ı���

	HWND Create(HWND ah_parent_wnd, const wchar_t *ap_title_name, INT32 a_sx, INT32 a_sy, INT32 a_ex, INT32 a_ey, INT32 a_ctrl_id, UINT32 a_style = 0);
	// ��Ʈ���� Owner Draw �Ӽ��� True�� ��쿡 ó���ϴ� �Լ�
	// ��ư�� �׸��� ���� ��������
	virtual void OnDrawItem(DRAWITEMSTRUCT *ap_dis);
};

#define EN_ENTER_KEYUP          1000

// Edit ��Ʈ�ѿ� ���Ӱ� ������ ������ ���ν���
LRESULT APIENTRY TW_EditProcForEnterKeyUp(HWND ah_wnd, UINT message_id, WPARAM wParam, LPARAM lParam);

class TW_EditWnd : public TW_CtrlWnd
{
protected:
	// Edit ��Ʈ���� ���� ����ϴ� ���ν����� �ּҸ� �����ϱ� ���� �Լ��� ������
	WNDPROC m_org_edit_proc;

public:
	TW_EditWnd();  // ��ü ������
	virtual ~TW_EditWnd(); // ��ü �ı���

    // �⺻ �Ӽ����� Edit ��Ʈ���� �����ϴ� �Լ�
	HWND Create(HWND ah_parent_wnd, INT32 a_sx, INT32 a_sy, INT32 a_ex, INT32 a_ey, INT32 a_ctrl_id);
	HWND CreateUserStyle(HWND ah_parent_wnd, INT32 a_sx, INT32 a_sy, INT32 a_ex, INT32 a_ey, INT32 a_ctrl_id, UINT32 a_style = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL);
	// ����Ŭ������ �ϴ� �Լ�. �⺻���� �����Ǵ� TW_EditProcForEnterKeyUp ���������� 
	// ����Ű�� üũ�ؼ� EN_ENTER_KEYUP �޽����� �����ϴ� ����� �߰��Ǿ� �ִ�.
	void SubclassWnd(WNDPROC a_new_proc = TW_EditProcForEnterKeyUp);
	// WM_CTRLCOLOREDIT �޽������� ����ϴ� �Լ�
	virtual INT32 OnCtrlColor(HDC ah_dc, HWND ah_wnd);
	// Edit ��Ʈ�ѿ��� ���ڿ��� ���̸� ��� �Լ�
	UINT32 GetTextLen();
	// Edit ��Ʈ�ѿ��� ���ڿ��� ��� �Լ�
	UINT32 GetText(TW_String &r_str);
	// Edit ��Ʈ�ѿ��� ���ڿ��� ��� �Լ�
	UINT32 GetText(wchar_t *ap_string, UINT32 a_max_length);
	// Edit ��Ʈ�ѿ� ���ڿ��� �����ϴ� �Լ�
	void SetText(const wchar_t *ap_string);
};

class TW_ListBoxWnd : public TW_CtrlWnd
{
public:
	TW_ListBoxWnd(); // ��ü ������
	virtual ~TW_ListBoxWnd(); // ��ü �ı���

	// �⺻ �Ӽ����� ListBox�� �����ϴ� �Լ�
	HWND Create(HWND ah_parent_wnd, INT32 a_sx, INT32 a_sy, INT32 a_ex, INT32 a_ey, INT32 a_ctrl_id, UINT32 a_style = LBS_STANDARD);
	// Ȯ�� �Ӽ��� ����ؼ� ListBox�� �����ϴ� �Լ�
	HWND CreateEx(HWND ah_parent_wnd, INT32 a_sx, INT32 a_sy, INT32 a_ex, INT32 a_ey, INT32 a_ctrl_id, UINT32 a_style = 0, UINT32 a_ex_style = WS_EX_STATICEDGE);
	// ���ڿ��� ListBox�� �߰��ϴ� �Լ�, LBS_SORT �ɼ��� ������ �ִٸ� ������������ �����ؼ� �߰��Ѵ�.
	INT32 AddString(const wchar_t *ap_string, UINT8 a_auto_select = 1);
	// ���ڿ��� ����ڰ� ������ ��ġ�� �߰��ϴ� �Լ�
	INT32 InsertString(INT32 a_index, const wchar_t *ap_string, UINT8 a_auto_select = 1);
	// ����ڰ� ������ �׸� �߰��� ���ڿ��� ���̸� ��´�.
	INT32 GetTextLength(INT32 a_index);
	// ����ڰ� ������ �׸񿡼� ���ڿ��� �����ؼ� �����´�.
	INT32 GetText(INT32 a_index, wchar_t *ap_string, int a_max_len);
	// ����ڰ� ������ �׸񿡼� ���ڿ��� �����ؼ� �����´�. 
	INT32 GetText(INT32 a_index, TW_String &r_str);
	// ListBox�� Ư�� �׸��� �����Ѵ�.
	void SetCurSel(INT32 a_index);
	// ListBox���� � �׸��� ���õǾ� �ִ��� ��ġ ���� ��´�.
	INT32 GetCurSel();
	// ListBox�� �߰��� �׸��� ������ ��´�.
	INT32 GetCount();
	// ListBox�� �߰��� ��� �׸��� �����Ѵ�.
	void ResetContent();
	// ������ �׸� �߰� 4����Ʈ ������ �����Ѵ�. (������ ����)
	void SetItemDataPtr(INT32 a_index, void *ap_data);
	// ������ ��� InsertString�� ����ϰ� SetItemDataPtr �Լ��� ����ؾ� ������
	// �� �Լ��� ����ϸ� �� �Լ��� ����� ���ÿ� �����մϴ�.  (������ ����)
	void SetItemDataPtrEx(INT32 a_index, const wchar_t *ap_string, void *ap_data, UINT8 a_auto_select = 1);
	// �׸� �߰��� 4����Ʈ �޸� ���� ��´�. (������ ����)
	void *GetItemDataPtr(INT32 a_index);
	// ������ �׸� �߰� 4����Ʈ ������ �����Ѵ�. (���� ����)
	void SetItemData(INT32 a_index, INT32 a_data);
	// ������ ��� InsertString�� ����ϰ� SetItemDataPtr �Լ��� ����ؾ� ������
	// �� �Լ��� ����ϸ� �� �Լ��� ����� ���ÿ� �����մϴ�.  (���� ����)
	void SetItemDataEx(INT32 a_index, const wchar_t *ap_string, INT32 a_data);
	// �׸� �߰��� 4����Ʈ �޸� ���� ��´�. (���� ����)
	INT32 GetItemData(INT32 a_index);
	// ListBox�� �߰��� ���ڿ����� ���ϴ� ���ڿ��� �˻��Ѵ�. �� �Լ��� ����ϸ�
	// �ڽ��� ã�� ���� ���ڿ��� 'abc'�� ��쿡 'abc'�Ӹ� �ƴ϶� 'abcdef'�� ����
	// �κ������� ��ġ�ϴ� ���ڿ��� ã�� �� �ֽ��ϴ�.
	INT32 FindString(INT32 a_index, const wchar_t *ap_string);
	// ListBox�� �߰��� ���ڿ����� ���ϴ� ���ڿ��� �˻��Ѵ�. �� �Լ��� ��Ȯ�ϰ�
	// ��ġ�ϴ� ���ڿ��� �˻��մϴ�.
	INT32 FindStringExact(INT32 a_index, const wchar_t *ap_string);

	virtual void OnDrawItem(DRAWITEMSTRUCT *ap_dis);
	virtual INT32 OnCtrlColor(HDC ah_dc, HWND ah_wnd);
};

class TW_Wnd
{
protected:
	// �� ��ü�� ������ �������� �ڵ�
	HWND mh_wnd;
	// ĸ���� ��� ����, ĸ���� ���� ��, ���� ��� ��� ����
	UINT8 m_use_caption, m_caption_height, m_use_notice_draw;
	// �ִ�ȭ ��ư ��뿩��, ���� �������� �ִ�ȭ ���� ����, �������� ũ�� ���� ���� ����
	UINT8 m_max_flag, m_is_maximized, m_resize_flag;
	// �ּ�ȭ ��ư ��뿩��, �ݱ� ��ư ��뿩��
	UINT8 m_min_flag, m_close_flag;
	// �������� �ּ� ũ�⸦ ���� (��, ����), ���� �������� ���� ����
	INT32 m_limit_x_size, m_limit_y_size, m_width, m_height;
	// ĸ���� Ŭ���Ǿ���������, ���� ���°� Ȱ��ȭ�� �������� ����, ��ü������ ������ �������� ������� ����
	UINT8 m_is_clicked, m_is_active_state, m_is_body_move, m_use_active_body_color;
	// �����츦 ���콺�� Ŭ���ؼ� �����̴µ� ����� ���� (���������� ���)
	POINT m_original_pos, m_clicked_pos;
	// �ý��� ��ư(�ּ�ȭ, �ִ�ȭ, �����ư)���� ���� ��
	INT32 m_sys_btn_margin, m_sys_btn_width, m_sys_btn_top, m_sys_btn_bottom;

	// Direct2D���� �������� Ŭ���̾�Ʈ ������ �׸��� �׸� ��ü
	ID2D1RenderTarget *mp_target;
	// Windows 8.1 �̻󿡼� ��� ������ �׸� �׸��� ��ɵ�!
	ID2D1Device *mp_device;
	ID2D1DeviceContext *mp_dc;
	ID2D1Bitmap1 *mp_dc_bitmap;
	IDXGISwapChain1 *mp_sc;
	DXGI_PRESENT_PARAMETERS m_sc_parameters;
		
	TW_StockSolidBrushManager *mp_sb_man;  // Brush ��ü�� ������ ��ü
	HCURSOR mh_wnd_cursor;  // ���� �������� ���콺 Ŀ��
	TW_String m_title_name;
	RECT m_client_rect;
	TW_Font_D2D *mp_font;  // �⺻������ gp_default_font �� ����ϰ� ����!

	// �����츦 �����ϴ� ��ҵ��� ������ ��� �Ϲ� ���¿� Ȱ��ȭ ������ �� ���� �������� ������.
	// 0�� �׸��� �Ϲ� �����̰� 1�� �׸��� Ȱ��ȭ �����̴�.
	// �׵θ� ����, ĸ�� ��� ����, Ŭ���̾�Ʈ ���� ����, Ÿ��Ʋ ���� ����, �ý��� ��ư ����
	D2D1_COLOR_F m_border_color[2], m_caption_color[2], m_body_bk_color[2], m_title_color[2], m_system_btn_color[2];

	// ��ġ ���� ����
	TouchData m_touch_data[MAX_TOUCH_COUNT];
	INT32 m_touch_count;
	UINT32 m_last_touch_id;

	// �� ��Ʈ�� ���� ����
	TW_Ctrl *mp_control_list[MAX_CONTROL_COUNT];
	UINT32 m_focused_index, m_captured_index, m_control_count;

public:
	TW_Wnd();  // ��ü ������
	virtual ~TW_Wnd();  // ��ü �ı���

    // ��ü�� �����ϴ� mh_wnd ���� ��ų� ���ο� �ڵ� ���� �����ϴ� �Լ�
	inline HWND GetHandle() { return mh_wnd; }
	inline void SetHandle(HWND ah_wnd) { mh_wnd = ah_wnd; }

	inline INT32 GetTouchCount() { return m_touch_count; }
	inline TouchData *GetTouchData() { return m_touch_data;	}
	INT32 GetTouchIndex(DWORD a_id);

	// �����츦 ������ ������� �ʰ� ��� �����ϵ��� �ϴ� �Լ�
	inline INT32 UpdateWindow() { return ::UpdateWindow(mh_wnd); }
	// �����츦 ȭ�鿡 ��� ��½�ų �������� �����ϴ� �Լ�
	inline INT32 ShowWindow(INT32 a_show_type = SW_SHOW) { return ::ShowWindow(mh_wnd, a_show_type); }
	// ȭ�� ��ǥ�踦 Ŭ���̾�Ʈ ��ǥ��� ��ȯ�ϴ� �Լ�
	void ScreenToClient(RECT *ap_rect);
	// �����츦 �ı��� �� ����ϴ� �Լ�
	inline void DestoryWindow() { if (mh_wnd != NULL) ::DestroyWindow(mh_wnd); }
	// ������ ȭ���� ��ȿȭ ���Ѽ� WM_PAINT �޽����� �߻���Ű�� �Լ�
	inline void Invalidate() { ::InvalidateRect(mh_wnd, NULL, FALSE); }
	inline void InvalidateRect(RECT *ap_rect){ ::InvalidateRect(mh_wnd, ap_rect, FALSE); }
	void GetDlgItemRect(INT32 a_ctrl_rect, RECT *ap_rect);
	// �������� Ŭ���̾�Ʈ ������ �������� ������ ũ�⸦ �����ϴ� �Լ�
	void ResizeWindow(int a_width, int a_height);
	// �ڽ��� �����츦 Ÿ�������� �����մϴ�.
	void ChangeEllipticWnd(int a_x_radius, int a_y_radius);
	void GetClientRect(D2D1_RECT_F *ap_rect);

	void CreateRenderTarget();
	void DestroyRenderTarget();
	virtual void ReloadResourceForRenderTarget();

	inline ID2D1RenderTarget *GetRenderTarget() { return mp_target; }
	inline TW_StockSolidBrushManager *GetSbMan() { return mp_sb_man; }
	inline void BeginDraw() { mp_target->BeginDraw(); }
	inline void EndDraw()
	{
		if (mp_target->EndDraw() == D2DERR_RECREATE_TARGET) ReloadResourceForRenderTarget();
		if (mp_sc != NULL) mp_sc->Present1(1, 0, &m_sc_parameters);
	}
	ID2D1SolidColorBrush *GetMyBrush(float a_red, float a_green, float a_blue);
	virtual void DrawCloseSystemBtn(UINT8 a_push_flag = 0);
	virtual void DrawMaxSystemBtn(UINT8 a_push_flag = 0);
	virtual void DrawMinSystemBtn(UINT8 a_push_flag = 0);
	virtual void CheckCaption(INT32 nFlags, POINT a_pos);
	virtual void DrawFrame();

	// �����츦 ������ �� ����ϴ� �Լ� (�ֻ��� ������ : WS_EX_TOPMOST, Drag&Drop : WS_EX_ACCEPTFILES) 
	void Create(const wchar_t *ap_title_name, INT32 a_x = 50, INT32 a_y = 50,
		INT32 a_width = 500, INT32 a_height = 300, DWORD a_style = WS_VISIBLE | WS_POPUP, DWORD a_ext_style = 0);

	void StartMoveWnd();
	void StartResizeWnd();
	void EndResizeWnd();

	void SetControlFocus(UINT32 a_index = T_INVALID_INDEX);
	void SetControlCapture(UINT32 a_index = T_INVALID_INDEX);
	UINT32 AddControl(TW_Ctrl *ap_control);

	virtual void OnNoticeDraw() { }
	virtual void OnDraw() { }

	// Ÿ�̸Ӹ� ����ϴ� �Լ�
	void SetTimer(UINT a_timer_id, UINT a_elapse_time);
	// Ÿ�̸Ӹ� �����ϴ� �Լ�
	void KillTimer(UINT a_timer_id);

	// WM_ACTIVATE �޽����� �߻��� �� �۾��� �Լ�.
	virtual void OnActivate(INT32 a_activate_flag);
	// WM_PAINT �޽����� �߻��� �� �۾��� �Լ�. �� �Լ��� �Ļ� Ŭ�������� ��κ� ������ ��
	virtual void OnPaint();
	// WM_CREATE �޽����� �߻��� �� �۾��� �Լ�.
	virtual int OnCreate(CREATESTRUCT *ap_create_info);
	// WM_TIMER �޽����� �߻��� �� �۾��� �Լ�.
	virtual void OnTimer(UINT a_timer_id) {  }
	virtual void OnControlTimer(UINT a_timer_id);
	// WM_COMMAND �޽����� �߻��� �� �۾��� �Լ�.
	virtual void OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, HWND ah_ctrl) { }
	virtual int OnPrevCommand(WPARAM wParam, LPARAM lParam) { return 0; }

	// WM_DESTROY �޽����� �߻��� �� �۾��� �Լ�.
	virtual void OnDestroy();
	// WM_CLOSE �޽����� �߻��� �� �۾��� �Լ�.
	virtual UINT32 OnClose();
	// WM_KEYDOWN �޽����� �߻��� �� �۾��� �Լ�.
	virtual int OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	// WM_KEYUP �޽����� �߻��� �� �۾��� �Լ�.
	virtual int OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	// WM_LBUTTONDOWN �޽����� �߻��� �� �۾��� �Լ�.
	virtual void OnLButtonDown(int a_flag, POINT a_pos);
	// WM_LBUTTONUP �޽����� �߻��� �� �۾��� �Լ�.
	virtual void OnLButtonUp(int a_flag, POINT a_pos);
	// WM_MOUSEMOVE �޽����� �߻��� �� �۾��� �Լ�.
	virtual void OnMouseMove(int a_flag, POINT a_pos);
	// WM_LBUTTONDBLCLK �޽����� �߻��� �� �۾��� �Լ�.
	virtual void OnLButtonDblClk(int a_flag, POINT a_pos);
	// WM_RBUTTONDOWN �޽����� �߻��� �� �۾��� �Լ�.
	virtual void OnRButtonDown(int a_flag, POINT a_pos);
	// WM_RBUTTONUP �޽����� �߻��� �� �۾��� �Լ�.
	virtual void OnRButtonUp(int a_flag, POINT a_pos);
	// WM_MOUSEWHEEL �޽����� �߻��� �� �۾��� �Լ�.
	virtual int OnMouseWheel(unsigned short a_flag, short a_z_delta, POINT a_pos);
	// WM_DROPFILES �޽����� �߻��� �� �۾��� �Լ�.
	virtual void OnDropFiles(HANDLE ah_drop_info) { }
	// WM_CTLCOLORLISTBOX �޽����� �߻��� �� �۾��� �Լ�.
	virtual int OnCtrlColorListBox(HDC ah_dc, HWND ah_wnd){ return 0; }
	// WM_CTLCOLOREDIT �޽����� �߻��� �� �۾��� �Լ�.
	virtual int OnCtrlColorEdit(HDC ah_dc, HWND ah_wnd) { return 0; }
	// WM_CTLCOLORSTATIC �޽����� �߻��� �� �۾��� �Լ�.
	virtual int OnCtrlColorStatic(HDC ah_dc, HWND ah_wnd) { return 0; }
	// WM_DRAWITEM �޽����� �߻��� �� �۾��� �Լ�. (�ڽ��� ó�������� 1��ȯ)
	virtual INT32 OnDrawItem(INT32 a_ctrl_id, DRAWITEMSTRUCT *ap_dis) { return 0; }
	// WM_MEASUREITEM �޽����� �߻��� �� �۾��� �Լ�. (�ڽ��� ó�������� 1��ȯ)
	// a_is_ctrl : 0 -> Menu, 1 -> ListBox & ComboBox
	virtual INT32 OnMeasureItem(UINT32 a_is_ctrl, MEASUREITEMSTRUCT *ap_mis) { return 0; }
	// WM_COPYDATA �޽����� �߻��� �� �۾��� �Լ�. (�ڽ��� ó�������� 1��ȯ)
	virtual INT32 OnCopyData(HWND ah_send_wnd, COPYDATASTRUCT *ap_copy_data) { return 0; }
	// WM_SETCURSOR �޽����� �߻��� �� �۾��� �Լ�.
	virtual INT32 OnSetCursor(WPARAM wParam, LPARAM lParam);
	// WM_SIZE �޽����� �߻��� �� �۾��� �Լ�.
	virtual void OnSize(UINT32 a_flags, INT32 a_width, INT32 a_height);
	// WM_TOUCH �޽����� �߻��� �� �۾��� �Լ�.
	virtual LRESULT OnTouch(WPARAM wParam, LPARAM lParam);
	// WM_GETMINMAXINFO �޽����� �߻��� �� �۾��� �Լ�.
	virtual void OnGetMinMaxInfo(MINMAXINFO *ap_info);
	// TW_Wnd Ŭ������ �������� ���� ������ �޽����� ó���ϴ� ��쿡 ����ϴ� �Լ�
	virtual LRESULT UserMessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam) { return 0;  }
};

struct DialogItemType
{
	DLGITEMTEMPLATE ctl_style;
	wchar_t *p_ctrl_text;
	UINT32 ctrl_text_length;  // \0 ���� ������ ũ��. "abc" �̸� 4�� �����
	UINT32 ctrl_type; // Button 0x0080, Edit 0x0081, static 0x0082, ...
};

DialogItemType *TWAPI_MakeItemType(UINT32 ap_ctrl_type, const wchar_t *ap_title_name, INT32 a_ctrl_id, INT32 a_style, INT32 a_x, INT32 a_y, INT32 a_cx, INT32 a_cy, INT32 a_ex_style);
HLOCAL TWAPI_MakeTemplateMemory(TW_ListExt *ap_item_list, const wchar_t *ap_caption_name, const wchar_t *ap_font_name, UINT32 a_font_size, DLGTEMPLATE *ap_dlg_style);
void TWAPI_DestroyDialogItemType(void *ap_object);

class TW_Dialog : public TW_Wnd
{
protected:
	HWND mh_notify_wnd;
	int m_resource_id;
	WNDPROC mfp_dialog_proc;

public:
	TW_Dialog(HWND ah_notify_wnd, int a_resource_id);
	virtual ~TW_Dialog();

	void CenterWindow();
	inline void SetDlgProc(WNDPROC afp_proc) { mfp_dialog_proc = afp_proc; }
	inline WNDPROC GetDlgProc() { return mfp_dialog_proc;  }

	virtual int OnInitDialog();
	virtual int DoModal();

	virtual void OnOk();
	virtual void OnCancel();
};

class TW_WinApp
{
protected:
	TW_Wnd *mp_wnd;    // ���� ������ ��ü�� �ּҸ� ����
	HINSTANCE mh_instance;  // WinMain�� ���޵Ǵ� ù ��° ���ڰ� ����
	wchar_t m_class_name[32]; // '������ Ŭ����' �̸��� ������ ����
	// WinMain �Լ��� ���� ��ȯ���� WM_QUIT�޽����� wParam ������ ����ϱ�
	// ���ؼ� �� ���� ������ ����
	int m_exit_state;

public:
	TW_WinApp(HINSTANCE ah_instance, const wchar_t *ap_class_name, int a_use_ext_direct2d = 0);
	virtual ~TW_WinApp();

	int GetExitState() { return m_exit_state; }
	const wchar_t *GetWindowClassName() { return m_class_name; }
	TW_Wnd *GetMainWindow() { return mp_wnd; }
	HINSTANCE GetInstanceHandle() { return mh_instance; }
	
	virtual void InitApplication();
	virtual void InitInstance();
	virtual void Run();
	virtual void ExitInstance();
	virtual void ExitApplication();

	virtual int NormalProcess();
};

extern TW_WinApp *gp_app;
extern TW_Font_D2D *gp_default_font;
extern TW_Font_D2D *gp_control_font;
extern HFONT gh_default_gdi_font;

void TWAPI_CreateWIC();
int TWAPI_MakeD2D1_Bitmap(IWICBitmapFrameDecode *ap_image_frame, ID2D1RenderTarget *ap_target, ID2D1Bitmap **ap_bitmap);
ID2D1Bitmap *TWAPI_CreateD2D1Bitmap(ID2D1RenderTarget *ap_target, HBITMAP ah_bitmap, D2D1_RECT_F *ap_rect, UINT8 a_remove_color_flag = 0, COLORREF a_remove_color = 0);
ID2D1Bitmap *TWAPI_CreateD2D1Bitmap(ID2D1RenderTarget *ap_target, UINT32 *ap_pattern_data, UINT32 a_pattern_width, UINT32 a_pattern_height, UINT8 a_remove_color_flag = 0, COLORREF a_remove_color = 0);
ID2D1Bitmap *TWAPI_CreateD2D1BitmapFromRC(ID2D1RenderTarget *ap_target, INT32 a_resource_id, D2D1_RECT_F *ap_rect, UINT8 a_remove_color_flag = 0, COLORREF a_remove_color = 0);
void TWAPI_MoveRect(D2D1_RECT_F *ap_rect, float a_move_x, float a_move_y);

int TWAPI_LoadImage(ID2D1RenderTarget *ap_target, ID2D1Bitmap **ap_bitmap, const wchar_t *ap_path);
ID2D1Bitmap *TWAPI_LoadImageFromMemory(ID2D1RenderTarget *ap_render_target, void *ap_encoding_image_data, UINT32 a_image_size);
void TWAPI_SaveImage(const wchar_t *ap_file_path, HBITMAP ah_src_bitmap, INT32 a_image_type = TW_IMAGE_PNG);
INT32 TWAPI_SaveImageToMemory(UINT8 *ap_image_data, UINT32 a_image_size, HBITMAP ah_src_bitmap, INT32 a_image_type = TW_IMAGE_PNG);

void TWAPI_CreateExtD2D(HWND ah_wnd, ID2D1Device **ap_graphic_device, ID2D1DeviceContext **ap_device_context, ID2D1Bitmap1 **ap_render_bitmap, IDXGISwapChain1 **ap_swap_chain);
ID2D1RadialGradientBrush *CreateRadialGradientBrush(ID2D1RenderTarget *ap_target, D2D1_RECT_F *ap_rect, D2D1_GRADIENT_STOP *ap_stop_list, UINT32 a_stop_count, D2D1_GAMMA a_gamma = D2D1_GAMMA_2_2, D2D1_EXTEND_MODE a_mode = D2D1_EXTEND_MODE_CLAMP);

// ���ڿ����� ���ϴ� ������ ��ġ(�ּ�)�� ã�´�.
const wchar_t *TWAPI_FindChar(const wchar_t *ap_src, wchar_t a_find_char);
// ���ڿ����� �������� ��ġ�� �� ������ ��ġ(�ּ�)�� ã�´�.
const wchar_t *TWAPI_FindTwoChar(const wchar_t *ap_src, wchar_t a_find_char, wchar_t a_find_next_char);
// ���ڿ����� ���ϴ� ���ڸ� �����ϴ� ��ġ(�ּ�)�� ã�´�.
const wchar_t *TWAPI_FindUniqueChar(const wchar_t *ap_src, wchar_t a_find_char, wchar_t a_except_next_char);

// ���ڿ��� �����ϴ� �Լ�. �� �Լ��� ��ȯ ���� ������ ������ �����̴�.
UINT32 TWAPI_CopyString(wchar_t *ap_dest, const wchar_t *ap_src);
// ���ڿ��� �����ϴ� �Լ�. �� �Լ��� '\0'�� �ƴϴ��� a_limit_char�� ������ 
// ���ڸ� ������ ���縦 �����Ѵ�. �� �Լ��� ��ȯ ���� ������ ������ �����̴�.
UINT32 TWAPI_CopyString(wchar_t *ap_dest, const wchar_t *ap_src, wchar_t a_limit_char);

#endif