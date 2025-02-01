#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

/* Definições de pinos GPIO para os LEDs do semáforo */
#define LED_VERDE 11    // LED verde - Sinal "Siga"
#define LED_AZUL 12  // LED AZUL - Sinal "Atenção"
#define LED_VERMELHO 13 // LED vermelho - Sinal "Pare"

/* Temporização do ciclo do semáforo (milissegundos) */
#define TEMPO_MUDANCA_SINAL 3000

/**
 * @brief Executa teste inicial de hardware
 * 
 * Verifica o funcionamento básico de todos os LEDs
 * Pisca cada LED individualmente por 500ms
 */
void teste_inicial_hardware() {
    printf("\n🚦 Iniciando Teste de Hardware\n");
    
    const uint LEDS[] = {LED_VERMELHO, LED_AZUL, LED_VERDE};
    const char* NOMES[] = {"Vermelho", "AZUL", "Verde"};
    
    for(int i = 0; i < 3; i++) {
        printf("Testando LED %s... ", NOMES[i]);
        gpio_put(LEDS[i], 1);
        sleep_ms(500);
        gpio_put(LEDS[i], 0);
        printf("OK\n");
        sleep_ms(200);
    }
    printf("✅ Teste de Hardware Concluído\n\n");
}

/**
 * @brief Inicializa os pinos GPIO para controle do semáforo
 */
void inicializar_gpio() {
    gpio_init(LED_VERDE);
    gpio_init(LED_AZUL);
    gpio_init(LED_VERMELHO);

    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);

    gpio_put(LED_VERMELHO, 1);
    gpio_put(LED_AZUL, 0);
    gpio_put(LED_VERDE, 0);
}

/**
 * @brief Função de callback do timer para mudanças de estado
 */
bool alterar_estado_semaforo(struct repeating_timer *timer) {
    static int estado_semaforo = 1;

    estado_semaforo = (estado_semaforo % 3) + 1;

    gpio_put(LED_VERDE, estado_semaforo == 3);
    gpio_put(LED_AZUL, estado_semaforo == 2);
    gpio_put(LED_VERMELHO, estado_semaforo == 1);

    // Teste de transição de estados
    printf("Teste de Estado: ");
    switch (estado_semaforo) {
        case 1:
            printf("Vermelho %s\n", 
                gpio_get(LED_VERMELHO) ? "OK" : "FALHA");
            break;
        case 2:
            printf("AZUL %s\n", 
                gpio_get(LED_AZUL) ? "OK" : "FALHA");
            break;
        case 3:
            printf("Verde %s\n", 
                gpio_get(LED_VERDE) ? "OK" : "FALHA");
            break;
    }
    return true;
}

int main() {
    stdio_init_all();
    inicializar_gpio();
    teste_inicial_hardware();

    struct repeating_timer timer;
    add_repeating_timer_ms(TEMPO_MUDANCA_SINAL, alterar_estado_semaforo, NULL, &timer);

    printf("🚦 Sistema Iniciado\n");
    while (true) {
        sleep_ms(1000);
    }
    return 0;
}