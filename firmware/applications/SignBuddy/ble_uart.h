#ifndef BLE_UART_H
#define BLE_UART_H

#include <stdint.h>
#include <stdbool.h>

#define BLE_UART_TX_BUFFER_SIZE    256
#define BLE_UART_RX_BUFFER_SIZE    64

/** Receive transmission from the bluetooth uart module
 * @retval    Byte of data received
 */
int ble_uart_read(uint8_t *data, uint32_t length);

/** Send transmission to the bluetooth uart module
 * @param data    Byte of data to transmit
 */
void ble_uart_write(uint8_t *data, uint32_t length);

bool ble_uart_is_writing(void);

/* Initialize ble_uart module */
void ble_uart_init(void);

#endif // BLE_UART_H
