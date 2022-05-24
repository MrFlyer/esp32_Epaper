from flask import Flask
from flask import request
from flask import jsonify
import pymysql
import json

app = Flask(__name__)
app.debug = True


# 查数据库目前已有的备忘录列表
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


# 增加新的备忘录条目到数据库中
def addData(msg_data):
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


# 删除数据库选择到的备忘录条目
def deltData(delt_data):
    try:
        connect = pymysql.connect(host="127.0.0.1", port=3306, user="root", passwd="*<zpA5uw(lGk", database="test",
                                  charset="utf8")
        cursor = connect.cursor()
        sql = "DELETE FROM test.table_name WHERE data=(%s)"
        cursor.execute(sql, delt_data)
        connect.commit()
        cursor.close()
        connect.close()
    except Exception as e:
        print(e)


# 获取已有目录路由
@app.route('/getdata', methods=["GET"])
def get_data():
    return jsonify({'todolist': getdata()})


# 增添条目路由
@app.route('/adddata', methods=["GET"])
def change_data():
    if request.method == 'GET':
        json_data = request.args.get("addtodo")
        print(json_data)
        addData(json_data)
        return jsonify({'todolist': getdata()})


# 删除条目路由
@app.route('/deltdata', methods=["GET"])
def delt_data():
    if request.method == 'GET':
        json_data = request.args.get('delttodo')
        print(json_data)
        deltData(json_data)
        return jsonify({'todolist': getdata()})


if __name__ == '__main__':
    app.run()
