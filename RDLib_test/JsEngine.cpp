#include "StdAfx.h"
#include "JsEngine.h"


JsEngine::JsEngine(void)
{
	_ctx = Context::New();
	_ctx->Enter();
}


JsEngine::~JsEngine(void)
{
	_ctx->Exit();
	_ctx.Dispose();
}

const Handle<Object>& JsEngine::global()
{
	return _ctx->Global();
}
