#include "stm32f4xx_hal.h"
#include "lcd.h"
#include "input.h"
#include "berechnung.h"
#include "output.h"
#include "error_drehgeber.h"
#include <stdio.h>

// Function prototypes
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void) {
    // Initialize HAL and board
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    // Initialize LCD
    lcdSetFont(16);
    lcdGotoXY(0, 0);
    lcdPrintS("Drehgeber Aufgabe 2");
    lcdGotoXY(0, 20);
    lcdPrintS("--------------------");

    int input = 0, output = 0, phasen = 0, reset = 0, zeit = 0;
    double winkel = 0, geschw = 0;

    while (1) {
        zeit = inputzeit();
        input = input_einlesen();
        reset = resetpressed();

        output = phasen_ueberpruefung(input, reset);
        phasen = getphasen();
        winkel = get_winkel();
        geschw = get_winkelgeschw();

        // Update LCD
        lcdGotoXY(0, 80);
        char text[32];
        sprintf(text, "Winkel: %.2f Grad", winkel);
        lcdPrintS(text);

        lcdGotoXY(0, 100);
        sprintf(text, "Geschw: %.2f Grad/s", geschw);
        lcdPrintS(text);

        if (output == PHASEUEBERSPRUNGEN) {
            error_number(output);
        }

        HAL_Delay(200);
    }
}

// ===== Helper functions =====
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

static void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // LEDs (D21, D22, D23)
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    // Input S0/S1
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}
