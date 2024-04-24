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

#define TOTALDURATION 3000  // total time in ms
#define DELAYDURATION 100   // delay per loop iteration in ms
#define SAMPLES (TOTALDURATION / DELAYDURATION)  // calculating number of samples

static const char *TAG = "MAIN";

char* get_current_time() {
    // Get the current time in seconds since the Unix epoch
    time_t now;
    time(&now);

    // Convert the time to a more readable format
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);

    // Allocate memory for the string
	char* time_string = (char*)malloc(10); // HH:MM:SS + null terminator

    // Format the time into a string
    strftime(time_string, 9, "%H:%M:%S", &timeinfo);

    return time_string;
}

void app_start(void *pvParameters)
{
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

	ESP_LOGI(TAG, "Trying to connect to our TCP");
	tcp_client();

	WAIT;

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
