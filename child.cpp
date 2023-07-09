#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define NUM_OF_THREAD 2 // dinh nghia so thread de tao
extern HANDLE smp;
char text[]="this is a text";
DWORD WINAPI task1(LPVOID){ //viet h�m API voi c�c xu l� l�m nhap nh�y TASKBAR
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
//Viet h�m API l�m nhi�m vu in d�ng text sau moi 1000mili secs;
DWORD WINAPI task2(LPVOID x){
	printf("Your taskbar is now twinkling...\nPress any key to stop.");
	return 0;
}

int main(){
	printf("begin\n");
	HANDLE thr[NUM_OF_THREAD]; //tao mang thr 2 ptu kieu Handle;
	DWORD thrid;

//tao thread cho task1()
//V� l� do kh�ng truyen tham so dau v�o n�n tham so thu 4 l� null
	thr[0]=CreateThread(NULL,0,task1,NULL,0,&thrid);
	
	Sleep(500);
//tao thread cho task2()
//Truyen bien text v�o v� �p kieu LPVOID o tham so thu 4
	thr[1]=CreateThread(NULL,0,task2, (LPVOID)text,0,&thrid);
	
///Sleep(500);
//L�c n�y 2 thread dang l�m viec

	getch(); //�oi nguoi dung 1 ph�m bat k�
	CloseHandle(thr[0]); //dong thread 1
	CloseHandle(thr[1]); //d�ng thred 2
	return 0;
}
