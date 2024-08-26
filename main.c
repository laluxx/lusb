// TODO play sound -p flag
// TODO FIX the tree

#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <stdlib.h>
#include <string.h>

/* #include "ecodes.h" */
/* #define ECODES_IMPLEMENTATIOn */


void print_endpoint(libusb_device *dev, const struct libusb_interface_descriptor *interdesc, int num_endpoints) {
    const struct libusb_endpoint_descriptor *epdesc;
    for (int k = 0; k < num_endpoints; k++) {
        epdesc = &interdesc->endpoint[k];
        printf("      │  │  %s─ \033[1;34m\033[1mEndpoint\033[0m %d: Type: %d, Address: %d\n", (k < num_endpoints - 1) ? "├" : "└", k, epdesc->bDescriptorType, epdesc->bEndpointAddress);
    }
}

void print_altsetting(libusb_device *dev, const struct libusb_interface *inter, int num_altsetting) {
    const struct libusb_interface_descriptor *interdesc;
    for (int j = 0; j < num_altsetting; j++) {
        interdesc = &inter->altsetting[j];
        printf("      │  %s─ \033[1;34m\033[1mAltSetting\033[0m %d: Interface Number: %d, Endpoints: %d\n", (j < num_altsetting - 1) ? "├" : "└", j, interdesc->bInterfaceNumber, interdesc->bNumEndpoints);
        
        print_endpoint(dev, interdesc, interdesc->bNumEndpoints);
        if (j == num_altsetting - 1 && num_altsetting > 1) printf("      │  └\n");
    }
}

void print_device_capabilities(libusb_device *dev) {
    struct libusb_device_descriptor desc;
    struct libusb_config_descriptor *config;
    const struct libusb_interface *inter;

    if (libusb_get_device_descriptor(dev, &desc) < 0) {
        fprintf(stderr, "Failed to get device descriptor\n");
        return;
    }

    putchar('\n');
    printf("\033[1;36m\033[1mDevice:\033[0m Vendor ID \033[1;33m\033[1m%04x\033[0m, Product ID \033[1;33m\033[1m%04x\033[0m\n", desc.idVendor, desc.idProduct);
    if (libusb_get_config_descriptor(dev, 0, &config) < 0) {
        fprintf(stderr, "Failed to get config descriptor\n");
        return;
    }

    printf("  └─ \033[1;34m\033[1mInterfaces\033[0m: %d\n", config->bNumInterfaces);
    for (int i = 0; i < config->bNumInterfaces; i++) {
        inter = &config->interface[i];
        printf("      %s─ \033[1;34m\033[1mInterface\033[0m %d\n", (i < config->bNumInterfaces - 1) ? "├" : "└", i);
        print_altsetting(dev, inter, inter->num_altsetting);
    }
    libusb_free_config_descriptor(config);
}

void list_devices(libusb_context *ctx) {
    libusb_device **list;
    ssize_t count = libusb_get_device_list(ctx, &list);
    if (count < 0) {
        fprintf(stderr, "Error getting device list\n");
        return;
    }

    printf("\033[1;34m\033[1m%zd devices in list.\033[0m\n", count);
    for (ssize_t idx = 0; idx < count; idx++) {
        print_device_capabilities(list[idx]);
    }
    libusb_free_device_list(list, 1);
}

// Hotplug callback function
static int hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data) {
    if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED) {
        printf("Device connected\n");
        print_device_capabilities(dev);
    } else if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT) {
        printf("Device disconnected\n");
    }
    return 0;
}

int main(int argc, char *argv[]) {
    libusb_context *ctx = NULL;
    libusb_init(&ctx);
    
    if (argc > 1 && strcmp(argv[1], "-r") == 0) {
        libusb_hotplug_callback_handle handle;
        if (LIBUSB_SUCCESS == libusb_hotplug_register_callback(ctx, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, 
                                                               LIBUSB_HOTPLUG_NO_FLAGS, LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, 
                                                               LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, NULL, &handle)) {
            printf("\033[1;34m\033[1mMonitoring for devices...\033[0m\n");
            while (1) {
                libusb_handle_events(ctx);
            }
            libusb_hotplug_deregister_callback(ctx, handle);
        } else {
            fprintf(stderr, "Error registering hotplug callback\n");
        }
    } else {
        list_devices(ctx);
    }

    libusb_exit(ctx);
    return 0;
}

