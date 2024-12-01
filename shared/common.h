#pragma once

#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1080
#define WINDOW_NAME "crossPlatformGame"


#ifdef LINUX_BUILD
#define START_DIR "../../../shared/"
#define ASSETS_DIR "../../../shared/assets/"
#endif // LINUX_BUILD

#ifdef WINDOWS_BUILD
#define START_DIR "../shared/"
#define ASSETS_DIR "../shared/assets/"
#endif // WINDOWS_BUILD




