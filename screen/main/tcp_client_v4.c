/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#include "sdkconfig.h"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>            // struct addrinfo
#include <arpa/inet.h>
#include "esp_netif.h"
#include "esp_log.h"
#if defined(CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN)
#include "addr_from_stdin.h"
#endif

#define HOST_IP_ADDR CONFIG_EXAMPLE_WIFI_SSID
#define PORT 50

#include "esp_random.h"
#include "tcp_client_v4.h"

static const char *TAG = "TCP_CLIENT";

void tcp_client(void)
{
    char rx_buffer[128];
    char host_ip[] = HOST_IP_ADDR;
    int addr_family = 0;
    int ip_protocol = 0;
    int send_error_count = 0;

    while (1) {

        struct sockaddr_in dest_addr;
        inet_pton(AF_INET, host_ip, &dest_addr.sin_addr);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;

        int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created, connecting to %s:%d", host_ip, PORT);
        ESP_LOGI(TAG, "Waiting to connect..");

        int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Connected");


        while (1) {
            send_and_receive(sock, host_ip, rx_buffer, &send_error_count);

            vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
        }

        if (sock != -1) {
            close_sock_and_restart(sock);
        }
    }
}

void close_sock_and_restart(int sock)
{
    ESP_LOGE(TAG, "Shutting down socket and restarting...");
    shutdown(sock, 0);
    close(sock);
    esp_restart();
}

void send_and_receive(int sock, char *host_ip, char *rx_buffer, int *error_count) {
    int err, len;

    uint32_t payload = esp_random();

    ESP_LOGI(TAG, "Sending..");
    err = send(sock, &payload, sizeof(payload), 0);
    if (err < 0) {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        (*error_count)++; 
        if (*error_count >= 10) { 
            close_sock_and_restart(sock);
        }
        return;
    } else {
        *error_count = 0; 
    }

    len = recv(sock, rx_buffer, 16, 0); // 15 for max 15 bytes of "Message received" + null terminator
    if (len < 0) {
        ESP_LOGE(TAG, "recv failed: errno %d", errno);
        return;
    } else {
        rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
        ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
        ESP_LOGI(TAG, "%s", rx_buffer);
    }
}
