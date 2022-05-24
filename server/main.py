from flask import Flask
from flask import request
from flask import jsonify
import pymysql
import json

app = Flask(__name__)
app.debug = True


def getdata():
    try:
        connect = pymysql.connect(host="127.0.0.1", port=3306, user="root", passwd="*<zpA5uw(lGk", database="test",
                                  charset="utf8")
        cursor = connect.cursor()
        cursor.execute("SELECT * FROM test.table_name")
        temp = cursor.fetchall()
        json_data = []
        for todolist in temp:
            for data in todolist:
                print(data)
                json_data.append(data)
        # json_data = json.dumps(json_data)
        print(json_data)
        cursor.close()
        connect.close()
        return json_data
    except Exception as e:
        print(e)


def changeData(msg_data):
    try:
        connect = pymysql.connect(host="127.0.0.1", port=3306, user="root", passwd="*<zpA5uw(lGk", database="test",
                                  charset="utf8")
        cursor = connect.cursor()
        sql = "INSERT INTO test.table_name(data) VALUES (%s)"
        cursor.execute(sql, msg_data)
        connect.commit()
        cursor.close()
        connect.close()
    except Exception as e:
        print(e)


@app.route('/getdata', methods=["GET"])
def get_data():
    return jsonify({'todolist': getdata()})


@app.route('/changedata', methods=["GET"])
def change_data():
    if request.method == 'GET':
        json_data = request.args.get("todo")
        print(json_data)
        changeData(json_data)
        return jsonify(getdata())


if __name__ == '__main__':
    app.run()
