#include "windows.h"
#include "Encode.h"
#include "Decode.h"
#include "IEnDeCode.h"
#include "EnDePolicy.h"


#pragma warning(disable : 4005)
#pragma warning(disable : 4007)


#ifdef WINDOWSCODE
	#define STDCALL	__stdcall
	#define DLLEXPORT __declspec(dllexport)
	#define CTYPENAMEFUNC extern "C"
#else
	#define STDCALL
	#define DLLEXPORT
	#define CTYPENAMEFUNC extern "C"
#endif


CTYPENAMEFUNC DLLEXPORT char*  STDCALL FetchModuleVersion()
{
	return GetModuleVersion();
}


CTYPENAMEFUNC DLLEXPORT InterfaceEncode*  STDCALL GetEncodeApi()
{
	return new Encode();
}


CTYPENAMEFUNC DLLEXPORT InterfaceDecode*  STDCALL GetDecodeApi()
{
	return new Decode();
}


#ifndef LINUXCODE
bool STDCALL DllMain( HANDLE hModule, DWORD  ul_reason_for_call,  LPVOID lpReserved )
{
    return true;
}
#endif








