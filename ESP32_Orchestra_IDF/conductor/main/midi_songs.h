#ifndef MIDI_SONGS_H
#define MIDI_SONGS_H

#include "orchestra_common.h"

// Note Event Structure for Orchestra
typedef struct {
    uint8_t note;
    uint16_t duration_ms;
    uint16_t delay_ms;
} note_event_t;

// Song Part Structure
typedef struct {
    const note_event_t* events;
    uint16_t event_count;
    const char* part_name;
} song_part_t;

// Complete Song Structure
typedef struct {
    const char* song_name;
    uint8_t song_id;
    uint8_t tempo_bpm;
    uint8_t part_count;
    const song_part_t* parts;
} orchestra_song_t;

// =============================================================
// 🎵 SONG: Let It Go (Hook Only) - 5 Parts
// =============================================================

// Part A: Main Melody (Vocal Line)
static const note_event_t let_it_go_melody_hook[] = {
    {NOTE_C5, 400, 100}, {NOTE_G4, 400, 100}, {NOTE_G4, 200, 50}, {NOTE_A4, 400, 100}, // Let it go, let it
    {NOTE_G4, 1000, 200}, // go
    {NOTE_C5, 400, 100}, {NOTE_G4, 400, 100}, {NOTE_G4, 200, 50}, {NOTE_A4, 400, 100}, // Can't hold it back an-
    {NOTE_G4, 1000, 200}, // y-more
    {NOTE_REST, 0, 0}
};

// Part B: Harmony
static const note_event_t let_it_go_harmony_hook[] = {
    {NOTE_E4, 400, 100}, {NOTE_E4, 400, 100}, {NOTE_E4, 200, 50}, {NOTE_F4, 400, 100},
    {NOTE_E4, 1000, 200},
    {NOTE_E4, 400, 100}, {NOTE_E4, 400, 100}, {NOTE_E4, 200, 50}, {NOTE_F4, 400, 100},
    {NOTE_E4, 1000, 200},
    {NOTE_REST, 0, 0}
};

// Part C: Bass Line
static const note_event_t let_it_go_bass_hook[] = {
    {NOTE_A2, 2200, 0}, // Corresponds to Am chord
    {NOTE_F2, 2200, 0}, // Corresponds to F chord
    {NOTE_C3, 2200, 0}, // Corresponds to C chord
    {NOTE_G2, 2200, 0}, // Corresponds to G chord
    {NOTE_REST, 0, 0}
};

// Part D: Arpeggio/Rhythm
static const note_event_t let_it_go_arpeggio_hook[] = {
    {NOTE_A3, 200, 50}, {NOTE_E4, 200, 50}, {NOTE_A4, 200, 50}, {NOTE_E4, 200, 150},
    {NOTE_A3, 200, 50}, {NOTE_E4, 200, 50}, {NOTE_A4, 200, 50}, {NOTE_E4, 200, 150},
    {NOTE_F3, 200, 50}, {NOTE_C4, 200, 50}, {NOTE_F4, 200, 50}, {NOTE_C4, 200, 150},
    {NOTE_F3, 200, 50}, {NOTE_C4, 200, 50}, {NOTE_F4, 200, 50}, {NOTE_C4, 200, 150},
    {NOTE_C3, 200, 50}, {NOTE_G3, 200, 50}, {NOTE_C4, 200, 50}, {NOTE_G3, 200, 150},
    {NOTE_C3, 200, 50}, {NOTE_G3, 200, 50}, {NOTE_C4, 200, 50}, {NOTE_G3, 200, 150},
    {NOTE_G2, 200, 50}, {NOTE_D3, 200, 50}, {NOTE_G3, 200, 50}, {NOTE_D3, 200, 150},
    {NOTE_G2, 200, 50}, {NOTE_D3, 200, 50}, {NOTE_G3, 200, 50}, {NOTE_D3, 200, 150},
    {NOTE_REST, 0, 0}
};

// Part E: Pad (Sustained Notes)
static const note_event_t let_it_go_pad_hook[] = {
    {NOTE_C4, 2200, 0},
    {NOTE_A3, 2200, 0},
    {NOTE_E4, 2200, 0},
    {NOTE_B3, 2200, 0},
    {NOTE_REST, 0, 0}
};


// Let It Go (Hook) Parts Array
static const song_part_t let_it_go_hook_parts[] = {
    {let_it_go_melody_hook,   sizeof(let_it_go_melody_hook)/sizeof(note_event_t) - 1,   "Melody"},
    {let_it_go_harmony_hook,  sizeof(let_it_go_harmony_hook)/sizeof(note_event_t) - 1,  "Harmony"},
    {let_it_go_bass_hook,     sizeof(let_it_go_bass_hook)/sizeof(note_event_t) - 1,     "Bass"},
    {let_it_go_arpeggio_hook, sizeof(let_it_go_arpeggio_hook)/sizeof(note_event_t) - 1, "Arpeggio"},
    {let_it_go_pad_hook,      sizeof(let_it_go_pad_hook)/sizeof(note_event_t) - 1,      "Pad"}
};

// =============================================================
// 🎵 All Songs Database
// =============================================================
static const orchestra_song_t all_songs[] = {
    {
        .song_name = "Let It Go (Hook)",
        .song_id = SONG_LET_IT_GO,
        .tempo_bpm = 125, // ปรับความเร็วให้เร็วขึ้นเล็กน้อย
        .part_count = 5,
        .parts = let_it_go_hook_parts
    }
};

#define TOTAL_SONGS (sizeof(all_songs) / sizeof(orchestra_song_t))

// Helper function to get song by ID
static inline const orchestra_song_t* get_song_by_id(uint8_t song_id) {
    for (int i = 0; i < TOTAL_SONGS; i++) {
        if (all_songs[i].song_id == song_id) {
            return &all_songs[i];
        }
    }
    return NULL;
}

#endif // MIDI_SONGS_H