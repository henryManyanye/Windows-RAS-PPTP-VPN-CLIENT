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
#pragma comment(lib, "RASAPI32.LIB")



// int main()
extern "C" int __declspec(dllexport) SampleMethod()
{
	 LPRASDIALPARAMS     lpRasDialParams = NULL;
	LPRASENTRY pRasEntry = NULL;

	DWORD dwCb = 0;
	DWORD dwDevices = 0;
	LPRASDEVINFO lpRasDevInfo = NULL;
	DWORD dwRet = ERROR_SUCCESS;

	dwRet = RasEnumDevices(lpRasDevInfo, &dwCb, &dwDevices);
	lpRasDevInfo = (LPRASDEVINFO) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);
	lpRasDevInfo[0].dwSize = sizeof(RASDEVINFO);
	dwRet = RasEnumDevices(lpRasDevInfo, &dwCb, &dwDevices);
	if (ERROR_SUCCESS == dwRet){
		printf("The following RAS devices were found:\n");
		// for (DWORD i = 0; i < dwDevices; i++){
		wprintf(L"%s: ", lpRasDevInfo[1].szDeviceName);
		wprintf(L"%s\n ", lpRasDevInfo[1].szDeviceType);
		// }
	}

	DWORD cbb = sizeof(RASDIALPARAMS);
	lpRasDialParams = (LPRASDIALPARAMS)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cbb); 
	pRasEntry = (LPRASENTRY)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(RASENTRY));


		

	 pRasEntry->dwSize = sizeof (RASENTRY);
	printf("HERE\n");
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
	hr = StringCchCopy(pRasEntry->szDeviceName, ARRAYSIZE(pRasEntry->szDeviceName), lpRasDevInfo[1].szDeviceName);
	if(FAILED(hr))
	{
		printf("FAILED hr 2\n");
	}
	hr = StringCchCopy(pRasEntry->szDeviceType, ARRAYSIZE(pRasEntry->szDeviceType), lpRasDevInfo[1].szDeviceType);
	if(FAILED(hr))
	{
		printf("FAILED hr 3\n");
	}
	


	DWORD dwErr = RasSetEntryProperties(NULL,
		 L"myVpnEntry",
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
	StringCchCopy(lpRasDialParams->szEntryName, ARRAYSIZE(lpRasDialParams->szEntryName), L"myVpnEntry");
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
	 

	HRASCONN hRasConn = NULL;
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
	}else{
		printf("RasDial Failed %d\n", dwErr);
		RasHangUp(hRasConn);
	}

	if(hRasConn == NULL)
	{
		printf("HRASCONN == NULL\n");
	}

	/*********** EXPERIMENT WORKS ***********************/
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
	    		printf("RASCN_Disconnection Notification event signalled");
	    		break;

	    	default:
	            printf("WaitForSingleObject returned %d\n", nRet);
	            break;
	    }
	/**********************************************/

	








	






	return 0;
}