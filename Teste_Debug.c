#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Definições do Hardware
#define PIN_POTENTIOMETER 26
#define ADC_CHANNEL 0
#define CONVERSION_FACTOR 3.3f / (1 << 12) // 12-bit ADC (0-4095) -> 3.3V

// Função auxiliar para categorizar a tensão
const char* get_battery_status(float voltage) {
    if (voltage < 1.0f) {
        return "CRITICO";
    } else if (voltage < 2.0f) {
        return "BAIXO";
    } else {
        return "NORMAL";
    }
}

int main() {
    // Inicialização da comunicação Serial (USB/UART)
    stdio_init_all();

    // Inicializa o módulo ADC
    adc_init();
    // Prepara o GPIO 26 para função analógica
    adc_gpio_init(PIN_POTENTIOMETER);
    // Seleciona o canal 0 (que corresponde ao GPIO 26)
    adc_select_input(ADC_CHANNEL);

    printf("Iniciando Data Logger de Bateria (Modo Silencioso)...\n");

    uint32_t reading_count = 0;

    while (true) {
        // 1. Leitura do valor cru (0 - 4095)
        uint16_t raw_value = adc_read();

        // 2. Conversão para Tensão
        float voltage = raw_value * CONVERSION_FACTOR;

        // 3. Lógica de decisão
        const char* status = get_battery_status(voltage);

        // 4. Data Logging
        printf("[%d] Raw: %d | Tensão: %.2f V | Status: %s\n", 
               reading_count, raw_value, voltage, status);

        // Incrementa contador
        reading_count++;

        // Delay de 1 segundo entre leituras
        sleep_ms(1000); 
    }
}