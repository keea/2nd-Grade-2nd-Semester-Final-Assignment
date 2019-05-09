#include "stdafx.h"
#pragma once
//char * to wchar_t *
LPWSTR ConvertLPCSTRToLPWSTR(const char* pCstring)
{
	LPWSTR pszOut = NULL;
	if (pCstring != NULL)
	{
		int nInputStrLen = strlen(pCstring);
		// Double NULL Termination
		int nOutputStrLen = MultiByteToWideChar(CP_ACP, 0, pCstring, nInputStrLen, NULL, 0) + 2;
		pszOut = new WCHAR[nOutputStrLen];

		if (pszOut)
		{
			memset(pszOut, 0x00, sizeof(WCHAR)*nOutputStrLen);
			MultiByteToWideChar(CP_ACP, 0, pCstring, nInputStrLen, pszOut, nInputStrLen);
		}

	}

	return pszOut;
}