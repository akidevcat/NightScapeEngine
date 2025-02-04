#pragma once

#include <SDL.h>
#include <gl/glew.h>
#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <gl/glu.h>
#endif
#include <entt/entt.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdio>
#include <string>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <random>
#include <utility>
#include <iostream>
#include <chrono>

#include "Engine/Runtime/Core/Ref.h"
#include "Engine/Runtime/Math/Math.h"