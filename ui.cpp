#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <strsafe.h>
#include <stdio.h>
#include <RASERROR.H>
#include <Winerror.h>
#include <ras.h>
#include <tchar.h>

#include <richedit.h>

#include <Shlobj.h>

#pragma comment(lib, "RASAPI32.LIB")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")

#pragma comment(lib, "Shell32.lib")

#define ID_BUTTON_start_disconnect_vpn  50

#define ID_BUTTON_user_manual  14

#define ID_BUTTON_about_us  16

HWND btn;
HWND btn2;
HWND btn3;

HRASCONN hRasConn = NULL;

LPRASDIALPARAMS     lpRasDialParams = NULL;
LPRASENTRY pRasEntry = NULL;
LPRASDEVINFO lpRasDevInfo = NULL;

int disconnect()
{
	
	if(ERROR_SUCCESS == RasHangUp(hRasConn))
	{
		

    	/* if(lpRasDevInfo && lpRasDialParams && pRasEntry)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)lpRasDevInfo);
			HeapFree(GetProcessHeap(), 0, (LPVOID)lpRasDialParams);
			HeapFree(GetProcessHeap(), 0, (LPVOID)pRasEntry);
		} */

		 

		Sleep(3000);

		SendMessage(btn, WM_SETTEXT, 0, (LPARAM) L"Start the VPN");
    	RasDeleteEntry(NULL, L"Alto Grade PPTP VPN");

	}else{
		MessageBox(
								NULL,
								(LPCWSTR)L"FAILED RasHangUp(...",
								(LPCWSTR)L"Eyyyy",
								MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
								);

		
	}

	return 0;
}

DWORD WINAPI start(LPVOID arg)
// int start()
{

	 


	DWORD dwCb = sizeof(RASDEVINFO);
	DWORD dwDevices = 0;
	
	DWORD dwRet = ERROR_SUCCESS;



	dwRet = RasEnumDevices(lpRasDevInfo, &dwCb, &dwDevices);

	lpRasDevInfo = (LPRASDEVINFO) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);

	lpRasDevInfo[0].dwSize = sizeof(RASDEVINFO);

	dwRet = RasEnumDevices(lpRasDevInfo, &dwCb, &dwDevices);

	int indexOfPPTPDevice = -1;

	if (ERROR_SUCCESS == dwRet)
	{
		for (DWORD i = 0; i < dwDevices; i++)
		{
			 

			if(NULL != wcsstr(lpRasDevInfo[i].szDeviceName, L"PPTP"))
			{
				wprintf(L"%s: ", lpRasDevInfo[i].szDeviceName);
				wprintf(L"%s\n", lpRasDevInfo[i].szDeviceType);

				/********** EXPERIMENT WORKS******************/
					  MessageBox(
								NULL,
								(LPCWSTR) lpRasDevInfo[i].szDeviceName,
								(LPCWSTR)L"Eyyyy",
								MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
								);

					MessageBox(
								NULL,
								(LPCWSTR) lpRasDevInfo[i].szDeviceType,
								(LPCWSTR)L"Eyyyy",
								MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
								);  

				/*******************************************/

				indexOfPPTPDevice = i;
				break;
			}
		}
		
	}else{
		

		if (dwRet == ERROR_BUFFER_TOO_SMALL)
		{
			MessageBox(
								NULL,
								(LPCWSTR) L"FAILED RasEnumDevices ERROR_BUFFER_TOO_SMALL",
								(LPCWSTR)L"Eyyyy",
								MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
								);

			lpRasDevInfo = (LPRASDEVINFO) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);
			lpRasDevInfo[0].dwSize = sizeof(RASDEVINFO);
			if (lpRasDevInfo == NULL)
			{
				 

				MessageBox(
								NULL,
								(LPCWSTR) L"HeapAlloc failed!",
								(LPCWSTR)L"Eyyyy",
								MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
								);
				return 0;
			}else{
				dwRet = RasEnumDevices(lpRasDevInfo, &dwCb, &dwDevices);
				for (DWORD i = 0; i < dwDevices; i++)
				{
					 

					if(NULL != wcsstr(lpRasDevInfo[i].szDeviceName, L"PPTP"))
					{
						wprintf(L"%s: ", lpRasDevInfo[i].szDeviceName);
						wprintf(L"%s\n", lpRasDevInfo[i].szDeviceType);

						/********** EXPERIMENT WORKS******************/
							  MessageBox(
										NULL,
										(LPCWSTR) lpRasDevInfo[i].szDeviceName,
										(LPCWSTR)L"Eyyyy",
										MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
										);

							MessageBox(
										NULL,
										(LPCWSTR) lpRasDevInfo[i].szDeviceType,
										(LPCWSTR)L"Eyyyy",
										MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
										);  

						/*******************************************/

						indexOfPPTPDevice = i;
						break;
					}
				}

			}
		}

		 
	}

	printf("indexOfPPTPDevice: %d\n", indexOfPPTPDevice);




	DWORD cbb = sizeof(RASDIALPARAMS);
	lpRasDialParams = (LPRASDIALPARAMS)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cbb); 
	pRasEntry = (LPRASENTRY)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(RASENTRY));

	pRasEntry->dwSize = sizeof (RASENTRY);
	pRasEntry->dwfOptions = RASEO_ModemLights | RASEO_RequirePAP | RASEO_RequireCHAP | RASEO_RequireMsCHAP | RASEO_RequireMsCHAP2 | RASEO_NetworkLogon;
	pRasEntry->dwFramingProtocol = RASFP_Ppp;
	pRasEntry->dwType = RASET_Vpn;
	pRasEntry->dwVpnStrategy = VS_PptpOnly;
	pRasEntry->dwfNetProtocols = RASNP_Ip; 
	pRasEntry->dwEncryptionType = ET_Require; 




	HRESULT hr;
	hr = StringCchCopy(pRasEntry->szLocalPhoneNumber, ARRAYSIZE(pRasEntry->szLocalPhoneNumber), L"192.168.1.6");
	if(FAILED(hr))
	{
		printf("FAILED hr 1\n");
	}

	hr = StringCchCopy(pRasEntry->szDeviceName, ARRAYSIZE(pRasEntry->szDeviceName), lpRasDevInfo[indexOfPPTPDevice].szDeviceName);
	if(FAILED(hr))
	{
		printf("FAILED hr 2\n");
	}

	hr = StringCchCopy(pRasEntry->szDeviceType, ARRAYSIZE(pRasEntry->szDeviceType), lpRasDevInfo[indexOfPPTPDevice].szDeviceType);
	if(FAILED(hr))
	{
		printf("FAILED hr 3\n");
	}

	DWORD dwErr = RasSetEntryProperties(NULL,
		 L"Alto Grade PPTP VPN",
		pRasEntry,
		sizeof (RASENTRY),
		NULL,
		0
		);

	if(dwErr == ERROR_SUCCESS)
	{
		printf("RasSetEntryProperties Successful\n");
	}else{
		printf("RasDial Failed %d", dwErr);
		
	}

	lpRasDialParams->dwSize = sizeof(RASDIALPARAMS);
	 StringCchCopy(lpRasDialParams->szPhoneNumber, ARRAYSIZE(lpRasDialParams->szPhoneNumber), L"");
	if(FAILED(hr))
	{
		printf("FAILED hr 4\n");
	} 
	StringCchCopy(lpRasDialParams->szEntryName, ARRAYSIZE(lpRasDialParams->szEntryName), L"Alto Grade PPTP VPN");
	if(FAILED(hr))
	{
		printf("FAILED hr 5\n");
	}
	StringCchCopy(lpRasDialParams->szCallbackNumber, ARRAYSIZE(lpRasDialParams->szCallbackNumber), (STRSAFE_LPCWSTR)"");
	if(FAILED(hr))
	{
		printf("FAILED hr 6\n");
	}
	StringCchCopy(lpRasDialParams->szUserName, ARRAYSIZE(lpRasDialParams->szUserName), L"hman");
	if(FAILED(hr))
	{
		printf("FAILED hr 7\n");
	}
	StringCchCopy(lpRasDialParams->szPassword, ARRAYSIZE(lpRasDialParams->szPassword), L"password");
	if(FAILED(hr))
	{
		printf("FAILED hr 8\n");
	}

	// HRASCONN hRasConn = NULL;
	hRasConn = NULL;
	dwErr = RasDial(NULL,
		NULL,
		lpRasDialParams,
		NULL,
		NULL,
		&hRasConn
		);

	if(dwErr == ERROR_SUCCESS)
	{
		printf("RasDial Successful\n");
		SendMessage(btn, WM_SETTEXT, 0, (LPARAM) L"Disconnect");

	}else{
		MessageBox(
								NULL,
								(LPCWSTR)L"FAILED RasDial(...",
								(LPCWSTR)L"Eyyyy",
								MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
								);

		/******************* EXPERIMENT ****************************/
				/* WCHAR lpszErrorString[1024];
				RasGetErrorString(dwErr, lpszErrorString, 1024);
				MessageBox(
								NULL,
								(LPCWSTR) lpszErrorString,
								(LPCWSTR)L"Eyyyy",
								MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
								); */

		/**********************************************************/

		printf("RasDial Failed %d\n", dwErr);
		RasHangUp(hRasConn);

		


		return 0;

	}

	if(hRasConn == NULL)
	{
		printf("HRASCONN == NULL\n");
	}

	HANDLE gEvent_handle;
	DWORD           nRet = 0;
	gEvent_handle = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!gEvent_handle)
    {
        nRet = GetLastError();
        printf("CreateEvent failed: Error = %d", nRet);	        
    }



	RasConnectionNotification(hRasConn, gEvent_handle, RASCN_Disconnection);

	nRet = WaitForSingleObject(gEvent_handle, INFINITE);
    switch (nRet)
    {
    	case WAIT_OBJECT_0:
    		/**************** THIS PART WORKS ***************************/
    		    printf("RASCN_Disconnection Notification event signalled");

    			SendMessage(btn, WM_SETTEXT, 0, (LPARAM) L"Start the VPN");

    			RasDeleteEntry(NULL, L"Alto Grade PPTP VPN");  

    		/**************************************************************/

    		 

    			if(lpRasDevInfo && lpRasDialParams && pRasEntry)
    			{
    				HeapFree(GetProcessHeap(), 0, (LPVOID)lpRasDevInfo);
					HeapFree(GetProcessHeap(), 0, (LPVOID)lpRasDialParams);
					HeapFree(GetProcessHeap(), 0, (LPVOID)pRasEntry);
    			}

    		break;

    	default:
            printf("WaitForSingleObject returned %d\n", nRet);
            break;
    }

	 
	
	return 0;
}
 

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_COMMAND:
		{
			 switch(HIWORD(wParam))
			 {
			 	case BN_CLICKED:
			 	{
			 		switch(LOWORD(wParam))
			 		{
			 			case ID_BUTTON_user_manual:
			 			{
			 				LoadLibrary(TEXT("Msftedit.dll"));
			 				 
			 				HWND hwndEdit= CreateWindowEx(0, MSFTEDIT_CLASS, TEXT("Type here"),
																  ES_MULTILINE | ES_READONLY | WS_VISIBLE |  WS_TABSTOP | WS_SYSMENU,
																0, 0,  												 
																  600,
																  600,
																NULL,
																// hwnd,
																NULL, // Menu
																 (HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
																NULL // Additional application data
																);

			 				 COLORREF rgbGreen = 0x00401210;
			 				 SendMessage(hwndEdit, EM_SETBKGNDCOLOR, (WPARAM) 0,  (LPARAM) rgbGreen);
			 				 SendMessage(hwndEdit, WM_SETICON, (WPARAM) 1, (LPARAM) ExtractIcon(NULL, L"myLogo.ico", 0));

			 				 CHARFORMAT charFormat;
			 				 charFormat.cbSize = sizeof(CHARFORMAT);
			 				 charFormat.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE;
			 				 charFormat.yHeight = 300;
			 				 COLORREF rgbWhite = 0x00FFFFFF;
			 				 charFormat.crTextColor = rgbWhite;
			 				 // StringCchCopy(charFormat.szFaceName, LF_FACESIZE, L"Calibri");
			 				 StringCchCopy(charFormat.szFaceName, LF_FACESIZE, L"Gabriola");

			 				 SendMessage(hwndEdit, EM_SETCHARFORMAT, (WPARAM) SCF_ALL, (LPARAM) &charFormat);

			 				 TCHAR lpszLatin[] = L"You suck";
			 				 SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM) lpszLatin);
			 				  

			 				break;
			 			}

			 			case ID_BUTTON_about_us:
			 			{
			 				LoadLibrary(TEXT("Msftedit.dll"));
			 				HWND hwndEdit= CreateWindowEx(0, MSFTEDIT_CLASS, TEXT("About us"),
																  ES_MULTILINE | ES_READONLY | WS_VISIBLE |  WS_TABSTOP | WS_SYSMENU,
																0, 0,  												 
																  600,
																  600,
																NULL,
																// hwnd,
																NULL, // Menu
																 (HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
																NULL // Additional application data
																);
			 				 COLORREF rgbGreen = 0x00401210;
			 				 SendMessage(hwndEdit, EM_SETBKGNDCOLOR, (WPARAM) 0,  (LPARAM) rgbGreen);
			 				 SendMessage(hwndEdit, WM_SETICON, (WPARAM) 1, (LPARAM) ExtractIcon(NULL, L"myLogo.ico", 0));

			 				 CHARFORMAT charFormat;
			 				 charFormat.cbSize = sizeof(CHARFORMAT);
			 				 charFormat.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE;
			 				 charFormat.yHeight = 300;
			 				 COLORREF rgbWhite = 0x00FFFFFF;
			 				 charFormat.crTextColor = rgbWhite;
			 				 // StringCchCopy(charFormat.szFaceName, LF_FACESIZE, L"Calibri");
			 				 StringCchCopy(charFormat.szFaceName, LF_FACESIZE, L"Gabriola");

			 				 SendMessage(hwndEdit, EM_SETCHARFORMAT, (WPARAM) SCF_ALL, (LPARAM) &charFormat);

			 				 TCHAR lpszLatin[] = L"Website:\t www.altogradesoftwares.tk \n"
			 				 					 L"Telegram:\t +263 775 267 926 \n"
			 				 					 L"Twitter:\t\t twitter.com/AltoGrade \n"
			 				 					 L"Facebook:\t facebook.com/AltoGradeSoftwares \n"
			 				 					 L"WhatsApp:\t +263 775 267 926";
			 				 SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM) lpszLatin);

			 				break;
			 			}

			 			case ID_BUTTON_start_disconnect_vpn:
			 			{

			 				wchar_t btnText[260];
			 				SendMessage(btn, WM_GETTEXT, 260, (LPARAM) btnText);
			 				if(NULL != wcsstr(btnText, L"Start the VPN"))
			 				{
			 					MessageBox(
								NULL,
								(LPCWSTR)L"btnText CONTAINS \"Start the VPN\"",
								(LPCWSTR)L"Eyyyy",
								MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
								);

								DWORD threadIdentifier;
								HANDLE thread;
								  thread = CreateThread(
									NULL,
									0,
									start,
									hwnd,
									0,
									&threadIdentifier); 

								 
			 				}

			 				if(NULL != wcsstr(btnText, L"Disconnect"))
			 				{
			 					MessageBox(
								NULL,
								(LPCWSTR)L"btnText CONTAINS Disconnect",
								(LPCWSTR)L"Eyyyy",
								MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
								);

								disconnect();
			 				}



  
			 				break;
			 			}
			 		  
			 			

			 			default:
			 				 break;
			 		}
			 	}
			 }

			return 0;
		}

		 

		case WM_CREATE:
		{

			 


			      btn = CreateWindow(
								 L"BUTTON", // Predefined class; Unicode assumed
								 (LPCWSTR)   L"Start the VPN", // Button text
								WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
								100, // x position ((WIDTH OF WINDOW / 2) - (WIDTH OF BUTTON / 2))
							//	10, // x position
								200, // y position
							//	40, // y position
								200, // Button width
								50, // Button height
								hwnd, // Parent window       // // // // EXPERIMENT
								// hwnd, // Parent window
								(HMENU) ID_BUTTON_start_disconnect_vpn, // No menu.
								(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
								NULL);  

			      btn2 = CreateWindow(
								 L"BUTTON", // Predefined class; Unicode assumed
								 (LPCWSTR)   L"User manual", // Button text
								WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
								100, // x position ((WIDTH OF WINDOW / 2) - (WIDTH OF BUTTON / 2))
								275, // ((HEIGHT OF WINDOW / 2) - (HEIGHT OF BUTTON / 2))								 
								200, // Button width
								50, // Button height
								hwnd, // Parent window       // // // // EXPERIMENT
								// hwnd, // Parent window
								(HMENU) ID_BUTTON_user_manual, // No menu.
								(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
								NULL);

			      btn3 = CreateWindow(
								 L"BUTTON", // Predefined class; Unicode assumed
								 (LPCWSTR)   L"About Us", // Button text
								WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
								100, // x position ((WIDTH OF WINDOW / 2) - (WIDTH OF BUTTON / 2))
								350, // y position
								// 200, // y position
								200, // Button width
								50, // Button height
								hwnd, // Parent window       // // // // EXPERIMENT
								// hwnd, // Parent window
								(HMENU) ID_BUTTON_about_us, // No menu.
								(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
								NULL);




			    LOGFONT lf; 
			    ZeroMemory(&lf, sizeof(lf));
			    // StringCchCopy(lf.lfFaceName, LF_FACESIZE, L"Ink Free");
			    //  StringCchCopy(lf.lfFaceName, LF_FACESIZE, L"Gabriola");
			      StringCchCopy(lf.lfFaceName, LF_FACESIZE, L"Calibri");
			    // StringCchCopy(lf.lfFaceName, LF_FACESIZE, L"AR CARTER");
			    // StringCchCopy(lf.lfFaceName, LF_FACESIZE, L"Times New Roman");
			    lf.lfWeight = FW_NORMAL;
			    HFONT hFont = CreateFontIndirect(&lf);
			    SendMessage(btn, WM_SETFONT, (WPARAM)hFont,  MAKELPARAM(TRUE, 0));
			    SendMessage(btn2, WM_SETFONT, (WPARAM)hFont,  MAKELPARAM(TRUE, 0));				   
			    SendMessage(btn3, WM_SETFONT, (WPARAM)hFont,  MAKELPARAM(TRUE, 0));	


				

							 
		 				 
				 



			return 0;
		}

		case WM_SIZE:
		{
			int width = LOWORD(lParam);

			SetWindowPos(btn, hwnd, (width / 2) - 100, 200, 0, 0, SWP_NOSIZE | SWP_NOZORDER);  // 100 is width of button / 2. 40 is the unchanged y position
			SetWindowPos(btn2, hwnd, (width / 2) - 100, 275, 0, 0, SWP_NOSIZE | SWP_NOZORDER);  // 100 is width of button / 2. 120 is the unchanged y position
			SetWindowPos(btn3, hwnd, (width / 2) - 100, 350, 0, 0, SWP_NOSIZE | SWP_NOZORDER);  // 100 is width of button / 2. 200 is the unchanged y position

			return S_OK;
		}

		case WM_PAINT:
		{
			    PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);
				EndPaint(hwnd, &ps);

				 




				return 0;
		}

		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}


			


	}


	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
PWSTR lpCmdLine, INT nCmdShow)
{

	 
	COLORREF rgbGreen = 0x00401210;
	HBRUSH hbrush = CreateSolidBrush(rgbGreen);
	 
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	// WNDCLASS  wc   = { };
	
    WNDCLASSEX wc = { };
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW; 
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;  


	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName =  CLASS_NAME;
	wc.hbrBackground = hbrush;

	 
	 
	RegisterClassEx(&wc);

	HWND hwnd = CreateWindowEx(
		0, // Optional window styles.
		 CLASS_NAME, // Window class
		 L"Alto Grade PPTP VPN client", // Window text
		   WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS, // Window style
		 // Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 600,
		// CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, // Parent window
		NULL, // Menu
		hInstance, // Instance handle
		NULL // Additional application data
		);

	ShowWindow(hwnd, nCmdShow);



	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}



	return 0;
}