# DllLoadPath

Showcasing two different techniques for changing DLL load order by using undocumented APIs.
These are not novel techniques but I never saw them documented anywhere.

 The proper signatures for RtlCreateProcessParameter and RtlCreateProcessParameters are:
 
 ```cpp
RtlCreateUserProcess(
  IN PUNICODE_STRING      ImagePath,
  IN ULONG                ObjectAttributes,
  IN OUT PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
  IN PSECURITY_DESCRIPTOR ProcessSecurityDescriptor OPTIONAL,
  IN PSECURITY_DESCRIPTOR ThreadSecurityDescriptor OPTIONAL,
  IN HANDLE               ParentProcess,
  IN BOOLEAN              InheritHandles,
  IN HANDLE               DebugPort OPTIONAL,
  IN HANDLE               ExceptionPort OPTIONAL,
  OUT PRTL_USER_PROCESS_INFORMATION ProcessInformation );
  
RtlCreateProcessParameters(
    _Out_ PRTL_USER_PROCESS_PARAMETERS *pProcessParameters,
    _In_ PUNICODE_STRING ImagePathName,
    _In_opt_ PUNICODE_STRING DllPath,
    _In_opt_ PUNICODE_STRING CurrentDirectory,
    _In_opt_ PUNICODE_STRING CommandLine,
    _In_opt_ PVOID Environment,
    _In_opt_ PUNICODE_STRING WindowTitle,
    _In_opt_ PUNICODE_STRING DesktopInfo,
    _In_opt_ PUNICODE_STRING ShellInfo,
    _In_opt_ PUNICODE_STRING RuntimeData
    );
  ```
  
By changing some values in RtlCreateProcessParameters we can achieve interesting behavior.

# Changing the working DLL load order directory entirely

Writing this one up for people was inspired by this [tweet](https://twitter.com/Octoberfest73/status/1642165975805050881), thank you for reminding me of it :)

![image](https://user-images.githubusercontent.com/74931194/230739781-bef31578-3812-44f1-944f-e757d1697454.png)

```cpp
  const wchar_t Path[] = LR"(C:/Windows/System32/WerFault.exe)";
  const wchar_t CmdLine[] = LR"(C:/ProgramData/WerFault.exe)";
  const wchar_t DllLocation[] = LR"()";
  ...
  status = RtlCreateProcessParameters(&UserProcessParam, &CommandLine, &CommandLine, NULL, &CommandLine, NULL, NULL, NULL, NULL, NULL);
```

# Changing the second search path of the DLL search order

This one will will cause the application to still look into it's own directory before the one you choose, but it may come useful in some cases.

![image](https://user-images.githubusercontent.com/74931194/230739994-c927101d-aa32-4eba-b0a9-91f19ce86afa.png)

```cpp
  const wchar_t Path[] = LR"(C:/Tools/TestApplication.exe)";
  const wchar_t CmdLine[] = LR"()";
  const wchar_t DllLocation[] = LR"(C:/ProgramData/)";
  ...
  status = RtlCreateProcessParameters(&UserProcessParam, &ImagePathName, &DllPath, NULL, &CommandLine, NULL, NULL, NULL, NULL, NULL);
 ```
 
Many thanks to [this post](http://www.rohitab.com/discuss/topic/41379-running-native-applications-with-rtlcreateuserprocess/) for helping me work out some parts.
