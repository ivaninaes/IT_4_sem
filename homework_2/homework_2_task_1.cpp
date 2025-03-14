//1
#include <iostream>
using namespace std;
#include <deque>
class Task {
public:
    std::string descr;
    bool priority;

    Task(const std::string& d, bool p) : descr(d), priority(p) {}
};
class TaskQueue {
private:
    std::deque<Task> queue;
public:
    void addTask(const std::string& d, bool priority) {
        Task newTask(d, priority);
        if (priority) {
            queue.push_front(newTask);
        }
        else {
            queue.push_back(newTask);
        }
    }
    void doTask() {
        if (!queue.empty()) {
            Task task = queue.front();
            std::cout << "In progress: task " << task.descr << std::endl;
            queue.pop_front();
        }
        else {
            std::cout << "No tasks to do" << std::endl;
        }
    }
    void shownumber() {
        std::cout << "Number of tasks to do: " << queue.size() << std::endl;
    }
};
int main() {
    TaskQueue taskQueue;
    taskQueue.addTask("Hello World", true);
    taskQueue.addTask("Wow World", false);
    taskQueue.addTask("Goodbye World", false);
    taskQueue.shownumber();
    taskQueue.doTask();
    taskQueue.doTask();
    taskQueue.doTask();
    taskQueue.shownumber();
    return 0;

}
