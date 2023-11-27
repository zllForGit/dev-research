#ifndef _SPIFFS_
#define _SPIFFS_

#define PATH_NAME "/spiffs/hello_this_is_long_name_products.json"

/* Function declaration */
// Initializing, mount and register SPIFFS.
void spiffs_init_mount_register();

// Unmount partition and disable SPIFFS
void unmount_spiffs();

// Write data to file
void write_file(char *content);

// Read data from file
void read_file(void);

#endif
