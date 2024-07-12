from flask import Flask, request, jsonify, render_template
import json

app = Flask(__name__)

sensor_data = []

@app.route('/data', methods=['POST'])
def receive_data():
    data = request.get_json()
    sensor_data.append(data)
    return 'Data received', 200

@app.route('/data', methods=['GET'])
def get_data():
    return jsonify(sensor_data)

@app.route('/')
def index():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(debug=True)
