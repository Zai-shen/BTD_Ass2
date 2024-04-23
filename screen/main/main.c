#include "esp_log.h"
#include "mpu6866controller.h"
#include "display.h"


static const char *TAG = "MAIN";

void app_start(void *pvParameters)
{
	ESP_LOGI(TAG, "i2c_master_init_MPU()");
    ESP_ERROR_CHECK(i2c_master_init_MPU());
	ESP_LOGI(TAG, "init_display()");
	init_display();
	ESP_LOGI(TAG, "Starting Display test_display()");
	char *messages[] = {
        "This",
        "is",
        "SPARTA"
    };
	display_textarea(messages, 3);

	while (1)
	{
		ESP_LOGI(TAG, "idling...");
		vTaskDelay(400);
	}
	
    // init_mpu6886();
}

void app_main(void)
{
	// init accelerometer
	// ESP_ERROR_CHECK(i2c_master_init_MPU());
	// ESP_LOGI(TAG, "I2C initialized successfully");

	// init_mpu6886();

	// int16_t ax, ay, az;
	// getAccelAdc(&ax, &ay, &az);

	xTaskCreate(app_start, "app_start", 32768, NULL, 10, NULL);
    vTaskDelete(NULL);
}
