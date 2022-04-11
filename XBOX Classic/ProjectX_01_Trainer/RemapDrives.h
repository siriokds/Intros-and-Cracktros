#include <string.h>


typedef struct _UNICODE_STRING 
{
	USHORT Length;
	USHORT MaximumLength;
	PSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

#define IN

extern "C" XBOXAPI DWORD WINAPI IoCreateSymbolicLink(IN PUNICODE_STRING SymbolicLinkName,IN PUNICODE_STRING DeviceName);
extern "C" XBOXAPI DWORD WINAPI IoDeleteSymbolicLink(IN PUNICODE_STRING SymbolicLinkName);


#define DriveD "\\??\\D:"
#define CdRom "\\Device\\Cdrom0"
UNICODE_STRING DDeviceName = { strlen(CdRom), strlen(CdRom)+1, CdRom };
UNICODE_STRING DSymbolicLinkName = { strlen(DriveD), strlen(DriveD)+1, DriveD };

#define DriveE "\\??\\E:"
#define DeviceE "\\Device\\Harddisk0\\Partition1"
UNICODE_STRING EDeviceName = { strlen(DeviceE) , strlen(DeviceE)+1, DeviceE};
UNICODE_STRING ESymbolicLinkName = { strlen(DriveE), strlen(DriveE)+1, DriveE };

#define DriveF "\\??\\F:"
#define DeviceF "\\Device\\Harddisk0\\Partition6"
UNICODE_STRING FDeviceName = { strlen(DeviceF) , strlen(DeviceF)+1, DeviceF};
UNICODE_STRING FSymbolicLinkName = { strlen(DriveF), strlen(DriveF)+1, DriveF };

#define DriveC "\\??\\C:"
#define DeviceC "\\Device\\Harddisk0\\Partition1"
UNICODE_STRING CDeviceName = {strlen(DeviceC), strlen(DeviceC)+1, DeviceC };
UNICODE_STRING CSymbolicLinkName = { strlen(DriveC), strlen(DriveC)+1, DriveC };

#define DriveT "\\??\\T:"
#define DeviceT "\\Device\\Harddisk0\\Partition1\\TDATA"
UNICODE_STRING TDeviceName = {strlen(DeviceT), strlen(DeviceT)+1, DeviceT };
UNICODE_STRING TSymbolicLinkName = { strlen(DriveT), strlen(DriveT)+1, DriveT };

#define DriveU "\\??\\U:"
#define DeviceU "\\Device\\Harddisk0\\Partition1\\UDATA"
UNICODE_STRING UDeviceName = {strlen(DeviceU), strlen(DeviceU)+1, DeviceU };
UNICODE_STRING USymbolicLinkName = { strlen(DriveU), strlen(DriveU)+1, DriveU };





void	RemapDrive(char drive)
{
	// Remap the CDROM, map E & F Drives

	switch (drive)
	{
		case 'C':
		case 'c':
			IoDeleteSymbolicLink(&CSymbolicLinkName);
			IoCreateSymbolicLink(&CSymbolicLinkName, &CDeviceName);
			break;

		case 'D':
		case 'd':
			IoDeleteSymbolicLink(&DSymbolicLinkName);
			IoCreateSymbolicLink(&DSymbolicLinkName, &DDeviceName);
			break;

		case 'E':
		case 'e':
			IoDeleteSymbolicLink(&ESymbolicLinkName);
			IoCreateSymbolicLink(&ESymbolicLinkName, &EDeviceName);
			break;

		case 'F':
		case 'f':
			IoDeleteSymbolicLink(&FSymbolicLinkName);
			IoCreateSymbolicLink(&FSymbolicLinkName, &FDeviceName);
			break;

		case 'T':
		case 't':
			IoDeleteSymbolicLink(&TSymbolicLinkName);
			IoCreateSymbolicLink(&TSymbolicLinkName, &TDeviceName);
			break;

		case 'U':
		case 'u':
			IoDeleteSymbolicLink(&USymbolicLinkName);
			IoCreateSymbolicLink(&USymbolicLinkName, &UDeviceName);
			break;

	}

}

