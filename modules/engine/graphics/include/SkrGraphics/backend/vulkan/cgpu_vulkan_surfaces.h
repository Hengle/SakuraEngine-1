#pragma once
#include "SkrGraphics/backend/vulkan/cgpu_vulkan.h"

#ifdef __cplusplus
extern "C" {
#endif

CGPU_API void cgpu_free_surface_vulkan(CGPUDeviceId device, CGPUSurfaceId surface);

#if defined(_WIN32) || defined(_WIN64)
CGPU_API CGPUSurfaceId cgpu_surface_from_hwnd_vulkan(CGPUDeviceId device, HWND window);
#elif defined(_MACOS)
CGPU_API CGPUSurfaceId cgpu_surface_from_ns_view_vulkan(CGPUDeviceId device, CGPUNSView* window);
#endif

#ifdef __cplusplus
} // end extern "C"
#endif