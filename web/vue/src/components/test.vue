<template>
    <div id="all">
        {{ msg }}
        <button v-on:click="getdata()">
            写点啥
        </button>
        <ol>
            <li v-bind:key="todo" v-for="todo in todolist">
                {{ todo }}
            </li>
        </ol>
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
            msg: 'hellodwwd',
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
