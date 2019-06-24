#include <Windows.h>
#include <iostream>
#include <memory>
#include <string_view>


#define BUFSIZE 512

constexpr std::wstring_view PipeName = L"\\\\.\\pipe\\PipeCom";

namespace g_vars {
	int buffer;
	DWORD NumofBytes;
}



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

	if (hpipe.get() == INVALID_HANDLE_VALUE || nullptr)
		return 0;
		
	std::cout << "[+] HANDLE created : " << hpipe << std::endl;


	if (ConnectNamedPipe(hpipe.get(), NULL != FALSE)) {
		std::cout << "[+] A client has been connected to the pipe!" << std::endl;
		while (ReadFile(hpipe.get(), &g_vars::buffer, sizeof(g_vars::buffer), &g_vars::NumofBytes, NULL) != FALSE) {
			std::cout << "int : " << g_vars::buffer << std::endl;
		}
	}
	
	std::cin.get();
	DisconnectNamedPipe(hpipe.get());
}