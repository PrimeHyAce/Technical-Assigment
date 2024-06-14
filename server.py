from flask import Flask, request, jsonify

app = Flask(__name__)

# Inisialisasi variabel global untuk menyimpan data
latest_data = {
    "temperature": None,
    "humidity": None,
    "timestamp": None
}

@app.route('/air_quality', methods=['POST'])
def receive_data():
    global latest_data
    data = request.get_json()
    latest_data = data

    # Print data yang diterima di terminal
    print("Received data from ESP32:")
    print(f"Temperature: {data['temperature']} °C")
    print(f"Humidity: {data['humidity']} %")
    print(f"Timestamp: {data['timestamp']}")
    
    return jsonify({"status": "success", "data": latest_data}), 200


@app.route('/air_quality', methods=['GET'])
def get_data():
    global latest_data
    return jsonify(latest_data), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
