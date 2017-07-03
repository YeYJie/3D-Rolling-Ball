#include "include.h"
#include <time.h>
#include <sys/time.h>

double gettime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + (double)tv.tv_usec*1e-6;
}

int main()
{
	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(100.0f), glm::vec3(0.0f),
									glm::vec3(0.0f, 1.0f, 0.0f));

	double total = 0.0;
	const int TIMES = 100000;
	for(int i = 0; i < TIMES; ++i) {
		double begin = gettime();

		glm::mat4 temp = glm::inverse(projection * view);

		double end = gettime();
		total += end - begin;
	}
	cout << total / TIMES;
}
