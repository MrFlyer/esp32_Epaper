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
                    查询
                </el-button>
            </el-col>
        </el-row>
        <el-row>
            <el-col :span="12" :offset="6">
                <ul style="list-style-type: none;padding:0; margin:0;"  >
                    <li v-bind:key="todo" v-for="(todo,index) in tododata" id="list" @click="test(index)">
                        <!-- <el-tag closable size="medium"> -->
                            <el-link :underline="false">
                                {{ todo }}
                            </el-link>
                            <el-divider></el-divider>
                        <!-- </el-tag> -->
                    </li>
                    <el-input
                        class="input-new-tag"
                        v-if="inputVisible"
                        v-model="inputValue"
                        ref="saveTagInput"
                        clearable
                        autosize
                        @keyup.enter.native="handleInputConfirm"
                        @blur="handleInputConfirm"
                    >
                    </el-input>
                    <el-button  type="success" v-else class="button-new-tag" size="medium" @click="showInput">添加新备忘录</el-button>
                </ul>
            </el-col>
        </el-row>
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
            tododata : '',
            inputVisible: false,
            inputValue: ''
        };
    },

    mounted() {

    },

    methods: {
        test(e){
            let temp = ""
            temp = this.tododata[e]
            console.log(temp)
            this.$confirm('删除"' + temp + '"该条备忘录', '真的完成了吗', {
                confirmButtonText: '确定',
                cancelButtonText: '取消',
                type: 'warning'
            }).then(() => {
                this.deltdata(temp)
                this.$message({
                    showClose: true,
                    type: 'success',
                    message: '删除成功!'
                });
            }).catch(() => {
                this.$message({
                    showClose: true,
                    type: 'info',
                    message: '看来还是没做完嗷'
                });
            });
        },
        getdata(){
            axios({
                method:'get',
                url: '/api/getdata'
            }).then(response => {
                this.tododata = response.data.todolist
            })
        },
        adddata(input_data){
            axios({
                method:'get',
                url: '/api/adddata',
                params: {
                    addtodo: input_data}
            }).then(response=>{
                this.tododata = response.data.todolist
            })
        },
        deltdata(delt_data){
            axios({
                method:'get',
                url: '/api/deltdata',
                params: {
                    delttodo: delt_data
                }
            }).then(response=>{
                this.tododata = response.data.todolist
            })
        },
        showInput() {
            this.inputVisible = true;
            this.$nextTick(_ => {
            this.$refs.saveTagInput.$refs.input.focus();
        });
        },

        handleInputConfirm() {
        let inputValue = this.inputValue;
        if (inputValue) {
            this.adddata(inputValue);
        }
        this.inputVisible = false;
        this.inputValue = '';
        }
    },
};
</script>

<style scoped>
#list{
    margin-bottom: 10px;
    size: mini;
    /* width: 30px; */
    /* text-align: center; */
    font-size: 20px;
}
#but{
    margin-top: 10px;
    margin-bottom: 30px;
    size:mini;
    width: 40%;
    font-size: 150%;


}
#font{
    /* font-family: "Helvetica Neue",Helvetica,"PingFang SC","Hiragino Sans GB","Microsoft YaHei","微软雅黑",Arial,sans-serif; */
    font-family: "font_family" !important;
    font-size: 35px;
    font-style: normal;
}
#table{
    size: medium ;
}
.button-new-tag {
    margin-left: 10px;
    height: 40px;
    line-height: 40px;
    padding-top: 0;
    padding-bottom: 0;
}
.input-new-tag {
    width: 70%;
    margin-left: 10px;
    vertical-align: bottom;
}
</style>
