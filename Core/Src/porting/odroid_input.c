#include "odroid_system.h"
#include "odroid_input.h"
#include "gw_buttons.h"
/*typedef enum
{
       ODROID_INPUT_UP = 0,
    ODROID_INPUT_RIGHT,
    ODROID_INPUT_DOWN,
    ODROID_INPUT_LEFT,
    ODROID_INPUT_SELECT,
    ODROID_INPUT_START,
    ODROID_INPUT_A,
    ODROID_INPUT_B,
    ODROID_INPUT_MENU,
    ODROID_INPUT_VOLUME,

       ODROID_INPUT_MAX,

    ODROID_INPUT_ANY,
} odroid_gamepad_key_t;

typedef struct
{
    uint8_t values[ODROID_INPUT_MAX];
    uint16_t bitmask;
} odroid_gamepad_state_t;
*/
void update_gamepad_state(odroid_gamepad_state_t *state, uint32_t buttons, odroid_gamepad_key_t odroid_key, uint32_t gw_key) {
    if(buttons & gw_key) {
        state->values[odroid_key] = 1;
    } else {
        state->values[odroid_key] = 0;
    }
}
 
 void odroid_input_read_gamepad(odroid_gamepad_state_t* out_state)
 {
    memset(out_state, '\x00', sizeof(odroid_gamepad_state_t));

    uint32_t buttons = buttons_get();
    update_gamepad_state(out_state, buttons, ODROID_INPUT_UP, B_Up);
    update_gamepad_state(out_state, buttons, ODROID_INPUT_RIGHT, B_Right);
    update_gamepad_state(out_state, buttons, ODROID_INPUT_DOWN, B_Down);
    update_gamepad_state(out_state, buttons, ODROID_INPUT_LEFT, B_Left);
    update_gamepad_state(out_state, buttons, ODROID_INPUT_START, B_GAME);
    update_gamepad_state(out_state, buttons, ODROID_INPUT_SELECT, B_TIME);
    update_gamepad_state(out_state, buttons, ODROID_INPUT_VOLUME, B_PAUSE);
    update_gamepad_state(out_state, buttons, ODROID_INPUT_POWER, B_POWER);
    update_gamepad_state(out_state, buttons, ODROID_INPUT_A, B_A);
    update_gamepad_state(out_state, buttons, ODROID_INPUT_B, B_B);
    // if(buttons & B_Left)

}

void odroid_input_wait_for_key(odroid_gamepad_key_t key, bool pressed)
{
}

bool odroid_input_key_is_pressed(odroid_gamepad_key_t key)
{
    odroid_gamepad_state_t joystick;
    odroid_input_read_gamepad(&joystick);

    if (key == ODROID_INPUT_ANY) {
        uint32_t buttons = buttons_get();
        if(buttons) {
            return true;
        }
        return false;
    }

    return joystick.values[key];
}

int vBatAverage = 0;

odroid_battery_state_t odroid_input_read_battery()
{
    int vBat = battery_voltage_get();

    vBatAverage -= ((vBatAverage - vBat)/10);

    // Conversion offset of 3v
    int batPercent = vBatAverage - 10200; // subtract 3V

    if (batPercent < 0)
        batPercent = 0;

    // Conversion multiplier
    batPercent = batPercent / 40;

    if (batPercent > 100)
        batPercent = 100;

    odroid_battery_state_t ret = {
        .millivolts = 1337,
        .percentage = batPercent,
    };

    return ret;
}
