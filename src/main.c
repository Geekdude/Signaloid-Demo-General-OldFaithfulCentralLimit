/*
 *	Authored 2022, Damien Zufferey.
 *
 *	Copyright (c) 2022, Signaloid.
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */

#include <stdbool.h>
#include <stdio.h>
#include <uncertain.h>

/*
 *	Number of Samples
 */
#define N 272

/*
 *	Old Faithful waiting time in minutes
 */
float waitingTime[N] = {
	79, 54, 74, 62, 85, 55, 88, 85, 51, 85, 54, 84, 78, 47, 83, 52, 62, 84, 52, 79, 51, 47, 78,
	69, 74, 83, 55, 76, 78, 79, 73, 77, 66, 80, 74, 52, 48, 80, 59, 90, 80, 58, 84, 58, 73, 83,
	64, 53, 82, 59, 75, 90, 54, 80, 54, 83, 71, 64, 77, 81, 59, 84, 48, 82, 60, 92, 78, 78, 65,
	73, 82, 56, 79, 71, 62, 76, 60, 78, 76, 83, 75, 82, 70, 65, 73, 88, 76, 80, 48, 86, 60, 90,
	50, 78, 63, 72, 84, 75, 51, 82, 62, 88, 49, 83, 81, 47, 84, 52, 86, 81, 75, 59, 89, 79, 59,
	81, 50, 85, 59, 87, 53, 69, 77, 56, 88, 81, 45, 82, 55, 90, 45, 83, 56, 89, 46, 82, 51, 86,
	53, 79, 81, 60, 82, 77, 76, 59, 80, 49, 96, 53, 77, 77, 65, 81, 71, 70, 81, 93, 53, 89, 45,
	86, 58, 78, 66, 76, 63, 88, 52, 93, 49, 57, 77, 68, 81, 81, 73, 50, 85, 74, 55, 77, 83, 83,
	51, 78, 84, 46, 83, 55, 81, 57, 76, 84, 77, 81, 87, 77, 51, 78, 60, 82, 91, 53, 78, 46, 77,
	84, 49, 83, 71, 80, 49, 75, 64, 76, 53, 94, 55, 76, 50, 82, 54, 75, 78, 79, 78, 78, 70, 79,
	70, 54, 86, 50, 90, 54, 54, 77, 79, 64, 75, 47, 86, 63, 85, 82, 57, 82, 67, 74, 54, 83, 73,
	73, 88, 80, 71, 83, 56, 79, 78, 84, 58, 83, 43, 60, 75, 81, 46, 90, 46, 74};

/*
 *	Time to Hike To Mallard Lake and back
 */
float travelTime = 240;

/*
 *	How long to wait until the next eruption
 */
float timeWindow = 10;

/*
 *	Returns the distribution made from the waiting time
 */
float
waitingTimeDistribution()
{
	return libUncertainFloatDistFromSamples(waitingTime, N);
}

/*
 *	Compute the chances that an eruption starts in [travelTime; travelTime+timeWindow]
 */
float
eruptionStartsInInterval(float timeToNextEruption)
{
	double lowerProb = libUncertainFloatProbabilityGT(timeToNextEruption, travelTime);
	double upperProb = libUncertainFloatProbabilityGT(timeToNextEruption, travelTime + timeWindow);
	return lowerProb - upperProb;
}

/*
 *	Index to text
 */
void
printErruption(unsigned int i, float timeToEruption)
{
	switch (i)
	{
	case 1:
		printf("Start time of the 1st eruption: %f\n", timeToEruption);
		break;
	case 2:
		printf("Start time of the 2nd eruption: %f\n", timeToEruption);
		break;
	case 3:
		printf("Start time of the 3rd eruption: %f\n", timeToEruption);
		break;
	default:
		printf("Start time of the %dth eruption: %f\n", i, timeToEruption);
	}
}

int
main(int argc, char * argv[])
{
	printf("travelTime set to %f.\n", travelTime);
	printf("timeWindow set to %f.\n\n", timeWindow);

	float        timeToNextEruption = waitingTimeDistribution();
	float        chancetoSeeEruption = 0;
	float        totalChancetoSeeEruption = 0;
	bool         seenSomething = false;
	unsigned int i = 1;

	chancetoSeeEruption = eruptionStartsInInterval(timeToNextEruption);
	while (!seenSomething || chancetoSeeEruption > 0)
	{
		printErruption(i, timeToNextEruption);
		i++;
		totalChancetoSeeEruption += chancetoSeeEruption;
		timeToNextEruption += waitingTimeDistribution();
		chancetoSeeEruption = eruptionStartsInInterval(timeToNextEruption);
		seenSomething = seenSomething || (chancetoSeeEruption > 0);
	}

	printf("\nTotal chances to see an eruption: %f %%.\n", totalChancetoSeeEruption * 100);

	return 0;
}
