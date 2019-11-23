#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
#define BACK_LEFT_WHEELS_PORT 2
#define FRONT_LEFT_WHEELS_PORT 1
#define BACK_RIGHT_WHEELS_PORT 12
#define FRONT_RIGHT_WHEELS_PORT 11
#define INTAKE_LEFT 10
#define INTAKE_RIGHT 20

void opcontrol() {
  pros::Motor back_left_wheels (BACK_LEFT_WHEELS_PORT);
  // pros::Motor front_left_wheels (FRONT_LEFT_WHEELS_PORT);
  pros::Motor back_right_wheels (BACK_RIGHT_WHEELS_PORT, true);
  // pros::Motor front_right_wheels (FRONT_RIGHT_WHEELS_PORT, true);
  // pros::Motor arm (ARM_PORT, MOTOR_GEARSET_36); // The arm motor has the 100rpm (red) gearset
  // pros::Motor claw (CLAW_PORT, MOTOR_GEARSET_36);
  pros::Motor intake_left (INTAKE_LEFT);
  pros::Motor intake_right (INTAKE_RIGHT, true); // invert
  pros::Controller master (CONTROLLER_MASTER);

  // arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  // claw.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  while (true) {
    /**
     * This will do tank drive
     */
    back_left_wheels.move(master.get_analog(ANALOG_LEFT_Y));
    // front_left_wheels.move(master.get_analog(ANALOG_LEFT_Y));
    back_right_wheels.move(master.get_analog(ANALOG_RIGHT_Y));
    // front_right_wheels.move(master.get_analog(ANALOG_RIGHT_Y));

    // int power = master.get_analog(ANALOG_RIGHT_Y);
    // int turn = master.get_analog(ANALOG_LEFT_X);
    // int left = power + turn;
    // int right = power - turn;
    // left_wheels.move(left);
    // right_wheels.move(right);

    if (master.get_digital(DIGITAL_R1)) {
      intake_left.move_velocity(100); // This is 100 because it's a 100rpm motor
      intake_right.move_velocity(100);
    }
    else if (master.get_digital(DIGITAL_R2)) {
      intake_left.move_velocity(-100); // This is 100 because it's a 100rpm motor
      intake_right.move_velocity(-100);
    }
    else {
      intake_left.move_velocity(0); // This is 100 because it's a 100rpm motor
      intake_right.move_velocity(0);
    }

    // if (master.get_digital(DIGITAL_L1)) {
    //   claw.move_velocity(100);
    // }
    // else if (master.get_digital(DIGITAL_L2)) {
    //   claw.move_velocity(-100);
    // }
    // else {
    //   claw.move_velocity(0);
    // }

    pros::delay(2);
  }
}