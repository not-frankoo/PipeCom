#include <Windows.h>
#include <iostream>
#include <memory>
#include <string_view>






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

	constexpr std::wstring_view pipe = L"\\\\.\\pipe\\PipeCom";
	int ToSend = 0;
	DWORD NumofBytes = 0;
	
		auto hpipe = std::unique_ptr<HANDLE, HandleDisposer>(CreateFileW(pipe.data(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL));
		if (hpipe.get() == INVALID_HANDLE_VALUE)
			return 0;
		else
			std::cout << "[+] connected to pipe server !" << std::endl;
	    
		do
		{
			std::cout << "Enter an integer to send!" << std::endl;
			std::cin >> ToSend;
			WriteFile(hpipe.get(), &ToSend, sizeof(ToSend), &NumofBytes, NULL);
		} while (ToSend != 0);
	

		std::cin.get();
}