#include <windows.h>
#include <stdio.h>

int main(int argc, char** argv) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi[2];
	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	si.cb = sizeof(si);

	if (argc != 2 || strlen(argv[1]) > 20)
	{
		fprintf(stderr, "nieprawidlowy argument: podaj 1 argument o dlugosci max 20 znakow\n");
		return 1;
	}
	if (strlen(argv[1]) == 1) return ((int)argv[1][0]);
	char lancuch1[10] = "", lancuch2[10] = "", argline[50] = "";
	int a = 0, dlugosc = strlen(argv[1]);
	for (int i = 0; i < dlugosc / 2; i++)
	{
		lancuch1[i] = argv[1][i];
	}
	for (int i = dlugosc / 2; i < dlugosc; i++)
	{
		lancuch2[a] = argv[1][i];
		a++;
	}

	sprintf_s(argline, 50, "%s %s", argv[0], lancuch1);
	if (CreateProcessA(NULL, argline, NULL, NULL, 1, 0, NULL, NULL, &si, pi) == 0)
	{
		printf("CreateProcess #%d zakonczony niepowodzeniem (%d).\n", 1, GetLastError());
		return 2;
	}

	sprintf_s(argline, 50, "%s %s", argv[0], lancuch2);
	if(CreateProcessA(NULL, argline, NULL, NULL, 1, 0, NULL, NULL, &si, pi+1) == 0)
	{
		printf("CreateProcess #%d zakonczony niepowodzeniem (%d).\n", 2, GetLastError());
		return 2;
	}

	HANDLE children[2] = { pi[0].hProcess,pi[1].hProcess};
	WaitForMultipleObjects(2, children, 1, INFINITE);
	DWORD ExitCode1, ExitCode2;

	GetExitCodeProcess(children[0], &ExitCode1);
	GetExitCodeProcess(children[1], &ExitCode2);

	printf("%d  %d      %s  %c\n", GetCurrentProcessId(), pi[0].dwProcessId, lancuch1, (char)ExitCode1);
	printf("%d  %d      %s  %c\n", GetCurrentProcessId(), pi[1].dwProcessId, lancuch2, (char)ExitCode2);

	for (int i = 1; i >= 0; i--) {
		CloseHandle(pi[i].hProcess);
		CloseHandle(pi[i].hThread);
	}

	return ExitCode1 > ExitCode2 ? ExitCode1 : ExitCode2;
}
