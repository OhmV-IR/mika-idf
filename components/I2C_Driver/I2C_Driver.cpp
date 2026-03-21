#include "I2C_Driver.h"
#include <driver/i2c.h>

void I2C_Init(void) {
  i2c_config_t conf = {
	  .mode = I2C_MODE_MASTER,
	  .sda_io_num = (gpio_num_t)I2C_SDA_PIN,
	  .scl_io_num = (gpio_num_t)I2C_SCL_PIN,
	  .sda_pullup_en = GPIO_PULLUP_ENABLE,
	  .scl_pullup_en = GPIO_PULLUP_ENABLE
  };
  conf.master.clk_speed = 400000;
  i2c_param_config(I2C_NUM_0, &conf);
  i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);           
}


bool I2C_Read(uint8_t Driver_addr, uint8_t Reg_addr, uint8_t *Reg_data, uint32_t Length)
{
  esp_err_t ret = i2c_master_write_read_device(I2C_NUM_0, Driver_addr, &Reg_addr, 1, Reg_data, Length, 1000/portTICK_PERIOD_MS);
  if (ret != ESP_OK){
    printf("The I2C transmission fails. - I2C Read\r\n");
    return -1;
  }
  return 0;
}
bool I2C_Write(uint8_t Driver_addr, uint8_t Reg_addr, const uint8_t *Reg_data, uint32_t Length)
{
  uint8_t buffer[1+Length];
  buffer[0] = Reg_addr;
  for(int i = 0; i < Length; i++){
    buffer[1+i] = Reg_data[i];
  }

  esp_err_t ret = i2c_master_write_to_device(I2C_NUM_0, Driver_addr, buffer, 1+Length, 1000/portTICK_PERIOD_MS);
  if (ret != ESP_OK)
  {
    printf("The I2C transmission fails. - I2C Write\r\n");
    return -1;
  }
  return 0;
}
