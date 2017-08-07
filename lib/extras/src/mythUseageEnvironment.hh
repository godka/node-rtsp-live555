#pragma once
#ifndef _BASIC_USAGE_ENVIRONMENT0_HH
#include "BasicUsageEnvironment0.hh"
#endif

class mythUseageEnvironment : public BasicUsageEnvironment0 {
public:
	static mythUseageEnvironment* createNew(TaskScheduler& taskScheduler);

	// redefined virtual functions:
	virtual int getErrno() const;

	virtual UsageEnvironment& operator<<(char const* str);
	virtual UsageEnvironment& operator<<(int i);
	virtual UsageEnvironment& operator<<(unsigned u);
	virtual UsageEnvironment& operator<<(double d);
	virtual UsageEnvironment& operator<<(void* p);

protected:
	mythUseageEnvironment(TaskScheduler& taskScheduler);
	// called only by "createNew()" (or subclass constructors)
	virtual ~mythUseageEnvironment();
};

