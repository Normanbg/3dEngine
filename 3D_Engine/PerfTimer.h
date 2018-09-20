#ifndef __PERFTIMER_H__
#define __PERFTIMER_H__



class PerfTimer
{
public:

	PerfTimer();

	void Start();
	double ReadMs() const;
	
	double ReadTicks() const;

private:
	double	started_at;
	static double frequency;
};

#endif //__PERFTIMER_H__