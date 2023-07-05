#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define NUM_OF_THREAD 2 // dinh nghia so thread de tao
extern HANDLE smp;
char text[]="this is a text";
DWORD WINAPI task1(LPVOID){ //viet hàm API voi các xu lý làm nhap nháy TASKBAR
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
//Viet hàm API làm nhiêm vu in dòng text sau moi 1000mili secs;
DWORD WINAPI task2(LPVOID x){
	printf("Your taskbar is now twinkling...\nPress any key to stop.");
	return 0;
}

int main(){
	printf("begin\n");
	HANDLE thr[NUM_OF_THREAD]; //tao mang thr 2 ptu kieu Handle;
	DWORD thrid;

//tao thread cho task1()
//Vì lý do không truyen tham so dau vào nên tham so thu 4 là null
	thr[0]=CreateThread(NULL,0,task1,NULL,0,&thrid);
	
	Sleep(500);
//tao thread cho task2()
//Truyen bien text vào và ép kieu LPVOID o tham so thu 4
	thr[1]=CreateThread(NULL,0,task2, (LPVOID)text,0,&thrid);
	
///Sleep(500);
//Lúc này 2 thread dang làm viec

	getch(); //Ðoi nguoi dung 1 phím bat kì
	CloseHandle(thr[0]); //dong thread 1
	CloseHandle(thr[1]); //dóng thred 2
	return 0;
}
