#pragma once

#if (__cplusplus >= 202002L)
#define MAP_CONTAINS(map, key) (map.contains(key))
#else
#define MAP_CONTAINS(map, key) (map.find(key) != map.end())
#endif
