#ifndef MATTER_I2C_H_
#define MATTER_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

int matter_i2c_init(void);
int matter_i2c_release(void);
int matter_i2c_write(uint8_t *p_data, uint16_t length);
int matter_i2c_read(uint8_t *p_data, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif // MATTER_I2C_H_
