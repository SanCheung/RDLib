var test = {

	//尝试访问全局变量
	dumpArray:function()
	{
		printf("[fromJavaScriptToCpp::dumpArray]\n");
		//测试访问全局变量testArray
		var _type = typeof(testArray);
		var len = testArray.length;
		printf("testArray.length="+len+"\n");
		printf("typeof(testArray)="+_type+"\n");
		
		testArray.push("another string.....");
		printf("printf(testArray.join('|'))="+testArray.join("|"));

		printf("\n\n");
	},
	
	//仍旧是全局变量，只不过这次是对象
	dumpObject:function()
	{
		printf("[fromJavaScriptToCpp::dumpObject]\n");
		//测试访问全局变量testObject
		printf("testObject.strVal="+testObject.strVal+"\n");
		printf("\n");
	},

	//Socket内部包含了两个c++类型, 一个socket，一个...忘了
	testSocket:function()
	{
		printf("[fromJavaScriptToCpp::testSocket]\n");

		//将socket移植到javascript中
		var socket = new Socket(Socket.PF_INET, Socket.SOCK_STREAM, Socket.IPPROTO_TCP);
		socket.setOption(Socket.SO_REUSEADDR, true);

		var conn = socket.connect("www.cppblog.com", 80);
		if (conn)
		{
			//http://www.cppblog.com/pansunyou
			var httpHeader = "GET /pansunyou  HTTP/1.1\r\nHost: www.cppblog.com\r\n\r\n"; 
			socket.send(httpHeader);
			var responseText = socket.receive(12000);
			printf("responseText="+responseText+"\n\n");
		}
		socket.close();
	},

	//TestObj这个class中包含了一个CTestObj*指针
	testTestObj:function()
	{
		printf("[fromJavaScriptToCpp::testTestObj]\n");
		
		var obj = new TestObj;
		obj.printName();

		obj.setName("pan");
		obj.printName();

		obj.setName("pansun");
		obj.printName();

		obj.setName("pansunyou");
		obj.printName();
		
		obj.dispose();
	}
}

printf("fromJavaScriptToCpp.js\n");
test.dumpArray();
test.dumpObject();
//test.testSocket();
test.testTestObj();


// globaTestObj 是一个在c++中创建的"类"
// 其性质和 var obj = new TestObj; 完全一致.
// globaTestObj在脚本执行前就已经构造好了.
printf("\n\nglobaTestObj\n");
globaTestObj.setName("colour wolf");
globaTestObj.printName();

