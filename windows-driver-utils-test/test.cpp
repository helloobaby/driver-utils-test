#include<ntifs.h>
#include<ntddk.h>
#include<wdm.h>


template<typename... types>
void print(types... args)
{
	DbgPrintEx(DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, args...);
}


//
//注册表相关操作测试
//
#include<wdmlib/cm/cmregutil.h>
void test1() {
	/*++
	 
		测试读取一个键值
	 
	--*/


	HANDLE KeyHandle;
	NTSTATUS Status;
	ULONG Value;

	Status = CmRegUtilOpenExistingWstrKey(NULL, CM_REGISTRY_MACHINE("SYSTEM\\CurrentControlSet\\Services\\ACPI"),KEY_ALL_ACCESS,&KeyHandle);
	if (!NT_SUCCESS(Status)) {
		print("CmRegUtilOpenExistingWstrKey failed with %x\n", Status);
		return;
	}

	Status = CmRegUtilWstrValueGetDword(KeyHandle, L"Type", 0, &Value);
	if (!NT_SUCCESS(Status)) {
		print("CmRegUtilWstrValueGetDword failed with %x\n", Status);
		return;
	}

	print("reg read value %d\n", Value);
	return;
}

void test2() {
	/*++
	
	测试创建一个键

	ACPI
		-test
			-test2

	--*/

	HANDLE KeyHandle;
	NTSTATUS Status;

	Status = CmRegUtilCreateWstrKey(NULL, CM_REGISTRY_MACHINE("SYSTEM\\CurrentControlSet\\Services\\ACPI\\test"), KEY_ALL_ACCESS, REG_OPTION_NON_VOLATILE, NULL, NULL, &KeyHandle);
	if (!NT_SUCCESS(Status)) {
		print("CmRegUtilCreateWstrKey failed with %x\n", Status);
		return;
	}

	CmRegUtilCreateWstrKey(NULL, CM_REGISTRY_MACHINE("SYSTEM\\CurrentControlSet\\Services\\ACPI\\test\\test2"), KEY_ALL_ACCESS, REG_OPTION_NON_VOLATILE, NULL, NULL, &KeyHandle);
	if (!NT_SUCCESS(Status)) {
		print("CmRegUtilCreateWstrKey failed with %x\n", Status);
		return;
	}
}

void test3() {
	/*++
		
		测试设置键值

	--*/

	HANDLE KeyHandle;
	NTSTATUS Status;
	
	//ValueName和ValueData一定要是数组,不要字符串常量
	WCHAR ValueName[] = L"0";
	WCHAR ValueData[] = L"hello world";
	
	ULONG ValueToSet = 0x12345678;

	Status = CmRegUtilOpenExistingWstrKey(NULL, CM_REGISTRY_MACHINE("SYSTEM\\CurrentControlSet\\Services\\ACPI\\Enum"), KEY_ALL_ACCESS, &KeyHandle);
	if (!NT_SUCCESS(Status)) {
		print("CmRegUtilOpenExistingWstrKey failed with %x\n", Status);
		return;
	}
	Status = CmRegUtilWstrValueSetWstrString(KeyHandle, ValueName,ValueData);
	if (!NT_SUCCESS(Status)) {
		print("CmRegUtilWstrValueSetWstrString failed with %x\n", Status);
		return;
	}

	Status = CmRegUtilWstrValueSetFullBuffer(KeyHandle, L"Count", REG_DWORD, &ValueToSet, sizeof(ValueToSet));
	if (!NT_SUCCESS(Status)) {
		print("CmRegUtilWstrValueSetFullBuffer failed with %x\n", Status);
		return;
	}

}