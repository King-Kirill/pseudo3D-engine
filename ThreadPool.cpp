#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads)
{
	for (int i = 0; i < numThreads; ++i)
	{
		workers.emplace_back(std::thread(
			[this] {
				while (true)
				{
					// переменная для хранения задачи, которую нужно выполнить
					std::function<void()> task;

					// скобки нужны для того, чтобы мьютекс был заблокирован только на время доступа к очереди задач
					{
						// блокируем мьютекс для безопасного доступа к очереди задач
						std::unique_lock<std::mutex> lock(mx);

						cv.wait(lock, [this] { return stop || !tasks.empty(); });

						if (stop && tasks.empty())
							return;

						task = std::move(tasks.front());
						tasks.pop();
					}

					// выполняем void таску, которая была извлечена из очереди
					task();
				}
			}
		));
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(mx);
		stop = true;
	}
	// будим все потоки, чтобы они могли завершить работу
	cv.notify_all();

	for(auto& th: workers)
	{
		if (th.joinable())
		{
			th.join();
		}
	}
}
