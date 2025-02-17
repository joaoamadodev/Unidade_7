#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "inc/harry.h"
#include "inc/harry_black.h"
#include "inc/harry_white.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

// Definição dos pinos originais
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
const uint LED_VERMELHO = 16;
const uint LED_AMARELO = 17;
const uint LED_VERDE = 18;
const uint LED_RED_PIN = 13;   // LED RGB Vermelho
const uint LED_GREEN_PIN = 11; // LED RGB Verde
const uint BUZZER_PIN = 10;

// Inicializar o display OLED
void init_display() {
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();
}

// Exibir frames alternando no display
void display_frame(bool is_black) {
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);

    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);
    if (is_black) {
        memcpy(ssd, harry_black, sizeof(harry_black));
    } else {
        memcpy(ssd, harry_white, sizeof(harry_white));
    }
    render_on_display(ssd, &frame_area);
}

// Inicializar os LEDs do semáforo
void init_leds() {
    gpio_init(LED_VERMELHO);
    gpio_init(LED_AMARELO);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_set_dir(LED_AMARELO, GPIO_OUT);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_put(LED_VERMELHO, 0);
    gpio_put(LED_AMARELO, 0);
    gpio_put(LED_VERDE, 0);

    gpio_init(LED_RED_PIN);
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    gpio_put(LED_RED_PIN, 0);
    gpio_put(LED_GREEN_PIN, 0);
}

// Controlar a cor do LED RGB (somente vermelho e verde)
void set_rgb_color(const char *color) {
    gpio_put(LED_RED_PIN, 0);
    gpio_put(LED_GREEN_PIN, 0);

    if (strcmp(color, "red") == 0) {
        gpio_put(LED_RED_PIN, 1);
    } else if (strcmp(color, "green") == 0) {
        gpio_put(LED_GREEN_PIN, 1);
    } else if (strcmp(color, "yellow") == 0) {
        gpio_put(LED_RED_PIN, 1);
        gpio_put(LED_GREEN_PIN, 1);
    } else if (strcmp(color, "blue") == 0) {
        // Simula azul alternando vermelho e verde rapidamente
        for (int i = 0; i < 10; i++) {
            gpio_put(LED_RED_PIN, 1);
            sleep_ms(50);
            gpio_put(LED_RED_PIN, 0);
            gpio_put(LED_GREEN_PIN, 1);
            sleep_ms(50);
            gpio_put(LED_GREEN_PIN, 0);
        }
    }
}

// Inicializar o buzzer
void init_buzzer() {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);
}

// Tocar som no buzzer
void play_sound(uint frequency, uint duration_ms) {
    if (frequency == 0) {
        gpio_put(BUZZER_PIN, 0);
        return;
    }

    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    uint channel_num = pwm_gpio_to_channel(BUZZER_PIN);

    pwm_set_clkdiv(slice_num, 125.0f);
    pwm_set_wrap(slice_num, 1000000 / frequency);
    pwm_set_chan_level(slice_num, channel_num, 500000 / frequency);
    pwm_set_enabled(slice_num, true);
    sleep_ms(duration_ms);
    pwm_set_enabled(slice_num, false);
    gpio_put(BUZZER_PIN, 0);
}

// Definir variáveis da música
int tempo = 180; // Aumentado de 144 para 180
int melody[] = {
    REST, 2, NOTE_D4, 4,
    NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
    NOTE_G4, 2, NOTE_D5, 4,
    NOTE_C5, -2, NOTE_A4, -2,
    NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
    NOTE_F4, 2, NOTE_GS4, 4,
    NOTE_D4, -1, NOTE_D4, 4,
    NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
    NOTE_G4, 2, NOTE_D5, 4,
    NOTE_F5, 2, NOTE_E5, 4,
    NOTE_DS5, 2, NOTE_B4, 4,
    NOTE_DS5, -4, NOTE_D5, 8, NOTE_CS5, 4,
    NOTE_CS4, 2, NOTE_B4, 4,
    NOTE_G4, -1
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote;

// Função para tocar a música e sincronizar com os LEDs
void play_music_with_leds() {
    int divider = 0, noteDuration = 0;

    for (int i = 0; i < notes * 2; i += 2) {
        divider = melody[i + 1];
        if (divider > 0) {
            noteDuration = wholenote / divider;
        } else {
            noteDuration = (wholenote / abs(divider)) * 1.5;
        }

        if (melody[i] != REST) {
            // Aciona um LED diferente conforme a posição da nota
            int led_index = (i / 2) % 4;
            switch (led_index) {
                case 0:
                    gpio_put(LED_VERMELHO, 1);
                    set_rgb_color("red");
                    break;
                case 1:
                    gpio_put(LED_VERDE, 1);
                    set_rgb_color("green");
                    break;
                case 2:
                    gpio_put(LED_AMARELO, 1);
                    set_rgb_color("yellow");
                    break;
                case 3:
                    set_rgb_color("blue");
                    break;
            }

            // Tocar o som correspondente
            play_sound(melody[i], noteDuration * 0.9);
            sleep_ms(noteDuration);

            // Desliga todos os LEDs após a nota
            gpio_put(LED_VERMELHO, 0);
            gpio_put(LED_VERDE, 0);
            gpio_put(LED_AMARELO, 0);
            set_rgb_color("off");
        } else {
            sleep_ms(noteDuration);
        }
    }
}

// Função principal
int main() {
    stdio_init_all();
    init_display();
    init_leds();
    init_buzzer();

    // Calcular a duração da nota inteira com base no tempo
    wholenote = (60000 * 4) / tempo;

    bool frame_black = true;

    while (true) {
        // Alterna os frames no display
        display_frame(frame_black);
        frame_black = !frame_black;

        // Toca a música tema com LEDs sincronizados
        play_music_with_leds();
    }

    return 0;
}
