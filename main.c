#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

/* Definições de pinos GPIO para os LEDs do semáforo */
#define LED_GREEN 11    // LED verde - Sinal "Siga"
#define LED_BLUE 12  // LED azul na bitdoglab e amarelo no simulador Wokwi - Sinal "Atenção"
#define LED_RED 13 // LED vermelho - Sinal "Pare"

/* Temporização do ciclo do semáforo (milissegundos) */
#define TIMER_CHANGE_SINAL 3000

/**
 * @brief Inicializa os pinos GPIO para controle do semáforo
 * 
 * Configura os pinos dos LEDs como saídas e define estado inicial:
 * - LED vermelho LIGADO
 * - LED amarelo DESLIGADO
 * - LED verde DESLIGADO
 */
void init_gpio() {
    // Inicializa os pinos GPIO
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);
    gpio_init(LED_RED);

    // Configura direção dos pinos como saída
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_set_dir(LED_RED, GPIO_OUT);

    // Define estado inicial (Vermelho ligado)
    gpio_put(LED_RED, 1);
    gpio_put(LED_BLUE, 0);
    gpio_put(LED_GREEN, 0);
}

/**
 * @brief Função de callback do timer para mudanças de estado do semáforo
 * 
 * @param timer Ponteiro para estrutura de timer repetitivo
 * @return true Manter o timer ativo
 * @return false Parar o timer (não utilizado)
 * 
 * Implementa o ciclo completo do semáforo:
 * 1. Vermelho -> Amarelo -> Verde
 * Atualiza os LEDs e registra mudanças de estado na saída serial
 */
bool change_trafficLight(struct repeating_timer *timer) {
    static int trafficLight_status = 1; // Estado inicial: Vermelho (1)

    // Ciclo de estados: 1 → 2 → 3 → 1...
    trafficLight_status = (trafficLight_status % 3) + 1;

    // Atualiza saídas dos LEDs conforme estado atual
    gpio_put(LED_GREEN, trafficLight_status == 3);
    gpio_put(LED_BLUE, trafficLight_status == 2);
    gpio_put(LED_RED, trafficLight_status == 1);

    // Registra estado atual na saída serial
    switch (trafficLight_status) {
        case 1:
            printf("Pare: Sinal vermelho ativo\n");
            break;
        case 2:
            printf("Atenção: Sinal amarelo ativo (led azul da BitDogLab)\n");
            break;
        case 3:
            printf("Siga: Sinal verde ativo\n");
            break;
    }

    return true;
}

/**
 * @brief Ponto principal de entrada do programa
 * 
 * Inicializa componentes do sistema e executa loop infinito
 */
int main() {
    stdio_init_all(); // Inicializa comunicação serial
    init_gpio(); // Configuração inicial dos GPIOs

    // Configura timer repetitivo para mudanças de estado
    struct repeating_timer timer;
    add_repeating_timer_ms(
        TIMER_CHANGE_SINAL,
        change_trafficLight,
        NULL,
        &timer
    );

    // Loop principal de supervisão
    while (true) {
        printf("Sistema de Semáforo\n");
        sleep_ms(1000); // Evita consumo excessivo de CPU
    }

    return 0; // Inalcançável em operação normal
}