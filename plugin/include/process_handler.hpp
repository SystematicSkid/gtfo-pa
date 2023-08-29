#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <vector>

class ProcessHandler
{
public:
    ProcessHandler( ) = default;

    ~ProcessHandler( )
    {
        if ( process_handle )
            CloseHandle( process_handle );
    }

    bool open_handle( DWORD pid );
    bool get_module_address( const std::string& module_address, uintptr_t& base_address );
    bool read_mem( uintptr_t address, void* buffer, size_t size ) const;
    bool read_vec3( uintptr_t address, float* buffer ) const;
    bool read_int( uintptr_t address, int* buffer ) const;
    bool read_ptr(uintptr_t address, uintptr_t& buffer ) const;

private:
    HANDLE process_handle = nullptr;
    std::map<std::string, uintptr_t> module_addresses;
};