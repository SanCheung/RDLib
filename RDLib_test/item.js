printf("hello item.js!!!\n");

//globalItem.printName();
//globalItem.setName("222222");
//globalItem.printName();



//// javascript中new对象
//var item = new ItemObj;
//item.OnInfo = function ( v ) {
//    printf( v + "\n" );
//    printf("item.OnInfo!!!\n");
//}

//item.setName("111");
//item.printName();

//printf(item.getName());
//printf("\n");

//item.setName("222");
//item.printName();

//item.title = "title11111";
//printf(item.title);
//printf("\n");
//item.title = "title22222";

// c++中定义的全局变量
printf("js  **************\n");
GItem.OnInfo = function (arg) {
    printf("GITEM.ONINFO!\n");
    printf(this.title + "\n");
    printf("arg:" + arg + "\n");
    printf(111 + 222 + "\n");
}

GItem.JSFun = function (v) {    printf("GITEM.JSFun! <==== v = " + v + "\n" );
}

GItem.title = "gitem.title";
GItem.printName();


//var item2 = new ItemObj;
//item2.OnInfo = function () {
//    printf("item2.OnInfo!!!\n");
//}

//item2.setName("aaa");
//item2.printName();

//printf(item2.getName());
//printf("\n");

//item2.setName("bbb");
//item2.printName();





