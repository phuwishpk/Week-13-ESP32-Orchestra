#ifndef ORCHESTRA_COMMON_H
#define ORCHESTRA_COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_timer.h"

// ESP-NOW Configuration
#define BROADCAST_ADDR {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
#define MAX_MUSICIANS 5
#define ESPNOW_CHANNEL 7

// Message Types for Orchestra Communication
typedef enum {
    MSG_SONG_START = 1,     // เริ่มเพลง
    MSG_PLAY_NOTE = 2,      // เล่นโน๊ต
    MSG_STOP_NOTE = 3,      // หยุดโน๊ต
    MSG_SONG_END = 4,       // จบเพลง
    MSG_SYNC_TIME = 5,      // ซิงค์เวลา
    MSG_HEARTBEAT = 6       // ตรวจสอบการเชื่อมต่อ
} message_type_t;

// Song IDs (เปลี่ยนเป็นเพลง Let It Go)
typedef enum {
    SONG_LET_IT_GO = 1     // Let It Go (from Frozen)
} song_id_t;

// Musician Parts
typedef enum {
    PART_A = 0,    // Melody
    PART_B = 1,    // Harmony
    PART_C = 2,    // Bass
    PART_D = 3,    // Arpeggio/Rhythm
    PART_E = 4     // Pad/Counter-Melody
} part_id_t;

// Orchestra Message Structure
typedef struct {
    message_type_t type;
    uint8_t song_id;
    uint8_t part_id;
    uint8_t note;
    uint8_t velocity;
    uint32_t timestamp;
    uint16_t duration_ms;
    uint8_t tempo_bpm;
    uint8_t checksum;
} __attribute__((packed)) orchestra_message_t;

// Note definitions (MIDI note numbers)
#define NOTE_C6  84
#define NOTE_B5  83
#define NOTE_A5  81
#define NOTE_G5  79
#define NOTE_F5  77
#define NOTE_E5  76
#define NOTE_D5  74
#define NOTE_C5  72
#define NOTE_B4  71
#define NOTE_A4  69
#define NOTE_G4  67
#define NOTE_F4  65
#define NOTE_E4  64
#define NOTE_D4  62
#define NOTE_C4  60

#define NOTE_B3  59
#define NOTE_A3  57
#define NOTE_G3  55
#define NOTE_F3  53
#define NOTE_E3  52
#define NOTE_D3  50
#define NOTE_C3  48

#define NOTE_B2  47
#define NOTE_A2  45
#define NOTE_G2  43
#define NOTE_F2  41
#define NOTE_E2  40
#define NOTE_D2  38
#define NOTE_C2  36

#define NOTE_REST 0   // เงียบ

// GPIO Pins
#define BUZZER_PIN    18
#define STATUS_LED    2
#define BUTTON_PIN    0

// LEDC Configuration
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES           LEDC_TIMER_8_BIT
#define LEDC_DUTY               (128)
#define LEDC_FREQUENCY          (4000)

// Utility Functions
static inline uint8_t calculate_checksum(const orchestra_message_t* msg) {
    uint8_t sum = 0;
    const uint8_t* data = (const uint8_t*)msg;
    for (size_t i = 0; i < sizeof(orchestra_message_t) - 1; i++) {
        sum += data[i];
    }
    return sum;
}

static inline bool verify_checksum(const orchestra_message_t* msg) {
    return calculate_checksum(msg) == msg->checksum;
}

static inline float midi_note_to_frequency(uint8_t note) {
    if (note == 0) return 0.0;
    return 440.0f * powf(2.0f, (note - 69) / 12.0f);
}

static inline uint32_t get_time_ms(void) {
    return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

// Status LED patterns
typedef enum {
    LED_OFF, LED_ON, LED_SLOW_BLINK, LED_FAST_BLINK, LED_HEARTBEAT
} led_pattern_t;

#endif // ORCHESTRA_COMMON_H