from flask import Flask
import pymysql
import json
app = Flask(__name__)
app.debug = True

def getData():
    connect = pymysql.connect(host="127.0.0.1", port=3306 ,user="root",passwd ="*<zpA5uw(lGk",database="test",charset="utf8")
    cursor = connect.cursor()
    cursor.execute("SELECT * FROM test.table_name")
    temp = cursor.fetchall()
    temp_json = json.dumps(temp)
    json_data = []
    for todolist in temp:
        for data in todolist:
            print(data)
            json_data.append(data)
    json_data = json.dumps(json_data)
    print(json_data)
    cursor.close()
    connect.close()
    return json_data

@app.route('/')
def index():
    temp = str(getData())
    return temp;

if __name__ == '__main__' :
    app.run()