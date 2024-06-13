from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/air_quality', methods=['POST'])
def air_quality():
    data = request.get_json()
    if 'temperature' in data and 'humidity' in data and 'timestamp' in data:
        temperature = data['temperature']
        humidity = data['humidity']
        timestamp = data['timestamp']
        print(f"Received temperature: {temperature} °C")
        print(f"Received humidity: {humidity} %")
        print(f"Timestamp: {timestamp}")
        return jsonify({"message": "Data received successfully", "temperature": temperature, "humidity": humidity, "timestamp": timestamp}), 200
    else:
        return jsonify({"message": "Invalid data"}), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
