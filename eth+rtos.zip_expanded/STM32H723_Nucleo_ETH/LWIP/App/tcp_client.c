#include "lwip.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include <string.h>
#include <errno.h> // สำหรับ errno
extern  ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart3;  // กำหนด UART_HandleTypeDef สำหรับ UART3
char json_data[] = "{\"LNUM\":\"UT000001\",\"TC\":100,\"OH\":\"25.22\",\"DCC\":5,\"ALWT\":35.11,\"CDCC\":25,\"LILM\":65,\"TEMC\":25  }";

void parse_led_status(char *buffer);

// เพิ่มฟังก์ชันสำหรับตั้งค่า timeout
void set_socket_timeout(int sock) {
    struct timeval timeout;
    timeout.tv_sec = 5;  // 5 วินาที
    timeout.tv_usec = 0;

    // ตั้งค่า timeout สำหรับการเชื่อมต่อ
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
}

void send_post_request(int sock, const char *json_data) {

    char *http_request_template =
        "POST /non/api/getoverview.php HTTP/1.1\r\n"
        "Host: tanakulgroup.com\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n"
        "%s"; // JSON data จะถูกแทรกที่นี่

    // รับข้อมูลจาก server
    int json_length = strlen(json_data);
    char http_request[512]; // สร้าง buffer สำหรับ HTTP request
    snprintf(http_request, sizeof(http_request), http_request_template, json_length, json_data);

    // ส่งข้อมูลไปยังเซิร์ฟเวอร์
    if (send(sock, http_request, strlen(http_request), 0) < 0) {
        int err_code = errno; // รับรหัสข้อผิดพลาด
        char error_message[50];
        snprintf(error_message, sizeof(error_message), "Error %d sending data\r\n", err_code);
        HAL_UART_Transmit(&huart3, (uint8_t *)error_message, strlen(error_message), HAL_MAX_DELAY);
        closesocket(sock);
    }
}

void send_get_request(int sock) {
    char *http_request_template =
        "GET /lift/user_input.php HTTP/1.1\r\n"
        "Host: tanakulgroup.com\r\n"
        "Connection: close\r\n\r\n";

    // สร้าง buffer สำหรับ HTTP request
    char http_request[256]; // สร้าง buffer สำหรับ HTTP request
    snprintf(http_request, sizeof(http_request), "%s", http_request_template);

    // ส่งข้อมูลไปยังเซิร์ฟเวอร์
    if (send(sock, http_request, strlen(http_request), 0) < 0) {
        int err_code = errno; // รับรหัสข้อผิดพลาด
        char error_message[50];
        snprintf(error_message, sizeof(error_message), "Error %d sending data\r\n", err_code);
        HAL_UART_Transmit(&huart3, (uint8_t *)error_message, strlen(error_message), HAL_MAX_DELAY);
        closesocket(sock);
    }
}

void send_post_request_adc(int sock, const char *json_data) {

    char *http_request_template =
        "POST /lift/getADC.php HTTP/1.1\r\n"
        "Host: tanakulgroup.com\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n"
        "%s"; // JSON data จะถูกแทรกที่นี่

    // รับข้อมูลจาก server
    int json_length = strlen(json_data);
    char http_request[512]; // สร้าง buffer สำหรับ HTTP request
    snprintf(http_request, sizeof(http_request), http_request_template, json_length, json_data);

    // ส่งข้อมูลไปยังเซิร์ฟเวอร์
    if (send(sock, http_request, strlen(http_request), 0) < 0) {
        int err_code = errno; // รับรหัสข้อผิดพลาด
        char error_message[50];
        snprintf(error_message, sizeof(error_message), "Error %d sending data\r\n", err_code);
        HAL_UART_Transmit(&huart3, (uint8_t *)error_message, strlen(error_message), HAL_MAX_DELAY);
        closesocket(sock);
    }
}
void connect_to_server(void) {
    struct sockaddr_in server_addr;
    int sock;
    char *server_ip = "13.67.106.185";  // IP ของ server
    // char *server_ip = "104.26.10.187";  // IP ของ JSONPlaceholder
    // char *server_ip = "jsonplaceholder.typicode.com";  // ใช้ชื่อเซิร์ฟเวอร์แทน IP
    int server_port = 80;               // HTTP port

    // สร้าง socket ใหม่
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        char *error_message = "Error creating socket\r\n";
        HAL_UART_Transmit(&huart3, (uint8_t *)error_message, strlen(error_message), HAL_MAX_DELAY);
        return;
    }

    // ตั้งค่า server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    set_socket_timeout(sock);    // ตั้งค่า timeout

    // เชื่อมต่อไปยัง server
       while (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
           int err_code = errno; // รับรหัสข้อผิดพลาด
           char error_message[50];
           snprintf(error_message, sizeof(error_message), "Error %d connecting to server\r\n", err_code);
           HAL_UART_Transmit(&huart3, (uint8_t *)error_message, strlen(error_message), HAL_MAX_DELAY);
           closesocket(sock);
           // รอ 2 วินาทีก่อนลองเชื่อมต่อใหม่
           //HAL_Delay(2000);
           // สร้าง socket ใหม่
           sock = socket(AF_INET, SOCK_STREAM, 0);
           if (sock < 0) {
               HAL_UART_Transmit(&huart3, (uint8_t *)"Error creating socket\r\n", 25, HAL_MAX_DELAY);
               return;
           }
           // ตั้งค่า timeout
           set_socket_timeout(sock);
       }
    // ส่ง POST request
    send_post_request(sock, json_data);
    char buffer[512];
    int len;
    while ((len = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[len] = '\0';  // Null terminate string
        // ส่งข้อมูลที่ได้รับผ่าน UART3
        HAL_UART_Transmit(&huart3, (uint8_t *)buffer, len, HAL_MAX_DELAY);
       // parse_led_status(buffer);
    }
    if (len < 0) {
        int err_code = errno; // รับรหัสข้อผิดพลาด
        char error_message[50];
        snprintf(error_message, sizeof(error_message), "Error %d receiving data\r\n", err_code);
        HAL_UART_Transmit(&huart3, (uint8_t *)error_message, strlen(error_message), HAL_MAX_DELAY);
    }
    // ปิด socket
    closesocket(sock);
}

void parse_led_status(char *buffer) {
	// ตรวจสอบ SW1
	    char *sw1_data = strstr(buffer, "\"LNUM\":\"SW1\"");
	    if (sw1_data != NULL) {
	        char *led_status = strstr(sw1_data, "\"led_status\":\"");
	        if (led_status != NULL) {
	            led_status += strlen("\"led_status\":\"");
	            char status = led_status[0];
	            HAL_UART_Transmit(&huart3, (uint8_t *)&status, 1, HAL_MAX_DELAY);
	            HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);

	            if (status == '1') {
	                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);  // สมมุติว่าใช้ LED สีน้ำเงิน
	            } else if (status == '0') {
	                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
	            }
	        }
	    } else {
	        HAL_UART_Transmit(&huart3, (uint8_t *)"SW1 data not found\r\n", 21, HAL_MAX_DELAY);
	    }
	// ตรวจสอบ SW2
	    char *sw2_data = strstr(buffer, "\"LNUM\":\"SW2\"");
	    if (sw2_data != NULL) {
	        char *led_status = strstr(sw2_data, "\"led_status\":\"");
	        if (led_status != NULL) {
	            led_status += strlen("\"led_status\":\"");
	            char status = led_status[0];
	            HAL_UART_Transmit(&huart3, (uint8_t *)&status, 1, HAL_MAX_DELAY);
	            HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);

	            if (status == '1') {
	                HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);  // สมมุติว่าใช้ LED สีแดง
	            } else if (status == '0') {
	                HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
	            }
	        }
	    } else {
	        HAL_UART_Transmit(&huart3, (uint8_t *)"SW2 data not found\r\n", 21, HAL_MAX_DELAY);
	    }
	//ตรวจสอบ SW3
    char *sw3_data = strstr(buffer, "\"LNUM\":\"SW3\""); // ค้นหาข้อมูลของ SW3
    if (sw3_data != NULL) {
        // หากเจอข้อมูล SW3 ให้ค้นหาข้อมูล led_status ที่อยู่หลังจาก SW3
        char *led_status = strstr(sw3_data, "\"led_status\":\"");
        if (led_status != NULL) {
            led_status += strlen("\"led_status\":\"");  // ข้ามข้อความไปยังค่า led_status
            char status = led_status[0];  // ดึงค่า led_status (0 หรือ 1)

            // แสดงค่า led_status ผ่าน UART (คุณสามารถลบหรือปรับแก้ให้ควบคุม LED จริงได้)
            HAL_UART_Transmit(&huart3, (uint8_t *)&status, 1, HAL_MAX_DELAY);
            HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);

            // ควบคุม LED จริง ๆ บน STM32
            if (status == '1') {
                // เปิด LED
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET); // ตัวอย่างการเปิด LED
            } else if (status == '0') {
                // ปิด LED
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET); // ตัวอย่างการปิด LED
            }
        }
    } else {
        HAL_UART_Transmit(&huart3, (uint8_t *)"SW3 data not found\r\n", 21, HAL_MAX_DELAY);
    }
}

void read_adc_and_send_data(void) {
	 // สมมุติว่า ADC_HandleTypeDef ถูกกำหนดไว้เรียบร้อยแล้ว
	    HAL_ADC_Start(&hadc1);  // เริ่มต้น ADC
	    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
	        uint32_t adc_value = HAL_ADC_GetValue(&hadc1);  // อ่านค่า ADC

	        // สร้าง JSON ใหม่เพื่อส่งข้อมูล
	        char json_adc_data[100];
	        snprintf(json_adc_data, sizeof(json_adc_data), "{\"adc_value\":%lu}", adc_value);

	        // เชื่อมต่อกับเซิร์ฟเวอร์
	        struct sockaddr_in server_addr;
	        int sock;
	        char *server_ip = "13.67.106.185";  // IP ของ server
	        int server_port = 80;               // HTTP port

	        // สร้าง socket ใหม่
	        sock = socket(AF_INET, SOCK_STREAM, 0);
	        if (sock < 0) {
	            char *error_message = "Error creating socket\r\n";
	            HAL_UART_Transmit(&huart3, (uint8_t *)error_message, strlen(error_message), HAL_MAX_DELAY);
	            return;
	        }

	        // ตั้งค่า server address
	        server_addr.sin_family = AF_INET;
	        server_addr.sin_port = htons(server_port);
	        server_addr.sin_addr.s_addr = inet_addr(server_ip);
	        set_socket_timeout(sock);    // ตั้งค่า timeout

	        // เชื่อมต่อไปยัง server
	        // เชื่อมต่อไปยัง server
	           while (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
	               int err_code = errno; // รับรหัสข้อผิดพลาด
	               char error_message[50];
	               snprintf(error_message, sizeof(error_message), "Error %d connecting to server\r\n", err_code);
	               HAL_UART_Transmit(&huart3, (uint8_t *)error_message, strlen(error_message), HAL_MAX_DELAY);
	               closesocket(sock);
	               // รอ 2 วินาทีก่อนลองเชื่อมต่อใหม่
	               //HAL_Delay(2000);
	               // สร้าง socket ใหม่
	               sock = socket(AF_INET, SOCK_STREAM, 0);
	               if (sock < 0) {
	                   HAL_UART_Transmit(&huart3, (uint8_t *)"Error creating socket\r\n", 25, HAL_MAX_DELAY);
	                   return;
	               }
	               // ตั้งค่า timeout
	               set_socket_timeout(sock);
	           }

	        // ส่ง POST request
	        send_post_request_adc(sock, json_adc_data);
	        char buffer[512];
	            int len;
	            while ((len = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
	                buffer[len] = '\0';  // Null terminate string
	                // ส่งข้อมูลที่ได้รับผ่าน UART3
	                HAL_UART_Transmit(&huart3, (uint8_t *)buffer, len, HAL_MAX_DELAY);
	                //parse_led_status(buffer);
	            }
	            if (len < 0) {
	                int err_code = errno; // รับรหัสข้อผิดพลาด
	                char error_message[50];
	                snprintf(error_message, sizeof(error_message), "Error %d receiving data\r\n", err_code);
	                HAL_UART_Transmit(&huart3, (uint8_t *)error_message, strlen(error_message), HAL_MAX_DELAY);
	            }
	        // ปิด socket
	        closesocket(sock);
	    }
	    HAL_ADC_Stop(&hadc1);  // หยุด ADC
}

void user_input(void)
{
    struct sockaddr_in server_addr;
    int sock;
    char *server_ip = "13.67.106.185";  // IP ของ server
    // char *server_ip = "104.26.10.187";  // IP ของ JSONPlaceholder
    // char *server_ip = "jsonplaceholder.typicode.com";  // ใช้ชื่อเซิร์ฟเวอร์แทน IP
    int server_port = 80;               // HTTP port

    // สร้าง socket ใหม่
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        char *error_message = "Error creating socket\r\n";
        HAL_UART_Transmit(&huart3, (uint8_t *)error_message, strlen(error_message), HAL_MAX_DELAY);
        return;
    }

    // ตั้งค่า server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    set_socket_timeout(sock);    // ตั้งค่า timeout

    // เชื่อมต่อไปยัง server
       while (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
           int err_code = errno; // รับรหัสข้อผิดพลาด
           char error_message[50];
           snprintf(error_message, sizeof(error_message), "Error %d connecting to server\r\n", err_code);
           HAL_UART_Transmit(&huart3, (uint8_t *)error_message, strlen(error_message), HAL_MAX_DELAY);
           closesocket(sock);
           // รอ 2 วินาทีก่อนลองเชื่อมต่อใหม่
           //HAL_Delay(2000);
           // สร้าง socket ใหม่
           sock = socket(AF_INET, SOCK_STREAM, 0);
           if (sock < 0) {
               HAL_UART_Transmit(&huart3, (uint8_t *)"Error creating socket\r\n", 25, HAL_MAX_DELAY);
               return;
           }
           // ตั้งค่า timeout
           set_socket_timeout(sock);
       }
    // ส่ง POST request
       send_get_request(sock);


    char buffer[512];
    int len;
    while ((len = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[len] = '\0';  // Null terminate string
        // ส่งข้อมูลที่ได้รับผ่าน UART3
        HAL_UART_Transmit(&huart3, (uint8_t *)buffer, len, HAL_MAX_DELAY);
       // parse_led_status(buffer);
    }
    if (len < 0) {
        int err_code = errno; // รับรหัสข้อผิดพลาด
        char error_message[50];
        snprintf(error_message, sizeof(error_message), "Error %d receiving data\r\n", err_code);
        HAL_UART_Transmit(&huart3, (uint8_t *)error_message, strlen(error_message), HAL_MAX_DELAY);
    }
    // ปิด socket
    closesocket(sock);
}
