#include <string.h>
#include "esp_log.h"
#include "wifi_connect.h"
#include "http_test.h"
#include "spiffs.h"

extern char *url_response_data;
extern char *parse_ret;

void app_main(void)
{
    //Initialize NVS
    nvs_init();

    //Initialize wifi as a station
    wifi_init_sta();
    
    // http client config, send http request, ande get response data
    httpClient_config_requestData();

    // parse data
    json_parse(url_response_data, "brand");

    // Verify that 'url_response_data' and 'parse_ret' have been stored
    printf("url===========================\n%s\n", url_response_data);
    printf("brand:========================\n%s\n", parse_ret);
   
    printf("======The above is the test log and results for question one======\n"); 
    printf("====The following is the test log and results for question two====\n");

    // Initializing, mount and register SPIFFS.
    spiffs_init_mount_register();
    
    // Write data to file
    write_file(parse_ret);

    // Read data from file
    read_file();
    
    // Unmount partition and disable SPIFFS
    unmount_spiffs();

    // disconnect wifi from AP
    wifi_disconnect();

    free(url_response_data);
    url_response_data = NULL;

    free(parse_ret);
    parse_ret = NULL;
}
