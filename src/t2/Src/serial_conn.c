#include "serial_conn.h"

// Private variables
static uint8_t _header[CONN_HEADER_SIZE];

uint8_t _calc_checksum(SerialCommand cmd, size_t size, uint8_t data[]) {
	uint8_t checksum = cmd ^ size;
	for (uint8_t i = 0; i < size; i++) {
		checksum ^= data[i];
	}
	return checksum;
}

ReceiveStatus receive_packet(UART_HandleTypeDef *uart, size_t *size, uint8_t data[]) {
	uint8_t received_checksum = 0;

	if (HAL_UART_Receive(uart, _header, CONN_HEADER_SIZE, CONN_MIN_TIMEOUT) != HAL_OK)
		return RECEIVE_FAIL_INIT;

	SerialCommand cmd = (SerialCommand) _header[0];
	*size = (size_t) _header[1];
	if (*size > MAX_PACKET_SIZE || cmd != CMD_SORT)
		return RECEIVE_FAIL_INIT;

	if (HAL_UART_Receive(uart, data, *size, CONN_MAX_TIMEOUT) != HAL_OK)
		return RECEIVE_FAIL_DATA;

	if (HAL_UART_Receive(uart, &received_checksum, 1, CONN_MAX_TIMEOUT) != HAL_OK
			|| received_checksum != _calc_checksum(CMD_SORT, *size, data))
		return RECEIVE_FAIL_END;

	return RECEIVE_OK;
}

SendStatus send_packet(UART_HandleTypeDef *uart, size_t size, uint8_t data[]) {
	uint8_t checksum = _calc_checksum(CMD_SORT, size, data);
	_header[0] = (uint8_t) CMD_SORT;
	_header[1] = (uint8_t) size;
	if (HAL_UART_Transmit(uart, _header, CONN_HEADER_SIZE, HAL_MAX_DELAY) != HAL_OK)
		return SEND_FAIL_INIT;

	if (HAL_UART_Transmit(uart, data, size, HAL_MAX_DELAY) != HAL_OK)
		return SEND_FAIL_DATA;

	if (HAL_UART_Transmit(uart, &checksum, 1, HAL_MAX_DELAY) != HAL_OK)
		return SEND_FAIL_END;

	return SEND_OK;
}

SendStatus send_msg(UART_HandleTypeDef *uart, uint8_t code) {
	_header[0] = (uint8_t) CMD_MSG;
	_header[1] = code;
	if (HAL_UART_Transmit(uart, _header, CONN_HEADER_SIZE, HAL_MAX_DELAY) != HAL_OK)
		return SEND_FAIL_INIT;

	return SEND_OK;
}
