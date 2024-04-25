2024 Botball Link Code
Vishruth 3/29
#include <kipr/wombat.h>

#define MOTORl 0
#define MOTORr 1
#define CLAW 2
#define ARM 0
#define WRIST 2
//starting arm servo position = 390
//open claw at arm value = 1710
//grab claw at arm value = 1910
//wrist for grabbing poms = 300
//wrist for dump in truck = 2047
//arm for dump in truck = 670

void move_arm_down(int start_pos, int end_pos) {
            int current_pos = start_pos;
	while (current_pos <= end_pos) {
                       set_servo_position(ARM, current_pos);
                       msleep(20); // try 30 and 50
                       current_pos += 100; // try 200 also
         		if (current_pos > end_pos) {
                         set_servo_position(ARM, end_pos);
                      }
            }
}

void move_arm_up(int start_pos, int end_pos) {
        int current_pos = start_pos;
        while (current_pos >= end_pos) {
                       set_servo_position(ARM, current_pos);
                       msleep(20); // try 30 and 50
                       current_pos -= 100; // try 200 also
         		If (current_pos  < end_pos) {
                         set_servo_position(ARM, end_pos);
                      }

            }
	
 } 



int main() {
enable_servos();
set_servo_position(0,390);
mav(MOTORl, 1000);
mav(MOTORr, 1000);
msleep(1000);
off(MOTORl);
off(MOTORr);
move_arm_down(390, 1710);
set_servo_position(WRIST, 300);
mav(CLAW, 100000); // close at start
msleep(7900);
//set_servo_position(ARM, 1980); // starting to close
move_arm_down(1710, 1980);
mav(CLAW, -10000);
msleep(16650);
//set_servo_position(ARM, 670);
move_arm_up(1980, 670);
set_servo_position(WRIST, 2047);
mav(CLAW, 100000);
msleep(9250);
/*set_servo_position(ARM, 670);
set_servo_positon(WRIST, 2047);*/

/*mav(MOTORl, 1000);
mav(MOTORr, 400);
msleep(2000);
    
msleep(1000);
mav(MOTORl, -1000);
mav(MOTORr, -1000); 
msleep(3000);*/
Serena 4/7

/*
1. Calibrate black and white
2. Set the program to compete mode
3. make sure cables are secure
4. setup the left arm to be at the right of the poms
5. align the front of the robot to be as forward as possible
6. move the top arm to be at the right
*/

#include <kipr/wombat.h>
#include <stdio.h>

#define PROGRAM TEST_MODE //the actual mode that will be performed

//modes
#define RUN_MODE 0
#define CHECK_SENSOR_MODE 1
#define TEST_MODE 2
#define COMPETE_MODE 3 //run mode + lights

//sensors
#define SENSORl 0 //left color sensor
#define SENSORr 1 //right color sensor

//motors
#define MOTORl 0 //left wheel
#define MOTORr 1 //right wheel
#define CLAW 2
#define BED 3

//servos, 1 is fried
#define ARM 0
#define WRIST 2
#define HOOK 3

//terms
#define LEFT_TURN 1 //turn left mode
#define RIGHT_TURN 0 //turn right mode
#define FORWARD 0 //forward mode
#define BACKWARD 1 //backward mode
#define OPEN 0 //open claw mode
#define CLOSE 1 //close claw mode

//numbers
/*starting arm servo position = 390
open claw at arm value = 1710
grab claw at arm value = 1910
wrist for grabbing poms = 300
wrist for dump in truck = 2047
arm for dump in truck = 670*/
#define BLACK 4000
#define WHITE 2400
#define GRAY 3200
#define MAX_MOTOR_SPEED 1500
#define LEFT_MOTOR_CORRECTION 1.0
#define RIGHT_MOTOR_CORRECTION 0.93

//constantly gives color sensor numbers, only used in CHECK_SENSOR_MODE
void check_line_sensors(){
    while(1){
        printf("right value = %d\n", analog(SENSORr));
        printf("left value = %d\n", analog(SENSORl));
        msleep(300);
    }
}

void move_claw(int direction, int duration) {
    if (direction == OPEN) {
        mav(CLAW, MAX_MOTOR_SPEED);
        msleep(duration);
    }
    else if (direction == CLOSE) {
        mav(CLAW, MAX_MOTOR_SPEED * -1);
    	msleep(duration);
    }
}
    
void move_arm(int end_pos) {
    if (get_servo_position(ARM) <= end_pos) {
        // If start position is higher, lower arm. 
        while (get_servo_position(ARM) <= end_pos) {
            set_servo_position(ARM, get_servo_position(ARM) + 30);
            msleep(50); // try 30 and 20
        }
    } else {
        // If start position is lower, raise arm.
        while (get_servo_position(ARM) >= end_pos) {
        	set_servo_position(ARM, get_servo_position(ARM) - 30);
        	msleep(50); // try 30 and 20
        } 
    }    
}

void move_wrist(int end_pos) {
    if (get_servo_position(WRIST) <= end_pos) {
        // If start position is higher, lower arm. 
        while (get_servo_position(WRIST) <= end_pos) {
            set_servo_position(WRIST, get_servo_position(WRIST) + 30);
            msleep(50); // try 30 and 20
        }
    } else {
        // If start position is lower, raise arm.
        while (get_servo_position(WRIST) >= end_pos) {
        	set_servo_position(WRIST, get_servo_position(WRIST) - 30);
        	msleep(50); // try 30 and 20
        } 
    }    
}

//used in driving
void set_mav_speed(int motor_id, int speed) {
    if (motor_id == MOTORl) {
        mav(MOTORl, LEFT_MOTOR_CORRECTION * speed);
    } else if (motor_id == MOTORr) {
        mav(MOTORr, RIGHT_MOTOR_CORRECTION * speed);
    } else {
        printf("invalid motor_id: %d\n", motor_id);
    }
}

//used in turning
void set_motor_speed(int motor_id, int speed) {
    if (motor_id == MOTORl) {
        motor(MOTORl, LEFT_MOTOR_CORRECTION * speed);
    } else if (motor_id == MOTORr) {
        motor(MOTORr, RIGHT_MOTOR_CORRECTION * speed);
    } else {
        printf("invalid motor_id: %d\n", motor_id);
    }
}

void spin_turn(int turn_direction, int degree) {
    cmpc(MOTORr);
    if (turn_direction == LEFT_TURN) {
        while(gmpc(MOTORr) <= (float) degree / (float) 90 * 1100){
            set_motor_speed(MOTORl, -60);
            set_motor_speed(MOTORr, 60);
        }
    } else if (turn_direction == RIGHT_TURN) {
        while(gmpc(MOTORr) >= (float) degree / (float) 90 * -1100){
            set_motor_speed(MOTORl, 60);
            set_motor_speed(MOTORr, -60);
        }
    } else {
        printf("Unknown turn direction: %d\n", turn_direction);
    }
    set_motor_speed(MOTORl, 0);
    set_motor_speed(MOTORr, 0);
    msleep(50);
    ao();
    msleep(50);
}

void pivot_turn(int turn_direction, int degree) {
    cmpc(MOTORl);
    cmpc(MOTORr);
    if (turn_direction == LEFT_TURN) {
        while(gmpc(MOTORr) <= (float) degree / (float) 90 * 2300){
            set_motor_speed(MOTORl, 0);
            set_motor_speed(MOTORr, 100);
        }
    } else if (turn_direction == RIGHT_TURN) {
        while(gmpc(MOTORl) <= (float) degree / (float) 90 * 2275){
            set_motor_speed(MOTORl, 100);
            set_motor_speed(MOTORr, 0);
        }
    } else {
        printf("Unknown turn direction: %d\n", turn_direction);
    }
    set_motor_speed(MOTORl, 0);
    set_motor_speed(MOTORr, 0);
    msleep(50);
    ao();
    msleep(50);
}

void pivot_turn_backward(int turn_direction, int degree) {
    cmpc(MOTORl);
    cmpc(MOTORr);
    if (turn_direction == LEFT_TURN) {
        while(gmpc(MOTORl) >= (float) degree / (float) 90 * -2275){
            set_motor_speed(MOTORl, -100);
            set_motor_speed(MOTORr, 0);
        }
    } else if (turn_direction == RIGHT_TURN) {
        while(gmpc(MOTORr) >= (float) degree / (float) 90 * -2275){
            set_motor_speed(MOTORl, 0);
            set_motor_speed(MOTORr, -100);
        }
    } else {
        printf("Unknown turn direction: %d\n", turn_direction);
    }
    set_motor_speed(MOTORl, 0);
    set_motor_speed(MOTORr, 0);
    msleep(50);
    ao();
    msleep(50);
}

//straight forward/backward
void drive(int direction, int distance_cm, float multiplier) {
    float speed = MAX_MOTOR_SPEED * multiplier;
    float duration = distance_cm * 61 / multiplier;
    if (direction == FORWARD) {
        set_mav_speed(MOTORl, speed);
        set_mav_speed(MOTORr, speed);
        msleep(duration);
    } else if (direction == BACKWARD) {
        set_mav_speed(MOTORl, -1 * speed);
        set_mav_speed(MOTORr, -1 * speed);
        msleep(duration);
    }
    set_mav_speed(MOTORl, 0);
    set_mav_speed(MOTORr, -10);
    msleep(50);
    ao();
    msleep(50);
}

//straight forward/backward until color sensors both detect black.
void drivetillblk(int direction){
    //drive until both sensors detect black.
    while (analog(SENSORl) <= GRAY || analog(SENSORr) <= GRAY) {
//while both sensors detect white, or one of them detects white
        if (direction == FORWARD){
            set_mav_speed(MOTORl, MAX_MOTOR_SPEED);
            set_mav_speed(MOTORr, MAX_MOTOR_SPEED);
        }
        else if(direction == BACKWARD){
            set_mav_speed(MOTORl, -1 * MAX_MOTOR_SPEED);
            set_mav_speed(MOTORr, -1 * MAX_MOTOR_SPEED);
        }
    }
    ao();
    msleep(100);
}

//straight forward following a black line
void line_follow(float distance) {
    //follow black line for set distance.
    int high = MAX_MOTOR_SPEED;
    int low = 0.7 * MAX_MOTOR_SPEED;
    cmpc(MOTORl);
    cmpc(MOTORr);
    while (gmpc(MOTORl) < distance && gmpc(MOTORr) < distance) {
        msleep(1);
        // if both sensors do not detect black, drive forward
        // if the left sensor detects black, turn left
        // if the right sensor detects black, turn right
        if (analog(SENSORl) < GRAY && analog(SENSORr) < GRAY) {  
            set_mav_speed(MOTORl, high);
            set_mav_speed(MOTORr, high);
        } else if (analog(SENSORl) >= GRAY) {
            set_mav_speed(MOTORl, low);
            set_mav_speed(MOTORr, high);
        } else if(analog(SENSORr) >= GRAY) {
            set_mav_speed(MOTORl, high);
            set_mav_speed(MOTORr, low);
        }
    }
    set_mav_speed(MOTORl, 0);
    set_mav_speed(MOTORr, -10);
    msleep(50);
    ao();
    msleep(50);
}

//actual program mode
void run(){
    enable_servos();
    if (PROGRAM == COMPETE_MODE){
        wait_for_light(3);
        shut_down_in(119);
        printf("light detected\n");
    }
    drivetillblk(FORWARD);
    drivetillblk(FORWARD);
    spin_turn(RIGHT_TURN, 90);
    disable_servos();
}

//test/practice mode
void test(){
    enable_servos();
    drivetillblk(FORWARD);
    disable_servos();
}

int main(){
    switch(PROGRAM){
        case RUN_MODE:
        case COMPETE_MODE:
	printf(“COMPETE_MODE\n”);
            run();
            break;
        case CHECK_SENSOR_MODE:
	printf(“CHECK_SENSOR_MODE\n”);
            check_line_sensors();
            break;
        case TEST_MODE:
	printf(“TEST_MODE\n”);
            test();
            break;
        default:
            printf("invalid mode: %d\n", PROGRAM);
            break;
    }
    return 0;
}

Serena 4/19

/*
1. Calibrate black and white
2. Set the program to compete mode
3. make sure cables are secure
4. setup the left arm to be at the right of the poms
5. align the front of the robot to be as forward as possible
6. move the top arm to be at the right
*/

#include <kipr/wombat.h>
#include <stdio.h>

#define PROGRAM RUN_MODE //the actual mode that will be performed

//modes
#define RUN_MODE 0
#define CHECK_SENSOR_MODE 1
#define TEST_MODE 2
#define COMPETE_MODE 3 //run mode + lights

//sensors
#define SENSORl 0 //left color sensor
#define SENSORr 1 //right color sensor

//motors
#define MOTORl 0 //left wheel
#define MOTORr 1 //right wheel
#define CLAW 2
#define BED 3

//servos, 1 is fried
#define ARM 0
#define WRIST 2
#define HOOK 3

//terms
#define LEFT 1 //turn left mode
#define RIGHT 0 //turn right mode
#define FORWARD 0 //forward mode
#define BACKWARD 1 //backward mode
#define OPEN 0 //open claw mode
#define CLOSE 1 //close claw mode

//numbers
/*starting arm servo position = 390
open claw at arm value = 1710
grab claw at arm value = 1910
wrist for grabbing poms = 300
wrist for dump in truck = 2047
arm for dump in truck = 670*/
#define BLACK 4000
#define WHITE 2400
#define GRAY 3200
#define MAX_MOTOR_SPEED 1500
#define LEFT_MOTOR_CORRECTION 1.0
#define RIGHT_MOTOR_CORRECTION 0.93

//constantly gives color sensor numbers, only used in CHECK_SENSOR_MODE
void check_line_sensors(){
    while(1){
        printf("right value = %d\n", analog(SENSORr));
        printf("left value = %d\n", analog(SENSORl));
        msleep(300);
    }
}

void move_claw(int direction, int duration) {
    //duration in milliseconds
    if (direction == OPEN) {
        mav(CLAW, MAX_MOTOR_SPEED);
        msleep(duration * 1000);
    }
    else if (direction == CLOSE) {
        mav(CLAW, MAX_MOTOR_SPEED * -1);
        msleep(duration * 1000);
    }
}

void move_arm(int end_pos, float multiplier) {
    // speed discount = multiplier
    if (get_servo_position(ARM) <= end_pos) {
        // If start position is higher, lower arm. 
        while (get_servo_position(ARM) <= end_pos) {
            set_servo_position(ARM, get_servo_position(ARM) + 30 * multiplier);
            msleep(50); // try 30 and 20
        }
    } else {
        // If start position is lower, raise arm.
        while (get_servo_position(ARM) >= end_pos) {
            set_servo_position(ARM, get_servo_position(ARM) - 30 * multiplier);
            msleep(50); // try 30 and 20
        } 
    }    
}

void move_wrist(int end_pos) {
    if (get_servo_position(WRIST) <= end_pos) {
        // If start position is higher, lower arm. 
        while (get_servo_position(WRIST) <= end_pos) {
            set_servo_position(WRIST, get_servo_position(WRIST) + 30);
            msleep(50); // try 30 and 20
        }
    } else {
        // If start position is lower, raise arm.
        while (get_servo_position(WRIST) >= end_pos) {
            set_servo_position(WRIST, get_servo_position(WRIST) - 30);
            msleep(50); // try 30 and 20
        } 
    }    
}

//used in driving
void set_mav_speed(int motor_id, int speed) {
    if (motor_id == MOTORl) {
        mav(MOTORl, LEFT_MOTOR_CORRECTION * speed);
    } else if (motor_id == MOTORr) {
        mav(MOTORr, RIGHT_MOTOR_CORRECTION * speed);
    } else {
        printf("invalid motor_id: %d\n", motor_id);
    }
}

//used in turning
void set_motor_speed(int motor_id, int speed) {
    if (motor_id == MOTORl) {
        motor(MOTORl, LEFT_MOTOR_CORRECTION * speed);
    } else if (motor_id == MOTORr) {
        motor(MOTORr, RIGHT_MOTOR_CORRECTION * speed);
    } else {
        printf("invalid motor_id: %d\n", motor_id);
    }
}

void spin_turn(int turn_direction, int degree) {
    cmpc(MOTORr);
    if (turn_direction == LEFT) {
        while(gmpc(MOTORr) <= 1050 * (float) degree / (float) 90){
            set_motor_speed(MOTORl, -50);
            set_motor_speed(MOTORr, 50);
        }
    } else if (turn_direction == RIGHT) {
        while(gmpc(MOTORr) >= -1030 * (float) degree / (float) 90){
            set_motor_speed(MOTORl, 60);
            set_motor_speed(MOTORr, -60);
        }
    } else {
        printf("Unknown turn direction: %d\n", turn_direction);
    }
    set_motor_speed(MOTORl, 0);
    set_motor_speed(MOTORr, 0);
    msleep(50);
    ao();
    msleep(50);
}

void pivot_turn(int turn_direction, int degree) {
    cmpc(MOTORl);
    cmpc(MOTORr);
    if (turn_direction == LEFT) {
        while (gmpc(MOTORr) <= 2210 * (float) degree / (float) 90) {
            set_motor_speed(MOTORl, 0);
            set_motor_speed(MOTORr, 100);
        }
    } else if (turn_direction == RIGHT) {
        while (gmpc(MOTORl) <= 2120 * (float) degree / (float) 90) {
            set_motor_speed(MOTORl, 100);
            set_motor_speed(MOTORr, 0);
        }
    } else {
        printf("Unknown turn direction: %d\n", turn_direction);
    }
    set_motor_speed(MOTORl, 0);
    set_motor_speed(MOTORr, 0);
    msleep(50);
    ao();
    msleep(50);
}

void pivot_turn_backward(int turn_direction, int degree) {
    cmpc(MOTORl);
    cmpc(MOTORr);
    if (turn_direction == LEFT) {
        while(gmpc(MOTORl) >= -2275 * (float) degree / (float) 90){
            set_motor_speed(MOTORl, -100);
            set_motor_speed(MOTORr, 0);
        }
    } else if (turn_direction == RIGHT) {
        while(gmpc(MOTORr) >= -2275 * (float) degree / (float) 90){
            set_motor_speed(MOTORl, 0);
            set_motor_speed(MOTORr, -100);
        }
    } else {
        printf("Unknown turn direction: %d\n", turn_direction);
    }
    set_motor_speed(MOTORl, 0);
    set_motor_speed(MOTORr, 0);
    msleep(50);
    ao();
    msleep(50);
}

//straight forward/backward
void drive(int direction, int distance_cm, float multiplier) {
    float speed = MAX_MOTOR_SPEED * multiplier;
    float duration = distance_cm * 61 / multiplier;
    if (direction == FORWARD) {
        set_mav_speed(MOTORl, speed);
        set_mav_speed(MOTORr, speed);
        msleep(duration);
    } else if (direction == BACKWARD) {
        set_mav_speed(MOTORl, -1 * speed);
        set_mav_speed(MOTORr, -1 * speed);
        msleep(duration);
    }
    set_mav_speed(MOTORl, 0);
    set_mav_speed(MOTORr, -10);
    msleep(50);
    ao();
    msleep(50);
}

// straight forward until color sensors both detect black.
void drivetillblk(){
    float multiplier = 0.91;
    float high = MAX_MOTOR_SPEED;
    float low = MAX_MOTOR_SPEED * 0.5; 
    while (analog(SENSORl) <= GRAY || analog(SENSORr) <= GRAY) {
        // while at least one sensor detects white
        if (analog(SENSORl) <= GRAY && analog(SENSORr) <= GRAY) {
            set_mav_speed(MOTORl, MAX_MOTOR_SPEED * multiplier);
            set_mav_speed(MOTORr, MAX_MOTOR_SPEED);
        }
        if (analog(SENSORl) >= GRAY && analog(SENSORr) <= GRAY) {        
            set_mav_speed(MOTORl, low);
            set_mav_speed(MOTORr, high);
        }
        if (analog(SENSORr) >= GRAY && analog(SENSORl) <= GRAY) {        
            set_mav_speed(MOTORr, low);
            set_mav_speed(MOTORl, high);
        }
    }  
    ao();
    msleep(100);
}

//straight forward following a black line
void line_follow(float distance) {
    //follow black line for set distance.
    int high = MAX_MOTOR_SPEED;
    int low = 0.6 * MAX_MOTOR_SPEED;
    cmpc(MOTORl);
    cmpc(MOTORr);
    while (gmpc(MOTORl) < distance && gmpc(MOTORr) < distance) {
        msleep(1);
        // if both sensors do not detect black, drive forward
        // if the left sensor detects black, turn left
        // if the right sensor detects black, turn right
        if (analog(SENSORl) < GRAY && analog(SENSORr) < GRAY) {  
            set_mav_speed(MOTORl, high);
            set_mav_speed(MOTORr, high);
        } else if (analog(SENSORl) >= GRAY) {
            set_mav_speed(MOTORl, low);
            set_mav_speed(MOTORr, high);
        } else if(analog(SENSORr) >= GRAY) {
            set_mav_speed(MOTORl, high);
            set_mav_speed(MOTORr, low);
        }
    }
    set_mav_speed(MOTORl, 0);
    set_mav_speed(MOTORr, -10);
    msleep(50);
    ao();
    msleep(50);
}

//actual program mode
void run(){
    enable_servos();
    if (PROGRAM == COMPETE_MODE){
        wait_for_light(3);
        shut_down_in(119);
        printf("light detected\n");
    }
    move_arm(500, 1);
    move_wrist(350);
    drivetillblk();
    drive(FORWARD, 5, 1);
    drivetillblk();
    msleep(500);
    pivot_turn(RIGHT, 90);
    msleep(1000);
    line_follow(1400);
    pivot_turn(LEFT, 90);  
    line_follow(5300);
    spin_turn(RIGHT, 231);
    move_wrist(240);
    move_arm(1650, 1);
    move_claw(OPEN, 6);
    move_arm(1800, 0.5);
    move_claw(CLOSE, 6);
    move_arm(600, 1);
    disable_servos();
}

//test/practice mode
void test(){
    enable_servos();
    /* for (int i = 0; i < 4; i++) {
        spin_turn(LEFT, 90);
    } */
    move_claw(OPEN, 5);
    disable_servos();
}

int main(){
    switch(PROGRAM){
        case RUN_MODE:
            printf("RUN_MODE\n");
        case COMPETE_MODE:
            printf("COMPETE_MODE\n");
            run();
            break;
        case CHECK_SENSOR_MODE:
            printf("CHECK_SENSOR_MODE\n");
            check_line_sensors();
            break;
        case TEST_MODE:
            printf("TEST_MODE\n");
            test();
            break;
        default:
            printf("invalid mode: %d\n", PROGRAM);
            break;
    }
    return 0;
}





