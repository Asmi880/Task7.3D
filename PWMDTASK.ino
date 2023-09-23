import RPi.GPIO as GPIO
from gpiozero import PWMLED
import time

# Set up GPIO pins
GPIO.setmode(GPIO.BCM)  # Use BCM GPIO numbering
TRIG_PIN = 4
ECHO_PIN = 17
LED_PIN = 26

# Set up PWM for the LED
led = PWMLED(LED_PIN)

# Function to measure distance using the ultrasonic sensor
def UltrasonicSensor():
    GPIO.setup(TRIG_PIN, GPIO.OUT)
    GPIO.setup(ECHO_PIN, GPIO.IN)

    # Ensure TRIG is initially set to False
    GPIO.output(TRIG_PIN, False)

    # Give some time for the sensor to settle
    time.sleep(0.2)

    # Send a 10µs pulse to trigger the sensor
    GPIO.output(TRIG_PIN, True)
    time.sleep(0.00001)
    GPIO.output(TRIG_PIN, False)

    # Wait for the ECHO pin to go high (start of pulse)
    pulse_start = time.time()
    while GPIO.input(ECHO_PIN) == 0:
        pulse_start = time.time()

    # Wait for the ECHO pin to go low (end of pulse)
    pulse_end = time.time()
    while GPIO.input(ECHO_PIN) == 1:
        pulse_end = time.time()

    # Calculate the pulse duration
    pulse_duration = pulse_end - pulse_start

    # Speed of sound is approximately 343 meters per second
    # Distance = (Time x Speed of Sound) / 2
    distance = (pulse_duration * 34300) / 2

    # Ignore readings that are too small (likely noise)
    if distance < 2 or distance > 400:
        return None

    distance = round(distance, 2)
    return distance

try:
    while True:
        distance = UltrasonicSensor()
        if distance is not None:
            # Adjust LED intensity based on distance
            intensity = 1 - (distance / 100)  # Adjust as needed
            led.value = intensity
        else:
            led.value = 0  # Turn off the LED for invalid readings

        time.sleep(0.5)  # Adjust the delay as needed for your application

except KeyboardInterrupt:
    GPIO.cleanup()
