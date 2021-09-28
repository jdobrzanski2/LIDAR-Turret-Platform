// main_file.c
// Author: Joseph Dobrzanski
// Sets up SPI screen and displays points based on LIDAR distance measurements and the motor angular position.
// search "jd" for comments of each thread

#define xdc__strict //gets rid of #303-D typedef warning re Uint16, Uint32

#include "Peripheral_Headers/F2802x_Device.h"
#include "spi_screen.h"
#include <xdc/std.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/utils/Load.h>

// values for encoder (angle) things
#define ENCODER_ANG 16       // number of degrees expected from each pulse of the encoder: 224.4count/rev -> 360deg/rev * SF
#define SF  10               // scale factor (to get around floating point numbers)
#define MAX_ANG  360         // maximum angle in circle (360 degrees)

#define BACKGROUND_COLOR 0xFFFF
#define TARGET_COLOR 0x0000

int32 angle = 0;
int32 last_points_angle = 360;
int32 ref_ang = 0;
int16 distance = 0;

// TEST VARIABLE THINGS
#define TEST_DEFAULT 100
int16 test_distance = 0;

// values for sine and cosine approximation
Uint32 temp_val = 0; // used for intermediate steps
int32 x_=0;
int32 y_=0;
int16 quadrant = 0;
int16 x_coord = 0;
int16 y_coord = 0;
int16 x_quadrant_corr = 0;
int16 y_quadrant_corr = 0;

// value for getting CPU utilization data
Uint32 CPU_data;

// values for detected points on screen
#define NUM_POINTS 230
#define NO_ANG_DATA -1
int16 points[NUM_POINTS][2] = {}; // about 950 the limit uint8_t
int16 points_angle[NUM_POINTS];
int16 array_index = 0;

int16 clear_index = 0;
//int16 last_array_index = 0;
int16 last_point[2] = {};

/* Swi handle defined in main_file.cfg */
extern const Swi_Handle mySwi;

/* Semaphore handle defined in main_file.cfg */
extern const Semaphore_Handle draw_Sem;
extern const Semaphore_Handle clear_Sem;
extern const Semaphore_Handle lock_Sem;
extern const Semaphore_Handle redraw_Sem;

//function prototypes:
extern void DeviceInit(void);

// MAIN FUNCTION (Initial setup things)
Int main()
{
    DeviceInit(); //initialize peripherals
    screen_begin(); // configure screen

    int index;// set initial values for angle object array
    for (index = 0; index < NUM_POINTS; index++) {
        points_angle[index] = NO_ANG_DATA;
    }

    fillScreen(0x0000); //set black background (red = 0x001F)
    drawCircle(65,65,0,65,0x0000, 0xFFFF);
    BIOS_start();    // start SYS/BIOS
    return(0);
}


// IDLE THREAD
Void myIdleFxn(Void) 
{
    GpioDataRegs.GPASET.bit.GPIO7 = 1; // set HIGH to allow for CPU utilization measurement via oscilloscope

    /*
    // TEST: for simulating an inputed distance value inputed through "Expressions" watch list
    if (test_distance != TEST_DEFAULT)
    {
        distance = test_distance;
        test_distance = TEST_DEFAULT;
        Swi_post(mySwi);
    }
    */

    //while (SciaRegs.SCIFFRX.bit.RXFFST < 1) {}
    if (!(SciaRegs.SCIFFRX.bit.RXFFST < 1))
    {
        distance = SciaRegs.SCIRXBUF.bit.RXDT;
        Swi_post(mySwi);
    }
    CPU_data = Load_getCPULoad();
}

// jd: HWI for incrementing angle
//      Activates on every motor encoder pulse
Void encoder_Fxn(Void)
{
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1; // // set LOW to allow for CPU utilization measurement via oscilloscope

    // increment angle
    angle = angle + ENCODER_ANG;
    array_index = (array_index + 1) % NUM_POINTS;

    // in case IR LED breaks, system should (in theory) continue to detect objects
    if (angle >= (MAX_ANG*SF)) {
        angle = 0; //angle - (MAX_ANG*SF);
        array_index = 0;
        clear_index = 0;
    }

    Semaphore_post(clear_Sem);
}

// jd: HWI for clearing angle (setting it back to 0 degrees)
//      Activates when when IR pulse hit
Void IR_Fxn(Void)
{
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1; // // set LOW to allow for CPU utilization measurement via oscilloscope
    array_index = 0;
    angle = 0;
    clear_index = 0;
}

// jd: SWI for converting polar coordinates into Cartesian coordinates
//      Activates when SPI data comes in
Void polar_to_cart_Fxn(UArg arg)
{
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1; // // set LOW to allow for CPU utilization measurement via oscilloscope

    // find quadrant of angle
    quadrant = angle/(SF*90);

    // ensure coordinates put into correct quadrant and acquire reference angle
     if (quadrant == 0) {         // from 0 - 90 degrees
         x_quadrant_corr = 1;
         y_quadrant_corr = 1;
         ref_ang = angle;
     } else if (quadrant == 1) {  // from 90 - 180 degrees
         x_quadrant_corr = -1;
         y_quadrant_corr = 1;
         ref_ang = SF*180-angle;
     } else if (quadrant == 2) {  // from 180 - 270 degrees
         x_quadrant_corr = -1;
         y_quadrant_corr = -1;
         ref_ang = angle-SF*180;
     } else if (quadrant == 3) {  // from 270 - 360 degrees
         x_quadrant_corr = 1;
         y_quadrant_corr = -1;
         ref_ang = SF*360-angle;
     }

    // calculate y coordinate with sine approximation
    temp_val = SF*180 - ref_ang;
    y_ = SF*(Uint32)distance*4*(Uint32)ref_ang*temp_val / (40500*SF*SF-((Uint32)ref_ang)*temp_val); // calculate sine approx
    y_ = (y_+SF/2)/SF; // round value and scale back down

    // calculate x coordinate with cosine approximation
    temp_val = (Uint32)ref_ang*(Uint32)ref_ang;
    x_ = SF*(Uint32)distance - SF*(Uint32)distance*5*temp_val / ((Uint32)32400*SF*SF + temp_val); // calculate cosine approx
    x_ = (x_ + SF/2)/SF; // round value and scale back down

    // determine coordinate to display coordinates
    y_coord = _height/2 + y_quadrant_corr*y_;
    x_coord = _width/2 + x_quadrant_corr*x_;

    // store prior point in array
    last_point[0] = points[array_index][0];
    last_point[1] = points[array_index][1];

    points[array_index][0] = x_coord;
    points[array_index][1] = y_coord;
    // if another point comes in for the same angle measurement, clear prior point before drawing new point.
    if (last_points_angle == angle)
    {
        Semaphore_post(redraw_Sem);
    }
    else
    {
        // store new point in array
        points_angle[array_index] = angle;
        Semaphore_post(draw_Sem);
    }

    last_points_angle = angle;

}

// jd: TSK for drawing pixels to screen
//      Activates after SPI SWI activates
Void draw_point_Fxn(Void)
{
    while(TRUE)
    {
        Semaphore_pend(draw_Sem, BIOS_WAIT_FOREVER);

        GpioDataRegs.GPACLEAR.bit.GPIO7 = 1; // // set LOW to allow for CPU utilization measurement via oscilloscope

        Semaphore_pend(lock_Sem, BIOS_WAIT_FOREVER); // lock out other TSK's
        drawPixel(points[array_index][0], points[array_index][1], TARGET_COLOR);// draw pixel to screen
        Semaphore_post(lock_Sem); // release lock
    }
}

// jd: TSK for redrawing a pixel that has the same angular data
//      Activates after screen made full revolution after drawing a point
Void redraw_point_Fxn(Void)
{
    while(TRUE)
    {
        Semaphore_pend(redraw_Sem, BIOS_WAIT_FOREVER);

        GpioDataRegs.GPACLEAR.bit.GPIO7 = 1; // // set LOW to allow for CPU utilization measurement via oscilloscope

        Semaphore_pend(lock_Sem, BIOS_WAIT_FOREVER); // lock out other TSK's
        drawPixel(last_point[0], last_point[1], BACKGROUND_COLOR); // clear pixel from screen
        drawPixel(x_coord, y_coord, TARGET_COLOR);// draw current pixel to screen
        Semaphore_post(lock_Sem); // release lock
    }
}

// jd: TSK for clearing pixels from screen
//      Activates after screen made full revolution after drawing a point
Void clear_point_Fxn(Void)
{
    while(TRUE)
    {
        Semaphore_pend(clear_Sem, BIOS_WAIT_FOREVER);

        GpioDataRegs.GPACLEAR.bit.GPIO7 = 1; // // set LOW to allow for CPU utilization measurement via oscilloscope

        Semaphore_pend(lock_Sem, BIOS_WAIT_FOREVER); // lock out other TSK's
        while(((array_index + 1) % NUM_POINTS) != clear_index)
        {
            drawPixel(points[clear_index][0], points[clear_index][1], BACKGROUND_COLOR);
            clear_index = (clear_index + 1) % NUM_POINTS;
        }
        Semaphore_post(lock_Sem); // release lock
    }
}
