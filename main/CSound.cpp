#include "CSound.h"
#include <fmod_errors.h>
#include <iostream>

FMOD_SYSTEM* CSound::g_sound_system = nullptr;
bool CSound::g_initialized = false;

CSound::CSound(const char* path, bool loop) {
    if (!g_initialized) {
        std::cerr << "[CSound] FMOD system not initialized. Call CSound::Init() before creating sounds.\n";
        m_sound = nullptr;
        m_channel = nullptr;
        m_volume = SOUND_DEFAULT;
        return;
    }

    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;
    FMOD_RESULT result = FMOD_System_CreateSound(g_sound_system, path, mode, 0, &m_sound);

    if (result != FMOD_OK) {
        std::cerr << "[CSound] Failed to create sound: " << FMOD_ErrorString(result) << "\n";
        m_sound = nullptr;
    }

    m_channel = nullptr;
    m_volume = SOUND_DEFAULT;
}

CSound::~CSound() {
    if (m_sound) {
        FMOD_RESULT result = FMOD_Sound_Release(m_sound);
        if (result != FMOD_OK) {
            std::cerr << "[CSound] Failed to release sound: " << FMOD_ErrorString(result) << "\n";
        }
    }
}

int CSound::Init() {
    if (g_initialized) return 0;

    FMOD_RESULT result = FMOD_System_Create(&g_sound_system, FMOD_VERSION); // ← 두 번째 인자 추가됨
    if (result != FMOD_OK) {
        std::cerr << "[CSound] Failed to create FMOD system: " << FMOD_ErrorString(result) << "\n";
        return -1;
    }

    result = FMOD_System_Init(g_sound_system, 32, FMOD_INIT_NORMAL, NULL);
    if (result != FMOD_OK) {
        std::cerr << "[CSound] Failed to initialize FMOD system: " << FMOD_ErrorString(result) << "\n";
        return -1;
    }

    g_initialized = true;
    return 0;
}

int CSound::Release() {
    if (!g_initialized) return 0;

    FMOD_RESULT result;

    result = FMOD_System_Close(g_sound_system);
    if (result != FMOD_OK) {
        std::cerr << "[CSound] Failed to close FMOD system: " << FMOD_ErrorString(result) << "\n";
    }

    result = FMOD_System_Release(g_sound_system);
    if (result != FMOD_OK) {
        std::cerr << "[CSound] Failed to release FMOD system: " << FMOD_ErrorString(result) << "\n";
    }

    g_sound_system = nullptr;
    g_initialized = false;

    return 0;
}

int CSound::play() {
    if (!m_sound) return -1;

    FMOD_RESULT result = FMOD_System_PlaySound(g_sound_system, m_sound, NULL, false, &m_channel);
    if (result != FMOD_OK) {
        std::cerr << "[CSound] Play failed: " << FMOD_ErrorString(result) << "\n";
        return -1;
    }

    FMOD_Channel_SetVolume(m_channel, m_volume);

    return 0;
}

int CSound::pause() {
    if (!m_channel) return -1;
    return FMOD_Channel_SetPaused(m_channel, true);
}

int CSound::resume() {
    if (!m_channel) return -1;
    return FMOD_Channel_SetPaused(m_channel, false);
}

int CSound::stop() {
    if (!m_channel) return -1;
    return FMOD_Channel_Stop(m_channel);
}

int CSound::volumeUp() {
    if (m_volume < SOUND_MAX) {
        m_volume += SOUND_WEIGHT;
        if (m_volume > SOUND_MAX) m_volume = SOUND_MAX;
    }

    if (m_channel) {
        FMOD_Channel_SetVolume(m_channel, m_volume);
    }

    return 0;
}

int CSound::volumeDown() {
    if (m_volume > SOUND_MIN) {
        m_volume -= SOUND_WEIGHT;
        if (m_volume < SOUND_MIN) m_volume = SOUND_MIN;
    }

    if (m_channel) {
        FMOD_Channel_SetVolume(m_channel, m_volume);
    }

    return 0;
}

int CSound::Update() {
    if (!m_channel) return -1;

    FMOD_Channel_IsPlaying(m_channel, &m_bool);

    if (m_bool && g_initialized) {
        FMOD_System_Update(g_sound_system);
    }

    return 0;
}
