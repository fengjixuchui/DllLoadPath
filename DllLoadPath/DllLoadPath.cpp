#include <phnt_windows.h>
#include <phnt.h>
#include <cstdio>

#pragma comment(lib,"ntdll.lib")

int main()
{
    const wchar_t Path[] = LR"(C:/Tools/TestApplication.exe)";
    // Modifying CmdLine/CommandLine will cause DLLs to start being loaded from a different path
    const wchar_t CmdLine[] = LR"()";
    // Modifying DllLocation/DllPath will cause the second path in the load order to be changed
    const wchar_t DllLocation[] = LR"(C:/ProgramData/)";

    UNICODE_STRING ImagePathName = {};
    UNICODE_STRING CommandLine = {};
    UNICODE_STRING DllPath = {};

    PRTL_USER_PROCESS_PARAMETERS UserProcessParam = {};
    RTL_USER_PROCESS_INFORMATION ProcessInfo = {};

    RtlDosPathNameToNtPathName_U(Path, &ImagePathName, NULL, NULL);
    RtlInitUnicodeString(&CommandLine, CmdLine);
    RtlInitUnicodeString(&DllPath, DllLocation);

    NTSTATUS status = STATUS_SUCCESS;
    status = RtlCreateProcessParameters(&UserProcessParam, &ImagePathName, &DllPath, NULL, &CommandLine, NULL, NULL, NULL, NULL, NULL);
    printf("RtlCreateProcessParameters NTSTATUS: %u\n", status);
    status = RtlCreateUserProcess(&ImagePathName, 0, UserProcessParam, NULL, NULL, NULL, FALSE, NULL, NULL, &ProcessInfo);
    printf("RtlCreateUserProcess NTSTATUS: %u\n", status);
    status = NtResumeThread(ProcessInfo.ThreadHandle, nullptr);
    printf("NtResumeThread NTSTATUS: %u\n", status);
    status = WaitForSingleObject(ProcessInfo.ThreadHandle, INFINITE);
    printf("WaitForSingleObject NTSTATUS: %u\n", status);
    return 0;
}