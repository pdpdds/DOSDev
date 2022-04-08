// dos_launcher.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <Windows.h>
#include <string>

void CreateProcessAndWait(char* proc) {
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));


	// Start the child process. 
	if (!CreateProcessA(NULL,   // No module name (use command line)
		proc,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

//int main(int argc, char** argv)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	char command[0x200];
	snprintf(command, sizeof(command), "\"dosbox-x\" -c \"cd dosdev\\tetris\" -c \"bc /d tetris.prj\" -display2");
	//snprintf(command, sizeof(command), "\"dosbox-x\" -c \"cd dosdev\\tetris\" -c \"bc /b tetris.prj\"");

	CreateProcessAndWait((char*)command);

	return 0;

}

