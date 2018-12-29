#ifndef YASP_TIMER_H
#define YASP_TIMER_H
#include <chrono>

namespace yasp
{

	class Timer
	{
	private:
		std::chrono::high_resolution_clock::time_point t0;
		std::chrono::high_resolution_clock::time_point t1;
	public:
		Timer() {
			t0 = std::chrono::high_resolution_clock::now();
		}
		void Start() { t0 = t1 = std::chrono::high_resolution_clock::now(); }
		void Stop() { t1 = std::chrono::high_resolution_clock::now(); }
		float ElapsedFromStartToStop() const { return std::chrono::duration_cast<std::chrono::duration<float>>(t1 - t0).count(); }
		float ElapsedSinceStart() const {
			return std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - t0).count();
		}
		float Tick() {
			t1 = std::chrono::high_resolution_clock::now();
			float result = std::chrono::duration_cast<std::chrono::duration<float>>(t1 - t0).count();
			t0 = t1;
			return result;
		}
	};
};
#endif
