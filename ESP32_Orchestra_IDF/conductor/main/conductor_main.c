/*
 * ESP32 Orchestra Conductor Main - ESP-IDF Version
 * ผู้บังคับการวงดนตรีที่ควบคุมการเล่นเพลงผ่าน ESP-NOW
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "orchestra_common.h"
#include "midi_songs.h"
#include "espnow_conductor.h"

static const char *TAG = "MAIN";

// External function from espnow_conductor.c
extern void conductor_send_song_events(void);

// Button and LED state
static led_pattern_t current_led_pattern = LED_SLOW_BLINK;
static uint32_t led_last_update = 0;
static bool led_state = false;

// Task handles
static TaskHandle_t button_task_handle = NULL;
static TaskHandle_t led_task_handle = NULL;
static TaskHandle_t orchestra_task_handle = NULL;

// Function Prototypes
static void setup_gpio(void);
static void button_task(void *pvParameters);
static void led_task(void *pvParameters);
static void orchestra_task(void *pvParameters);
static void handle_button_press(void);

void app_main(void) {
    ESP_LOGI(TAG, "🎵 ESP32 Orchestra Conductor Starting...");
    
    setup_gpio();
    
    if (espnow_conductor_init() != ESP_OK) {
        ESP_LOGE(TAG, "❌ Failed to initialize ESP-NOW");
        current_led_pattern = LED_FAST_BLINK;
    } else {
        current_led_pattern = LED_SLOW_BLINK;
        ESP_LOGI(TAG, "✅ Conductor ready!");
    }
    
    ESP_LOGI(TAG, "🎼 Now Playing: %s (%d parts, %d BPM)", 
             all_songs[0].song_name, all_songs[0].part_count, all_songs[0].tempo_bpm);
    ESP_LOGI(TAG, "📝 Press BOOT button to START/STOP the song!");
    
    xTaskCreate(button_task, "button_task", 2048, NULL, 5, &button_task_handle);
    xTaskCreate(led_task, "led_task", 2048, NULL, 3, &led_task_handle);
    xTaskCreate(orchestra_task, "orchestra_task", 4096, NULL, 4, &orchestra_task_handle);
    
    ESP_LOGI(TAG, "🚀 All tasks created, conductor is running!");
}

static void setup_gpio(void) {
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << STATUS_LED);
    io_conf.mode = GPIO_MODE_OUTPUT;
    gpio_config(&io_conf);
    
    io_conf.pin_bit_mask = (1ULL << BUTTON_PIN);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    
    ESP_LOGI(TAG, "✅ GPIO setup complete");
}

static void button_task(void *pvParameters) {
    bool last_button_state = true;
    while (1) {
        bool current_button_state = gpio_get_level(BUTTON_PIN);
        if (last_button_state == true && current_button_state == false) {
            vTaskDelay(pdMS_TO_TICKS(20)); // Debounce
            if(gpio_get_level(BUTTON_PIN) == false) {
                 ESP_LOGI(TAG, "🔘 Button pressed");
                 handle_button_press();
            }
        }
        last_button_state = current_button_state;
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

static void handle_button_press(void) {
    // Since there's only one song, any press will toggle play/stop
    if (is_conductor_playing()) {
        if (stop_song()) {
            ESP_LOGI(TAG, "⏹️ Song stopped");
            current_led_pattern = LED_SLOW_BLINK;
        } else {
            ESP_LOGE(TAG, "❌ Failed to stop song");
            current_led_pattern = LED_FAST_BLINK;
        }
    } else {
        // VVVVVV ---- จุดที่แก้ไข ---- VVVVVV
        if (start_song(SONG_LET_IT_GO)) {
            const orchestra_song_t* song = get_song_by_id(SONG_LET_IT_GO);
            ESP_LOGI(TAG, "▶️ Playing: %s", song ? song->song_name : "Unknown");
            current_led_pattern = LED_ON;
        } else {
            ESP_LOGE(TAG, "❌ Failed to start song");
            current_led_pattern = LED_FAST_BLINK;
        }
    }
}

static void led_task(void *pvParameters) {
    while (1) {
        uint32_t current_time = get_time_ms();
        switch (current_led_pattern) {
            case LED_OFF:
                gpio_set_level(STATUS_LED, 0);
                break;
            case LED_ON:
                gpio_set_level(STATUS_LED, 1);
                break;
            case LED_SLOW_BLINK:
                if (current_time - led_last_update > 500) {
                    led_state = !led_state;
                    gpio_set_level(STATUS_LED, led_state);
                    led_last_update = current_time;
                }
                break;
            case LED_FAST_BLINK:
                if (current_time - led_last_update > 100) {
                    led_state = !led_state;
                    gpio_set_level(STATUS_LED, led_state);
                    led_last_update = current_time;
                }
                break;
            default: break;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void orchestra_task(void *pvParameters) {
    uint32_t last_heartbeat = 0;
    while (1) {
        uint32_t current_time = get_time_ms();
        conductor_send_song_events();
        
        if (current_time - last_heartbeat > 5000) {
            send_heartbeat();
            last_heartbeat = current_time;
        }
        
        update_conductor_status();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}