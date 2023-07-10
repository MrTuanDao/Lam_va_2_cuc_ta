#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define NUM_OF_THREAD 2 // dinh nghia so thread de tao
extern HANDLE smp;
char text[]="this is a text";
DWORD WINAPI task1(LPVOID){ 
	HWND hWin;

	hWin = FindWindow("Shell_TrayWnd",NULL); //tim thanh taskbar
	EnableWindow(hWin,true);

	while(1){
		ShowWindow(hWin,false);
		Sleep(1000);
		ShowWindow(hWin,true);
		Sleep(1000);
	}

	return 0;
}

DWORD WINAPI task2(LPVOID x){
	printf("Your taskbar is now twinkling...\nPress any key to stop.");
	return 0;
}

int main(){
	printf("begin\n");
	HANDLE thr[NUM_OF_THREAD]; //tao mang thr 2 ptu kieu Handle;
	DWORD thrid;

//tao thread cho task1()

	thr[0]=CreateThread(NULL,0,task1,NULL,0,&thrid);
	
	Sleep(500);
//tao thread cho task2()

	thr[1]=CreateThread(NULL,0,task2, (LPVOID)text,0,&thrid);
	
///Sleep(500);

	getch(); 
	CloseHandle(thr[0]);
	CloseHandle(thr[1]); 
	return 0;
}
