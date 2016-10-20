#include "mbed.h"

#include "flex_sensor.h"

#include "BNO055_fusion/BNO055.h"

Serial pc(USBTX, USBRX);

DigitalOut led(LED1);

AnalogIn flex(A1);
float brightness = 0.0;

void blink() {
    // (note the calls to Thread::wait below for delays)
    while (true) {
        led = !led;
        pc.printf("This is a thing\r\n");
        Thread::wait(1000);
    }
}

void imu_test() {
    DigitalOut pwr_onoff(p13);
    I2C i2c(I2C_SDA0, I2C_SCL0); // p30, p7
    BNO055 imu(i2c, p12);  // Reset =D7, addr = BNO055_G_CHIP_ADDR, mode = MODE_NDOF <- as default
    led = 1;
    Timer t;

    BNO055_ID_INF_TypeDef       bno055_id_inf;
    BNO055_EULER_TypeDef        euler_angles;
    BNO055_QUATERNION_TypeDef   quaternion;
    BNO055_LIN_ACC_TypeDef      linear_acc;
    BNO055_GRAVITY_TypeDef      gravity;
    BNO055_TEMPERATURE_TypeDef  chip_temp;

    pc.printf("Starting IMU test\r\n");
    /*

    imu.set_mounting_position(MT_P6);
    pwr_onoff = 0;
    pc.printf("\r\n\r\nIf pc terminal soft is ready, please hit any key!\r\n");
    char c = pc.getc();
    pc.printf("Bosch Sensortec BNO055 test program on " __DATE__ "/" __TIME__ "\r\n");
    // Is BNO055 avairable?

    if (imu.chip_ready() == 0){
        do {
            pc.printf("Bosch BNO055 is NOT avirable!!\r\n Reset\r\n");
            pwr_onoff = 1;  // Power off
            wait(0.1);
            pwr_onoff = 0;  // Power on
            wait(0.02);
        } while(imu.reset());
    }
    pc.printf("Bosch BNO055 is available now!!\r\n");
    pc.printf("AXIS_REMAP_CONFIG:0x%02x, AXIS_REMAP_SIGN:0x%02x\r\n",
            imu.read_reg0(BNO055_AXIS_MAP_CONFIG), imu.read_reg0(BNO055_AXIS_MAP_SIGN));
    imu.read_id_inf(&bno055_id_inf);
    pc.printf("CHIP ID:0x%02x, ACC ID:0x%02x, MAG ID:0x%02x, GYR ID:0x%02x, ",
            bno055_id_inf.chip_id, bno055_id_inf.acc_id, bno055_id_inf.mag_id, bno055_id_inf.gyr_id);
    pc.printf("SW REV:0x%04x, BL REV:0x%02x\r\n",
            bno055_id_inf.sw_rev_id, bno055_id_inf.bootldr_rev_id);
    pc.printf("If you would like to calibrate the BNO055, please hit 'y' (No: any other key)\r\n");
    c = pc.getc();
    if (c == 'y'){
        // calibrate
    }
    */
    pc.printf("[E]:Euler Angles[deg],[Q]:Quaternion[],[L]:Linear accel[m/s*s],");
    pc.printf("[G]:Gravity vector[m/s*s],[T]:Chip temperature,Acc,Gyr[degC],[S]:Status,[M]:time[mS]\r\n");
    for (;;) {
        imu.get_Euler_Angles(&euler_angles);
        pc.printf("[E],Y,%+6.1f,R,%+6.1f,P,%+6.1f,",
                euler_angles.h, euler_angles.r, euler_angles.p);
        imu.get_quaternion(&quaternion);
        pc.printf("[Q],W,%d,X,%d,Y,%d,Z,%d,",
                quaternion.w, quaternion.x, quaternion.y, quaternion.z);
        imu.get_linear_accel(&linear_acc);
        pc.printf("[L],X,%+6.1f,Y,%+6.1f,Z,%+6.1f,",
                linear_acc.x, linear_acc.y, linear_acc.z);
        imu.get_gravity(&gravity);
        pc.printf("[G],X,%+6.1f,Y,%+6.1f,Z,%+6.1f,",
                gravity.x, gravity.y, gravity.z);
        imu.get_chip_temperature(&chip_temp);
        pc.printf("[T],%+d,%+d,",
                chip_temp.acc_chip, chip_temp.gyr_chip);
        pc.printf("[S],0x%x,[M],%d\r\n",
                imu.read_calib_status(), t.read_ms());

        led = !led;
        Thread::wait(1000);
    }
}

void echo_term() {
    pc.printf("Press 'u' to turn LED1 brightness up, 'd' to turn it down\n");
    for (;;) {
        char c = pc.getc();
        pc.putc(c);

        if ((c == 'u') && (brightness < 0.5)) {
            brightness += 0.01;
            led = brightness;
        }
        if ((c == 'd') && (brightness > 0.0)) {
            brightness -= 0.01;
            led = brightness;
        }

    }
}

void flex_read() {

    flex_sensor_t* f0 = initFlexSensor(A0);

    for (;;) {
        led = !led;
        pc.printf("Flex0 reading is: 0x%x\r\n", flex.read_u16());

        updateFlexSensorDeflection(f0);
        pc.printf("Flex1 reading is: 0x%x\r\n", f0->deflection);
        Thread::wait(500);
    }
}

int main() {

    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
     */

    imu_test();
}
