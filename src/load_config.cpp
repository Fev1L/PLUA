//
// Created by Fev1L on 15.05.2026.
//

#include "../headers/config_loader.h"
#include "../third_party/json/json.hpp"
#include <fstream>

#ifdef __APPLE__
  #include <mach-o/dyld.h>
  #include <limits.h>
#endif

#ifdef _WIN32
  #include <windows.h>
#endif

namespace fs = std::filesystem;
using json = nlohmann::json;

static fs::path GetAppDir() {
#ifdef __APPLE__
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        fs::path exec = fs::canonical(path);
        fs::path contents_macos = exec.parent_path();
        if (contents_macos.filename() == "MacOS") {
            fs::path resources = contents_macos.parent_path() / "Resources";
            if (!fs::exists(resources))
                fs::create_directories(resources);
            return resources;
        }
        return exec.parent_path();
    }
#endif

#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    return fs::path(path).parent_path();
#endif

    return fs::current_path();
}

std::string ConfigPath(const std::string& filename) {
    return (GetAppDir() / "config" / filename).string();
}

std::string FontPath(const std::string& filename) {
    return (GetAppDir() / "fonts" / filename).string();
}

std::string ModelPath(const std::string& filename) {
    return (GetAppDir() / "models" / filename).string();
}

static void CreateDefaultCharacter(const fs::path& filepath) {
    json j;
    j["name"]        = "PLUA";
    j["tone"]        = "calm";
    j["style"]       = "friendly";
    j["description"] = "A quiet and thoughtful friend.";
    j["behavior_rules"] = {
        "Do not speak like an assistant",
        "Keep answers natural",
        "Sometimes ask follow-up questions"
    };

    std::ofstream f(filepath);
    f << j.dump(2);
}

static void CreateDefaultSettings(const fs::path& filepath) {
    json j;
    j["window_width"]  = 1280;
    j["window_height"] = 720;
    j["font_size"]     = 16;
    j["theme"]         = "dark";

    std::ofstream f(filepath);
    f << j.dump(2);
}

void EnsureConfigDir() {
    fs::path app_dir   = GetAppDir();
    fs::path config_dir = app_dir / "config";

    if (!fs::exists(config_dir))
        fs::create_directories(config_dir);

    fs::path char_path     = config_dir / "character.json";
    fs::path settings_path = config_dir / "settings.json";

    if (!fs::exists(char_path))
        CreateDefaultCharacter(char_path);

    if (!fs::exists(settings_path))
        CreateDefaultSettings(settings_path);
}

bool LoadCharacterConfig(CharacterConfig& out, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    json j;
    try { file >> j; }
    catch (...) { return false; }

    out.name        = j.value("name",        "PLUA");
    out.tone        = j.value("tone",        "calm");
    out.style       = j.value("style",       "friendly");
    out.description = j.value("description", "A quiet and thoughtful friend.");

    out.behavior_rules.clear();
    if (j.contains("behavior_rules") && j["behavior_rules"].is_array()) {
        for (const auto& rule : j["behavior_rules"])
            out.behavior_rules.push_back(rule.get<std::string>());
    }
    return true;
}

bool LoadSettingsConfig(SettingsConfig& out, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    json j;
    try { file >> j; }
    catch (...) { return false; }

    out.window_width  = j.value("window_width",  1280);
    out.window_height = j.value("window_height", 720);
    out.font_size     = j.value("font_size",      16.0f);
    out.theme         = j.value("theme",          "dark");
    return true;
}