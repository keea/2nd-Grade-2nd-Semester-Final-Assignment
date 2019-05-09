#ifndef _TWAPI_W01_GDI_H_
#define _TWAPI_W01_GDI_H_

// Brush ��ü�� �ִ� 100�������� �����Ѵ�.
#define MAX_STOCK_BRUSH          100

class TW_BrushManager
{
private:
	// ������ Brush ��ü�� �ڵ� ���
	HBRUSH mh_brush[MAX_STOCK_BRUSH];
	// ������ Brush ��ü�� ���� ���
	COLORREF m_brush_color[MAX_STOCK_BRUSH];
	// ���� ����ϰ� �ִ� Brush ��ü�� ��
	int m_brush_count;

public:
	TW_BrushManager();
	~TW_BrushManager();

	// �����ϴ� ��ü���� a_color�� ������� Brush ��ü�� �ִٸ� �ش� ��ü�� �ڵ��� 
	// ��ȯ�ϰ� ������ Brush ��ü�� �����ϰ� ������ Brush ��ü�� �ڵ鰪�� ��ȯ�Ѵ�.
	HBRUSH GetBrush(COLORREF a_color);
	// �����ϴ� ��� Brush ��ü�� �����Ѵ�.
	void DeleteAllBrush();
	// ���� ����ϰ� �ִ� Brush ��ü�� ���� ��ȯ�Ѵ�.
	int GetCount() { return m_brush_count; }
};

// Pen ��ü�� �ִ� 100�������� �����Ѵ�.
#define MAX_STOCK_PEN   100

class TW_PenManager
{
private:
	// ������ Pen ��ü�� �ڵ� ���
	HPEN mh_pen[MAX_STOCK_PEN];
	// ������ Pen ��ü�� �Ӽ� ���
	COLORREF m_pen_color[MAX_STOCK_PEN];
	int m_pen_tick[MAX_STOCK_PEN];
	int m_pen_style[MAX_STOCK_PEN];
	// ���� ����ϰ� �ִ� Pen ��ü�� ��
	int m_pen_count;

public:
	TW_PenManager();
	~TW_PenManager();

	// �����ϴ� ��ü���� a_color�� ������� Pen ��ü�� �ִٸ� �ش� ��ü�� �ڵ��� 
	// ��ȯ�ϰ� ������ Pen ��ü�� �����ϰ� ������ Pen ��ü�� �ڵ鰪�� ��ȯ�Ѵ�.
	HPEN GetPen(int a_style, int a_tick, COLORREF a_color);
	// �����ϴ� ��� Pen ��ü�� �����Ѵ�.
	void DeleteAllPen();
	// ���� ����ϰ� �ִ� Pen ��ü�� ���� ��ȯ�Ѵ�.
	int GetCount() { return m_pen_count; }
};

class TW_Image
{
protected:
	// Bitmap ��ü�� �ڵ� �� ����
	HBITMAP mh_bitmap;
	// Bitmap ��ü�� ������ DC�� �ڵ� �� ����
	HDC mh_mem_dc;
	// Bitmap ��ü�� ���� ����. HBITMAP���� ���� �� ������ 
	// �Ź� �Լ��� ����ϴ� ���� ���ϱ� ���ؼ� ������ ����
	INT32 m_width, m_height;

public:
	TW_Image();           // ��ü ������
	virtual ~TW_Image();  // ��ü �ı���

	// ����ڰ� ������ �Ӽ����� ��Ʈ���� �����Ѵ�.
	void CreateBitmap(INT32 a_width, INT32 a_height, UINT32 a_color_depth = 32, void *ap_pattern = NULL, INT8 a_create_dc_flag = 1);
	// ȭ�� �Ӽ����� ��Ʈ���� �����Ѵ�.
	void CreateScreenBitmap(INT8 a_create_dc_flag = 1);
	// ���α׷��� Resource ������ ����� Bitmap�� �����ͼ� Bitmap ��ü�� �����.
	void LoadBitmapFromRC(INT32 a_resource_id, INT32 a_width, INT32 a_height, INT8 a_create_dc_flag = 1);
	// ���α׷��� Resource ������ ����� Bitmap�� �����ͼ� Bitmap ��ü�� ����� �ڵ� ���� ��ȯ�Ѵ�.
	static HBITMAP LoadBitmapFromRC(INT32 a_resource_id);
	// Bitmap ��ü�� ���� ��´�.
	inline INT32 GetWidth() { return m_width; }
	// Bitmap ��ü�� ���̸� ��´�.
	inline INT32 GetHeight() { return m_height; }
	// Bitmap ��ü�� �ڵ� ���� ��´�.
	inline HBITMAP GetBitmap() { return mh_bitmap; }
	// Bitmap ��ü�� ����� DC �ڵ� ���� ��´�.
	inline HDC GetDC() { return mh_mem_dc; }

	// ȭ�� �̹����� �����Ѵ�.
	void CaptureScreen();
	// TW_Image�� �����ϴ� Memory DC�� �����ϴ� �Լ�. ah_base_dc�� NULL�� �����ϸ� 
	// ���� ����ϴ� �����ġ�� ȣȯ�Ǵ� Memory DC�� �����Ѵ�.
	HDC CreateMemoryDC(HDC ah_base_dc = NULL);
	// TW_Image�� �����ϰ� �ִ� mh_mem_dc�� �����Ѵ�.
	void ReleaseDC();
	// ���ϴ� ������ �����ϰ� Bitmap ��ü�� ����ϴ� �Լ�
	void DrawBitmap(HDC ah_dc, INT32 a_x, INT32 a_y, COLORREF a_remove_color);
	// Bitmap ��ü�� �״�� ����ϴ� �Լ�
	void DrawBitmap(HDC ah_dc, INT32 a_x, INT32 a_y);
};

extern TW_BrushManager *gp_stock_brush;
extern TW_PenManager *gp_stock_pen;


// ũ�⸦ �𸣴� Bitmap�� ������ �������� ��쿡 �����.
// ��Ʈ ������ ����Ǿ� ��ȯ�� �޸𸮴� delete[]�� ����Ͽ� �ݵ�� ���� �����ؾ� �մϴ�.
UINT32 *TWAPI_GetBitmapPattern(HBITMAP ah_bitmap, UINT32 *ap_width = NULL, UINT32 *ap_height = NULL,
							UINT16 *ap_color_bytes = NULL, UINT32 *ap_pattern_size = NULL);
// HFONT ������ �۲��� �����ϴ� �Լ�
HFONT TWAPI_CreateFont(const wchar_t *ap_font_name = L"���� ���", INT32 ah_size = 13, INT32 ah_weight = FW_NORMAL);


#endif