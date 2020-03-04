RGB_MATRIX_EFFECT(RANDOM_COLORS)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static uint8_t hsv_v = 0;
static led_flags_t flags = 0;
static bool cont = false;

static bool RANDOM_COLORS(effect_params_t* params) {
    if (params->init || cont || hsv_v != rgb_matrix_config.hsv.v || flags != params->flags) {
        RGB_MATRIX_USE_LIMITS(led_min, led_max);
        for (int i = led_min; i < led_max; i++) {
            if (HAS_ANY_FLAGS(g_led_config.flags[i], params->flags)) {
                HSV hsv = {rand() % RANDOM_COLORS_WIDTH + RANDOM_COLORS_OFFSET, UINT8_MAX, rgb_matrix_config.hsv.v};
                RGB rgb = hsv_to_rgb(hsv);
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }
        }
        hsv_v = rgb_matrix_config.hsv.v;
        flags = params->flags;
        cont = led_max < DRIVER_LED_TOTAL;
        return cont;
    } else {
        return false;
    }
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
