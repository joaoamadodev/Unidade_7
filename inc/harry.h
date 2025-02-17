#ifndef HARRY_H
#define HARRY_H

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definição das notas musicais
#define REST 0
#define NOTE_D4  294
#define NOTE_G4  392
#define NOTE_AS4 466
#define NOTE_A4  440
#define NOTE_D5  587
#define NOTE_C5  523
#define NOTE_F4  349
#define NOTE_GS4 415
#define NOTE_F5  698
#define NOTE_E5  659
#define NOTE_DS5 622
#define NOTE_B4  494
#define NOTE_CS5 554
#define NOTE_G4  392
#define NOTE_CS4 277 // Adicionado para corrigir o erro anterior

// Declarações externas (serão definidas no harry.c)
extern int tempo;
extern int melody[];
extern int notes;
extern int wholenote;

// Função para tocar a música
void play_music();

// Função para tocar um som com frequência e duração específicas
void play_sound(uint frequency, uint duration_ms);

#endif // HARRY_H
