#include <stdio.h>
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_log.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "time.h"

#define BUF_SIZE (1024)

#define TXD_PIN (GPIO_NUM_21) // Tx
#define RXD_PIN (GPIO_NUM_20) // Rx

bool success;

void uart_init(void) 

{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,          // UART driver setting 
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

}

bool parse_data(uint8_t* data, size_t len, int timeout_ms , const char* word) // function for pasing data 
{
    char resp[BUF_SIZE] = {0};   // Allocate a buffer to hold the received data

    TickType_t start_time = xTaskGetTickCount(); // get the start time of the loop
    
    printf("\nparsing data start\n");

    while ((xTaskGetTickCount() - start_time) < (timeout_ms / portTICK_PERIOD_MS))
    {
        int bytes_read = uart_read_bytes(UART_NUM_0, data, len, pdMS_TO_TICKS(1000));

        if (bytes_read > 0)
        {
            // Append the received data to the buffer
            strncat(resp, (const char*)data, bytes_read);

            // Check if the word is present in the buffer
            if (strstr(resp, word) != NULL)
            {
                return true;
            }
        }
    }
    
    return false;
}



bool sendATCommand(char* command, char* expectedResponse, int timeoutMs) // Sending AT Command
{
    uint8_t buffer[BUF_SIZE];

    memset(buffer , 0 ,BUF_SIZE);

    uart_write_bytes(UART_NUM_0, command, strlen(command));

    //printf("\nWrite done\n");

    bool responseReceived = parse_data(buffer, BUF_SIZE , timeoutMs, expectedResponse);

    if (responseReceived) 
    {
        printf("\nCommand sent successfully!\n");

        printf("Response: %s\n", buffer); // printing response

        return true;

    } else 
    {
        printf("Failed to receive expected response!\n");
        return false;
    }
}

void module_init(void)
{
    success = sendATCommand("AT\r\n", "OK", 5000);

    if (success)
    {
        printf("Module Initialization done successfully\n");
    } 

    else
    {
        printf("Module Initialization fail !\n");
    }
}

void check_SIM_reg(void)
{
    success = sendATCommand("AT+CREG?", "+CREG", 5000);

    if (success)
    {
        printf("Checking module Registration\n");
    } 

    else
    {
        printf("Module Registration fail !\n");
    }
}

void changing_mode(void)
{
    success = sendATCommand("AT+CREG=2", "OK", 5000);

    if (success)
    {
        printf("Changing mode of opration\n");
    } 

    else
    {
        printf("fail to change mode of opration!\n");
    }
}

void Enable_GPS(void)
{
    success = sendATCommand("AT+CGPS=1", "OK", 5000);

    if (success)
    {
        printf("Module GPS Enabled \n");
    } 

    else
    {
        printf("Module GPS fail to Enabled!\n");
    }    
}

void location_INFO(void)
{
    vTaskDelay(120000 / portTICK_PERIOD_MS);

    success = sendATCommand("AT+CGPSINFO", "OK", 5000);

    if (success)
    {
        printf("Locaton info command sent\n");
    } 

    else
    {
        printf("fail to send location info cmmmand!\n");
    }
    
}


void app_main(void)

{

    uart_init();

    vTaskDelay(18000 / portTICK_PERIOD_MS);

    //Initiate module 
    module_init();
    // Check SIM register with network 
    check_SIM_reg();
    // Changing mode of operation 
    changing_mode();
    // Enble Module GPS 
    Enable_GPS();
    // Geting location 
    location_INFO();
}
