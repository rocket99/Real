//
//TKPrefix.h
//

#ifndef __TKPrefix_h__
#define __TKPrefix_h__

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cmath>

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <queue>
#include <thread>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/geometric.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_platform.h>
#include <SDL2/SDL_vulkan.h>
#include <SDL2/SDL_video.h>

#include <vulkan/vulkan_core.h>
#include <vulkan/vk_platform.h>
#include <vulkan/vk_layer.h>
#include <vulkan/vk_icd.h>

#include <rapidjson/reader.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>


namespace TK {};

namespace Model{};

typedef struct _vertex {
	float position[3];
	float normal[3];
	float color[4];
	float uv[2];
} Vertex;

typedef struct _light {
	int mode;
	float position[3];
	float direction[3];
	float diffuse[4];
	float specular[4];
} Light;

#define TKlog(format, ...) printf("%s[%d]:" format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define TKwarn(format, ...) printf("warning:%s[%d]:" format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define TKerror(format, ...) printf("error:%s[%d]:" format "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#endif // TKPrefix.h
