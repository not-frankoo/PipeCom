#include <Windows.h>
#include <iostream>
#include <memory>
#include <string_view>


#define BUFSIZE 512
int buffer;
DWORD NumofBytes;
std::wstring_view PipeName = L"\\\\.\\pipe\\PipeCom";



struct HandleDisposer
{
	using pointer = HANDLE;
	void operator()(HANDLE handle) const
	{
		if (handle != NULL || handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
		}
	}
};




int main() {
	
	auto hpipe = std::unique_ptr<HANDLE, HandleDisposer>(
		CreateNamedPipeW(
		PipeName.data(),
		PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		BUFSIZE, 
		BUFSIZE, 
		0, 
		NULL));

	if (!hpipe)
		return 0;
		
	std::cout << "[+] HANDLE created : " << hpipe << std::endl;


	if (ConnectNamedPipe(hpipe.get(), NULL != FALSE)) {
		std::cout << "[+] A client has been connected to the pipe!" << std::endl;
		while (ReadFile(hpipe.get(), &buffer, sizeof(buffer), &NumofBytes, NULL) != FALSE) {	
			std::cout << "int : " << buffer << std::endl;
		}
	}
	
	system("pause");
	DisconnectNamedPipe(hpipe.get());
	return 0;
}