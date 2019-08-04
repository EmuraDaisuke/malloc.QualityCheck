#pragma once



#include <chrono>
#include "./CLog.h"



class Lapse final {
	public:
		~Lapse();
		Lapse(const char* p, std::size_t s);
	
	private:
		double Now();
	
	
	private:
		double mLapse;
		const char* mp;
		std::size_t ms;
};



Lapse::~Lapse()
{
	clog(mp, ms, Now() - mLapse);
}



Lapse::Lapse(const char* p, std::size_t s)
:mLapse(Now())
,mp(p)
,ms(s)
{
}



double
Lapse::Now()
{
	using namespace std::chrono;
	return static_cast<double>(duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count()) / 1000000000;
}
