<template>
    <div id="all">
        <p id="font">{{ msg }}</p>
        <el-button type="primary" v-on:click="getdata()" id="but">
            瞅瞅有没有啥忘得
        </el-button>
        <ul style="list-style-type: none;padding:0; margin:0;">
            <li v-bind:key="todo" v-for="todo in todolist" id="list">
                <el-tag closable>
                    {{ todo }}
                </el-tag>
            </li>
        </ul>
        <!-- <el-table :data="todolist" style="width: 100%">
            <el-table-column  prop="" label="待办事项" width="180"></el-table-column>
        </el-table> -->
    </div>
</template>


<script>
import axios from 'axios'
var a = 0;
export default {
    name: 'VueTest',

    data() {
        return {
            a : 0,
            msg: '来看看备忘录里面有点嘛',
            todolist : ''
        };
    },

    mounted() {

    },

    methods: {
        changemsg:function(){
            if (this.a == 0){
                this.msg = 'nihao'
                this.a = 1
                console.log(a)
            }else  if (this.a == 1) {
                this.msg = 'hellowwd'
                this.a = 0
                console.log(a)
            }
        },
        getdata:function(){
            axios({
                method:'get',
                url: '/api/getdata'
            }).then(response => {
                this.todolist = response.data
            })
        }
    },
};
</script>

<style scoped>
#list{
    margin-bottom: 10px;
    size: mini;
    /* width: 30px; */
    text-align: center;
}
#but{
    margin-top: 10px;
    margin-bottom: 30px;
    size:mini;
    width: 50%;
    font-size: 150%;


}
#font{
    /* font-family: "Helvetica Neue",Helvetica,"PingFang SC","Hiragino Sans GB","Microsoft YaHei","微软雅黑",Arial,sans-serif; */
    font-family: "font_family" !important;
    font-size: 40px;
    font-style: normal;
}
</style>
