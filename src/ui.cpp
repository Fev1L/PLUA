//
// Created by Fev1L on 01.05.2026.
//

#include "../headers/ui.h"
#include "imgui.h"

static char input_buf[1024] = "";

bool DrawUI(int window_w, int window_h,
            const ChatHistory& history,
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

    ImGui::Begin("PLUA Chat AI", nullptr, flags);

    float input_height = ImGui::GetFrameHeightWithSpacing() * 2 + 8;
    ImVec2 chat_size(window_w - 16, window_h - input_height - 16);

    ImGui::BeginChild("ChatArea", chat_size, true);

    for (const auto& msg : history) {
        if (msg.role == "user") {
            ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "You:");
        } else {
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "AI:");
        }
        ImGui::SameLine();
        ImGui::TextWrapped("%s", msg.content.c_str());
        ImGui::Spacing();
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();

    ImGui::Separator();

    ImGui::SetNextItemWidth(window_w - 80);
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