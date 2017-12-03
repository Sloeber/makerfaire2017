#include "makerfairRome2017Demo.h"
const int ledPin1 = LED_BUILTIN;

void setup() {
	pinMode(ledPin1, OUTPUT);
	Serial.begin(115200);
}

void loop() {
	const uint32_t currentMillis = millis();

    // calculate the state of the leds BETTER CODE
	int ledValue1 = getBlinkLedValue(currentMillis, 100, 2000);
	int ledValue2 = getFadeLedValue(currentMillis, 100, 2000);

    // decide  on which actions we need to do
	bool printAliveMessage = shouldIPrintKeepAliveMessage(currentMillis);
	bool shouldIPlotData = shouldIPlot(currentMillis);

	//change the led state
	digitalWrite(ledPin1, ledValue1);

	//Plot from time to time
	if (shouldIPlotData) {
		plot2(Serial, ledValue1 * 255, ledValue2);
	}

	//show we are still alive
	if (printAliveMessage) {
		Serial.println("I'm alive");
	}

}


/**
 * Based on the parameters tells you whether the led should
 * be  on or off
 */
int getBlinkLedValue(const uint32_t currentMillis, const uint32_t onInterval,
		const uint32_t offInterval) {
	uint32_t currentFrame = currentMillis % (onInterval + offInterval);
	int ret = currentFrame < onInterval;
	return ret;
}
/**
 * Based on the parameters tells you how hard the led should shine
 * return value between 0 255
 */
int getFadeLedValue(const uint32_t currentMillis, const uint32_t onInterval,
		const uint32_t offInterval) {
	uint32_t currentFrame = currentMillis % (onInterval + offInterval);
	if (currentFrame > onInterval) {
		return 255 - map(currentFrame - onInterval, 0, offInterval, 0, 255);
	};
	int ret = map(currentFrame, 0, onInterval, 0, 255);
	return ret;
}

/**
 *Tells you how many times there should have been triggered since the start of the cpu
 */
uint32_t numTriggers(const uint32_t currentMillis,
		const uint32_t triggerInterval) {
	uint32_t ret = currentMillis / triggerInterval;
	return ret;
}

/**
 * Decide whether plotting is needed or not.
 * Plotting is done every XX milliseconds
 */
bool shouldIPlot(const uint32_t currentMillis){
	static int prefNumPlottriggers = 0;
	int curNumPlotTriggers = numTriggers(currentMillis, 20);
	bool plotData = (curNumPlotTriggers != prefNumPlottriggers);
	prefNumPlottriggers = curNumPlotTriggers;
	return plotData;
}

/**
  * Decide whether the sending a keep alive message is needed or not.
 * Plotting is done every XX milliseconds
 */
bool shouldIPrintKeepAliveMessage(const uint32_t currentMillis){
	static int prefNumtriggers = 0;
	int curNumTriggers = numTriggers(currentMillis, 2000);
	bool printAliveMessage = (curNumTriggers != prefNumtriggers);
	prefNumtriggers = curNumTriggers;
	return printAliveMessage;
}
