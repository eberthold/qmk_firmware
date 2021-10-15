RGB_MATRIX_EFFECT(ALPHAS_MODS_SPLIT_INVERT)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// alphas = color1, mods = color2
bool ALPHAS_MODS_SPLIT_INVERT(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV hsv  = rgb_matrix_config.hsv;
    RGB rgb1 = rgb_matrix_hsv_to_rgb(hsv);
    hsv.h += rgb_matrix_config.speed;
    RGB rgb2 = rgb_matrix_hsv_to_rgb(hsv);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        bool isFirstHalf = i < (54 / 2);

        // another flag to get the underglow led below the modifiers
        int splitLedIndex = isFirstHalf ? i : (i - 27);
        if (splitLedIndex == 5){
            isFirstHalf = !isFirstHalf;
        }

        if (isFirstHalf != HAS_FLAGS(g_led_config.flags[i], LED_FLAG_MODIFIER)) {
            rgb_matrix_set_color(i, rgb2.r, rgb2.g, rgb2.b);
        } else {
            rgb_matrix_set_color(i, rgb1.r, rgb1.g, rgb1.b);
        }
    }
    return led_max < DRIVER_LED_TOTAL;
}

#    endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS