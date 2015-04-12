/*#include <winsock2.h>
#include <ws2bth.h>
#include <BluetoothAPIs.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "irprops.lib")

using namespace std;
int main(){

	//Initialising winsock
	WSADATA data;
	int result;
	result = WSAStartup(MAKEWORD(2, 2), &data);//initializing winsock
	if (result != 0){
		cout << "An error occured while initialising winsock, closing....";
		exit(result);
	}



	//Initialising query for device

	WSAQUERYSET queryset;
	memset(&queryset, 0, sizeof(WSAQUERYSET));
	queryset.dwSize = sizeof(WSAQUERYSET);
	queryset.dwNameSpace = NS_BTH;



	HANDLE hLookup;
	result = WSALookupServiceBegin(&queryset, LUP_CONTAINERS, &hLookup);
	if (result != 0){
		cout << "An error occured while initialising look for devices, closing....";
		exit(result);
	}



	//Initialisation succedd, start looking for devices

	BYTE buffer[4096];
	memset(buffer, 0, sizeof(buffer));
	DWORD bufferLength = sizeof(buffer);
	WSAQUERYSET *pResults = (WSAQUERYSET*)&buffer;
	while (result == 0){
		result = WSALookupServiceNext(hLookup, LUP_RETURN_NAME | LUP_CONTAINERS | LUP_RETURN_ADDR | LUP_FLUSHCACHE | LUP_RETURN_TYPE | LUP_RETURN_BLOB | LUP_RES_SERVICE, &bufferLength, pResults);
		if (result == 0){// A device found

			//print the name of the device
			LPTSTR s = pResults->lpszServiceInstanceName;
			wcout << s << "found. quering for services\n";

			//Initialise quering the device services
			WSAQUERYSET queryset2;
			memset(&queryset2, 0, sizeof(queryset2));
			queryset2.dwSize = sizeof(queryset2);
			queryset2.dwNameSpace = NS_BTH;
			queryset2.dwNumberOfCsAddrs = 0;
			
			CSADDR_INFO * addr = (CSADDR_INFO *)pResults->lpcsaBuffer;
			WCHAR addressAsString[1000];
			DWORD addressSize = sizeof(addressAsString);

			WSAAddressToString(addr->RemoteAddr.lpSockaddr, addr->RemoteAddr.iSockaddrLength, NULL, addressAsString, &addressSize);
			queryset2.lpszContext = addressAsString;
			GUID protocol = L2CAP_PROTOCOL_UUID;
			queryset2.lpServiceClassId = &protocol;
			HANDLE hLookup2;
			int result2 = WSALookupServiceBegin(&queryset2, LUP_FLUSHCACHE | LUP_RETURN_NAME | LUP_RETURN_TYPE | LUP_RETURN_ADDR | LUP_RETURN_BLOB | LUP_RETURN_COMMENT, &hLookup2);
			if (result2 != 0){
				cout << "An error occured while initializing query for services";
				exit(result);
			}



			//Start quering for device services

			while (result2 == 0){
				BYTE buffer2[4096];
				memset(buffer2, 0, sizeof(buffer2));
				DWORD bufferLength2 = sizeof(buffer2);
				WSAQUERYSET *pResults2 = (WSAQUERYSET*)&buffer2;
				result2 = WSALookupServiceNext(hLookup2, LUP_FLUSHCACHE | LUP_RETURN_NAME | LUP_RETURN_TYPE | LUP_RETURN_ADDR | LUP_RETURN_BLOB | LUP_RETURN_COMMENT, &bufferLength2, pResults2);
				if (result2 == 0)
					wcout << "Service found: " << pResults2->lpszServiceInstanceName << "\n";
			}
		}
	}
	return 0;
}


*/

/*

#include <winsock2.h>
#include <ws2bth.h>
#include <BluetoothAPIs.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "irprops.lib")

SOCKADDR_BTH sockAddr;
SOCKET btSocket;
int error;

int main(void){
	btSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.addressFamily = AF_BTH;
	sockAddr.serviceClassId = RFCOMM_PROTOCOL_UUID;
	sockAddr.port = BT_PORT_ANY;
	sockAddr.btAddr = 0x00066661DCE1;

	int i = 0;
	do{
		error = connect(btSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
		i++;
		if (error != -1)
			std::cout << "YEAH" << std::endl;
	} while (i < 10000 || error != -1);

	return 0;
}

*/


#include <windows.h>
#include "bthdef.h"
#include "BluetoothAPIs.h"
#include <tchar.h>
#include <string>
#include <iostream>
#include <vector>
#include <ws2bth.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "irprops.lib")
#pragma comment(lib, "bthprops.lib")

using namespace std;

BLUETOOTH_DEVICE_SEARCH_PARAMS bdsp;
BLUETOOTH_DEVICE_INFO bdi;
HBLUETOOTH_DEVICE_FIND hbf;

vector<BLUETOOTH_DEVICE_INFO> scanDevices()
{
	vector<BLUETOOTH_DEVICE_INFO> res;
	
	ZeroMemory(&bdsp, sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS));

	// set options for how we want to load our list of BT devices
	bdsp.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
	bdsp.fReturnAuthenticated = TRUE;
	bdsp.fReturnRemembered = TRUE;
	bdsp.fReturnUnknown = TRUE;
	bdsp.fReturnConnected = TRUE;
	bdsp.fIssueInquiry = TRUE;
	bdsp.cTimeoutMultiplier = 4;
	bdsp.hRadio = NULL;

	bdi.dwSize = sizeof(bdi);

	// enumerate our bluetooth devices
	hbf = BluetoothFindFirstDevice(&bdsp, &bdi);
	if (hbf)
	{
		do
		{
			res.push_back(bdi);
		} while (BluetoothFindNextDevice(hbf, &bdi));

		// close our device enumerator
		BluetoothFindDeviceClose(hbf);
	}

	return res;
}

BOOL CALLBACK bluetoothAuthCallback(LPVOID param, PBLUETOOTH_AUTHENTICATION_CALLBACK_PARAMS params)
{
	cout << "callback happened" << endl;
	return TRUE;
}

void pairDevice(BLUETOOTH_DEVICE_INFO device)
{
	wstring ws = device.szName;
	cout << "Pairing device " << string(ws.begin(), ws.end()) << endl;

	// register callback
	cout << "Registering callback" << endl;
	HBLUETOOTH_AUTHENTICATION_REGISTRATION hCallbackHandle = 0;
	DWORD result = BluetoothRegisterForAuthenticationEx(&device, &hCallbackHandle, (PFN_AUTHENTICATION_CALLBACK_EX)&bluetoothAuthCallback, NULL);
	if (result != ERROR_SUCCESS)
	{
		cout << "Failed to register callback" << endl;
		return;
	}

	// authenticate
	//result = BluetoothAuthenticateDeviceEx(NULL, NULL, &device, NULL, MITMProtectionNotRequired);
	result = BluetoothAuthenticateDeviceEx(NULL, NULL, &device, NULL, MITMProtectionRequired);
	//result = BluetoothAuthenticateDeviceEx(NULL, NULL, &device, NULL, MITMProtectionNotRequiredBonding);
	//result = BluetoothAuthenticateDeviceEx(NULL, NULL, &device, NULL, MITMProtectionRequiredBonding);
	//result = BluetoothAuthenticateDeviceEx(NULL, NULL, &device, NULL, MITMProtectionNotRequiredGeneralBonding);
	//result = BluetoothAuthenticateDeviceEx(NULL, NULL, &device, NULL, MITMProtectionRequiredGeneralBonding);
	//result = BluetoothAuthenticateDeviceEx(NULL, NULL, &device, NULL, MITMProtectionNotDefined);
	switch (result)
	{
	case ERROR_SUCCESS:
		cout << "pair device success" << endl;
		break;

	case ERROR_CANCELLED:
		cout << "pair device failed, user cancelled" << endl;
		break;

	case ERROR_INVALID_PARAMETER:
		cout << "pair device failed, invalid parameter" << endl;
		break;

	case ERROR_NO_MORE_ITEMS:
		cout << "pair device failed, device appears paired already" << endl;
		break;

	default:
		cout << "pair device failed, unknown error, code " << (unsigned int)result << endl;
		break;
	}
}

int _tmain(int argc, _TCHAR *argv[])
{
	cout << "Scanning bluetooth devices..." << endl;
	cout.flush();

	// scan devices
	vector<BLUETOOTH_DEVICE_INFO> devices = scanDevices();

	cout << "Got " << devices.size() << " devices" << endl;

	// list all devices
	int pdIndex = -1;
	int foundDev = -1;
	vector<BLUETOOTH_DEVICE_INFO>::const_iterator devci;
	for (devci = devices.begin(); devci != devices.end(); devci++)
	{
		pdIndex++;
		wstring ws = (*devci).szName;
		cout << "Device: " << string(ws.begin(), ws.end()) << endl;

		// see if we find our device (case sensitive)
		if (ws.find(L"RNBT-DCE1") != string::npos)
			foundDev = pdIndex;
	}

	// pick our ismp device
	if (foundDev == -1)
	{
		cout << "Could not find a device to pair" << endl;
		return 1;
	}

	BLUETOOTH_DEVICE_INFO pd = devices[foundDev];
	wstring ws = pd.szName;
	cout << "Found device to pair, " << string(ws.begin(), ws.end()) << endl;

	// attempt to pair device
	//pairDevice(pd);

	WSADATA wsd; int name; int err;
	WSAStartup(MAKEWORD(1, 0), &wsd);

	SOCKET server_socket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);

	SOCKADDR_BTH sa;
	memset(&sa, 0, sizeof(sa));
	sa.addressFamily = AF_BTH;
	sa.port = BT_PORT_ANY;
	BTH_ADDR add = (BTH_ADDR) 0x00066661DCE1;
	sa.btAddr = 0;

	if (bind(server_socket, (SOCKADDR *)&sa, sizeof(sa)))
	{
		  //Perform error handling
		err = WSAGetLastError();
		closesocket(server_socket);
		//return 0;
	}
	//err = getsockname(server_socket, (SOCKADDR*)&sa, &name);
	sa.btAddr = add;
	err = connect(server_socket, (SOCKADDR*)&sa, sizeof(sa));


	getchar();
	return 0;
}





/*

#include <stdio.h>
#include <initguid.h>
// Link to ws2_32.lib
#include <winsock2.h>
#include <ws2bth.h>

// {B62C4E8D-62CC-404b-BBBF-BF3E3BBB1374}
DEFINE_GUID(g_guidServiceClass, 0xb62c4e8d, 0x62cc, 0x404b, 0xbb, 0xbf, 0xbf, 0x3e, 0x3b, 0xbb, 0x13, 0x74);

#define CXN_BDADDR_STR_LEN                17   // 6 two-digit hex values plus 5 colons
#define CXN_TRANSFER_DATA_LENGTH          100  // length of the data to be transferred
#define CXN_MAX_INQUIRY_RETRY             3
#define CXN_DELAY_NEXT_INQUIRY            15

char g_szRemoteName[BTH_MAX_NAME_SIZE + 1] = { 0 };  // 1 extra for trailing NULL character
char g_szRemoteAddr[CXN_BDADDR_STR_LEN + 1] = { 0 }; // 1 extra for trailing NULL character
// This just redundant!!!!
int  g_ulMaxCxnCycles = 1, g_iOutputLevel = 0;

ULONG NameToBthAddr(const char * pszRemoteName, BTH_ADDR * pRemoteBthAddr);
ULONG AddrStringToBtAddr(IN const char * pszRemoteAddr, OUT BTH_ADDR * pRemoteBtAddr);
// ULONG RunClientMode( ULONGLONG ululRemoteBthAddr,  int iMaxCxnCycles = 1);
ULONG RunClientMode(ULONGLONG ululRemoteBthAddr, int iMaxCxnCycles);
// ULONG RunServerMode( int iMaxCxnCycles = 1);
ULONG RunServerMode(int iMaxCxnCycles);
void  ShowCmdLineHelp(void);
ULONG ParseCmdLine(int argc, char * argv[]);

int main(int argc, char *argv[])
{
	ULONG      ulRetCode = 0;
	WSADATA    WSAData = { 0 };
	ULONGLONG  ululRemoteBthAddr = 0;

	ShowCmdLineHelp();
	// Parse the command line
	if ((ulRetCode = ParseCmdLine(argc, argv)) == 0)
	{
		// Ask for Winsock version 2.2.
		if ((ulRetCode = WSAStartup(MAKEWORD(2, 2), &WSAData)) != 0)
		{
			printf("-FATAL- | Unable to initialize Winsock version 2.2\n");
			goto CleanupAndExit;
		}
		else
			printf("WSAStartup() is OK!\n");

		if (g_szRemoteName[0] != '\0')
		{
			printf("Running in the client mode...\n");
			// Get address from name of the remote device and run the application in client mode
			if ((ulRetCode = NameToBthAddr(g_szRemoteName, (BTH_ADDR *)&ululRemoteBthAddr)) != 0)
			{
				printf("-FATAL- | Unable to get address of the remote radio having name %s\n", g_szRemoteName);
				goto CleanupAndExit;
			}

			ulRetCode = RunClientMode(ululRemoteBthAddr, g_ulMaxCxnCycles);
		}
		else if (g_szRemoteAddr[0] != '\0')
		{
			printf("Running in the client mode...\n");
			// Get address from formatted address-string of the remote device and run the application in client mode
			//  should be calling the WSAStringToAddress()
			if (0 != (ulRetCode = AddrStringToBtAddr(g_szRemoteAddr, (BTH_ADDR *)&ululRemoteBthAddr)))
			{
				printf("-FATAL- | Unable to get address of the remote radio having formatted address-string %s\n", g_szRemoteAddr);
				goto CleanupAndExit;
			}

			ulRetCode = RunClientMode(ululRemoteBthAddr, g_ulMaxCxnCycles);
		}
		else
		{
			// No remote name/address specified.  Run the application in server mode
			printf("No remote name/address specified, running the server mode...\n");
			ulRetCode = RunServerMode(g_ulMaxCxnCycles);
		}
	}
	else if (ulRetCode == 1)
	{
		// Command line syntax error.  Display cmd line help
		ShowCmdLineHelp();
	}
	else
	{
		printf("-FATAL- | Error in parsing the command line!\n");
	}

CleanupAndExit:
	return (int)(ulRetCode);
}

// This just redundant!!!!
//
// TODO: use inquiry timeout SDP_DEFAULT_INQUIRY_SECONDS
// NameToBthAddr converts a bluetooth device name to a bluetooth address,
// if required by performing inquiry with remote name requests.
// This function demonstrates device inquiry, with optional LUP flags.
ULONG NameToBthAddr(const char * pszRemoteName, BTH_ADDR * pRemoteBtAddr)
{
	INT          iResult = 0, iRetryCount = 0;
	BOOL         bContinueLookup = FALSE, bRemoteDeviceFound = FALSE;
	ULONG        ulFlags = 0, ulPQSSize = sizeof(WSAQUERYSET);
	HANDLE       hLookup = 0;
	PWSAQUERYSET pWSAQuerySet = NULL;

	if ((pszRemoteName == NULL) || (pRemoteBtAddr == NULL))
	{
		printf("Remote name or address is NULL!\n");
		goto CleanupAndExit;
	}

	if ((pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulPQSSize)) == NULL)
	{
		printf("!ERROR! | Unable to allocate memory for WSAQUERYSET!\n");
		goto CleanupAndExit;
	}
	else
		printf("HeapAlloc() for WSAQUERYSET is OK!\n");

	// Search for the device with the correct name
	for (iRetryCount = 0; !bRemoteDeviceFound && (iRetryCount < CXN_MAX_INQUIRY_RETRY); iRetryCount++)
	{
		// WSALookupServiceXXX() is used for both service search and device inquiry
		// LUP_CONTAINERS is the flag which signals that we're doing a device inquiry. 
		ulFlags = LUP_CONTAINERS;
		// Friendly device name (if available) will be returned in lpszServiceInstanceName
		ulFlags |= LUP_RETURN_NAME;
		// BTH_ADDR will be returned in lpcsaBuffer member of WSAQUERYSET
		ulFlags |= LUP_RETURN_ADDR;
		// Similar to:
		// ulFlags = LUP_CONTAINERS | LUP_RETURN_NAME | LUP_RETURN_ADDR;

		if (iRetryCount == 0)
		{
			printf("*INFO* | Inquiring device from cache...\n");
		}
		else
		{
			// Flush the device cache for all inquiries, except for the first inquiry
			//
			// By setting LUP_FLUSHCACHE flag, we're asking the lookup service to do
			// a fresh lookup instead of pulling the information from device cache.
			ulFlags |= LUP_FLUSHCACHE;

			// Pause for some time before all the inquiries after the first inquiry
			//
			// Remote Name requests will arrive after device inquiry has
			// completed.  Without a window to receive IN_RANGE notifications,
			// we don't have a direct mechanism to determine when remote
			// name requests have completed.
			printf("*INFO* | Unable to find device.  Waiting for %d seconds before re-inquiry...\n", CXN_DELAY_NEXT_INQUIRY);
			printf("I am sleeping for a while...\n");
			Sleep(CXN_DELAY_NEXT_INQUIRY * 1000);

			printf("*INFO* | Inquiring device ...\n");
		}

		// Start the lookup service
		iResult = 0;
		hLookup = 0;
		bContinueLookup = FALSE;
		ZeroMemory(pWSAQuerySet, ulPQSSize);
		pWSAQuerySet->dwNameSpace = NS_BTH;
		pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);

		iResult = WSALookupServiceBegin(pWSAQuerySet, ulFlags, &hLookup);

		if ((iResult == NO_ERROR) && (hLookup != NULL))
		{
			printf("WSALookupServiceBegin() is fine!\n");
			bContinueLookup = TRUE;
		}
		else if (0 < iRetryCount)
		{
			printf("=CRITICAL= | WSALookupServiceBegin() failed with error code %d, Error = %d\n", iResult, WSAGetLastError());
			goto CleanupAndExit;
		}

		while (bContinueLookup)
		{
			// Get information about next bluetooth device
			//
			// Note you may pass the same WSAQUERYSET from LookupBegin
			// as long as you don't need to modify any of the pointer
			// members of the structure, etc.
			//
			// ZeroMemory(pWSAQuerySet, ulPQSSize);
			// pWSAQuerySet->dwNameSpace = NS_BTH;
			// pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);
			if (WSALookupServiceNext(hLookup, ulFlags, &ulPQSSize, pWSAQuerySet) == NO_ERROR)
			{
				printf("WSALookupServiceNext() is OK lol!\n");

				if ((pWSAQuerySet->lpszServiceInstanceName != NULL))
				{
					// Found a remote bluetooth device with matching name.
					// Get the address of the device and exit the lookup.
					printf("Again, remote name: %S\n", pWSAQuerySet->lpszServiceInstanceName);
					// Need to convert to the 'standard' address format
					printf("Local address: %012X\n", pWSAQuerySet->lpcsaBuffer->LocalAddr);
					printf("Remote address: %012X\n", pWSAQuerySet->lpcsaBuffer->RemoteAddr);
					CopyMemory(pRemoteBtAddr, &((PSOCKADDR_BTH)pWSAQuerySet->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr,
						sizeof(*pRemoteBtAddr));
					bRemoteDeviceFound = TRUE;
					bContinueLookup = FALSE;
				}
			}
			else
			{
				if ((iResult = WSAGetLastError()) == WSA_E_NO_MORE) //No more data
				{
					// No more devices found.  Exit the lookup.
					printf("No more device found...\n");
					bContinueLookup = FALSE;
				}
				else if (iResult == WSAEFAULT)
				{
					// The buffer for QUERYSET was insufficient.
					// In such case 3rd parameter "ulPQSSize" of function "WSALookupServiceNext()" receives
					// the required size.  So we can use this parameter to reallocate memory for QUERYSET.
					HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
					pWSAQuerySet = NULL;
					if (NULL == (pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulPQSSize)))
					{
						printf("!ERROR! | Unable to allocate memory for WSAQERYSET...\n");
						bContinueLookup = FALSE;
					}
					else
						printf("HeapAlloc() for WSAQERYSET is OK!\n");
				}
				else
				{
					printf("=CRITICAL= | WSALookupServiceNext() failed with error code %d\n", iResult);
					bContinueLookup = FALSE;
				}
			}
		}
		// End the lookup service
		WSALookupServiceEnd(hLookup);
	}

CleanupAndExit:
	if (NULL != pWSAQuerySet)
	{
		HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
		pWSAQuerySet = NULL;
	}

	if (bRemoteDeviceFound)
	{
		return(0);
	}
	else
	{
		return(1);
	}
}

// This just redundant!!!!
//
// Convert a formatted BTH address string to populate a BTH_ADDR (actually a ULONGLONG)
ULONG AddrStringToBtAddr(const char * pszRemoteAddr, BTH_ADDR * pRemoteBtAddr)
{
	ULONG ulAddrData[6], ulRetCode = 0;
	BTH_ADDR BtAddrTemp = 0;
	int i;

	if ((pszRemoteAddr == NULL) || (pRemoteBtAddr == NULL))
	{
		ulRetCode = 1;
		goto CleanupAndExit;
	}

	*pRemoteBtAddr = 0;

	// Populate a 6 membered array of unsigned long integers
	// by parsing the given address in string format
	sscanf_s(pszRemoteAddr, "%02x:%02x:%02x:%02x:%02x:%02x", &ulAddrData[0], &ulAddrData[1], &ulAddrData[2],
		&ulAddrData[3], &ulAddrData[4], &ulAddrData[5]);

	// Construct a BTH_ADDR from 6 integers stored in the array
	printf("Constructing the BTH_ADDR...\n");
	for (i = 0; i<6; i++)
	{
		// Extract data from the first 8 lower bits.
		BtAddrTemp = (BTH_ADDR)(ulAddrData[i] & 0xFF);
		// Push 8 bits to the left
		*pRemoteBtAddr = ((*pRemoteBtAddr) << 8) + BtAddrTemp;
	}
	printf("Remote address: 0X%X\n", pRemoteBtAddr);

CleanupAndExit:
	return ulRetCode;
}

// RunClientMode runs the application in client mode.  It opens a socket, connects it to a
// remote socket, transfer some data over the connection and closes the connection.
ULONG RunClientMode(ULONGLONG ululRemoteAddr, int iMaxCxnCycles)
{
	ULONG              ulRetCode = 0;
	int                iCxnCount = 0;
	char               szData[CXN_TRANSFER_DATA_LENGTH] = { 0 };
	SOCKET             LocalSocket = INVALID_SOCKET;
	SOCKADDR_BTH       SockAddrBthServer = { 0 };

	// Setting address family to AF_BTH indicates winsock2 to use Bluetooth sockets
	// Port should be set to 0 if ServiceClassId is spesified.
	SockAddrBthServer.addressFamily = AF_BTH;
	SockAddrBthServer.btAddr = (BTH_ADDR)ululRemoteAddr;
	SockAddrBthServer.serviceClassId = g_guidServiceClass;
	// Valid ports are 1 - 31
	SockAddrBthServer.port = 1;

	// Create a static data-string, which will be transferred to the remote Bluetooth device
	//  may make this #define and do strlen() of the string
	strncpy_s(szData, sizeof(szData), "~!@#$%^&*()-_=+?<>1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
		CXN_TRANSFER_DATA_LENGTH - 1);

	// Run the connection/data-transfer for user specified number of cycles
	for (iCxnCount = 0; (0 == ulRetCode) && (iCxnCount < iMaxCxnCycles || iMaxCxnCycles == 0); iCxnCount++)
	{
		printf("\n");

		// Open a bluetooth socket using RFCOMM protocol
		if ((LocalSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM)) == INVALID_SOCKET)
		{
			printf("=CRITICAL= | socket() call failed. Error = [%d]\n", WSAGetLastError());
			ulRetCode = 1;
			goto CleanupAndExit;
		}

		if ((2 <= g_iOutputLevel) | (LocalSocket != INVALID_SOCKET))
		{
			printf("*INFO* | socket() call succeeded. Socket = [0x%X]\n", LocalSocket);
		}

		if (1 <= g_iOutputLevel)
		{
			printf("*INFO* | connect() attempt with Remote BTHAddr = [0x%X]\n", ululRemoteAddr);
		}

		// Connect the socket (pSocket) to a given remote socket represented by address (pServerAddr)
		if (connect(LocalSocket, (struct sockaddr *) &SockAddrBthServer, sizeof(SOCKADDR_BTH)) == SOCKET_ERROR)
		{
			printf("=CRITICAL= | connect() call failed. Error=[%d]\n", WSAGetLastError());
			ulRetCode = 1;
			goto CleanupAndExit;
		}

		if ((2 <= g_iOutputLevel) | (connect(LocalSocket, (struct sockaddr *) &SockAddrBthServer, sizeof(SOCKADDR_BTH)) != SOCKET_ERROR))
		{
			printf("*INFO* | connect() call succeeded!\n");
		}

		// send() call indicates winsock2 to send the given data
		// of a specified length over a given connection.
		printf("*INFO* | Sending the following data string:\n\t%s\n", szData);
		if (send(LocalSocket, szData, CXN_TRANSFER_DATA_LENGTH, 0) == SOCKET_ERROR)
		{
			printf("=CRITICAL= | send() call failed w/socket = [0x%X], szData = [%p], dataLen = [%d]. WSAGetLastError=[%d]\n",
				LocalSocket, szData, CXN_TRANSFER_DATA_LENGTH, WSAGetLastError());
			ulRetCode = 1;
			goto CleanupAndExit;
		}

		if (2 <= g_iOutputLevel)
		{
			printf("*INFO* | send() call succeeded\n");
		}

		// Close the socket
		if (SOCKET_ERROR == closesocket(LocalSocket))
		{
			printf("=CRITICAL= | closesocket() call failed w/socket = [0x%X]. Error=[%d]\n", LocalSocket, WSAGetLastError());
			ulRetCode = 1;
			goto CleanupAndExit;
		}

		LocalSocket = INVALID_SOCKET;

		if (2 <= g_iOutputLevel)
		{
			printf("*INFO* | closesocket() call succeeded!");
		}
	}

CleanupAndExit:
	if (LocalSocket != INVALID_SOCKET)
	{
		closesocket(LocalSocket);
		LocalSocket = INVALID_SOCKET;
	}

	return ulRetCode;
}
// RunServerMode runs the application in server mode.  It opens a socket, connects it to a
// remote socket, transfer some data over the connection and closes the connection.
ULONG RunServerMode(int iMaxCxnCycles)
{
	ULONG              ulRetCode = 0;
	int                iAddrLen = sizeof(SOCKADDR_BTH), iCxnCount = 0, iLengthReceived = 0, iTotalLengthReceived = 0;
	char               szDataBuffer[CXN_TRANSFER_DATA_LENGTH] = { 0 };
	char *             pszDataBufferIndex = NULL;
	LPTSTR             lptstrThisComputerName = NULL;
	DWORD              dwLenComputerName = MAX_COMPUTERNAME_LENGTH + 1;
	SOCKET             LocalSocket = INVALID_SOCKET, ClientSocket = INVALID_SOCKET;
	WSAVERSION         wsaVersion = { 0 };
	WSAQUERYSET        wsaQuerySet = { 0 };
	SOCKADDR_BTH       SockAddrBthLocal = { 0 };
	LPCSADDR_INFO      lpCSAddrInfo = NULL;
	BOOL bContinue;

	// Both of these fixed-size allocations can be on the stack
	if ((lpCSAddrInfo = (LPCSADDR_INFO)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(CSADDR_INFO))) == NULL)
	{
		printf("!ERROR! | Unable to allocate memory for CSADDR_INFO\n");
		ulRetCode = 1;
		goto CleanupAndExit;
	}
	else
		printf("HeapAlloc() for CSADDR_INFO (address) is OK!\n");

	if ((lptstrThisComputerName = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwLenComputerName)) == NULL)
	{
		printf("!ERROR! | Unable to allocate memory for CSADDR_INFO\n");
		ulRetCode = 1;
		goto CleanupAndExit;
	}
	else
		printf("HeapAlloc() for CSADDR_INFO (local computer name) is OK!\n");

	if (!GetComputerName(lptstrThisComputerName, &dwLenComputerName))
	{
		printf("=CRITICAL= | GetComputerName() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
		ulRetCode = 1;
		goto CleanupAndExit;
	}
	else
	{
		printf("GetComputerName() is pretty fine!\n");
		printf("Local computer name: %S\n", lptstrThisComputerName);
	}

	// Open a bluetooth socket using RFCOMM protocol
	printf("Opening local socket using socket()...\n");
	if ((LocalSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM)) == INVALID_SOCKET)
	{
		printf("=CRITICAL= | socket() call failed. WSAGetLastError = [%d]\n", WSAGetLastError());
		ulRetCode = 1;
		goto CleanupAndExit;
	}

	if ((2 <= g_iOutputLevel) | (LocalSocket != INVALID_SOCKET))
	{
		printf("*INFO* | socket() call succeeded! Socket = [0x%X]\n", LocalSocket);
	}

	// Setting address family to AF_BTH indicates winsock2 to use Bluetooth port
	SockAddrBthLocal.addressFamily = AF_BTH;
	// Valid ports are 1 - 31
	// SockAddrBthLocal.port = BT_PORT_ANY;
	SockAddrBthLocal.port = 1;

	// bind() associates a local address and port combination
	// with the socket just created. This is most useful when
	// the application is a server that has a well-known port
	// that clients know about in advance.
	if (bind(LocalSocket, (struct sockaddr *) &SockAddrBthLocal, sizeof(SOCKADDR_BTH)) == SOCKET_ERROR)
	{
		printf("=CRITICAL= | bind() call failed w/socket = [0x%X]. Error=[%d]\n", LocalSocket, WSAGetLastError());
		ulRetCode = 1;
		goto CleanupAndExit;
	}

	if ((2 <= g_iOutputLevel) | (bind(LocalSocket, (struct sockaddr *) &SockAddrBthLocal, sizeof(SOCKADDR_BTH)) != SOCKET_ERROR))
	{
		printf("*INFO* | bind() call succeeded!\n");
	}

	if ((ulRetCode = getsockname(LocalSocket, (struct sockaddr *)&SockAddrBthLocal, &iAddrLen)) == SOCKET_ERROR)
	{
		printf("=CRITICAL= | getsockname() call failed w/socket = [0x%X]. WSAGetLastError=[%d]\n", LocalSocket, WSAGetLastError());
		ulRetCode = 1;
		goto CleanupAndExit;
	}
	else
	{
		printf("getsockname() is pretty fine!\n");
		printf("Local address: 0x%x\n", SockAddrBthLocal.btAddr);
	}

	// CSADDR_INFO
	lpCSAddrInfo[0].LocalAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
	lpCSAddrInfo[0].LocalAddr.lpSockaddr = (LPSOCKADDR)&SockAddrBthLocal;
	lpCSAddrInfo[0].RemoteAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
	lpCSAddrInfo[0].RemoteAddr.lpSockaddr = (LPSOCKADDR)&SockAddrBthLocal;
	lpCSAddrInfo[0].iSocketType = SOCK_STREAM;
	lpCSAddrInfo[0].iProtocol = BTHPROTO_RFCOMM;

	// If we got an address, go ahead and advertise it.
	ZeroMemory(&wsaQuerySet, sizeof(WSAQUERYSET));
	wsaQuerySet.dwSize = sizeof(WSAQUERYSET);
	wsaQuerySet.lpServiceClassId = (LPGUID)&g_guidServiceClass;
	// should be something like "Sample Bluetooth Server"
	wsaQuerySet.lpszServiceInstanceName = lptstrThisComputerName;
	wsaQuerySet.lpszComment = "Example Service instance registered in the directory service through RnR";
	wsaQuerySet.dwNameSpace = NS_BTH;
	wsaQuerySet.dwNumberOfCsAddrs = 1;      // Must be 1.
	wsaQuerySet.lpcsaBuffer = lpCSAddrInfo; // Required.

	// As long as we use a blocking accept(), we will have a race
	// between advertising the service and actually being ready to
	// accept connections.  If we use non-blocking accept, advertise
	// the service after accept has been called.
	if (WSASetService(&wsaQuerySet, RNRSERVICE_REGISTER, 0) == SOCKET_ERROR)
	{
		printf("=CRITICAL= | WSASetService() call failed. Error=[%d]\n", WSAGetLastError());
		ulRetCode = 1;
		goto CleanupAndExit;
	}
	else
		printf("WSASetService() looks fine!\n");

	// listen() call indicates winsock2 to listen on a given socket for any incoming connection.
	if (listen(LocalSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("=CRITICAL= | listen() call failed w/socket = [0x%X]. Error=[%d]\n", LocalSocket, WSAGetLastError());
		ulRetCode = 1;
		goto CleanupAndExit;
	}

	if ((2 <= g_iOutputLevel) | (listen(LocalSocket, SOMAXCONN) != SOCKET_ERROR))
	{
		printf("*INFO* | listen() call succeeded!\n");
	}

	for (iCxnCount = 0; (0 == ulRetCode) && ((iCxnCount < iMaxCxnCycles) || (iMaxCxnCycles == 0)); iCxnCount++)
	{
		printf("\n");

		// accept() call indicates winsock2 to wait for any 
		// incoming connection request from a remote socket.
		// If there are already some connection requests on the queue,
		// then accept() extracts the first request and creates a new socket and
		// returns the handle to this newly created socket. This newly created
		// socket represents the actual connection that connects the two sockets.
		if ((ClientSocket = accept(LocalSocket, NULL, NULL)) == INVALID_SOCKET)
		{
			printf("=CRITICAL= | accept() call failed. Error=[%d]\n", WSAGetLastError());
			ulRetCode = 1;
			break; // Break out of the for loop
		}

		if ((2 <= g_iOutputLevel) | (ClientSocket != INVALID_SOCKET))
		{
			printf("*INFO* | accept() call succeeded. CientSocket = [0x%X]\n", ClientSocket);
		}

		// Read data from the incoming stream
		bContinue = TRUE;
		pszDataBufferIndex = &szDataBuffer[0];
		while (bContinue && (iTotalLengthReceived < CXN_TRANSFER_DATA_LENGTH))
		{
			// recv() call indicates winsock2 to receive data
			// of an expected length over a given connection.
			// recv() may not be able to get the entire length
			// of data at once.  In such case the return value,
			// which specifies the number of bytes received,
			// can be used to calculate how much more data is
			// pending and accordingly recv() can be called again.
			iLengthReceived = recv(ClientSocket, pszDataBufferIndex, (CXN_TRANSFER_DATA_LENGTH - iTotalLengthReceived), 0);

			switch (iLengthReceived)
			{
			case 0: // socket connection has been closed gracefully
				printf("Socket connection has been closed gracefully!\n");
				bContinue = FALSE;
				break;
			case SOCKET_ERROR:
				printf("=CRITICAL= | recv() call failed. Error=[%d]\n", WSAGetLastError());
				bContinue = FALSE;
				ulRetCode = 1;
				break;
			default: // most cases when data is being read
				pszDataBufferIndex += iLengthReceived;
				iTotalLengthReceived += iLengthReceived;
				if ((2 <= g_iOutputLevel) | (iLengthReceived != SOCKET_ERROR))
				{
					printf("*INFO* | Receiving data of length = [%d]. Current Total = [%d]\n", iLengthReceived, iTotalLengthReceived);
				}
				break;
			}
		}

		if (ulRetCode == 0)
		{
			if (CXN_TRANSFER_DATA_LENGTH != iTotalLengthReceived)
			{
				printf("+WARNING+ | Data transfer aborted mid-stream. Expected Length = [%d], Actual Length = [%d]\n",
					CXN_TRANSFER_DATA_LENGTH, iTotalLengthReceived);
			}

			printf("*INFO* | Received following data string from remote device:\n%s\n", szDataBuffer);

			// Close the connection
			if (closesocket(ClientSocket) == SOCKET_ERROR)
			{
				printf("=CRITICAL= | closesocket() call failed w/socket = [0x%X]. Error=[%d]\n", LocalSocket, WSAGetLastError());
				ulRetCode = 1;
			}
			else
			{
				// Make the connection invalid regardless
				ClientSocket = INVALID_SOCKET;

				if ((2 <= g_iOutputLevel) | (closesocket(ClientSocket) != SOCKET_ERROR))
				{
					printf("*INFO* | closesocket() call succeeded w/socket=[0x%X]\n", ClientSocket);
				}
			}
		}
	}

CleanupAndExit:
	if (INVALID_SOCKET != ClientSocket)
	{
		closesocket(ClientSocket);
		ClientSocket = INVALID_SOCKET;
	}

	if (INVALID_SOCKET != LocalSocket)
	{
		closesocket(LocalSocket);
		LocalSocket = INVALID_SOCKET;
	}

	if (NULL != lptstrThisComputerName)
	{
		HeapFree(GetProcessHeap(), 0, lptstrThisComputerName);
		lptstrThisComputerName = NULL;
	}

	if (NULL != lpCSAddrInfo)
	{
		HeapFree(GetProcessHeap(), 0, lpCSAddrInfo);
		lpCSAddrInfo = NULL;
	}

	return ulRetCode;
}

// ShowCmdLineSyntaxHelp displays the command line usage
void ShowCmdLineHelp(void)
{
	printf(
		"  Bluetooth example application for demonstrating connection and data transfer."
		"\n"
		"\n  BTHCxnDemo        [-n<RemoteName> | -a<RemoteAddress>] "
		"\n                 [-c<ConnectionCycles>] [-o<Output Level>]"
		"\n"
		"\n  Switches applicable for Client mode:"
		"\n    -n<RemoteName>        Specifies name of remote BlueTooth-Device."
		"\n"
		"\n    -a<RemoteAddress>     Specifies address of remote BlueTooth-Device."
		"\n                          The address is in form XX:XX:XX:XX:XX:XX"
		"\n                          where XX is a hexidecimal byte"
		"\n"
		"\n    One of the above two switches is required for client."
		"\n"
		"\n  Switches applicable for both Client and Server mode:"
		"\n    -c<ConnectionCycles>  Specifies number of connection cycles."
		"\n                          Default value for this parameter is 1.  Specify 0 to "
		"\n                          run infinite number of connection cycles."
		"\n"
		"\n    -o<OutputLevel>       Specifies level of information reporting in cmd window."
		"\n                          Default value for this parameter is 0 (minimal info)."
		"\n                          Possible values: 1 (more info), 2 (all info)."
		"\n"
		"\n  Command Line Examples:"
		"\n    \"BTHCxnDemo -c0\""
		"\n    Runs the BTHCxnDemo server for infinite connection cycles."
		"\n    The application reports minimal information onto the cmd window."
		"\n"
		"\n    \"BTHCxnDemo -nServerDevice -c50 -o2\""
		"\n    Runs the BTHCxnDemo client connecting to remote device (having name "
		"\n    \"ServerDevice\" for 50 connection cycles."
		"\n    The application reports minimal information onto the cmd window."
		"\n\n"
		);
}

// ParseCmdLine parses the command line and sets the global variables accordingly.
// It returns 0 if successful, returns 1 if command line usage is to be displayed,
// returns 2 in case of any other error while parsing
ULONG ParseCmdLine(int argc, char * argv[])
{
	int   iStrLen = 0;
	ULONG ulRetCode = 0;
	int i;

	for (i = 1; i < argc; i++)
	{
		char * pszToken = argv[i];
		if (*pszToken == '-' || *pszToken == '/')
		{
			char token;

			// skip over the "-" or "/"
			pszToken++;
			// Get the command line option
			token = *pszToken;
			// Go one past the option the option-data
			pszToken++;
			// Get the option-data
			switch (token)
			{
			case 'n':
				iStrLen = lstrlen((LPCWSTR)pszToken);
				if ((0 < iStrLen) && (BTH_MAX_NAME_SIZE >= iStrLen))
				{
					lstrcpy((LPWSTR)g_szRemoteName, (LPCWSTR)pszToken);
					printf("The remote name is %s\n", g_szRemoteName);
				}
				else
				{
					ulRetCode = 2;
					printf("!ERROR! | cmd line | Unable to parse -n<RemoteName>, length error (min 1 char, max %d chars)\n", BTH_MAX_NAME_SIZE);
				}
				break;
			case 'a':
				iStrLen = lstrlen((LPCWSTR)pszToken);
				if (CXN_BDADDR_STR_LEN == iStrLen)
				{
					lstrcpy((LPWSTR)g_szRemoteAddr, (LPCWSTR)pszToken);
				}
				else
				{
					ulRetCode = 2;
					printf("!ERROR! | cmd line | Unable to parse -a<RemoteAddress>, Remote bluetooth radio address string length expected %d | Found: %d)\n",
						CXN_BDADDR_STR_LEN, iStrLen);
				}
				break;
			case 'c':
				if (0 < lstrlen((LPCWSTR)pszToken))
				{
					sscanf_s(pszToken, "%d", &g_ulMaxCxnCycles);
					if (0 > g_ulMaxCxnCycles)
					{
						ulRetCode = 2;
						printf("!ERROR! | cmd line | Must provide +ve or 0 value with -c option\n");
					}
				}
				else
				{
					ulRetCode = 2;
					printf("!ERROR! | cmd line | Must provide a value with -c option\n");
				}
				break;
			case 'o':
				if (0 < lstrlen((LPCWSTR)pszToken))
				{
					sscanf_s(pszToken, "%d", &g_iOutputLevel);
					if (0 > g_iOutputLevel)
					{
						ulRetCode = 2;
						printf("!ERROR! | cmd line | Must provide a +ve or 0 value with -o option");
					}
				}
				else
				{
					ulRetCode = 2;
					printf("!ERROR! | cmd line | Must provide a value with -o option");
				}
				break;
			case '?':
			case 'h':
			case 'H':
			default:
				ulRetCode = 1;
			}
		}
		else
		{
			ulRetCode = 1;
			printf("!ERROR! | cmd line | Bad option prefix, use '/' or '-' \n");
		}
	}

	return ulRetCode;
}
*/