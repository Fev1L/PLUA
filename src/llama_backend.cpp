//
// Created by Fev1L on 01.05.2026.
//

#include "../headers/llama_backend.h"
#include "llama.h"
#include <vector>

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

static std::string BuildPrompt(const ChatHistory& history) {
    std::string prompt = "<|begin_of_text|>";
    for (const auto& msg : history) {
        prompt += "<|start_header_id|>" + msg.role + "<|end_header_id|>\n\n";
        prompt += msg.content + "<|eot_id|>";
    }
    prompt += "<|start_header_id|>assistant<|end_header_id|>\n\n";
    return prompt;
}

bool InitLlama() {
    llama_backend_init();
    llama_model_params mparams = llama_model_default_params();
    model = llama_model_load_from_file(
        "/Users/fesedevladicah/Documents/PLUA/third_party/llama.cpp/models/Llama3.gguf", mparams);
    if (!model) return false;

    llama_context_params cparams = llama_context_default_params();
    cparams.n_ctx = 4096;
    ctx = llama_init_from_model(model, cparams);
    return ctx != nullptr;
}

void RunLlama(ChatHistory& history) {
    if (!model || history.empty()) return;
    if (ctx) llama_free(ctx);

    llama_context_params cparams = llama_context_default_params();
    cparams.n_ctx = 4096;
    ctx = llama_init_from_model(model, cparams);

    std::string formatted_prompt = BuildPrompt(history);

    const llama_vocab* vocab = llama_model_get_vocab(model);
    std::vector<llama_token> tokens(formatted_prompt.size() + 64);
    int n_tokens = llama_tokenize(vocab, formatted_prompt.c_str(),
                                  (int)formatted_prompt.size(),
                                  tokens.data(), (int)tokens.size(), true, true);
    tokens.resize(n_tokens);

    llama_batch batch = llama_batch_init(4096, 0, 1);
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
        int len = llama_token_to_piece(vocab, next, buf, sizeof(buf), 0, true);
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