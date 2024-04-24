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

#define HOST_IP_ADDR "192.168.0.148"
#define PORT 50

#include "esp_random.h"
#include "tcp_client_v4.h"

static const char *TAG = "TCP_CLIENT";
static int sock;
static char rx_buffer[128];
static char host_ip[] = HOST_IP_ADDR;
static int send_error_count = 0;

void tcp_connect_ver4(void)
{
    int addr_family = 0;
    int ip_protocol = 0;

    const int retries = 5;
    for (int turn = 0; turn < retries; turn++) {

        struct sockaddr_in dest_addr;
        inet_pton(AF_INET, host_ip, &dest_addr.sin_addr);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;

        sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "Socket created, connecting to %s:%d", host_ip, PORT);

        int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(TAG, "Connected");
    }
}

bool is_connected(void){
    return sock != -1;
}

void close_sock_and_restart(int sock)
{
    ESP_LOGE(TAG, "Shutting down socket and restarting...");
    shutdown(sock, 0);
    close(sock);
    esp_restart();
}

void send_and_receive(void) {
    if (!is_connected())
    {
        ESP_LOGE(TAG, "Not connected, therefore not sending/receiving!");
        return;
    }
    int err, len;
    uint32_t payload = esp_random();

    ESP_LOGI(TAG, "Sending..");
    err = send(sock, &payload, sizeof(payload), 0);
    if (err < 0) {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        (send_error_count)++; 
        if (send_error_count >= 10) { 
            close_sock_and_restart(sock);
        }
        return;
    } else {
        send_error_count = 0; 
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
