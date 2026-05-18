<div align="center">
  <img src="assets/icon.ico" alt="PLUA Logo" width="200"/>

  <h1>PLUA</h1>

  <p><em>Your thoughts stay yours.</em></p>

  <p>
    <img src="https://img.shields.io/badge/version-0.1--alpha-red?style=flat-square"/>
    <img src="https://img.shields.io/badge/platform-macOS%20%7C%20Windows-blue?style=flat-square"/>
    <img src="https://img.shields.io/badge/built%20with-C%2B%2B17-yellow?style=flat-square"/>
    <img src="https://img.shields.io/badge/AI-llama.cpp-red?style=flat-square"/>
    <img src="https://img.shields.io/badge/license-MIT-blue?style=flat-square"/>
  </p>
</div>

---

## What is PLUA?

**PLUA** stands for **Poland–Ukraine** — a name born from a personal story. The creator is Ukrainian, currently living in Poland. Two countries, two cultures, one idea: build something meaningful.

PLUA is a **local AI companion** — a desktop chat application that runs entirely on your machine. No cloud. No servers. No subscriptions. No data sent anywhere — ever.

Every word you type, every thought you share, every conversation you have stays on your device and only on your device. Not stored in a data center. Not used to train someone else's model. Not read by anyone. Just yours.

---

## The Idea

Every AI assistant you use today knows more about you than you realise. Your questions, your problems, your doubts, your plans — all of it flows through someone else's servers, gets logged, analysed, and stored indefinitely.

PLUA was built as a response to that reality.

The core belief is simple: **a conversation is a private thing**. When you talk to a friend, you do not expect a third party to be listening. Your AI companion should work the same way.

PLUA runs the entire language model locally on your hardware. There is no API call leaving your machine. There is no account to create. There is no company receiving your data. The AI lives on your computer — and that is the only place it exists.

Privacy here is not a feature. It is the foundation everything else is built on.

---

## How It Works

PLUA uses **llama.cpp** to run a local language model directly on your CPU or GPU — on Apple Silicon it uses Metal acceleration, making it genuinely fast. The interface is built with **Dear ImGui** and **SDL3**, giving it a clean, native feel on both macOS and Windows.

Conversations are stored in daily log files **on your own disk**. The AI reads up to 7 days of history on startup, giving it a natural rolling memory. Its personality, tone, and behavior are fully configurable through a simple JSON file that you control.

**Core technologies:**
- `llama.cpp` — local LLM inference, Metal GPU acceleration on Apple Silicon
- `Dear ImGui` — immediate-mode UI rendered via OpenGL
- `SDL3` — cross-platform window and input management
- `C++17` — the entire application is native, fast, and lightweight

**What never happens:**
- No network requests during conversation
- No telemetry or usage tracking
- No cloud sync
- No account, no login, no email

---

## Current State

PLUA is currently in **alpha**. The foundation is solid:

- Fully offline — zero network activity during use
- Persistent 7-day rolling memory stored locally
- Configurable AI personality via `character.json`
- Apple Glass–inspired UI with soft pastel design
- Custom font support (Nunito)
- macOS `.app` bundle with proper resource packaging
- Daily log rotation with automatic cleanup

---

## The Road Ahead

**Near term**
- Streaming responses — watch the AI type in real time
- Typing indicator while the model is thinking
- Ability to clear or reset conversation history from the UI
- Light / dark theme toggle

**Medium term**
- Multiple AI personalities — switch between different characters
- Voice input — fully local, no cloud speech recognition
- Search through past conversations
- Windows release with full feature parity

**Long term vision**
- PLUA as a daily companion that knows your routines, goals, and moods
- A memory system that goes beyond logs: structured personal knowledge
- Mobile companion app — local inference, same privacy guarantees
- Full offline voice: speak and listen without a single byte leaving your device

The direction is always the same: more capable, more personal, never connected.

---

## Philosophy

> *"Your thoughts are yours. They should stay that way."*

Every major AI product today is built on a trade: you give your data, you get intelligence in return. PLUA rejects that trade entirely.

It is not the smartest AI. It will not always have the best answer. But it is the one AI that will never use what you tell it against you — because what you tell it never leaves the room.

Built with care. Named with meaning. Designed to stay private.

---

<div align="center">
  <sub>Made by <strong>Fev1L</strong></sub>
</div>
