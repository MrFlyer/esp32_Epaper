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
        <el-row>
            <el-col :span="12" :offset="6">
                <ul style="list-style-type: none;padding:0; margin:0;">
                    <li v-bind:key="todo" v-for="todo in tododata" id="list">
                        <!-- <el-tag closable size="medium"> -->
                            {{ todo }}
                            <el-divider></el-divider>
                        <!-- </el-tag> -->
                    </li>
                    <el-input
                        class="input-new-tag"
                        v-if="inputVisible"
                        v-model="inputValue"
                        ref="saveTagInput"
                        size="small"
                        @keyup.enter.native="handleInputConfirm"
                        @blur="handleInputConfirm"
                    >
                    </el-input>
                    <el-button class="button-new-tag" size="medium" @click="showInput">+ New Tag</el-button>
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
    width: 90px;
    margin-left: 10px;
    vertical-align: bottom;
}
</style>
