<template>
    <div id="all">
        <el-row>
            <el-col :span="18" :offset="3">
                <p id="font">{{ msg }}</p>
            </el-col>
        </el-row>
        <el-row>
            <el-col :span="18" :offset="3">
                <el-button type="primary" v-on:click="getdata()" id="but">
                    瞅瞅有没有啥忘得
                </el-button>
            </el-col>
        </el-row>

        <ul style="list-style-type: none;padding:0; margin:0;">
            <li v-bind:key="todo" v-for="todo in tododata" id="list">
                <el-tag closable size="medium">
                    {{ todo }}
                </el-tag>
            </li>
            <el-button class="button-new-tag" size="small">+ New Tag</el-button>
        </ul>
        <!-- <el-table :data="tododata" style="width: 100%" id="table">
            <el-table-column  prop="todolist" label="待办事项" width="180"></el-table-column>
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
            tododata : ''
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
                this.tododata = response.data.todolist
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
#table{
    size: medium ;
}
.button-new-tag {
    margin-left: 10px;
    height: 32px;
    line-height: 30px;
    padding-top: 0;
    padding-bottom: 0;
}
</style>
