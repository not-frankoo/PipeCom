#include <Windows.h>
#include <iostream>
#include <memory>
#include <string_view>


constexpr std::wstring_view pipe = L"\\\\.\\pipe\\PipeCom";

namespace g_vars {
	int ToSend;
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
	
		auto hpipe = std::unique_ptr<HANDLE, HandleDisposer>(CreateFileW(pipe.data(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL));
		if (hpipe.get() == INVALID_HANDLE_VALUE || nullptr)
			return 0;
		else
			std::cout << "[+] connected to pipe server !" << std::endl;
	    
		do
		{
			std::cout << "Enter an integer to send!" << std::endl;
			std::cin >> g_vars::ToSend;
			WriteFile(hpipe.get(), &g_vars::ToSend, sizeof(g_vars::ToSend), &g_vars::NumofBytes, NULL);
		} while (g_vars::ToSend != 0);
	

		std::cin.get();
}