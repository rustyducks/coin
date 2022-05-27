#ifndef GPIOHANDLER_H
#define GPIOHANDLER_H

namespace rd {
/* blink.c
 *
 * Raspberry Pi GPIO example using sysfs interface.
 * Guillermo A. Amaral B. <g@maral.me>
 *
 * This file blinks GPIO 4 (P1-07) while reading GPIO 24 (P1_18).
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define VALUE_MAX 30

enum eInputOutput { IN = 0, OUT = 1 };
enum eLowHigh { LOW = 0, HIGH = 1 };

static int GPIOExport(int pin) {
#define BUFFER_MAX 3
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    while (-1 == fd) {
        fprintf(stderr, "Failed to open export for writing! Retrying\n");
        usleep(500000);
        fd = open("/sys/class/gpio/export", O_WRONLY);
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    return (0);
}

static int GPIODirection(int pin, int dir) {
    static const char s_directions_str[] = "in\0out";

#define DIRECTION_MAX 35
    char path[DIRECTION_MAX];
    int fd;

    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    while (-1 == fd) {
        fprintf(stderr, "Failed to open gpio direction for writing! Retrying\n");
        usleep(500000);
        fd = open(path, O_WRONLY);
    }

    while (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
        fprintf(stderr, "Failed to set direction!\n");
        usleep(500000);
    }

    close(fd);
    return (0);
}

static int GPIOWrite(int pin, int value) {
    static const char s_values_str[] = "01";

    char path[VALUE_MAX];
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_WRONLY);
    while (-1 == fd) {
        fprintf(stderr, "Failed to open gpio value for writing! Retrying\n");
        usleep(500000);
        fd = open(path, O_WRONLY);
    }

    while (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
        fprintf(stderr, "Failed to write value! Retying\n");
        usleep(500000);
    }

    close(fd);
    return (0);
}

int setupPin(int pin, eInputOutput mode) {
    if (-1 == GPIOExport(pin)) return (1);
    if (-1 == GPIODirection(pin, mode)) return (2);
    return 0;
}

void digitalWrite(int pin, eLowHigh state) { GPIOWrite(pin, state); }
}  // namespace rd

#endif /* GPIOHANDLER_H */
