#ifndef VL6180_H
#define VL6180_H

#include <stdint.h>
#include <unistd.h>

namespace rd {

class VL6180 {
   public:
    VL6180();
    int initialiseAddress(int device, int i2c_addr);

    int initialize(int device, int address);

    void changeAddr(int new_addr);

    bool isValid() const { return (status_ == 0x0); }
    bool isOverflow() const { return (status_ == 0b0111 || status_ == 0b1101 || status_ == 0b1111); }
    int getDistance() const { return distance_; }

    /// Set the scalinb mode (read datasheet to seee about the max range vs. precision deal on this sensor)
    ///  \param handle The handle to the sensor given by vl6180_initialise
    ///  \param scaling Index of the scaling mode to use
    void setScaling(int scaling);

    enum eState { IDLE, RANGING, RESULT_AVAILABLE };

    void sense();

   protected:
    int readByte(int reg);
    void writeByte(int reg, uint8_t data);

    void writeTwoBytes(int reg, uint16_t data);
    void startRange();
    bool pollRange();
    bool inError();

    void clearInterrupts();

    int handle_;
    eState state_;
    int distance_;
    uint8_t status_;
};
}  // namespace rd

#endif /* VL6180_H */
