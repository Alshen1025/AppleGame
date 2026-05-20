#pragma once
#include "INC_Windows.h"

namespace renderHelp
{
	struct WICInitializer;
	class BitmapInfo
	{
	public:
		HBITMAP GetBitmapHandle() const { return m_hBitmap; }
   		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }

	private:
		friend struct WICInitializer;

		BitmapInfo() = default;

		BitmapInfo(HBITMAP hBitmap)
		{
			m_hBitmap = hBitmap; 
			BITMAP bitmap;
			GetObject(hBitmap, sizeof(BITMAP), &bitmap);
			m_width = bitmap.bmWidth;
			m_height = bitmap.bmHeight;
		}

		~BitmapInfo()
		{
			if (m_hBitmap)
			{
				DeleteObject(m_hBitmap);
				m_hBitmap = nullptr;
			}
		}

	private:
		HBITMAP m_hBitmap = nullptr;

		int m_width = 0;
		int m_height = 0;

		BitmapInfo(const BitmapInfo&) = delete;
		BitmapInfo& operator=(const BitmapInfo&) = delete;
	};

	BitmapInfo* CreateBitmapInfo(LPCWSTR filename);
}

