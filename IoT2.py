import serial
import paho.mqtt.client as mqtt
import json
import time

# SERIAL
# UART_PORT = "/dev/serial0"
SERIAL_PORT = "COM7"
SERIAL_BAUDRATE = 115200

# MQTT
THINGSBOARD_HOST = "iot.makowski.edu.pl"
ACCESS_TOKEN = "GGePrRYdxgXEh34icpUy"
TELEMETRY_TOPIC = "v1/devices/me/telemetry"

ser = serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE, timeout=1)

# MQTT CLIENT
client = mqtt.Client()
client.username_pw_set(ACCESS_TOKEN)
client.connect(THINGSBOARD_HOST, 1883, 60)

def send_request():
    """Send 'x' character to STM32 to request data."""
    ser.write(b'x')  # character 'x' to init LoRa communication
    time.sleep(5)
def parse_response(data):
    """Parse response from STM32 into a dictionary."""
    telemetry = {}
    try:
        # Split the response into lines
        lines = data.strip().split("\n")
        
        for line in lines:
            # Extract data
            if "Temperature" in line and "sensor" not in line:
                telemetry["Temperature"] = float(line.split(":")[1].strip().split()[0])
            elif "Pressure" in line:
                telemetry["Pressure"] = float(line.split(":")[1].strip().split()[0])
            elif "Soil humidity" in line:
                telemetry["SoilHumidity"] = float(line.split(":")[1].strip().split()[0])
            elif "Temperature on sensor" in line:
                telemetry["SensorTemperature"] = float(line.split(":")[1].strip().split()[0])
            elif "Violet" in line:
                telemetry["Violet"] = float(line.split(":")[1].strip())
            elif "Blue" in line:
                telemetry["Blue"] = float(line.split(":")[1].strip())
            elif "Green" in line:
                telemetry["Green"] = float(line.split(":")[1].strip())
            elif "Yellow" in line:
                telemetry["Yellow"] = float(line.split(":")[1].strip())
            elif "Orange" in line:
                telemetry["Orange"] = float(line.split(":")[1].strip())
            elif "Red" in line:
                telemetry["Red"] = float(line.split(":")[1].strip())
    except Exception as e:
        print(f"Error parsing response: {e}")
    return telemetry

def main():
    while True:
        try:
            # Send request to STM32
            send_request()
            
            # Read response
            # raw_data = ser.read_until(b"[INFO] Response received:\n").decode("utf-8", errors="ignore")
            response = ser.read_until(b"\n\n").decode("utf-8", errors="ignore")  # Collect response
            print(f"Received: {response}")

            # Parse the response
            telemetry = parse_response(response)
            print(f"Parsed Telemetry: {telemetry}")

            # Publish to ThingsBoard if valid telemetry exists
            if telemetry:
                client.publish(TELEMETRY_TOPIC, json.dumps(telemetry))
                print(f"Published: {telemetry}")

        except Exception as e:
            print(f"Error: {e}")

        time.sleep(30)  

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Exiting...")
        ser.close()
        client.disconnect()