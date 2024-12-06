#include "include.h"

template<typename ObjectType, typename MethodType, typename... Args>
class PereodicEvent {
private:
    std::chrono::milliseconds period_;
    std::function <void()> event_;

    std::atomic<bool> running_;
    std::thread mainThread_;
    
    void executePeriodically() {
        while (running_) {
            event_();
            std::this_thread::sleep_for(period_);
        }
    }

public:
    
    PereodicEvent (std::chrono::milliseconds period, 
    std::shared_ptr<ObjectType> obj, MethodType method, Args&&... args) noexcept
        : period_(period),
          event_(std::bind(method, obj, std::forward<Args>(args)...)),
          running_(true),
          mainThread_(&PereodicEvent::executePeriodically, this)
    {}

    void handbrake() noexcept {
        running_ = false;
        if (mainThread_.joinable()) {
            mainThread_.join();
        }
    }

    ~PereodicEvent() noexcept {
        handbrake();
    }

};

