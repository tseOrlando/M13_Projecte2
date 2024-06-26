//
// Created by sel on 21/03/2024.
//

#ifndef HARD_MOTION_GINC_H
#define HARD_MOTION_GINC_H

/*
 * In order to not be redundant, add the needed libraries in common in one file!
 */

#include <stdio.h>
#include <vector>
#include <string>
#include <random>
#include <limits>
#include <chrono>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <map>
#include <sstream>
#include <thread>
#include <future>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"

#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>
#include <android/rect.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <jni.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "../menu/headers/wtools/wtools.h"
#include "../menu/headers/wtools/random.h"

#include "../menu/json.hpp"

#include "main/cpp/menu/entities/api_rest_fetch.h"

#endif //HARD_MOTION_GINC_H
