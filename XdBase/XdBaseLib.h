#pragma once

#ifdef XDBASE_EXPORTS
#define XDBASE_API __declspec(dllexport)
#else
#define XDBASE_API __declspec(dllimport)
#endif

#include "sstring.h"
using namespace sstring;

#include "Ini.h"
using namespace fblSystem;

#include "prs.h"
using namespace PRS;

#include "SectionCritical.h"

#include "PowerOff.h"

