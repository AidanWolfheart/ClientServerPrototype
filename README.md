To build, use Visual Studio (2017). Included solutions should have the settings for headers and libraries, 
otherwise:
Both should have [$(ProjectDir)\include] under Settings->C/C++->General->Additional Include Directories
and [$(ProjectDir)\lib] under Settings->Linker->General->Additional Library Directories

ClientServerProto should also include [libssl.lib;libcrypto.lib] in Linker->Input->Additional Dependencies

%%%%%%%%%%%%%%%%%%%%%%%%%%
Paths to debug executables:
()Server:
..\ClientServerProto\Debug\ClientServerProto.exe - this one should have two .dll files (libcrypto-1_1.dll and libssl-1_1.dll)

()Client:
..\ClientProto\Debug\ClientProto.exe
%%%%%%%%%%%%%%%%%%%%%%%%%%
1. Run the ClientServerProto
2. Run the ClientProto - 20 instances can run simultaneously
3. Type anything in ClientProto - Server would receive that in text 
4. Server sends back hashed string
