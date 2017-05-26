printf("hello item2.js!!!\n");



// c++中定义的全局变量
printf("js  **************\n");
GItem.OnInfo = function (arg) {
    printf("GITEM.ONINFO!\n");
    printf(this.title + "\n");
    printf("arg:" + arg + "\n");
    printf(111 + 222 + "\n");
}

GItem.JSFun = function (v) {
    printf("GITEM.JSFun! <==== v = " + v + "\n");
}

GItem.title = "gitem.title";
GItem.printName();

printf("\nItme2  **************\n");

Item2.OnInfo = function () {
    printf("\t\thello world!");
    printf( this.title + " &&&\n" );
}

Item2.OnInfo();
Item2.title = "))))((((((";
Item2.printName();


Item3.OnShow = function () {
    printf("\t\tItem3.OnShow\n");
    printf(this.title + "\n");
}

Item3.OnShow();
Item3.title = "Item3.title";
Item3.OnShow();





