#include<ntifs.h>
#include<ntddk.h>
#include<wdm.h>

void unload(PDRIVER_OBJECT) {
	return;
}

void test1();
void test2();
void test3();

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT drv, PUNICODE_STRING reg) {

	drv->DriverUnload = unload;
	test3();


	return STATUS_SUCCESS;
}