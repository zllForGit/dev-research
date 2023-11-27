/* This a file about SPIFFS filesystem */

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

#include "spiffs.h"

char *TAG_SPIFFS = "spiffs";
esp_vfs_spiffs_conf_t conf_global;

/* Initializing, mount and register SPIFFS. */
void spiffs_init_mount_register()
{
    ESP_LOGI(TAG_SPIFFS, "Initializing SPIFFS");
    // Initialize configuration
   esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };
   conf_global = conf; 

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG_SPIFFS, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG_SPIFFS, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG_SPIFFS, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }
    
    // Get SPIFFS partion information
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_SPIFFS, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG_SPIFFS, "Partition size: total: %d, used: %d", total, used);
    }
}

// Unmount partition and disable SPIFFS
void unmount_spiffs()
{
    esp_vfs_spiffs_unregister(conf_global.partition_label);
    ESP_LOGI(TAG_SPIFFS, "SPIFFS unmounted");
}

// Write data to file
void write_file(char *content)
{
    ESP_LOGI(TAG_SPIFFS, "Opening file");
    FILE* f = fopen(PATH_NAME, "w");
    if (f == NULL) {
        ESP_LOGE(TAG_SPIFFS, "Failed to open file for writing");
        return;
    }
    fprintf(f, content);
    fclose(f);
    ESP_LOGI(TAG_SPIFFS, "File written");
}

// Read data from file
void read_file()
{
    // Open renamed file for reading
    ESP_LOGI(TAG_SPIFFS, "Reading file");
    FILE* f = fopen(PATH_NAME, "r");
    if (f == NULL) {
        ESP_LOGE(TAG_SPIFFS, "Failed to open file for reading");
        return;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    // strip newline
    char* pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG_SPIFFS, "Read from file: %s", line);
}
