/* Lab5 Part1 : statistical analysis
Mean, standard deviation and correlation
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define SAMPLE_LEN 5

int main()
{

	int i;
	float samples[SAMPLE_LEN][2];
	char buff[20];

	printf("Enter 5 pairs of floats (one per line):\n");
	for (i = 0; i < SAMPLE_LEN; i++){
		scanf("%f%f", &samples[i][0], &samples[i][1]);
	}
	
	printf("Samples:\n");
	for (i = 0; i < SAMPLE_LEN; ++i)
	{
		printf("%.2f %.2f\n", samples[i][0], samples[i][1]);
	}

	// find sample mean of x and y
	float sumX = 0, sumY = 0;

	for (i = 0; i < SAMPLE_LEN; ++i)
	{
		sumX += samples[i][0];
		sumY += samples[i][1];
	}

	float meanX = sumX / SAMPLE_LEN, meanY = sumY / SAMPLE_LEN;

	printf("Mean of X is %.2f, Mean of Y is %.2f\n", meanX, meanY);
	float var2X = 0, var2Y = 0;

	for (i = 0; i < SAMPLE_LEN; ++i)
	{
		float varX = (samples[i][0] - meanX);
		var2X += varX * varX;
		float varY = (samples[i][1] - meanY);
		var2Y += varY * varY;
	}

	// sample standard deviation you divide by (n-1) NOT the population standard deviation where you divide by n
	float stdevX = sqrt(var2X / (SAMPLE_LEN - 1)), stdevY = sqrt(var2Y / (SAMPLE_LEN - 1));

	printf("Standard deviation of X is %.2f, Standard deviation of Y is %.2f\n", stdevX, stdevY);
	float rSum = 0;
	for (i = 0; i < SAMPLE_LEN; ++i)
	{
		float xScore = (samples[i][0] - meanX) / stdevX;
		float yScore = (samples[i][1] - meanY) / stdevY;
		rSum += xScore * yScore;
	}

	float r = rSum / (SAMPLE_LEN - 1);
	printf("Correlation is %.2f\n", r);
}
