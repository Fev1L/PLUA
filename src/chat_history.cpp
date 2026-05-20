#include "../headers/chat_history.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <filesystem>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

namespace fs = std::filesystem;

static fs::path GetLogsDir() {
#ifdef __APPLE__
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        fs::path exec = fs::canonical(path);
        fs::path contents_macos = exec.parent_path();
        if (contents_macos.filename() == "MacOS") {
            return contents_macos.parent_path() / "Resources" / "logs";
        }
        return exec.parent_path() / "logs";
    }
#endif
#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    return fs::path(path).parent_path() / "logs";
#endif
    return fs::current_path() / "logs";
}

static void EnsureLogsDir() {
    fs::path logs = GetLogsDir();
    if (!fs::exists(logs))
        fs::create_directories(logs);
}

std::string GetTodayDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);

    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d");
    return ss.str();
}

std::string GetTodayFilename() {
    EnsureLogsDir();
    return (GetLogsDir() / ("chat_" + GetTodayDate() + ".log")).string();
}

void CleanOldLogs() {
    EnsureLogsDir();
    fs::path logs_dir = GetLogsDir();
    std::vector<fs::path> logs;

    for (const auto& entry : fs::directory_iterator(logs_dir)) {
        const std::string name = entry.path().filename().string();
        if (name.rfind("chat_", 0) == 0 &&
            name.substr(name.size() - 4) == ".log") {
            logs.push_back(entry.path());
            }
    }

    std::sort(logs.begin(), logs.end());

    while (logs.size() > 7) {
        fs::remove(logs.front());
        logs.erase(logs.begin());
    }
}

bool SaveHistory(const ChatHistory& history, const std::string& filepath) {
    EnsureLogsDir();
    std::ofstream file(filepath);
    if (!file.is_open()) return false;

    for (const auto& msg : history) {
        file << (msg.role == "user" ? "USER" : "ASSISTANT") << "\n";
        file << msg.content << "\n";
        file << "END\n";
    }
    return true;
}

bool LoadHistory(ChatHistory& history, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;

    while (std::getline(file, line)) {
        Message msg;
        if      (line == "USER")      msg.role = "user";
        else if (line == "ASSISTANT") msg.role = "assistant";
        else continue;

        std::ostringstream content;
        bool first = true;
        while (std::getline(file, line)) {
            if (line == "END") break;
            if (!first) content << "\n";
            content << line;
            first = false;
        }

        msg.content = content.str();
        if (!msg.content.empty())
            history.push_back(msg);
    }
    return true;
}

void LoadAllHistory(ChatHistory& history) {
    EnsureLogsDir();
    history.clear();

    fs::path logs_dir = GetLogsDir();
    std::vector<fs::path> logs;

    for (const auto& entry : fs::directory_iterator(logs_dir)) {
        const std::string name = entry.path().filename().string();
        if (name.rfind("chat_", 0) == 0 &&
            name.size() > 4 &&
            name.substr(name.size() - 4) == ".log") {
            logs.push_back(entry.path());
            }
    }

    if (logs.empty()) return;

    std::sort(logs.begin(), logs.end());

    for (const auto& log_path : logs) {
        LoadHistory(history, log_path.string());
    }
}
