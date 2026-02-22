#ifndef PCH_H
#define PCH_H

#define PI 3.141592653

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <d3d11.h>
#include <dxgi.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "minhook-master/include/MinHook.h"

#include "imgui-master/imgui.h"
#include "imgui-master/backends/imgui_impl_dx11.h"
#include "imgui-master/backends/imgui_impl_win32.h"

#include "offsets/offsets.hpp"
#include "offsets/client_dll.hpp"
#include "offsets/buttons.hpp"

#include <cstdint>
#include <string>
#include <cmath>
#include <thread>
#include <map>
#include <vector>
#include <chrono>

#include "sdk/structs.h"
#include "sdk/CallStack-Spoofer.h"

#include "resources/images.h"

#endif //PCH_H
