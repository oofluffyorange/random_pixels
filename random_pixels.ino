#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 96

// Create the SSD1306 display instance (I2C connection)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// Array to store all pixel coordinates
struct Pixel {
  int x, y;
};
Pixel pixelList[SCREEN_WIDTH * SCREEN_HEIGHT];
int pixelIndex = 0; // Index to track lit pixels

void setup() {
  // Initialize the display
  if (!display.begin(SSD1306_PAGEADDR, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;); // Halt if initialization fails
  }

  // Initialize the shuffled list of pixel coordinates
  initializePixelList();

  // Clear the display
  display.clearDisplay();
  display.display();
}

void loop() {
  if (pixelIndex < SCREEN_WIDTH * SCREEN_HEIGHT) {
    // Light the next pixel
    int x = pixelList[pixelIndex].x;
    int y = pixelList[pixelIndex].y;

    display.drawPixel(x, y, SSD1306_WHITE);
    display.display(); // Update the screen

    pixelIndex++; // Move to the next pixel
  } else {
    // Reset the process
    delay(1000); // Pause for 1 second
    display.clearDisplay(); // Clear the display
    display.display();
    initializePixelList(); // Shuffle the pixel list again
    pixelIndex = 0;
  }

  delay(1); // Small delay to control update speed
}

void initializePixelList() {
  // Populate the list with all pixel coordinates
  int idx = 0;
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
      pixelList[idx++] = {x, y};
    }
  }

  // Shuffle the list using the Fisher-Yates algorithm
  for (int i = SCREEN_WIDTH * SCREEN_HEIGHT - 1; i > 0; i--) {
    int j = random(i + 1);
    Pixel temp = pixelList[i];
    pixelList[i] = pixelList[j];
    pixelList[j] = temp;
  }
}
