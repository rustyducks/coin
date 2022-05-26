#include "Coin/Sensors/VL6180.h"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <sys/ioctl.h>

#include <iostream>

namespace rd {
VL6180::VL6180() : handle_(-1), state_(IDLE), distance_(0), status_(0) {}

int VL6180::readByte(int reg) {
    if (handle_ == -1) {
        std::cout << "[VL6180] Reading byte from uninitialized sensor" << std::endl;
        return -1;
    }
    char data_write[2];
    char data_read[1];
    data_write[0] = (reg >> 8) & 0xFF;  // MSB of register address
    data_write[1] = reg & 0xFF;         // LSB of register address

    write(handle_, data_write, 2);
    read(handle_, data_read, 1);
    return data_read[0];
}

void VL6180::writeByte(int reg, uint8_t data) {
    if (handle_ == -1) {
        std::cout << "[VL6180] Writing byte from uninitialized sensor" << std::endl;
        return -1;
    }
    char data_write[3];
    data_write[0] = (reg >> 8) & 0xFF;
    ;                            // MSB of register address
    data_write[1] = reg & 0xFF;  // LSB of register address
    data_write[2] = data & 0xFF;
    write(handle_, data_write, 3);
}
void VL6180::writeTwoBytes(int reg, uint16_t data) {
    if (handle_ == -1) {
        std::cout << "[VL6180] Writing two bytes from uninitialized sensor" << std::endl;
        return -1;
    }
    char data_write[4];
    data_write[0] = (reg >> 8) & 0xFF;
    ;                            // MSB of register address
    data_write[1] = reg & 0xFF;  // LSB of register address
    data_write[2] = (data >> 8) & 0xFF;
    ;                             // MSB of data
    data_write[3] = data & 0xFF;  // LSB of data
    write(handle_, data_write, 4);
}

int VL6180::initialize(int device, int addr) {
    handle_ = -1;
    char buf[15];
    snprintf(buf, 15, "/dev/i2c-%d", device);
    if ((handle_ = open(buf, O_RDWR)) < 0) {
        return -2;
    }

    if (ioctl(handle_, I2C_SLAVE, addr) < 0) {
        return -3;
    }

    int setup = readByte(0x016);

    if (setup == 1) {
        writeByte(0x0207, 0x01);
        writeByte(0x0208, 0x01);
        writeByte(0x0096, 0x00);
        writeByte(0x0097, 0xfd);
        writeByte(0x00e3, 0x00);
        writeByte(0x00e4, 0x04);
        writeByte(0x00e5, 0x02);
        writeByte(0x00e6, 0x01);
        writeByte(0x00e7, 0x03);
        writeByte(0x00f5, 0x02);
        writeByte(0x00d9, 0x05);
        writeByte(0x00db, 0xce);
        writeByte(0x00dc, 0x03);
        writeByte(0x00dd, 0xf8);
        writeByte(0x009f, 0x00);
        writeByte(0x00a3, 0x3c);
        writeByte(0x00b7, 0x00);
        writeByte(0x00bb, 0x3c);
        writeByte(0x00b2, 0x09);
        writeByte(0x00ca, 0x09);
        writeByte(0x0198, 0x01);
        writeByte(0x01b0, 0x17);
        writeByte(0x01ad, 0x00);
        writeByte(0x00ff, 0x05);
        writeByte(0x0100, 0x05);
        writeByte(0x0199, 0x05);
        writeByte(0x01a6, 0x1b);
        writeByte(0x01ac, 0x3e);
        writeByte(0x01a7, 0x1f);
        writeByte(0x0030, 0x00);

        // Recommended : Public registers - See data sheet for more detail

        writeByte(0x0011, 0x10);  // Enables polling for ‘New Sample ready’ when measurement completes
        writeByte(0x010a, 0x30);  // Set the averaging sample period (compromise between lower noise and increased execution time)
        writeByte(0x003f, 0x46);  // Sets the light and dark gain (upper nibble). Dark gain should not be changed.
        writeByte(0x0031, 0xFF);  // sets the # of range measurements after which auto calibration of system is performed
        writeByte(0x0040, 0x63);  // Set ALS integration time to 100ms
        writeByte(0x002e, 0x01);  // perform a single temperature calibratio of the ranging sensor
        writeByte(0x001b, 0x09);  // Set default ranging inter-measurement period to 100ms
        writeByte(0x003e, 0x31);  // Set default ALS inter-measurement period to 500ms
        writeByte(0x0014, 0x24);  // Configures interrupt on ‘New Sample Ready threshold event’

        writeByte(0x016, 0x00);
    }

    setScaling(1);
}

void VL6180::setScaling(int scaling) {
    int scalerValues[] = {0, 253, 127, 84};
    int defaultCrosstalkValidHeight = 20;
    if (scaling < 1 || scaling > 3) {
        return;
    }

    int ptp_offset = readByte(0x24);

    writeTwoBytes(0x96, scalerValues[scaling]);
    writeByte(0x24, ptp_offset / scaling);
    writeByte(0x21, defaultCrosstalkValidHeight / scaling);
    int rce = readByte(0x2d);
    writeByte(0x2d, (rce & 0xFE) | (scaling == 1));
}

void VL6180::changeAddr(int newAddr) {
    writeByte(0x0212, newAddr);
    ioctl(handle_, I2C_SLAVE, newAddr);
}

void VL6180::startRange() { writeByte(0x018, 0x01); }

bool VL6180::pollRange() {
    char status;
    char range_status;

    // check the status
    status = readByte(0x04f);
    range_status = status & 0x07;

    // wait for new measurement ready status
    return range_status == 0x04;
}

void VL6180::clearInterrupts() { writeByte(0x015, 0x07); }

void VL6180::sense() {
    int range;
    if (state_ == IDLE) {
        startRange();
        state_ = RANGING;
    }

    if (state_ == RANGING) {
        if (pollRange()) {
            range = readByte(0x063);
            clearInterrupts();
            state_ = IDLE;
            status_ = readByte(0x004d) >> 4;
            distance_ = range;
        }
    }
}
}  // namespace rd
