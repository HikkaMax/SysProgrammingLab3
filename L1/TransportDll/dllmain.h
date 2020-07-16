#pragma once

struct header {
	int size;
	int threadNum;
};

extern "C" _declspec(dllexport) void __stdcall Init();
void Cleanup();
extern "C" _declspec(dllexport) void __stdcall sendMessage(char* pStr, int threadNum);
extern "C" _declspec(dllexport) void __stdcall sendInfo(int actionId);
extern "C" _declspec(dllexport) int __stdcall getAction(HANDLE & hInfo);
extern "C" _declspec(dllexport) char* __stdcall getMessage(header & messageHeader, HANDLE & hReadMessage);