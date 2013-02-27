/*
	LaunchAffinity.c

By Riccardo Cohen; source from :
	http://groups.yahoo.com/group/palm-dev-forum/message/87865
*/

#include <windows.h>

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR lpCmdLine, int nCmdShow ) {
	STARTUPINFO si = {0,};
	PROCESS_INFORMATION pi = {0,};

	si.cb = sizeof(si);

	CreateProcess(NULL, lpCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si,
	&pi);
	SetProcessAffinityMask(pi.hProcess, 1);
	return 0;
}