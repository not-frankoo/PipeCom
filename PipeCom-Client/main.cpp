#include <Windows.h>
#include <iostream>
#include <memory>
#include <string_view>


std::wstring_view pipe = L"\\\\.\\pipe\\PipeCom";
int ToSend;
DWORD NumofBytes;

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
		if (hpipe)
			std::cout << "[+] connected to pipe server !" << std::endl;
	    
		do
		{
			std::cout << "Enter an integer to send!" << std::endl;
			std::cin >> ToSend;
			WriteFile(hpipe.get(), &ToSend, sizeof(ToSend), &NumofBytes, NULL);
		} while (ToSend != 0);
	

	system("pause");
}