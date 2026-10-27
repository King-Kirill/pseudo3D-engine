#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>

class ThreadPool
{
public:
	ThreadPool(size_t numThreads);
	~ThreadPool();

    template <typename F, typename... Args>
    // принимает и lvalue и rvalue ссылки, чтобы можно было передавать как lvalue, так и rvalue объекты
    void add_task(F&& f, Args&&... args)
    {
		// создаем задачу, которая будет вызвана в потоке
        // здесь переданные переменные всегда передаются как lvalue, поэтому нужно избегать копирования
        auto task = [
            // передаем аргументы как args в теле лямбды, вместо параметров, иначе 
            // аргументы пришлось бы передавать каждый раз при вызове лямбды, а нам нужно сохранить их до момента выполнения задачи в другом потоке
            fn = std::forward<F>(f),
            ...args = std::forward<Args>(args)
        ]() mutable {
			// вызываем функцию с переданными аргументами
			// аргументы передаем как rvalue, чтобы избежать копирования
            fn(std::move(args)...);
            };

        {
			// блокируем мьютекс для безопасного доступа к очереди задач
            std::unique_lock<std::mutex> lock(mx);
			// добавляем задачу в очередь
            tasks.emplace(std::move(task));
        }
		// уведомляем один поток, что появилась новая задача
        cv.notify_one();
    }

private:
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;
	std::mutex mx;
	std::condition_variable cv;
	bool stop = false;
};

#endif /* ThreadPool_hpp */