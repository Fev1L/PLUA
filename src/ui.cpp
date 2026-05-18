#include "../headers/ui.h"
#include "../headers/config_loader.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <cmath>
#include <algorithm>

static char input_buf[2048] = "";

namespace G {
    inline ImVec4 BG_BASE()    { return {0.94f, 0.95f, 0.98f, 1.00f}; }
    inline ImVec4 BG_PANEL()   { return {1.00f, 1.00f, 1.00f, 0.72f}; }
    inline ImVec4 BG_HEADER()  { return {1.00f, 1.00f, 1.00f, 0.85f}; }
    inline ImVec4 BG_INPUT()   { return {1.00f, 1.00f, 1.00f, 0.90f}; }

    inline ImVec4 BUBBLE_USER(){ return {0.98f, 0.88f, 0.88f, 1.00f}; }
    inline ImVec4 BUBBLE_AI()  { return {0.88f, 0.93f, 1.00f, 1.00f}; }

    inline ImVec4 BORDER_USER(){ return {0.90f, 0.65f, 0.65f, 0.60f}; }
    inline ImVec4 BORDER_AI()  { return {0.65f, 0.78f, 0.95f, 0.60f}; }
    inline ImVec4 BORDER_GLASS(){ return {1.00f, 1.00f, 1.00f, 0.80f}; }

    inline ImVec4 RED()        { return {0.95f, 0.42f, 0.42f, 1.00f}; }
    inline ImVec4 BLUE()       { return {0.50f, 0.72f, 0.98f, 1.00f}; }
    inline ImVec4 YELLOW()     { return {0.99f, 0.88f, 0.45f, 1.00f}; }
    inline ImVec4 YELLOW_DIM() { return {0.99f, 0.88f, 0.45f, 0.30f}; }

    inline ImVec4 TEXT_DARK()  { return {0.18f, 0.20f, 0.26f, 1.00f}; }
    inline ImVec4 TEXT_MID()   { return {0.45f, 0.48f, 0.58f, 1.00f}; }
    inline ImVec4 TEXT_LIGHT() { return {0.72f, 0.75f, 0.82f, 1.00f}; }
    inline ImVec4 TEXT_NAME()  { return {0.75f, 0.42f, 0.42f, 1.00f}; }

    inline ImU32 u32(ImVec4 c)            { return ImGui::ColorConvertFloat4ToU32(c); }
    inline ImU32 u32a(ImVec4 c, float a)  { return ImGui::ColorConvertFloat4ToU32({c.x,c.y,c.z,a}); }
}

static void SoftShadow(ImDrawList* dl, ImVec2 pmin, ImVec2 pmax, float r) {
    for (int i = 4; i >= 1; i--) {
        float off = (float)i * 1.8f;
        float alpha = 0.04f - i * 0.005f;
        dl->AddRectFilled(
            {pmin.x - off, pmin.y + off * 0.5f},
            {pmax.x + off, pmax.y + off * 1.2f},
            G::u32a({0.18f, 0.25f, 0.45f, 1.f}, alpha),
            r + off);
    }
}

static void GlassRect(ImDrawList* dl, ImVec2 pmin, ImVec2 pmax, float r,
                      ImVec4 fill, ImVec4 border) {
    SoftShadow(dl, pmin, pmax, r);
    dl->AddRectFilled(pmin, pmax, G::u32(fill), r);
    dl->AddRectFilled(
        pmin,
        {pmax.x, pmin.y + (pmax.y - pmin.y) * 0.45f},
        G::u32a({1.f,1.f,1.f,1.f}, 0.28f), r, ImDrawFlags_RoundCornersTop);
    dl->AddRect(pmin, pmax, G::u32(border), r, 0, 1.0f);
}

static float DrawBubble(ImDrawList* dl,
                         ImVec2 win_pos,
                         float cursor_y,
                         float scroll_y,
                         float win_w,
                         const std::string& text,
                         bool is_user,
                         const std::string& ai_name) {

    const float PX       = 14.0f;
    const float PY       = 10.0f;
    const float MARGIN   = 18.0f;
    const float MAX_W    = win_w * 0.62f;
    const float R        = 18.0f;
    const float NAME_H   = is_user ? 0.0f : 19.0f;

    ImVec2 txt_sz = ImGui::CalcTextSize(
        text.c_str(), nullptr, false, MAX_W - PX * 2.0f);

    float bw = std::min(txt_sz.x + PX * 2.0f, MAX_W);
    float bh = txt_sz.y + PY * 2.0f + NAME_H;

    float screen_y = win_pos.y + cursor_y - scroll_y;

    ImVec2 pmin, pmax;
    if (is_user) {
        pmin = { win_pos.x + win_w - MARGIN - bw, screen_y };
        pmax = { win_pos.x + win_w - MARGIN,       screen_y + bh };
    } else {
        pmin = { win_pos.x + MARGIN,       screen_y };
        pmax = { win_pos.x + MARGIN + bw,  screen_y + bh };
    }

    ImVec4 fill   = is_user ? G::BUBBLE_USER() : G::BUBBLE_AI();
    ImVec4 border = is_user ? G::BORDER_USER() : G::BORDER_AI();

    GlassRect(dl, pmin, pmax, R, fill, border);

    if (!is_user) {
        dl->AddText({ pmin.x + PX, pmin.y + PY },
                    G::u32(G::TEXT_NAME()),
                    ai_name.c_str());
    }

    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
                { pmin.x + PX, pmin.y + PY + NAME_H },
                G::u32(G::TEXT_DARK()),
                text.c_str(), nullptr,
                MAX_W - PX * 2.0f);

    return bh + 12.0f;
}

void ApplyTheme(const std::string& ) {
    ImGuiStyle& s = ImGui::GetStyle();

    s.WindowRounding    = 0.0f;
    s.ChildRounding     = 16.0f;
    s.FrameRounding     = 14.0f;
    s.PopupRounding     = 12.0f;
    s.ScrollbarRounding = 10.0f;
    s.GrabRounding      = 10.0f;
    s.TabRounding       = 10.0f;

    s.WindowPadding     = { 0.0f,  0.0f  };
    s.FramePadding      = { 16.0f, 11.0f };
    s.ItemSpacing       = { 10.0f, 8.0f  };
    s.ScrollbarSize     = 5.0f;
    s.WindowBorderSize  = 0.0f;

    ImVec4* c = s.Colors;
    c[ImGuiCol_WindowBg]          = G::BG_BASE();
    c[ImGuiCol_ChildBg]           = {0.0f, 0.0f, 0.0f, 0.0f};
    c[ImGuiCol_FrameBg]           = G::BG_INPUT();
    c[ImGuiCol_FrameBgHovered]    = {0.95f, 0.96f, 1.00f, 1.00f};
    c[ImGuiCol_FrameBgActive]     = {0.92f, 0.94f, 1.00f, 1.00f};

    c[ImGuiCol_Button]            = G::RED();
    c[ImGuiCol_ButtonHovered]     = {0.99f, 0.55f, 0.55f, 1.00f};
    c[ImGuiCol_ButtonActive]      = {0.80f, 0.28f, 0.28f, 1.00f};

    c[ImGuiCol_ScrollbarBg]       = {0.0f, 0.0f, 0.0f, 0.0f};
    c[ImGuiCol_ScrollbarGrab]     = {0.72f, 0.80f, 0.95f, 0.70f};
    c[ImGuiCol_ScrollbarGrabHovered] = {0.60f, 0.72f, 0.95f, 1.00f};

    c[ImGuiCol_Border]            = {0.88f, 0.90f, 0.96f, 0.80f};
    c[ImGuiCol_Separator]         = {0.88f, 0.90f, 0.96f, 1.00f};

    c[ImGuiCol_Text]              = G::TEXT_DARK();
    c[ImGuiCol_TextDisabled]      = G::TEXT_LIGHT();

    c[ImGuiCol_Header]            = {0.88f, 0.93f, 1.00f, 0.80f};
    c[ImGuiCol_HeaderHovered]     = {0.80f, 0.88f, 1.00f, 1.00f};
}

void LoadAppFont(float size) {
    ImGuiIO& io = ImGui::GetIO();

    const char* font_names[] = {
        "Nunito-Regular.ttf",
        "Nunito-SemiBold.ttf",
    };

    bool loaded = false;
    for (const char* name : font_names) {
        std::string full_path = FontPath(name);
        if (FILE* f = fopen(full_path.c_str(), "rb")) {
            fclose(f);
            io.Fonts->AddFontFromFileTTF(full_path.c_str(), size);
            loaded = true;
            break;
        }
    }

    if (!loaded) {
        io.Fonts->AddFontDefault();
    }
}

bool DrawUI(int window_w, int window_h,
            const ChatHistory& history,
            const CharacterConfig& character,
            std::string& prompt_out) {

    const float W = (float)window_w;
    const float H = (float)window_h;

    static bool  scroll_to_bottom  = true;
    static size_t last_msg_count   = 0;

    if (history.size() != last_msg_count) {
        last_msg_count   = history.size();
        scroll_to_bottom = true;
    }

    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({W, H});
    ImGui::Begin("##root", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize  |
        ImGuiWindowFlags_NoMove     | ImGuiWindowFlags_NoScrollbar|
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImDrawList* dl = ImGui::GetWindowDrawList();

    dl->AddRectFilledMultiColor(
        {0, 0}, {W, H},
        G::u32({0.92f, 0.94f, 0.99f, 1.f}),
        G::u32({0.94f, 0.93f, 0.99f, 1.f}),
        G::u32({0.96f, 0.97f, 1.00f, 1.f}),
        G::u32({0.93f, 0.96f, 0.99f, 1.f}));

    dl->AddCircleFilled({W * 0.85f, H * 0.12f}, 180.f,
                        G::u32a(G::BLUE(),   0.07f));
    dl->AddCircleFilled({W * 0.10f, H * 0.80f}, 140.f,
                        G::u32a(G::RED(),    0.05f));
    dl->AddCircleFilled({W * 0.50f, H * 0.50f}, 220.f,
                        G::u32a(G::YELLOW(), 0.04f));

    const float HDR_H = 68.0f;

    GlassRect(dl, {0, 0}, {W, HDR_H},
              0.0f,
              G::BG_HEADER(), G::BORDER_GLASS());

    dl->AddRectFilled({0, HDR_H - 3}, {W, HDR_H},
                      G::u32(G::YELLOW()));

    const float AX = 38.f, AY = HDR_H * 0.5f, AR = 22.f;
    SoftShadow(dl, {AX-AR, AY-AR}, {AX+AR, AY+AR}, AR);
    dl->AddCircleFilled({AX, AY}, AR, G::u32(G::RED()));
    dl->AddCircleFilled({AX - 6.f, AY - 7.f}, AR * 0.45f,
                        G::u32a({1,1,1,1}, 0.22f));

    char ini[2] = { (char)toupper(character.name[0]), '\0' };
    ImVec2 isz  = ImGui::CalcTextSize(ini);
    dl->AddText({AX - isz.x * 0.5f, AY - isz.y * 0.5f},
                G::u32({1,1,1,1}), ini);

    dl->AddCircleFilled({AX + AR - 5.f, AY + AR - 5.f}, 5.5f,
                        G::u32({0.35f, 0.85f, 0.50f, 1.f}));
    dl->AddCircleFilled({AX + AR - 5.f, AY + AR - 5.f}, 3.5f,
                        G::u32({0.60f, 0.98f, 0.70f, 1.f}));

    ImGui::SetCursorPos({AX + AR + 14.f, 12.f});
    ImGui::TextColored(G::TEXT_DARK(), "%s", character.name.c_str());
    ImGui::SetCursorPos({AX + AR + 14.f, 34.f});
    ImGui::TextColored(G::TEXT_MID(), "%s", character.description.c_str());

    const float INPUT_H  = 62.0f;
    const float CHAT_TOP = HDR_H + 4.0f;
    const float CHAT_BOT = H - INPUT_H - 8.0f;
    const float CHAT_H   = CHAT_BOT - CHAT_TOP;

    ImGui::SetCursorPos({0, CHAT_TOP});
    ImGui::BeginChild("##chat", {W, CHAT_H}, false,
                      ImGuiWindowFlags_NoScrollbar);

    ImVec2 win_pos  = ImGui::GetWindowPos();
    float  scroll_y = ImGui::GetScrollY();
    float  cursor_y = 10.0f;

    ImVec2 clip_min = { win_pos.x,     win_pos.y };
    ImVec2 clip_max = { win_pos.x + W, win_pos.y + CHAT_H };
    dl->PushClipRect(clip_min, clip_max, true);

    for (size_t idx = 0; idx < history.size(); ++idx) {
        bool is_user = (history[idx].role == "user");
        float bh = DrawBubble(dl, win_pos, cursor_y, scroll_y,
                              W, history[idx].content, is_user, character.name);

        ImGui::SetCursorPosY(cursor_y);
        ImGui::Dummy({W, bh});
        cursor_y += bh;

        if (scroll_to_bottom && idx == history.size() - 1) {
            ImGui::SetScrollHereY(1.0f);
            scroll_to_bottom = false;
        }
    }

    dl->PopClipRect();

    ImGui::EndChild();

    GlassRect(dl, {0, CHAT_BOT + 2}, {W, H},
              0.0f, G::BG_HEADER(), G::BORDER_GLASS());

    dl->AddRectFilled({0, CHAT_BOT + 2}, {W, CHAT_BOT + 3.5f},
                      G::u32a(G::BLUE(), 0.45f));

    const float BTN_W = 72.0f;
    const float PAD   = 12.0f;
    const float FY    = CHAT_BOT + 12.0f;

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 22.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg,
                          G::u32(G::BG_INPUT()));
    ImGui::PushStyleColor(ImGuiCol_Border,
                          G::u32({0.80f, 0.87f, 0.99f, 0.90f}));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.5f);

    ImGui::SetCursorPos({PAD, FY});
    ImGui::SetNextItemWidth(W - BTN_W - PAD * 3.0f);
    bool submitted = ImGui::InputText("##msg", input_buf, sizeof(input_buf),
                                      ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(2);

    ImGui::SameLine(0, PAD);
    ImGui::SetCursorPosY(FY);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 22.0f);
    ImGui::PushStyleColor(ImGuiCol_Button,
                          G::u32(G::RED()));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          G::u32({0.99f, 0.58f, 0.58f, 1.f}));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          G::u32({0.78f, 0.28f, 0.28f, 1.f}));
    ImGui::PushStyleColor(ImGuiCol_Text,
                          G::u32({1.f, 1.f, 1.f, 1.f}));

    submitted = submitted || ImGui::Button("  ›  ", {BTN_W, 38.0f});

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar();

    if (submitted && input_buf[0] != '\0') {
        prompt_out = std::string(input_buf);
        input_buf[0] = '\0';
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();
    return submitted && !prompt_out.empty();
}