#include <iostream>
#include <string>
#include <functional>
#include <map>
#include "memory"

class Register {
 public:
  // 内部模板类型，不定参数形式
  template<typename... Args>
  using CallbackFunction = std::function<void(Args...)>;

  // 内部模板函数，用于注册回调
  template<typename... Args>
  void RegisterCallback(const std::string& function_name, CallbackFunction<Args...> callback) {
    // 将不定参数函数转换为void*保存
    callback_map_[function_name] = std::make_unique<CallbackHolderImpl<Args...>>(callback);
  }

  // 内部模板函数，用于运行回调
  template<typename... Args>
  void RunCallback(const std::string& function_name, Args... args) {
    auto it = callback_map_.find(function_name);
    if (it != callback_map_.end()) {
      // 强制转换回原始类型并调用
      auto holder = dynamic_cast<CallbackHolderImpl<Args...>*>(it->second.get());
      if (holder) {
        holder->callback(std::forward<Args>(args)...);
      } else {
        std::cout << "Callback function signature mismatch for: " << function_name << std::endl;
      }
    } else {
      std::cout << "No such callback function: " << function_name << std::endl;
    }
  }

 private:
  // 基类用于存储不同类型的回调
  struct CallbackHolderBase {
    virtual ~CallbackHolderBase() = default;
  };

  // 模板派生类用于实际存储具体类型的回调
  template<typename... Args>
  struct CallbackHolderImpl : CallbackHolderBase {
    CallbackHolderImpl(CallbackFunction<Args...> cb) : callback(cb) {}
    CallbackFunction<Args...> callback;
  };

  // 使用map存储回调函数
  std::map<std::string, std::unique_ptr<CallbackHolderBase>> callback_map_;
};

// 示例回调函数
void callbackFunction1(int a, double b) {
  std::cout << "CallbackFunction1 called with: " << a << ", " << b << std::endl;
}

void callbackFunction2(const std::string &s, int n, float f) {
  std::cout << "CallbackFunction2 called with: " << s << ", " << n << ", " << f << std::endl;
}

void callbackFunction3(const std::string s, int n, double f) {
  std::cout << "CallbackFunction3 called with: " << s << ", " << n << ", " << f << std::endl;
}

int main() {
  Register reg;

//  std::function<void (int, double)> function = callbackFunction1;
  Register::CallbackFunction<int, double> function = callbackFunction1;
  Register::CallbackFunction<const std::string &, int, float> function2 = callbackFunction2;
  Register::CallbackFunction<const std::string , int, float> function3 = callbackFunction3;
  function2(std::string("foo"), 452, 3.5);
  // 注册回调函数
  reg.RegisterCallback("callback1", function);
  reg.RegisterCallback("callback2", function2);
  reg.RegisterCallback("callback3", function3);

  // 运行回调函数
  reg.RunCallback("callback1", 10, 3.14);
  reg.RunCallback<const std::string&, int, float>("callback2", std::string("example"), 42, 2.71f);
  reg.RunCallback("callback2", std::string("example"), 42, 2.71f);

  reg.RunCallback("callback3", std::string("example"), 42, 2.71f);
  // 尝试运行一个未注册的回调函数
  reg.RunCallback("callback4", 42);

  return 0;
}