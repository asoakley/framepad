SET( SOURCES
    assets/fonts/Inter_12_400.c
    assets/fonts/Inter_13_400.c
    assets/fonts/Inter_14_500.c
    assets/fonts/Inter_16_400.c
    assets/fonts/Inter_16_700.c
    assets/fonts/Inter_24_600.c
    assets/images/upload_north_cascades_448x224_png.c
    assets/images/upload_soft_focus_artwork_88x88_png.c
    assets/images/upload_vector_2003_510_2334x2334_png.c
    assets/images/upload_vector_2003_513_2334x1983_png.c
    assets/images/upload_vector_2003_516_2334x1866_png.c
    assets/images/upload_vector_2003_519_1866x2334_png.c
    assets/images/upload_vector_2003_522_1634x2334_png.c
    assets/images/upload_vector_2003_525_1750x2334_png.c
    assets/images/upload_vector_2003_528_2000x18_png.c
    assets/images/upload_vector_2003_531_2000x2000_png.c
    assets/images/upload_vector_2003_534_18x1600_png.c
    assets/images/upload_vector_2003_537_1400x18_png.c
    assets/images/upload_vector_2003_540_18x1600_png.c
    assets/images/upload_vector_2003_543_18x2000_png.c
    behavior/animations.c
    behavior/events.c
    core/gui_core.c
    core/variables.c
    helpers/ui_helpers.c
    screens/Framepad.c
    screens/Framepad___Music_control.c
    screens/Framepad___Slideshow.c
)

add_library( GUI ${SOURCES} )

