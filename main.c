/* on the nano side */
#include <stdio.h>

struct star_location {
    float altitude;
    float azimuth;
};

struct star_location get_star_location(const char *star, float lat, float lon) {
    printf("S %s %f %f\n", star, lat, lon);
    struct star_location location;
    HAL_UART_Receive(&hlpuart1, &location, sizeof(location), 0xFFFF);
    return location;
}

void main() {
    struct star_location location = get_star_location("mars", 42.3583, -71.0636);
}
