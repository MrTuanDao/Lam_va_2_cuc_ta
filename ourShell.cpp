#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <bits/stdc++.h>
#include <tlhelp32.h> 
#include<vector>
#include <tchar.h>
#include<dirent.h>
#include <cstring>
#include<dirent.h>
#include "tchar.h"
#include<debugapi.h>
#include <processthreadsapi.h>
#include <fstream>
#pragma GCC diagnostic ignored "-Wwrite-strings"
#define MAX_PROCESS_RUNNING 100
using namespace std;
PROCESS_INFORMATION pi[MAX_PROCESS_RUNNING];/*PROCESS_INFORMATION là một cấu trúc trong Windows API được sử dụng để lưu trữ thông tin về tiến trình (process). Cấu trúc này bao gồm các trường như handle của tiến trình, handle của luồng chính của tiến trình, ID của tiến trình, và ID của luồng chính.*/
vector <PROCESSENTRY32> PE;/* Vector là một cấu trúc dữ liệu trong C++ cho phép mở rộng kích thước tự động và hỗ trợ các thao tác như thêm, xóa và truy cập các phần tử trong danh sách.*/
set <string>  processList; 
/* Hàm processList có thể được sử dụng để thực hiện các hoạt động trên tập hợp này, 
chẳng hạn như thêm phần tử vào tập hợp, xóa phần tử khỏi tập hợp, 
kiểm tra sự tồn tại của phần tử trong tập hợp, hoặc lặp qua các phần tử trong tập hợp.*/
int countProc=0;
string pp;
string vp;
char pc[200];
char a;
char currentfolder[256];
int ks=0;
bool compareFunction (std::string a, std::string b){ 
	for(int k=0;k<=a.size()-1;k++){ 
		a[k]=tolower(a[k]); 
	};
	for(int k=0;k<=b.size()-1;k++){
		b[k]=tolower(b[k]); 
	};
	return a<b; 
};
void kill(string p2t) {
		string namep = p2t; 
		string cmmdtsk="taskkill /IM "+namep+" /F"; 
		system(cmmdtsk.c_str());
}
// ket thuc tien trinh co ten la namep //trt

void myCreateProcessParallel(char* path) {
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	DWORD dwProcessID=0;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcess(   
        path,       
        NULL,          
        NULL, 
		NULL,          
        FALSE,          
        CREATE_NEW_CONSOLE,            
        NULL,          
        NULL,       
        &si,           
        &pi ) ) {
		cout<<"Failed to create process. Error code: "<< GetLastError()<<endl;
		return;
		
	}
	else{
	processList.insert(path);
	cout<<"Process ID = "<<pi.dwProcessId<<endl;
	cout<<"Thread ID = "<< pi.dwThreadId<<endl;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
void myCreateProcessOBO(char* path) {
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	DWORD dwProcessID=0;
	BOOL bProcess;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcess(   
        path,       
        NULL,          
        NULL, 
		NULL,          
        FALSE,          
        CREATE_NEW_CONSOLE,             
        NULL,          
        NULL,       
        &si,           
        &pi ) ) {
		cout<<"Failed to create process. Error code: "<< GetLastError()<<endl;
		return;
	}
	else{
	cout<<"Process ID = "<<pi.dwProcessId<<endl;
	cout<<"Thread ID = "<< pi.dwThreadId<<endl;
	}
	while(1) {//thêm while(1) để ourShell phải chờ cái này kết thúc 
		HANDLE hSnapShot ;
		PROCESSENTRY32 ProcessInfo ;
		ProcessInfo.dwSize =sizeof(PROCESSENTRY32);
		hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		if(INVALID_HANDLE_VALUE == hSnapShot) {
			cout<<"CreateToolhelp32SnapShot Function Failed" <<endl;
			cout<< "Error No - "<<GetLastError()<<endl;
		}
		DWORD exitCode=0;
		bool check=false;
		if( Process32First( hSnapShot, &ProcessInfo )){
        	do{
            	if(ProcessInfo.th32ProcessID==pi.dwProcessId){
					check=true;
					break;
				}
        	} while( Process32Next( hSnapShot, &ProcessInfo ) );
    	}
		if (!check) break;
		a=getch();
		if (check && a!=27 && a!='s' )
			cout<<"In OBO mode, you can press Esc to exit current process or press 's' to switch to Parallel mode!"<<endl;
		else{
			break;	
		} 
	} 
	if (a==27)
		kill(path);
	if(a==115) {
		processList.insert(path);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	
}

bool find(PROCESSENTRY32 p){
	for (int i=0;i<PE.size();i++){
		if (PE[i].th32ProcessID==p.th32ProcessID)
			return true;
	}
	return false;
}

void checkProcess() {
	cout<<"------------------------------------------PROCESS LISTING-------------------------------------------"<<endl;
	HANDLE hSnapShot =INVALID_HANDLE_VALUE;
	PROCESSENTRY32 ProcessInfo ={0};
	ProcessInfo.dwSize =sizeof(PROCESSENTRY32);
	int count =0;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(INVALID_HANDLE_VALUE == hSnapShot) {
		cout<<"CreatToolhelp32SnapShot Function Failed" <<endl;
		cout<< "Error No - "<<GetLastError()<<endl;
	}
	DWORD exitCode=0;
	cout<<"No.\t Number of Threads\tExecution File\t\tPPID\t\t PID\t\tStatus"<<endl;
	while(Process32Next(hSnapShot, &ProcessInfo) !=FALSE) {
		if(processList.find(ProcessInfo.szExeFile)==processList.end()) continue;
		cout<<"----------------------------------------------------------------------------------------------------"<<endl;
		if (find(ProcessInfo))
			cout<<++count<<".\t\t"<<ProcessInfo.cntThreads<<"\t\t"<<ProcessInfo.szExeFile<<"\t\t"<<ProcessInfo.th32ParentProcessID<<"\t\t"<<ProcessInfo.th32ProcessID<<"\t\tPending"<<endl;
		else
			cout<<++count<<".\t\t"<<ProcessInfo.cntThreads<<"\t\t"<<ProcessInfo.szExeFile<<"\t\t"<<ProcessInfo.th32ParentProcessID<<"\t\t"<<ProcessInfo.th32ProcessID<<"\t\tRunning"<<endl;
		cout<<"----------------------------------------------------------------------------------------------------"<<endl;

		}
	CloseHandle(hSnapShot);
}
void resume(){
	int n=PE.size();
	char pname[100];
	if (n==0){
	cout<<"No pending processes!"<<endl;
	return;
	}
	cout<<"---------------PENDING CHILD PROCESSES--------------"<<endl;
	for (int i=0;i<n;i++){
		cout<<i+1<<". "<<PE[i].szExeFile<<" - PID: "<< PE[i].th32ProcessID<<endl;
	}
	cout<<"Enter the process name to resume: ";
	fflush(stdin);
	gets(pname);
	for (int i=0;i<n;i++){
		if (!strcmp(PE[i].szExeFile,pname)){
			DebugActiveProcessStop(PE[i].th32ProcessID);
			PE.erase(PE.begin()+i);
		}
	}
}
void pause(){
	char pname[100];
	string name="ourShell.exe";
	HANDLE hSnapShot ;
	PROCESSENTRY32 ProcessInfo ;
	ProcessInfo.dwSize =sizeof(PROCESSENTRY32);
	int count =0;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(INVALID_HANDLE_VALUE == hSnapShot) {
		cout<<"CreateToolhelp32SnapShot Function Failed" <<endl;
		cout<< "Error No - "<<GetLastError()<<endl;
	}
	DWORD exitCode=0;
	cout<<"-------------RUNNING CHILD PROCESSES-------------"<<endl; 
	if( Process32First( hSnapShot, &ProcessInfo ) )
    {
        do{
             	if(processList.find(ProcessInfo.szExeFile)==processList.end() || find(ProcessInfo)) continue;
             	if(ProcessInfo.szExeFile==name) continue;
				cout<<++count<<". "<<ProcessInfo.szExeFile<<" - PID: "<< ProcessInfo.th32ProcessID<<endl;
        } while( Process32Next( hSnapShot, &ProcessInfo ) );
    }
    if (count==0) cout<<"No running child processes!"<<endl;
	else {
	cout<<"Enter the process name to pause: ";
	fflush(stdin);
	gets(pname);
	if( Process32First( hSnapShot, &ProcessInfo ) )
    {
        do{
            if(!strcmp(ProcessInfo.szExeFile,pname))
            {
				PE.push_back(ProcessInfo);
				countProc++;
				DebugActiveProcess(ProcessInfo.th32ProcessID);
				break;
            }
        }while( Process32Next( hSnapShot, &ProcessInfo ) );
    }
	}
    CloseHandle(hSnapShot);
	CloseHandle(&ProcessInfo);
}
void help() {
	cout<<"LIST OF COMMAND: "<<endl;
	cout<<"1. help         : List available commands in TinyShell"<<endl;
	cout<<"2. checkProcess : List all info of running Windows processes managed by TinyShell "<<endl;
	cout<<"3. checkThread  : List all info of running Windows threads managed by TinyShell"<<endl;
	cout<<"4. calculator   : Open calculator"<<endl;
	cout<<"5. clear        : Clear screen"<<endl;
	cout<<"6. killProcess  : Terminate a running process which is managed by TinyShell"<<endl;
	cout<<"7. clock        : Open digital clock"<<endl;
	cout<<"8. <bat>        : Execute a .bat file"<<endl;
	cout<<"9. pause        : Pause a running child process"<<endl;
	cout<<"10. resume      : Resume a pending process"<<endl;
	cout<<"11. dir         : List all components in the input directory"<<endl;
	cout<<"12. path        : List all environment variables"<<endl;
	cout<<"13. addPath     : Modify environment variables"<<endl;
	cout<<"14. cd          : Displays the name of or changes the current directory"<<endl;
	cout<<"15. run         : Run a file in current directory"<<endl;
	cout<<"16. date        : Displays or sets the date"<<endl;
	cout<<"17. exit        : Exit TinyShell."<<endl;
}

// lammmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm


void clear() {//goi cls của system sau đó in ra Intro và gọi help() 
	system("cls");
	cout<<"                      SHELL PROJECT                      "<<endl;
	cout<<"---------------------------------------------------------"<<endl;
	cout<<"AUTHOR: Nguyen Hoang Lam, Dao Quoc Tuan, Nguyen Trong Tuan "<<endl;
	cout<<"---------------------------------------------------------\n"<<endl;
	help();
}
void checkThread() {//Được gọi khi nhập "checkThread" - liệt kê tất cả thread đang chạy và quản lý bởi Tinyshell
	map < int ,string> ppip;
	// HANDLE là gì?
	//HANDLE đại diện cho một con trỏ tới một đối tượng hệ thống, chẳng hạn như tệp tin, quy trình, hoặc cửa sổ.
	HANDLE hhSnapShot =INVALID_HANDLE_VALUE; /*INVALID_HANDLE_VALUE biểu thị một handle (con trỏ) không hợp lệ hoặc không tồn tại.*/
	//
	PROCESSENTRY32 hProcessInfo ={0}; /*PROCESSENTRY32 là một cấu trúc dùng để lưu trữ thông tin về một tiến trình (process). Cấu trúc này bao gồm các trường như kích thước của cấu trúc, tham chiếu đến tiến trình cha, ID tiến trình, tên tiến trình, và nhiều thông tin khác.

	Khi bạn khởi tạo biến hProcessInfo với giá trị {0}, điều này có nghĩa là tất cả các byte trong biến được đặt thành giá trị 0. Đây là một cách phổ biến để đảm bảo rằng tất cả các trường trong cấu trúc được thiết lập ban đầu là giá trị mặc định hoặc trống.*/
	hProcessInfo.dwSize =sizeof(PROCESSENTRY32); /*Việc thiết lập dwSize bằng kích thước cấu trúc đảm bảo rằng hệ điều hành có thể truy cập vào các trường khác của cấu trúc một cách chính xác và đảm bảo rằng không có tràn bộ nhớ xảy ra.*/
	int count =0;
	hhSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); /* tạo một bản sao (snapshot) của các quá trình đang chạy trên hệ thống VÀ gán nó cho hhSnapShot*/
	
	//sườn thấy hàm if dưới đây khá vô dụng, ít nhất trong hhSnapShot luôn có tiến trình ourShell.exe
	// if(INVALID_HANDLE_VALUE == hhSnapShot) {
	// 	cout<<"CreatToolhelp32SnapShot Function Failed" <<endl;/* Chắc là bỏ đi cũng không sao*/
	// 	cout<< "Error No - "<<GetLastError()<<endl;
	// }
	/* Process32Next: Returns TRUE if the next entry of the process list has been copied to the buffer or FALSE otherwise. The ERROR_NO_MORE_FILES error value is returned by the GetLastError function if no processes exist or the snapshot does not contain process information.*/
	while(Process32Next(hhSnapShot, &hProcessInfo) !=FALSE) { 
		if(processList.find(hProcessInfo.szExeFile)==processList.end()) continue;  /*kiểm tra xem giá trị của chuỗi hProcessInfo.szExeFile có tồn tại trong processList hay không.*/
		/*szExeFile là một trường (field) trong biến hProcessInfo kiểu PROCESSENTRY32. Trường này chứa tên của tệp thực thi (executable file) của tiến trình.*/
		ppip[hProcessInfo.th32ProcessID]=hProcessInfo.szExeFile;
		}
	CloseHandle(hhSnapShot);
	cout<<"------------------------------------THREAD LISTING----------------------------------"<<endl;
	HANDLE hSnapShot =INVALID_HANDLE_VALUE;
	THREADENTRY32 ThreadInfo ={0};
	ThreadInfo.dwSize =sizeof(THREADENTRY32);
	count =0;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
	if(INVALID_HANDLE_VALUE == hSnapShot) {
		cout<<"CreatToolhelp32SnapShot Function Failed" <<endl;
		cout<< "Error No - "<<GetLastError()<<endl;
	}
	map<int,string> ::iterator it;
	cout<<"No.\t\tOwner Process ID\t\tOwner Process\t\tThread ID"<<endl;
	string prev="random_fhn2";
	while(Thread32Next(hSnapShot, &ThreadInfo) !=FALSE) {
		if(ppip.find(ThreadInfo.th32OwnerProcessID)==ppip.end()) continue;
		// if(ppip[ThreadInfo.th32OwnerProcessID]==prev) continue;
		prev = ppip[ThreadInfo.th32OwnerProcessID];
		cout<<"------------------------------------------------------------------------------------"<<endl;
		cout<<++count<<".\t\t     "<<ThreadInfo.th32OwnerProcessID<<"\t\t\t"<<ppip[ThreadInfo.th32OwnerProcessID]<<"\t\t  "<<ThreadInfo.th32ThreadID<<endl;
		cout<<"------------------------------------------------------------------------------------"<<endl;
		
	}
	CloseHandle(hSnapShot);
}
/*bỏ hàm SetTeColor vì cho rằng không cần thiết*/
void SetTeColor(WORD color){ 
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    wAttributes &= 0xfff0; wAttributes |= color;
    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}
void killProcess(){ 
	cout<<"################################################################################"<<endl; 
	cout<<"######################### Task Process Terminator ##############################"<<endl; 
	cout<<"################################################################################"<<endl; 
	cout<<""<<endl;
	cout<<""<<endl;
	cout<<""<<endl;
	int run_this_program=1; 
	HANDLE hProcessSnap; 
	HANDLE hProcess; 
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass; 
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	pe32.dwSize = sizeof( PROCESSENTRY32 ); 
	vector <string> process_name; 

	while( Process32Next( hProcessSnap, &pe32 ) ){ 
		process_name.push_back(pe32.szExeFile);	//Process32Next(hProcessSnap, &pe32) - Gọi hàm Process32Next để lấy thông tin về quá trình tiếp theo trong bản sao của danh sách quá trình (được lưu trữ trong hProcessSnap). Hàm này trả về TRUE nếu còn quá trình tiếp theo, và gán thông tin của quá trình đó vào biến pe32 kiểu PROCESSENTRY32.
	};
	
	vector <string> process_name_unique;
	vector <int> process_num_occur; 
	process_name_unique.push_back(process_name[3]);
	process_num_occur.push_back(1); 
	for(int k=3;k<=process_name.size()-1;k++){ 
		int add_unique=1; 
		for(int j=0;j<=process_name_unique.size()-1;j++){ 
			string st1=process_name[k]; 
			string st2=process_name_unique[j];
			if(strcmp(st1.c_str(),st2.c_str())==0 && add_unique==1 ){ 
																		
				process_num_occur[j]+=1; 
				add_unique=0; 
			}; 
		};  
		if(add_unique==1){
			process_name_unique.push_back(process_name[k]); 
			process_num_occur.push_back(1);
		}; 
	};
	
	vector <string> process_name_unique_unsorted; 
	vector <int> process_num_occur_sorted; 
	for(int k=0;k!=process_name_unique.size();k++) process_name_unique_unsorted.push_back(process_name_unique[k]);
	std::sort(process_name_unique.begin(),process_name_unique.end(),compareFunction);
	for(int k=0;k!=process_name_unique.size();k++) for(int j=0;j!=process_name_unique.size();j++){ // for each element of sorted(k) and unsorted (j) for process name [Double for loop defined in one line]
		if(strcmp(process_name_unique[k].c_str(),process_name_unique_unsorted[j].c_str())==0) process_num_occur_sorted.push_back(process_num_occur[j]); //if string of kth string (sorted) and jth string (unsorted) match;
	};                                                                                                                                                  //then, add jth process_num_occur element to new list
	
	SetTeColor(2);
	cout<<"---------------Kill processes that are running------------------"<<endl; 
	cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
	cout<<"PROCESS NAME || Number of processes of same name                                "<<endl; 
	for(int k=0;k<=(process_name_unique.size()-1);k++){ 
			if(processList.find(process_name_unique[k])==processList.end()) continue;
		cout<<process_name_unique[k]; 
		cout<<" || "<<process_num_occur_sorted[k]<<endl;
	};
	cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl; 
	SetTeColor(15);
	
	string p2t;
	cout<<endl<<endl;
	cout<<"For example, enter 'chrome.exe' to terminate all processes of that handle name."<<endl; 
	// cout<<"(exit the program if you do not wish to progress)"<<endl;
	cout<<""<<endl; 
	cout<<"Process (name) to terminate:";
	cin>>p2t;
	std::string namep = p2t;
	std::string cmmdtsk="taskkill /IM "+namep+" /F"; 
	system(cmmdtsk.c_str()); // Hàm c_str() được sử dụng để chuyển đổi chuỗi cmmdtsk thành một con trỏ c-style (mảng ký tự) được yêu cầu bởi hàm system.
	cout<<""<<endl; 
	cout<<""<<endl; 
}
void killAll() {
	  for (std::set<string>::iterator it=processList.begin(); it!=processList.end(); ++it){
		if(*it=="ourShell.exe" ) continue;
		string r= * it; //kill từng phẩn tử trong processList
		kill(r);
		}
}
void dir(){
	cout << "Input path: ";
    string path;
    cin >> path;
    DIR* dir;
    dirent* pdir;
    vector<string> files;
    dir = opendir(path.c_str());
    while (pdir = readdir(dir)) {
        files.push_back(pdir->d_name);
    }
    for(int i = 0; i < files.size(); ++i){
        cout << files[i] << '\n';
    }
	//dưới đây là cách của sườn
	// system("dir");
}
void path() {// in ra tất cả phần tử trong PATH - xem biến môi trường 
	char *value;value = getenv("PATH");
    for (int i=0;value[i]!='\0';i++) {
    	if(value[i]==';'&&value[i+1]==';') continue;
    	if(value[i]==';') cout<<endl;
    	else cout<<value[i];
    }
}
void addPath() { //đặt lại biến môi trường
	HKEY hkey;
    long regOpenResult;
    const char key_name[] = "Environment";
    cout << "Enter value path: ";
    string new_value;
    cin >> new_value;
    const char *path=new_value.c_str();                                               //new_value path need to update 

    regOpenResult = RegOpenKeyEx(HKEY_CURRENT_USER,key_name, 0, KEY_ALL_ACCESS, &hkey);
    string var = "Path";
    LPCSTR stuff = var.c_str();                                                   //Variable Name 
    RegSetValueEx(hkey,stuff,0,REG_SZ,(BYTE*) path, strlen(path));
    RegCloseKey(hkey);
}

//suonnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn
// Function for *.bat to read

void autoResume(){
	int n=PE.size();    // n la so tien trinh dang bi pause
	char pname[100];
	if (n==0){
	cout<<"No pending processes!"<<endl;
	return;
	}
	cout<<"---------------PENDING CHILD PROCESSES--------------"<<endl;
	for (int i=0;i<n;i++){
		cout<<i+1<<". "<<PE[i].szExeFile<<" - PID: "<< PE[i].th32ProcessID<<endl;
	}
	cout<<"Enter the process name to resume: ";
	Sleep(500);
	cout << pp << endl;		//nhap ten tien trinh 
	for (int i=0;i<n;i++){
		if (!strcmp(PE[i].szExeFile,pp.c_str())){			// ktra xem pp co khop voi ten 1 tien trinh dang pause k 
			DebugActiveProcessStop(PE[i].th32ProcessID);	// go lenh pause
			PE.erase(PE.begin()+i);							// xoa tien trinh vua dc go ra khoi ds cac tien trih dang pause
		}
	}
}
// cho 1 tien trinh dang bi pause chay tro lai

void autoPause(){
	char pname[100];
	string name="ourShell.exe";
	HANDLE hSnapShot ;						// ctdl tro den 1 tai nguyen nhu tep, thu muc
	PROCESSENTRY32 ProcessInfo ;			// ctdl luu thong tin tien trinh
	ProcessInfo.dwSize =sizeof(PROCESSENTRY32);		
	int count =0; 		// dem tham so tien trinh
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);		// tao 1 snapshot ds cac tien trih 
	if(INVALID_HANDLE_VALUE == hSnapShot) {
		cout<<"CreateToolhelp32SnapShot Function Failed" <<endl;
		cout<< "Error No - "<<GetLastError()<<endl;
	}
	DWORD exitCode=0;
	cout<<"-------------RUNNING CHILD PROCESSES-------------"<<endl; 
	if( Process32First( hSnapShot, &ProcessInfo ) )
    {
        do{
             	if(processList.find(ProcessInfo.szExeFile)==processList.end()) continue;
             	if(ProcessInfo.szExeFile==name) continue;
				cout<<++count<<". "<<ProcessInfo.szExeFile<<" - PID: "<< ProcessInfo.th32ProcessID<<endl;
        } while( Process32Next( hSnapShot, &ProcessInfo ) );
		// vong while nay in ra cac tien trinh dang chay (trong hSnapshot)
	}
    if (count==0) cout<<"No child processes!"<<endl;
	cout<<"Enter the process name to pause: ";
	Sleep(500);
	cout << pc << endl;
	if( Process32First( hSnapShot, &ProcessInfo ) )
    {
        do{
            if(!strcmp(ProcessInfo.szExeFile,pc))
            {
				PE.push_back(ProcessInfo);  	// them vao ds pause
				countProc++;
				DebugActiveProcess(ProcessInfo.th32ProcessID); // tam dung go loi, tao dk cho vc dung
				break;
            }
        }while( Process32Next( hSnapShot, &ProcessInfo ) );
    }
    CloseHandle(hSnapShot);
	CloseHandle(&ProcessInfo);
}

void autoKillProcess(){ 
	cout<<"################################################################################"<<endl; 
	cout<<"######################### Task Process Terminator ##############################"<<endl; 
	cout<<"################################################################################"<<endl; 
	cout<<""<<endl;
	cout<<""<<endl;
	int run_this_program=1; //parameter which determines if program will keep running
	// while program is running (this section is intended to repeat until user exits program)
		//initialize objects
		HANDLE hProcessSnap; //create handle identifier for object hprocesssnap
	  	HANDLE hProcess; //create handle identifier for object hprocess
	 	PROCESSENTRY32 pe32; //Describes an entry from a list of the processes residing in the system address space when a snapshot was taken.
	 	DWORD dwPriorityClass; //create dword class dwPriorityClass
	 	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ); //take "snapshot" of processes running
	 	pe32.dwSize = sizeof( PROCESSENTRY32 ); //size of PROCESSENTRY32 [unused]
	 	vector <string> process_name; //initialize vector named "process_name" where each element will be a string (process name)
	 	
	
		// record all detetected running processes #######################################
		while( Process32Next( hProcessSnap, &pe32 ) ){ //for each object (pe32) in hprocesssnap [Process32Next runs through processes]
			process_name.push_back(pe32.szExeFile); // add process name to list of processes		
		};
		//##################################################################################
		// Get list of unique process names and number of times that process name has process$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		vector <string> process_name_unique; //initialize vector of string elements (each element will be unique process name)
		vector <int> process_num_occur; //initialize vector of integer elements (each element will be count that unique process has occured)
		process_name_unique.push_back(process_name[3]); //initialise first process; start from 3 [0=blank 1='sys' var 2=another title 3=first process listed]
		process_num_occur.push_back(1); //initialise count for first process
		for(int k=3;k<=process_name.size()-1;k++){ // for each kth process name in complete list
			int add_unique=1; //initialise parameter that tells if proccess is unique (=1)
			for(int j=0;j<=process_name_unique.size()-1;j++){ // for each jth process in list of unique process names
				string st1=process_name[k]; //string 1 = process name from complete list (k)
				string st2=process_name_unique[j]; // process name from unique list (j)
				if(strcmp(st1.c_str(),st2.c_str())==0 && add_unique==1 ){ //if process names j&k match and if unique parameter=1 [c_str() converts cmmdtsk to c string, needed for strcmp]
																		  // note: str(a,b)=0 if string a and b are identical match
					process_num_occur[j]+=1; //add to count for re-occurance of process name
					add_unique=0; //set unique process parameter to 0 [process name from list k is already in list j]
				}; // if loop
			};  // for loop (j)
			if(add_unique==1){ //if process name from list k was unique (to be added to list)
				process_name_unique.push_back(process_name[k]); //add process name from list k to list of unique process
				process_num_occur.push_back(1);	 //initialise count of occurance for newly added process name
			}; // if loop
		};// for loop (k)
		//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		
		// sort process name alphabetically and then also sort "num occurance" accordingly&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		vector <string> process_name_unique_unsorted; //initialize vector that will be unsorted vector of process names;
		vector <int> process_num_occur_sorted; //initialize vector that will be unsorted vector of process names;
		for(int k=0;k!=process_name_unique.size();k++) process_name_unique_unsorted.push_back(process_name_unique[k]); //add elements from original list to new list [for loop in one line]
		std::sort(process_name_unique.begin(),process_name_unique.end(),compareFunction); // sortvector using compare function (output is that process_name_unique is sorted alphabetically using function "compareFunction")
		// sort process_num_occur alphabetically and as new vector "process_num_occur_sorted" (previously initialized but is still empty@@@@@@@@@@@@@@@@@@@@@@@@@@@
		for(int k=0;k!=process_name_unique.size();k++) for(int j=0;j!=process_name_unique.size();j++){ // for each element of sorted(k) and unsorted (j) for process name [Double for loop defined in one line]
			if(strcmp(process_name_unique[k].c_str(),process_name_unique_unsorted[j].c_str())==0) process_num_occur_sorted.push_back(process_num_occur[j]); //if string of kth string (sorted) and jth string (unsorted) match;
		};                                                                                                                                                  //then, add jth process_num_occur element to new list
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		
		// Print unique processes %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		SetTeColor(2);
		cout<<"---------------Child processes that are running------------------"<<endl; // string added for UI purposes
		cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl;
		cout<<"PROCESS NAME || Number of processes of same name                                "<<endl; // string added for UI purposes
		for(int k=0;k<=(process_name_unique.size()-1);k++){ // for each kth element in sorted list of process names
		    	if(processList.find(process_name_unique[k])==processList.end()) continue;
			cout<<process_name_unique[k]; // print kth process name
			cout<<" || "<<process_num_occur_sorted[k]<<endl; // print number of occurances for kth process name (also previously assorted correspondingly with vector above)
		};
		cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl; // string added for UI purposes
		SetTeColor(15);
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    cout<<endl<<endl;
		cout<<"For example, enter 'chrome.exe' to terminate all processes of that handle name."<<endl; // string added for UI purposes
		cout<<"(exit the program if you do not wish to progress)"<<endl; // string added for UI purposes
		cout<<""<<endl; // string added for UI purposes
		cout<<"Process (name) to terminate:"; // string added for UI purposes
		Sleep(500);
		cout<< pp << endl; // [AUTO OUTPUT FOR PROCESS NAME HANDLE OF PROCESSES/S TO TERMINATE]
		std::string namep = pp; //convert pp to string for use below (had to this for system() to work, yet to be optimized from this)
		std::string cmmdtsk="taskkill /IM "+namep+" /F"; //create full string to run in line below (had to this for system() to work, yet to be optimized from this)
		system(cmmdtsk.c_str()); //c_str() converts cmmdtsk to c string
		cout<<""<<endl; // string added for UI purposes
		cout<<""<<endl; // string added for UI purposes
}

void autoDir(){//sườn check thử thì lệnh autoDir này kh chạy, có thể cân nhắc bỏ nó
	cout << "Input path: ";
	Sleep(500);
    cout << pp << endl;
    DIR* dir;
    dirent* pdir;
    vector<string> files;
    dir = opendir(pp.c_str());
    while (pdir = readdir(dir)) {
        files.push_back(pdir->d_name);
    }
    for(int i = 0; i < files.size(); ++i){
        cout << files[i] << '\n';
    }
}
// liet ke cac file thu muc trong dg dan pp

void autoAddPath() {
	HKEY hkey;
    long regOpenResult;
    const char key_name[] = "Environment";
    cout << "Enter value path: ";
    Sleep(500);
    cout << pp << endl;
    const char *path=pp.c_str();                                               //new_value path need to update 
    regOpenResult = RegOpenKeyEx(HKEY_CURRENT_USER,key_name, 0, KEY_ALL_ACCESS, &hkey);
    cout << "Enter variable name: ";
    Sleep(500);
    cout << vp << endl;
    LPCSTR stuff = vp.c_str();                                                   //Variable Name 
    RegSetValueEx(hkey,stuff,0,REG_SZ,(BYTE*) path, strlen(path));
    RegCloseKey(hkey);
}
// deo hieu

void cdProcess() {
	system("dir");		// in ra ds file folder trong folfer hien tai
	cout<<"Enter the directory <DIR> folder: ";
	string direct;
	cin>>direct;
	if(direct==".") {
		return;
	} 
	char tmp[256];
	getcwd(tmp, 256);
	if(direct=="..") {
		for(int i=255;i>=0;i--) {
			if(tmp[i]=='/') {
			tmp[i]='\0';
			}			// tim "/" cuoi de cho thanh NULL
		}
		if( !SetCurrentDirectory(tmp))
        {
        printf("SetCurrentDirectory failed (%d)\n", GetLastError());
        return;
        }		
	}
	int i=0;
	for(i;i<=255;i++) {
		if(tmp[i]=='\0') break;
	}
	tmp[i]='/';
	for(int j=0;j<direct.size();j++) {
		i=i+1;
		tmp[i]=direct[j];
	}
	tmp[i+1]='\0';
	if( !SetCurrentDirectory(tmp))
   {
      printf("SetCurrentDirectory failed (%d)\n", GetLastError());
      return;
   }
}
// nhap mot folder con cua 1 folder dang lam viec
// vd: dang o c:\users thi nhap documents

void run() {
	system("dir");
	string ds;
	cout<<"Enter the executed file: ";
	cin>>ds;

	int n = ds.length();
    char char_array[n + 1];
    strcpy(char_array, ds.c_str());
	cout<<endl<<"1. Parallel (Processes run in parallel)"<<endl;
	    cout<<"2. One By One (Processes run one by one)"<<endl;
	    do {
		cout<<"Choose mode (1/2): ";
		cin>>a;
		} while (a!='1'&&a!='2');
	    if (a=='1') {
		    	myCreateProcessParallel(char_array);
	    }
	    else {
			myCreateProcessOBO(char_array);
	    }		
}

//tuannnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn


int main() {
	  map < string, char*>  demand; //ánh xạ từ string tới char*
	 demand.insert( std::make_pair<string, char*>("checkThread", "" ) );
	 demand.insert( std::make_pair<string, char*>( "checkProcess", "" ) );
	 demand.insert( std::make_pair<string, char*>( "killProcess", "" ) );
	 demand.insert( std::make_pair<string, char*>( "help", "" ) );
	 demand.insert( std::make_pair<string, char*>( "calculator", "calculator.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "clear", "" ) );
	 demand.insert( std::make_pair<string, char*>( "clock", "clock.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "exit", "" ) );
	 demand.insert( std::make_pair<string, char*>( "resume", "" ) );
	 demand.insert( std::make_pair<string, char*>( "pause", "" ) );
	 demand.insert( std::make_pair<string, char*>( "dir", "" ) );
	 demand.insert( std::make_pair<string, char*>( "path", "" ) );
	 demand.insert( std::make_pair<string, char*>( "addPath", "" ) );
	 demand.insert( std::make_pair<string, char*>( "<bat>", "" ) );
	 demand.insert( std::make_pair<string, char*>( "cd", "" ) );
	 demand.insert( std::make_pair<string, char*>( "run", "" ) );
	 demand.insert( std::make_pair<string, char*>( "date", "" ) );
	 demand.insert( std::make_pair<string, char*>( "autodir", "" ) );
	 demand.insert( std::make_pair<string, char*>( "hello", "hello.exe" ) );
	 processList.insert("clock.exe");
	 processList.insert("calculator.exe");
	 processList.insert("ourShell.exe");
	 getcwd(currentfolder, 256); //lấy đường dẫn và gán cho currentfolder 
	 string dm;
	 clear();
	 char a;
	 char tmp[256];
	 string data;
	 while(true) {        // đọc input và so sánh với command 
	 	cout<<endl<<"--------------------------------------------------------"<<endl;
        getcwd(tmp, 256);
        cout << tmp <<"> ";
	 	cin>>dm;
	 	if(dm=="exit") break;
	 	if(demand.find(dm)==demand.end()) {
	 		cout<<"The demand isn't available!"<<endl;
	 		continue;
	 	}
	 	if (dm=="<bat>"){
	 		string b;
	 		ifstream infile; /* Lớp ifstream là một lớp được sử dụng để đọc dữ liệu từ tệp tin.*/
   			infile.open("Shell.bat"); //mở file Shell.txt và chạy từng dòng 
	 		while(true) {
	 			Sleep(1000);
	 			cout<<endl<<"--------------------------------------------------------"<<endl;
				getcwd(tmp, 256);
				cout << tmp <<"> ";
	 			if (infile.eof()) break; 
	 			infile >> data; //đọc dữ liệu từ file và gán nó vào data, đọc theo dòng 
   				Sleep(500);
   				cout << data << endl;
   				
	 			if(data=="exit") {
					killAll();
					return 0;
				}
	 			if(demand.find(data)==demand.end() || data == "<bat>") {
	 				cout<<"The demand isn't available!"<<endl;
	 				continue;
	 			}
	 			if(data=="date") {
	 				system("DATE");
	 				continue;
				 }
	 			if (data=="pause"){
	 				infile >> pc; 
   					if (infile.eof()) break;
	 				autoPause();
	 				continue;
	 			}
	 			if (data=="resume"){
	 				infile >> pp; 
   					if (infile.eof()) break;
	 				autoResume();
	 				continue;
	 			}
	 			if (data=="help"){
	 				help();
	 				continue;
	 			}
	 			if (data=="clear"){
	 				clear();
	 				continue;
	 			}	
	 			if (data=="checkProcess"){
	 				checkProcess();
	 				continue;
	 			}
	 			if (data=="checkThread"){
	 				checkThread();
	 				continue;
	 			}
	 			if(data=="killProcess") {
	 				infile >> pp; 
   					if (infile.eof()) break;
	 		    	autoKillProcess();
	 				continue;
	 			}
				if(data=="dir") {
					infile >> pp; 
   					if (infile.eof()) break;
	 		    	autoDir();
	 				continue;
	 			}
	 			if(data=="path") {
	 		    	path();
	 				continue;
	 			}
	 			if(data=="addPath") {
	 				infile >> pp; 
   					if (infile.eof()) break;
   					infile >> vp; 
   					if (infile.eof()) break;
	 		    	autoAddPath();
	 				continue;
	 			}
	    		cout<<"1. Parallel (Processes run in parallel)"<<endl;
	    		cout<<"2. One By One (Processes run one by one)"<<endl;
	    		do {
	    			cout<<"Choose mode (1/2): ";
	    			infile >> b; 
   					if (infile.eof()) break;
   					Sleep(100);
					cout<< b << endl;
				} while (b!="1"&&b!="2");
	    		if (b=="1") {
					
					myCreateProcessParallel(demand[data]);
	    		}
	    		else if (b=="2") {
				
					myCreateProcessOBO(demand[data]);
	    		}
	    		else cout << "Failed to create a process!" << endl;
	    		if (data=="s") a=data[0];
			}
				
			infile.close(); 
		 	continue;
		}
		if(dm=="date") {
	 		system("DATE");
	 		continue;
		}
	 	if (dm=="pause"){
	 		pause();
	 		continue;
	 	}
	 	if (dm=="resume"){
	 		resume();
	 		continue;
	 	}
	 	if (dm=="help"){
	 		help();
	 		continue;
	 	}
	 	if (dm=="clear"){
	 		clear();
	 		continue;
	 	}
	 	if (dm=="checkProcess"){
	 		checkProcess();
	 		continue;
	 	}
	 	if (dm=="checkThread"){
	 		checkThread();
	 		continue;
	 	}
	 	if(dm=="killProcess") {
	 		    killProcess();
	 			continue;
	 	}
		if(dm=="dir") {
	 		    dir();
	 			continue;
	 	}
	 	if(dm=="path") {
	 		    path();
	 			continue;
	 	}
	 	if(dm=="addPath") {
	 		    addPath();
	 			continue;
	 	}
	 	if(dm=="cd") {
	 		cdProcess();
	 		continue;
		 }
		 if(dm=="run") {
		 	run();
		 	continue;
		 }
		//suon bo test ham
		if(dm=="autodir"){
			autoDir();
			continue;
		}
		
		//suon bo test ham o phias tren
		 char tmpp[256];
		 getcwd(tmpp, 256);
		 SetCurrentDirectory(currentfolder);
	    cout<<"1. Parallel (Processes run in parallel)"<<endl;
	    cout<<"2. One By One (Processes run one by one)"<<endl;
	    do {
		cout<<"Choose mode (1/2): ";
		cin>>a;
		} while (a!='1'&&a!='2');
	    if (a=='1') {
			char a;
			myCreateProcessParallel(demand[dm]);
	    }
	    else {
			char a;
			myCreateProcessOBO(demand[dm]);
	    }
	    SetCurrentDirectory(tmpp);
	}
	killAll();
	return 0;
}
