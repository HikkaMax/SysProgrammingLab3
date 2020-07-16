// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <process.h>
#include <string>
#include "dllmain.h"


HANDLE hRead, hWrite, hInfoRead, hInfoWrite;

extern "C" _declspec(dllexport) void __stdcall Init()
{
	SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
	CreatePipe(&hRead, &hWrite, &sa, 0);
	SetHandleInformation(hWrite, HANDLE_FLAG_INHERIT, 0);

	CreatePipe(&hInfoRead, &hInfoWrite, &sa, 0);
	SetHandleInformation(hInfoWrite, HANDLE_FLAG_INHERIT, 0);

	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = hRead;
	si.hStdError = hInfoRead;

	PROCESS_INFORMATION pi;

	CreateProcess(NULL, (LPSTR)"L1\\L1.exe", &sa, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
}

void Cleanup()
{
	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(hInfoRead);
	CloseHandle(hInfoWrite);
}

extern "C" _declspec(dllexport) char* __stdcall getMessage(header& messageHeader, HANDLE& hReadMessage)
{
	DWORD dwRead;
	ReadFile(hReadMessage, &messageHeader, sizeof(header), &dwRead, nullptr);
	char* message = new char[messageHeader.size];
	ReadFile(hReadMessage, message, messageHeader.size, &dwRead, nullptr);
	return message;
}

extern "C" _declspec(dllexport) int __stdcall getAction(HANDLE & hInfo)
{
	const int MAXLENGTH = sizeof(int) + 1;
	int action = 9;
	DWORD dwRead;
	ReadFile(hInfo, &action, MAXLENGTH, &dwRead, nullptr);

	return action;
}


extern "C" _declspec(dllexport) void __stdcall sendInfo(int actionId)
{
	DWORD dwWrite;
	WriteFile(hInfoWrite, &actionId, sizeof(int), &dwWrite, nullptr);
}

extern "C" _declspec(dllexport) void __stdcall sendMessage(char* pStr, int threadNum)
{
	DWORD dwWrite;

	header messageHeader;
	messageHeader.size = strlen(pStr) + 1;
	messageHeader.threadNum = threadNum;
	WriteFile(hWrite, &messageHeader, sizeof(header), &dwWrite, nullptr);
	WriteFile(hWrite, pStr, messageHeader.size, &dwWrite, nullptr);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		Cleanup();
		break;
	}
	return TRUE;
}