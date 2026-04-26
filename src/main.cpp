#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <iostream>
#include <string>
#include <vector>
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"
#include "llama.h"

static llama_model* model = nullptr;
static llama_context* ctx = nullptr;

static void llama_batch_add(llama_batch & batch, llama_token id, llama_pos pos, const std::vector<llama_seq_id> & seq_ids, bool logits) {
    batch.token[batch.n_tokens] = id;
    batch.pos[batch.n_tokens] = pos;
    batch.n_seq_id[batch.n_tokens] = seq_ids.size();
    for (size_t i = 0; i < seq_ids.size(); ++i) {
        batch.seq_id[batch.n_tokens][i] = seq_ids[i];
    }
    batch.logits[batch.n_tokens] = logits;
    batch.n_tokens++;
}

static bool InitLlama() {
    llama_backend_init();
    llama_model_params mparams = llama_model_default_params();
    model = llama_model_load_from_file("/Users/fesedevladicah/Documents/PLUA/third_party/llama.cpp/models/Llama3.gguf", mparams);
    if (!model) return false;
    llama_context_params cparams = llama_context_default_params();
    cparams.n_ctx = 2048;
    ctx = llama_init_from_model(model, cparams);
    return ctx != nullptr;
}

static std::string RunLlama(const std::string& prompt) {
    if (!model || prompt.empty()) return "";
    if (ctx) llama_free(ctx);

    llama_context_params cparams = llama_context_default_params();
    cparams.n_ctx = 2048;
    ctx = llama_init_from_model(model, cparams);

    std::string formatted_prompt =
        "<|begin_of_text|><|start_header_id|>user<|end_header_id|>\n\n" + prompt +
        "<|eot_id|><|start_header_id|>assistant<|end_header_id|>\n\n";

    const llama_vocab* vocab = llama_model_get_vocab(model);
    std::vector<llama_token> tokens(formatted_prompt.size() + 64);
    int n_tokens = llama_tokenize(vocab, formatted_prompt.c_str(), (int)formatted_prompt.size(), tokens.data(), (int)tokens.size(), true, true);
    tokens.resize(n_tokens);

    llama_batch batch = llama_batch_init(2048, 0, 1);
    for (int i = 0; i < n_tokens; i++) {
        llama_batch_add(batch, tokens[i], i, {0}, (i == n_tokens - 1));
    }

    llama_decode(ctx, batch);

    struct llama_sampler * smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());
    llama_sampler_chain_add(smpl, llama_sampler_init_temp(0.4f));
    llama_sampler_chain_add(smpl, llama_sampler_init_top_p(0.9f, 1));
    llama_sampler_chain_add(smpl, llama_sampler_init_dist(1234));

    std::string result = "";
    for (int i = 0; i < 400; i++) {
        llama_token next = llama_sampler_sample(smpl, ctx, -1);

        if (llama_vocab_is_eog(vocab, next) || next == llama_vocab_eos(vocab)) break;

        char buf[256];
        int len = llama_token_to_piece(vocab, next, buf, sizeof(buf), 0, true);
        if (len > 0) result.append(buf, len);

        batch.n_tokens = 0;
        llama_batch_add(batch, next, n_tokens + i, {0}, true);
        if (llama_decode(ctx, batch) != 0) break;
    }

    size_t think_end = result.find("</think>");
    if (think_end != std::string::npos) {
        result = result.substr(think_end + 8);
    }

    size_t start = result.find_first_not_of(" \n\r\t");
    if (start != std::string::npos) result = result.substr(start);

    llama_sampler_free(smpl);
    llama_batch_free(batch);
    return result;
}


int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) return -1;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow("PLUA AI", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl = SDL_GL_CreateContext(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForOpenGL(window, gl);
    ImGui_ImplOpenGL3_Init("#version 150");

    InitLlama();

    static char input_buf[1024] = "";
    std::string response;
    bool running = true;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT) running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("PLUA AI");
        if (ImGui::InputText("Input", input_buf, sizeof(input_buf), ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::Button("Send")) {
            response = RunLlama(input_buf);
            input_buf[0] = '\0';
        }
        ImGui::Separator();
        ImGui::TextWrapped("%s", response.c_str());
        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, 1280, 720);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    if (ctx) llama_free(ctx);
    if (model) llama_model_free(model);
    llama_backend_free();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(gl);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
