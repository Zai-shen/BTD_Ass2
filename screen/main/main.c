#include "esp_log.h"
#include "mpu6866controller.h"
#include "display.h"
#include "time.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "esp_event.h"
#include "tcp_client_v4.h"

#define WAIT vTaskDelay(300 / portTICK_PERIOD_MS)
#define WAITLONG vTaskDelay(5000 / portTICK_PERIOD_MS)

#define TOTALDURATION 3000  // total time
#define DELAYDURATION 100   // delay per loop iter
#define SAMPLES (TOTALDURATION / DELAYDURATION)  // calc number of samples

static const char *TAG = "MAIN";

// Get the current time in seconds since the Unix epoch
char* get_current_time() {
    time_t now;
    time(&now);

    struct tm timeinfo;
    localtime_r(&now, &timeinfo);

	char* time_string = (char*)malloc(10); // HH:MM:SS + null terminator
    strftime(time_string, 9, "%H:%M:%S", &timeinfo);

    return time_string;
}

void app_start(void *pvParameters)
{
	// I2C & DISPLAY INIT
	ESP_LOGI(TAG, "i2c_master_init_MPU()");
    ESP_ERROR_CHECK(i2c_master_init_MPU());
	ESP_LOGI(TAG, "init_display()");
	init_display();

	ESP_LOGI(TAG, "display_textarea()");
	char *messages[] = {
        "Display",
        "init",
        "success"
    };
	display_textarea(messages, 3);
	
	WAIT;

	// TCP INIT
	ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());

	ESP_LOGI(TAG, "NO TCP errors");
	char *messages2[] = {
        "NO",
        "TCP",
        "errors"
    };
	display_textarea(messages2, 3);

	ESP_LOGI(TAG, "Trying to connect to our TCP: %s", CONFIG_EXAMPLE_WIFI_SSID);
	char *messages3[] = {
        "Connecting",
        "to",
        CONFIG_EXAMPLE_WIFI_SSID
    };
	display_textarea(messages3, 3);
	tcp_connect_ver4();

	display_textline("Send + Recv");
	send_and_receive();

	WAIT;

	// ACCEL INIT
	ESP_LOGI(TAG, "init_mpu6886()");
    display_textline("Init MPU");
	init_mpu6886();

	WAIT;

    float ax, ay, az;
	int num_axis = 3;
    float accelData[SAMPLES][num_axis];  

	while (1)
	{
		ESP_LOGI(TAG, "Started recording chunk.. %s", get_current_time());
		display_textline("Recording chunk...");

		for (int i = 0; i < SAMPLES; i++)
		{
			getAccelData(&ax, &ay, &az);
			accelData[i][0] = ax;
			accelData[i][1] = ay;
			accelData[i][2] = az;
			ESP_LOGI(TAG, "Accel ADC: ax=%f, ay=%f, az=%f", ax, ay, az);
			float axis[] = {ax,ay,az};
			display_floatarea(axis, 3);

			vTaskDelay(DELAYDURATION / portTICK_PERIOD_MS);
		}
		ESP_LOGI(TAG, "Stopped recording chunk... %s", get_current_time());
		WAITLONG;
	}
	


	while (1)
	{
		ESP_LOGI(TAG, "idling...");
		WAIT;
	}
}

void app_main(void)
{
	xTaskCreate(app_start, "app_start", 32768, NULL, 10, NULL);
    vTaskDelete(NULL);
}
