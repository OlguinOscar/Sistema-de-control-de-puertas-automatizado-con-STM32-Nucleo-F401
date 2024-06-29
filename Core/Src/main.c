/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "stm32f4xx_hal.h"
#include <stdbool.h>

// Motor1
const int MotorPin1 = GPIO_PIN_2;
const int MotorPin2 = GPIO_PIN_3;
int enable = GPIO_PIN_5; // pwm

// Motor2
const int MotorPin3 = GPIO_PIN_4;
const int MotorPin4 = GPIO_PIN_7;
int enable2 = GPIO_PIN_6; // pwm

bool puerta_abierta = false;
bool puerta = false;
bool derecha_funcionado = false;
bool izquierda_funcionado = false;

int contacto_abajo_derecha; // contacto arriba puerta
int contacto_arriba_derecha; // contacto arriba puerta
int contacto_abajo_izquierda; // contacto arriba puerta
int contacto_arriba_izquierda; // contacto arriba puerta

int izquierda_arriba; // botones
int izquierda_abajo; // botones
int derecha_arriba; // botones
int derecha_abajo; // botones
int alterno_arriba; // botones
int alterno_abajo; // botones

void hacia_arriba(int enable, int motorpin1, int motorpin2){
    HAL_GPIO_WritePin(GPIOA, motorpin1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, motorpin2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, enable, GPIO_PIN_SET);
}

void hacia_abajo(int enable, int motorpin1, int motorpin2){
    HAL_GPIO_WritePin(GPIOA, motorpin1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, motorpin2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, enable, GPIO_PIN_SET);
}

void motor_apagado(int enable, int motorpin1, int motorpin2){
    HAL_GPIO_WritePin(GPIOA, motorpin1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, motorpin2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, enable, GPIO_PIN_RESET);
}

void movimiento(int arriba, int abajo, int contacto_arriba, int contacto_abajo, int enable, int motorpin1, int motorpin2){
    if (arriba && abajo){ // no aprieto nada
        motor_apagado(enable, motorpin1, motorpin2);
    }
    if (!arriba && !abajo){ // los dos al mismo tiempo
        motor_apagado(enable, motorpin1, motorpin2);
    }
    if (!arriba && abajo){
        if (!contacto_abajo){
            hacia_arriba(enable, motorpin1, motorpin2);
        }
        else{
            motor_apagado(enable, motorpin1, motorpin2);
        }
    }
    if (!abajo && arriba){
        if (!contacto_arriba){
            hacia_abajo(enable, motorpin1, motorpin2);
        }
        else{
            motor_apagado(enable, motorpin1, motorpin2);
        }
    }
}

void proceso(int arriba, int abajo){
    contacto_abajo_derecha = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
    contacto_arriba_derecha = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);
    contacto_abajo_izquierda = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
    contacto_arriba_izquierda = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
    izquierda_arriba = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
    izquierda_abajo = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
    derecha_arriba = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
    derecha_abajo = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
    alterno_arriba = arriba;
    alterno_abajo = abajo;

    movimiento(derecha_arriba, derecha_abajo, contacto_arriba_derecha, contacto_abajo_derecha, enable, MotorPin1, MotorPin2);
    movimiento(arriba, abajo, contacto_arriba_izquierda, contacto_abajo_izquierda, enable2, MotorPin3, MotorPin4);
}

void setup_pins(){
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Motor pins
    GPIO_InitStruct.Pin = MotorPin1 | MotorPin2 | MotorPin3 | MotorPin4 | enable | enable2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Input pins
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Contact pins
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

int main(void){
    HAL_Init();
    setup_pins();

    while(1){
        contacto_abajo_derecha = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
        contacto_arriba_derecha = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);
        contacto_abajo_izquierda = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
        contacto_arriba_izquierda = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);

        izquierda_arriba = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
        izquierda_abajo = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
        derecha_arriba = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
        derecha_abajo = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
        alterno_arriba = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
        alterno_abajo = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_13);

        int senial = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
        int senial_puerta = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

        movimiento(derecha_arriba, derecha_abajo, contacto_arriba_derecha, contacto_abajo_derecha, enable, MotorPin1, MotorPin2);

        if (!puerta_abierta){
            if (!senial_puerta && contacto_abajo_derecha && senial && contacto_abajo_izquierda){
                puerta_abierta = true;
                hacia_abajo(enable, MotorPin1, MotorPin2);
                HAL_Delay(250);
                motor_apagado(enable, MotorPin1, MotorPin2);
            }
        }
        else {
            if (senial_puerta){
                HAL_Delay(400);
                while (!contacto_abajo_derecha){
                    hacia_arriba(enable, MotorPin1, MotorPin2);
                    contacto_abajo_derecha = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
                }
                motor_apagado(enable, MotorPin1, MotorPin2);
                puerta_abierta = false;
            }
        }

        if (!puerta){
            if (senial_puerta && contacto_abajo_derecha && !senial && contacto_abajo_izquierda){
                puerta = true;
                hacia_abajo(enable2, MotorPin3, MotorPin4);
                HAL_Delay(300);
                motor_apagado(enable2, MotorPin3, MotorPin4);
            }
        }
        else {
            if (senial){
                HAL_Delay(400);
                while (!contacto_abajo_izquierda){
                    hacia_arriba(enable2, MotorPin3, MotorPin4);
                    contacto_abajo_izquierda = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
                }
                motor_apagado(enable2, MotorPin3, MotorPin4);
                puerta = false;
            }
        }

        while (!izquierda_arriba && alterno_arriba){
            izquierda_arriba = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
            izquierda_abajo = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
            proceso(izquierda_arriba, izquierda_abajo);
        }
        while (!izquierda_abajo && alterno_abajo){
            izquierda_arriba = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
            izquierda_abajo = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
            proceso(izquierda_arriba, izquierda_abajo);
        }

        while (izquierda_arriba && !alterno_arriba){
            alterno_arriba = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
            alterno_abajo = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_13);
            proceso(alterno_arriba, alterno_abajo);
        }
        while (izquierda_abajo && !alterno_abajo){
            alterno_arriba = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
            alterno_abajo = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_13);
            proceso(alterno_arriba, alterno_abajo);
        }
    }
}
