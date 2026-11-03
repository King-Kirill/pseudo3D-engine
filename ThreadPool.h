#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <thread>
#include <future>
#include <vector>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>

class ThreadPool
{
public:
	ThreadPool(int numThreads);
	~ThreadPool();

    template <typename F, typename... Args>
    // принимает и lvalue и rvalue ссылки, чтобы можно было передавать как lvalue, так и rvalue объекты
    auto add_task(F&& f, Args&&... args)
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

		// говорим компилятору, что тип возвращаемого значения функции task() будет использоваться в дальнейшем
		using Result = decltype(task());

		// связываем задачу с std::packaged_task, чтобы получить std::future для получения результата выполнения задачи
		// состояние функйии сохраняется в shared state, которая будет доступна через std::future
        std::packaged_task<Result()> packaged(
            std::move(task)
        );

		// заводим shared_ptr на std::packaged_task, чтобы передавать его в очередь задач, т.к function требует копируемый объект, а std::packaged_task не копируемый
		auto packaged_ptr = std::make_shared<std::packaged_task<Result()>>(std::move(packaged));

        // получаем std::future для получения результата выполнения задачи
        auto future = packaged_ptr->get_future();

        {
			// блокируем мьютекс для безопасного доступа к очереди задач
            std::unique_lock<std::mutex> lock(mx);
			// добавляем задачу в очередь
            tasks.emplace([packaged_ptr]() {
                (*packaged_ptr)();
            });
        }

		// уведомляем один поток, что появилась новая задача
        cv.notify_one();

        return future;
    }

private:
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;
	std::mutex mx;
	std::condition_variable cv;
	bool stop = false;
};

#endif /* ThreadPool_hpp */