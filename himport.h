#include <Windows.h>
#include <string>
#include <vector>

std::vector<std::uint8_t> trash_hash(std::string in) {
    std::vector<std::uint8_t> out;

    for (int i{}; in[i] != '\0'; i++) {
        auto tmp = in[i] % in.length();
        tmp += i * in.length();
        tmp ^= in[in.length() - i];
        out.push_back(tmp);
    }

    return out;
}

template <typename T>
T himport(std::string library, std::vector<std::uint8_t> hash) {
    HMODULE user32 = LoadLibrary(library.c_str());

    auto base = reinterpret_cast<BYTE*>(user32);

    auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(user32);
    auto nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(base + dos_header->e_lfanew);
    auto export_dir = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(base + nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

    auto ordinals = reinterpret_cast<unsigned short*>(base + export_dir->AddressOfNameOrdinals);
    auto names = reinterpret_cast<unsigned long*>(base + export_dir->AddressOfNames);
    auto functions = reinterpret_cast<unsigned long*>(base + export_dir->AddressOfFunctions);

    uintptr_t function_address{};

    for (unsigned long long i = 0; i < export_dir->NumberOfNames; i++) {
        auto function_name = reinterpret_cast<const char*>(base + names[i]);
        if (trash_hash(std::string(function_name)) == hash) {
            return reinterpret_cast<T>(reinterpret_cast<std::uintptr_t>(base + functions[ordinals[i]]));
        }
    }
    return nullptr;
}
