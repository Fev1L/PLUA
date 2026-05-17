//
// Created by Fev1L on 01.05.2026.
//

#include "../headers/ui.h"
#include "imgui.h"

static char input_buf[1024] = "";

void ApplyTheme(const std::string& theme) {
    if (theme == "dark") {
        ImGui::StyleColorsDark();
        ImGui::GetStyle().Colors[ImGuiCol_WindowBg]  = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
        ImGui::GetStyle().Colors[ImGuiCol_ChildBg]   = ImVec4(0.07f, 0.07f, 0.07f, 1.0f);
        ImGui::GetStyle().Colors[ImGuiCol_FrameBg]   = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    } else if (theme == "light") {
        ImGui::StyleColorsLight();
    } else if (theme == "classic") {
        ImGui::StyleColorsClassic();
    }
}

bool DrawUI(int window_w, int window_h,
            const ChatHistory& history,
            const CharacterConfig& character,
            std::string& prompt_out) {

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)window_w, (float)window_h));

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar         |
        ImGuiWindowFlags_NoResize           |
        ImGuiWindowFlags_NoMove             |
        ImGuiWindowFlags_NoScrollbar        |
        ImGuiWindowFlags_NoCollapse         |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("##main", nullptr, flags);

    ImGui::TextColored(ImVec4(0.6f, 0.9f, 0.6f, 1.0f),
                       "[ %s ]", character.name.c_str());
    ImGui::SameLine();
    ImGui::TextDisabled("— %s", character.description.c_str());
    ImGui::Separator();

    float input_height = ImGui::GetFrameHeightWithSpacing() * 2 + 8;
    ImVec2 chat_size((float)window_w - 16,
                     (float)window_h - input_height - 60);

    ImGui::BeginChild("ChatArea", chat_size, true);
    for (const auto& msg : history) {
        if (msg.role == "user") {
            ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "You:");
        } else {
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f),
                               "%s:", character.name.c_str());
        }
        ImGui::SameLine();
        ImGui::TextWrapped("%s", msg.content.c_str());
        ImGui::Spacing();
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::Separator();

    ImGui::SetNextItemWidth((float)window_w - 80);
    bool submitted = ImGui::InputText("##input", input_buf, sizeof(input_buf),
                                      ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    submitted = submitted || ImGui::Button("Send");

    if (submitted && input_buf[0] != '\0') {
        prompt_out = std::string(input_buf);
        input_buf[0] = '\0';
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();
    return submitted && !prompt_out.empty();
}