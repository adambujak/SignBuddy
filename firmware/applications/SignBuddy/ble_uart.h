#ifndef BLE_UART_H
#define BLE_UART_H

#include <stdint.h>

/** Receive transmission from the bluetooth uart module
 * @retval    Byte of data received
 */
uint8_t ble_uart_rx(void);

/** Send transmission to the bluetooth uart module
 * @param data    Byte of data to transmit
 */
void ble_uart_tx(uint8_t data);

/* Initialize ble_uart module */
void ble_uart_init(void);

#endif // BLE_UART_H
