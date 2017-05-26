#pragma once
class JsEngine
{
public:
	JsEngine(void);
	virtual ~JsEngine(void);

	const Handle<Object>&	global();

protected:
	Persistent<Context> _ctx;
};

