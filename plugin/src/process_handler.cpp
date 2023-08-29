#include <process_handler.hpp>

bool ProcessHandler::open_handle( DWORD pid )
{
    process_handle = OpenProcess( PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid );
    if (!process_handle) 
    {
        std::cerr << "[ - ] open_handle_for_reading: OpenProcess failed" << std::endl;
        return false;
    }
    return true;
}

bool ProcessHandler::get_module_address( const std::string &module_address, uintptr_t &base_address )
{
    if (!process_handle) 
    {
        std::cerr << "[ - ] get_module_address: process_handle is NULL" << std::endl;
        return false;
    }

    if ( const auto it = module_addresses.find(module_address); it != module_addresses.end( ) ) 
    {
        base_address = it->second;
        return true;
    }

    std::vector<HMODULE> hMods( 1024 );
    DWORD cbNeeded;

    if ( EnumProcessModules(process_handle, hMods.data(), hMods.size() * sizeof(HMODULE), &cbNeeded) ) 
    {
        for ( unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); ++i ) 
        {
            TCHAR szModName[MAX_PATH];
            if ( GetModuleFileNameEx( process_handle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR) ) ) 
            {
                std::string module_name = szModName;
                if ( module_name.find(module_address) != std::string::npos ) 
                {
                    base_address = reinterpret_cast<uintptr_t>(hMods[i]);
                    module_addresses[module_address] = base_address;
                    return true;
                }
            }
        }
    }
    return false;
}

bool ProcessHandler::read_mem( uintptr_t address, void *buffer, size_t size ) const
{
    SIZE_T bytes_read = 0;
    if ( !ReadProcessMemory( process_handle, reinterpret_cast<LPCVOID>(address), buffer, size, &bytes_read ) ) 
    {
        std::cerr << "[ - ] read_mem: ReadProcessMemory failed" << std::endl;
        return false;
    }
    return true;
}

bool ProcessHandler::read_vec3( uintptr_t address, float *buffer ) const
{
    return read_mem( address, buffer, sizeof(float) * 3 );
}

bool ProcessHandler::read_int( uintptr_t address, int *buffer ) const
{
    return read_mem( address, &buffer, sizeof(int) );
}

bool ProcessHandler::read_ptr( uintptr_t address, uintptr_t &buffer ) const
{
    return read_mem( address, &buffer, sizeof(uintptr_t) );
}
