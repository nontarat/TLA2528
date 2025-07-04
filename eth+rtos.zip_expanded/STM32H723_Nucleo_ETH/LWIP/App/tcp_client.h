#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H 1
extern UART_HandleTypeDef huart3;  // กำหนด UART_HandleTypeDef สำหรับ UART3

void connect_to_server(void);
void read_adc_and_send_data(void) ;
void user_input(void);

#endif
