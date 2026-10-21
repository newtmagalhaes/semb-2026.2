
#ifndef SERIAL_CONN_H
#define SERIAL_CONN_H

#include <stdint.h>
#include <stddef.h>
#include "stm32f0xx_hal.h"

#define MAX_PACKET_SIZE 4000u
#define CONN_HEADER_SIZE 2
#define CONN_MIN_TIMEOUT 100
#define CONN_MAX_TIMEOUT 1000

typedef enum {
	UNKNOWN_ERROR = 0x91,
} ExternalError;

typedef enum {
	SEND_OK = 0x80,
	SEND_FAIL_INIT,
	SEND_FAIL_DATA,
	SEND_FAIL_END,
} SendStatus;

typedef enum {
	RECEIVE_OK = 0x70,
	RECEIVE_FAIL_INIT,
	RECEIVE_FAIL_DATA,
	RECEIVE_FAIL_END,
} ReceiveStatus;

typedef enum {
	CMD_SORT = 0x01,
	CMD_MSG,
} SerialCommand;

/**
* Recebe vetor a ser ordenado
*
* @param uart: serial a ser ouvida;
* @param size: ponteiro para salvar tamanho do vetor
* @param data: vator para armazenar input. Espera-se que tenha tamanho o bastante para o limite
*
* @retval RECEIVE_OK: caso tenha lido input com sucesso.
*/
ReceiveStatus receive_packet(UART_HandleTypeDef *uart, size_t *size, uint8_t data[]);

SendStatus send_packet(UART_HandleTypeDef *uart, size_t size, uint8_t data[]);

SendStatus send_msg(UART_HandleTypeDef *uart, uint8_t code);

#endif
