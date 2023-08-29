#include <cstdint>
#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <mumble/MumblePlugin_v_1_0_x.h>
#include <mumble/MumbleAPI_v_1_0_x.h>
#include <string>
#include <unordered_map>
#include <process_handler.hpp>

#pragma comment(lib, "Psapi.lib")

static std::string description("GTFO (2023-06-21)"); // Plugin short description
static std::string shortname("GTFO"); // Plugin short name

std::string process_name = "GTFO.exe";
DWORD process_pid = 0;
constexpr uintptr_t snet_player_address = 0x3549AE8; // 48 8B 15 ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 83 FE 05 77 27 4C 8D 05 9B 57 2C FE
constexpr uintptr_t statics_offset = 0xB8; // statics list
constexpr uintptr_t player_array_offset = 0xC8; // localplayer
constexpr uintptr_t first_ptr_offset = 0xB0; // agent interface
// To update check: Player_PlayerManager__GetLocalPlayerAgent

std::unique_ptr<ProcessHandler> process_handler;

bool read_ptr_and_add_offset(uintptr_t& address, uintptr_t offset) {
    if (!process_handler->read_ptr(address, address)) {
        std::cerr << "[ - ] read_ptr_and_add_offset: read_ptr failed" << std::endl;
        return false;
    }
    address += offset;
    return true;
}


bool get_local_player( uintptr_t& localplayer ) 
{
    uintptr_t base_address = 0;
    if ( !process_handler->get_module_address("GameAssembly.dll", base_address ) ) 
    {
        std::cerr << "[ - ] get_local_player: get_module_address failed" << std::endl;
        return false;
    }

    uintptr_t player_manager_address = base_address + snet_player_address;

    if (!read_ptr_and_add_offset(player_manager_address, statics_offset) ||
        !read_ptr_and_add_offset(player_manager_address, player_array_offset) ||
        !read_ptr_and_add_offset(player_manager_address, first_ptr_offset) ||
        !process_handler->read_ptr(player_manager_address, player_manager_address)) 
    {
        return false;
    }

    localplayer = player_manager_address;
    return true;
}
struct MumbleAPI_v_1_0_x mumble_api;
mumble_plugin_id_t mumble_plugin_id;

/* mumble_init */
mumble_error_t mumble_init(mumble_plugin_id_t id)
{
    std::cout << "[ + ] mumble_init" << std::endl;
    mumble_plugin_id = id;
    return mumble_error_t::MUMBLE_EC_OK;
}

/* mumble_shutdown */
void mumble_shutdown()
{
    std::cout << "[ + ] mumble_shutdown" << std::endl;
    /* Delete process handler */
    process_handler.reset( );
}

/* mumble_getName */
struct MumbleStringWrapper mumble_getName()
{
    std::cout << "[ + ] mumble_getName" << std::endl;
    static const char *name = "GTFO";

	struct MumbleStringWrapper wrapper;
	wrapper.data = name;
	wrapper.size = strlen(name);
	wrapper.needsReleasing = false;

	return wrapper;
}

mumble_version_t mumble_getAPIVersion()
{
    std::cout << "[ + ] mumble_getAPIVersion" << std::endl;

    return MUMBLE_PLUGIN_API_VERSION;
}

void mumble_registerAPIFunctions(void *apiStruct)
{
    std::cout << "[ + ] mumble_registerAPIFunctions" << std::endl;
    mumble_api = MUMBLE_API_CAST(apiStruct);
}

void mumble_releaseResource(const void *pointer)
{
    std::cout << "[ + ] mumble_releaseResource" << std::endl;
    /* Free */
    free( const_cast<void*>( pointer ) );
}

struct MumbleStringWrapper mumble_getAuthor() {
        static const char* author = "Sebastien Spirit";

        struct MumbleStringWrapper wrapper;
        wrapper.data           = author;
        wrapper.size           = strlen(author);
        wrapper.needsReleasing = false;

        return wrapper;
}

mumble_version_t mumble_getVersion() {
        static const mumble_version_t version = {0, 1, 2};
        return version;
}

struct MumbleStringWrapper mumble_getDescription() {
        static const char* description = "Simple positional audio for GTFO.";

        struct MumbleStringWrapper wrapper;
        wrapper.data           = description;
        wrapper.size           = strlen(description);
        wrapper.needsReleasing = false;

        return wrapper;
}

uint8_t mumble_initPositionalData(const char* const* programNames,
                                  const uint64_t*    programPIDS,
                                  size_t             programCount)
{
    for (size_t i = 0; i < programCount; ++i) 
    {
        const std::string current_program = programNames[i];
        if (current_program.find(process_name) != std::string::npos) 
        {

            process_pid = programPIDS[i];
            process_handler = std::make_unique<ProcessHandler>();

            if (!process_handler->open_handle(process_pid)) 
            {
                return MUMBLE_PDEC_ERROR_TEMP;
            }

            return MUMBLE_PDEC_OK;
        }
    }

    return MUMBLE_PDEC_ERROR_TEMP;
}


bool mumble_fetchPositionalData(float* avatarPos, float* avatarDir,
                                float* avatarAxis, float* cameraPos,
                                float* cameraDir, float* cameraAxis,
                                const char** context, const char** identity) 
{
    /* Get localplayer */
    uintptr_t localplayer = 0;
    if ( !get_local_player( localplayer ) )
    {
        std::cout << "[ - ] mumble_fetchPositionalData: get_local_player failed" << std::endl;
        return false;
    }
    const uintptr_t eye_position_offset = 0xF0;
    const uintptr_t eye_direction_offset = 0x60;
    const uintptr_t position_offset = 0x48;
    /* Read vectors */
    float eye_position[3] = { 0 };
    if ( !process_handler->read_vec3( localplayer + eye_position_offset, eye_position ) )
    {
        std::cout << "[ - ] mumble_fetchPositionalData: read_vec3 failed" << std::endl;
        return false;
    }
    float eye_direction[3] = { 0 };
    if ( !process_handler->read_vec3( localplayer + eye_direction_offset, eye_direction ) )
    {
        std::cout << "[ - ] mumble_fetchPositionalData: read_vec3 failed" << std::endl;
        return false;
    }
    float position[3] = { 0 };
    if ( !process_handler->read_vec3( localplayer + position_offset, position ) )
    {
        std::cout << "[ - ] mumble_fetchPositionalData: read_vec3 failed" << std::endl;
        return false;
    }

    /* Write avatar_pos */
    avatarPos[0] = eye_position[0];
    avatarPos[1] = eye_position[1];
    avatarPos[2] = eye_position[2];

    /* Write avatar_dir */
    avatarDir[0] = eye_direction[0];
    avatarDir[1] = eye_direction[1];
    avatarDir[2] = eye_direction[2];

    /* Get height */
    float height = eye_position[1] - position[1];
    /* Write axis */
    avatarAxis[0] = 0;
    avatarAxis[1] = height;
    avatarAxis[2] = 0;

    cameraAxis[0] = 0;
    cameraAxis[1] = height;
    cameraAxis[2] = 0;
    /* Write eye position */
    cameraPos[0] = eye_position[0];
    cameraPos[1] = eye_position[1];
    cameraPos[2] = eye_position[2];
    /* Write camera direction */
    cameraDir[0] = eye_direction[0];
    cameraDir[1] = eye_direction[1];
    cameraDir[2] = eye_direction[2];

    const static char contextString[] = "GTFOPlayer";
    *context                    = contextString;

    /* Debug print eye position */
    //std::cout << "[ + ] mumble_fetchPositionalData: eye_position: " << eye_position[0] << ", " << eye_position[1] << ", " << eye_position[2] << std::endl;
    /* Debug direction */
    //std::cout << "[ + ] mumble_fetchPositionalData: eye_direction: " << eye_direction[0] << ", " << eye_direction[1] << ", " << eye_direction[2] << std::endl;

    return true;
}

uint32_t mumble_getFeatures() {
        return MUMBLE_FEATURE_POSITIONAL;
}

void mumble_shutdownPositionalData() 
{
    std::cout << "[ + ] mumble_shutdownPositionalData" << std::endl;
    /* Delete process handler */
    process_handler.reset( );
}

/* DllMain */
BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
    /* Create console on attach */
    if ( ul_reason_for_call == DLL_PROCESS_ATTACH )
    {
        // Debug only
        //AllocConsole();
        //freopen_s( reinterpret_cast<FILE**>( stdout ), "CONOUT$", "w", stdout );
        std::cout << "[ + ] DllMain: Attached" << std::endl;
    }
    /* Free */
    else if ( ul_reason_for_call == DLL_PROCESS_DETACH )
    {
        std::cout << "[ + ] DllMain: Detached" << std::endl;
        fclose( reinterpret_cast<FILE*>( stdout ) );
        FreeConsole();
    }
    return TRUE;
}