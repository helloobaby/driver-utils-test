#include<ntifs.h>
#include<ntddk.h>
#include<wdm.h>


template<typename... types>
void print(types... args)
{
	DbgPrintEx(DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, args...);
}


//
//ע�����ز�������
//
#include<wdmlib/cm/cmregutil.h>
void test1() {
	/*++
	 
		���Զ�ȡһ����ֵ
	 
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
	
	���Դ���һ����

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
		
		�������ü�ֵ

	--*/

	HANDLE KeyHandle;
	NTSTATUS Status;
	
	//ValueName��ValueDataһ��Ҫ������,��Ҫ�ַ�������
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