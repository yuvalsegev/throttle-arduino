typedef struct
{
    int32_t throttle_motors_connected;
    float right_val;
    float left_val;
    float sb_value;
    int32_t sb_move_enable;
    int32_t pb;
    int32_t pb_light;
    int32_t trim;
} throttle_data_down_t;

typedef struct
{
    float right_val;
    float left_val;
    int32_t r_at;
    int32_t r_toga;
    int32_t r_cutoff_low;
    int32_t l_at;
    int32_t l_toga;
    int32_t l_cutoff_low;
    int32_t l_cutoff_high;
    float sb_value;
    int32_t sb_moving;
    float flaps_val;
    int32_t pb;
} throttle_data_up_t;

#define BARKER (0xCA)