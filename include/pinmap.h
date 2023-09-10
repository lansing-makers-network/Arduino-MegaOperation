// Master pin map

// I2C pins: SCL=19. SDA=18

// Bluetooth serial pins
const byte PIN_BT_POWER_N = 17;     // OUT: Bluetooth module power (active low)
const byte PIN_BT_ATMODE = 16;      // OUT: Bluetooth module AT mode
const byte PIN_BT_STATE = 15;       //  IN: Bluetooth module state
const byte PIN_BT_TX = 10;          // OUT: Bluetooth serial transmit
const byte PIN_BT_RX = 9;           //  IN: Bluetooth serial recieve

// SPI pins: SCK=13, MISO=12; MOSI=11; 

// Addressable LEDs
const byte PIN_PIXEL_DATA = 8;      // OUT: WS2812 data pin

// Audio / SD card shield (pins fixed by shield)
const byte PIN_MCS = 7;             // OUT: VS1053 chip select pin
const byte PIN_AUDIO_DCS = 6;       // OUT: VS1053 Data/command select pin
const byte PIN_SDCARD_CS = 4;       // OUT: Chip select for SD card
const byte PIN_AUDIO_DREQ = 3;      //  IN: VS1053 Data request interrupt
const byte PIN_AUDIO_RESET = -1;    // Reset not present on shield

// Touch sensor
const byte PIN_TOUCH_INT = 2;       //  IN: Touch sensor interrupt

// Pins available
// 14, 5
