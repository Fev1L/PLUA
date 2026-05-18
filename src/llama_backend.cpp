//
// Created by Fev1L on 01.05.2026.
//

#include "../headers/llama_backend.h"
#include "llama.h"
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

static llama_model*   model = nullptr;
static llama_context* ctx   = nullptr;

static void llama_batch_add(llama_batch& batch, llama_token id, llama_pos pos,
                             const std::vector<llama_seq_id>& seq_ids, bool logits) {
    batch.token[batch.n_tokens]    = id;
    batch.pos[batch.n_tokens]      = pos;
    batch.n_seq_id[batch.n_tokens] = seq_ids.size();
    for (size_t i = 0; i < seq_ids.size(); ++i)
        batch.seq_id[batch.n_tokens][i] = seq_ids[i];
    batch.logits[batch.n_tokens] = logits;
    batch.n_tokens++;
}

static std::string GetCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);

    std::ostringstream ss;
    ss << std::put_time(&tm, "%A, %d %B %Y, %H:%M");
    return ss.str();
}

static std::string BuildPrompt(const ChatHistory& history,
                                const CharacterConfig& character) {
    std::string datetime = GetCurrentDateTime();

    std::string system_prompt =
        "Your name is " + character.name + ". "
        "Your tone is " + character.tone + " and your style is "
        + character.style + ". " + character.description + " ";

    if (!character.behavior_rules.empty()) {
        system_prompt += "Follow these rules strictly: ";
        for (size_t i = 0; i < character.behavior_rules.size(); ++i)
            system_prompt += std::to_string(i+1) + ") "
                           + character.behavior_rules[i] + ". ";
    }
    system_prompt += "Current date and time: " + datetime + ".";

    std::string prompt =
        "<|start_header_id|>system<|end_header_id|>\n\n"
        + system_prompt + "<|eot_id|>";

    const size_t MAX_MESSAGES = 40;
    size_t start_idx = history.size() > MAX_MESSAGES
                     ? history.size() - MAX_MESSAGES : 0;

    for (size_t i = start_idx; i < history.size(); ++i) {
        prompt += "<|start_header_id|>" + history[i].role
               + "<|end_header_id|>\n\n";
        prompt += history[i].content + "<|eot_id|>";
    }

    prompt += "<|start_header_id|>assistant<|end_header_id|>\n\n";
    return prompt;
}

bool InitLlama() {
    llama_backend_init();
    llama_model_params mparams = llama_model_default_params();
    std::string model_file = ModelPath("Llama3.gguf");
    model = llama_model_load_from_file(model_file.c_str(), mparams);
    if (!model) return false;

    llama_context_params cparams = llama_context_default_params();
    cparams.n_ctx = 4096;
    cparams.n_batch = 4096;
    ctx = llama_init_from_model(model, cparams);
    return ctx != nullptr;
}

void RunLlama(ChatHistory& history, const CharacterConfig& character) {
    if (!model || history.empty()) return;
    if (ctx) llama_free(ctx);

    llama_context_params cparams = llama_context_default_params();
    cparams.n_ctx = 4096;
    cparams.n_batch = 4096;
    ctx = llama_init_from_model(model, cparams);

    std::string formatted_prompt = BuildPrompt(history, character);

    const llama_vocab* vocab = llama_model_get_vocab(model);
    std::vector<llama_token> tokens(formatted_prompt.size() + 64);
    int n_tokens = llama_tokenize(vocab, formatted_prompt.c_str(),
                              (int)formatted_prompt.size(),
                              tokens.data(), (int)tokens.size(),
                              true,
                              true);
    tokens.resize(n_tokens);

    llama_batch batch = llama_batch_init(4096, 0, 1);

    if (n_tokens > 4096) {
        n_tokens = 4096;
        tokens.resize(n_tokens);
    }

    for (int i = 0; i < n_tokens; i++)
        llama_batch_add(batch, tokens[i], i, {0}, (i == n_tokens - 1));

    llama_decode(ctx, batch);

    llama_sampler* smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());
    llama_sampler_chain_add(smpl, llama_sampler_init_temp(0.4f));
    llama_sampler_chain_add(smpl, llama_sampler_init_top_p(0.9f, 1));
    llama_sampler_chain_add(smpl, llama_sampler_init_dist(1234));

    std::string result;
    for (int i = 0; i < 400; i++) {
        llama_token next = llama_sampler_sample(smpl, ctx, -1);
        if (llama_vocab_is_eog(vocab, next) || next == llama_vocab_eos(vocab)) break;

        char buf[256];
        int len = llama_token_to_piece(vocab, next, buf, sizeof(buf), 0, false);
        if (len > 0) result.append(buf, len);

        batch.n_tokens = 0;
        llama_batch_add(batch, next, n_tokens + i, {0}, true);
        if (llama_decode(ctx, batch) != 0) break;
    }

    size_t think_end = result.find("</think>");
    if (think_end != std::string::npos)
        result = result.substr(think_end + 8);

    size_t start = result.find_first_not_of(" \n\r\t");
    if (start != std::string::npos)
        result = result.substr(start);

    llama_sampler_free(smpl);
    llama_batch_free(batch);

    history.push_back({"assistant", result});
}

void ShutdownLlama() {
    if (ctx)   llama_free(ctx);
    if (model) llama_model_free(model);
    llama_backend_free();
}