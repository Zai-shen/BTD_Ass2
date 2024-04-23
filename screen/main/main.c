#include "esp_log.h"
#include "display.h"

static const char *TAG = "MAIN";

void app_start(void *pvParameters)
{
    // ESP_ERROR_CHECK(i2c_master_init());

	init_display();

    // write_screen("initializing", "connecting to Wifi");

    // ESP_ERROR_CHECK(nvs_flash_init());
    // ESP_ERROR_CHECK(esp_netif_init());
    // ESP_ERROR_CHECK(esp_event_loop_create_default());
    // ESP_ERROR_CHECK(example_connect());

    // ESP_LOGI(TAG, "I2C initialized successfully");

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
