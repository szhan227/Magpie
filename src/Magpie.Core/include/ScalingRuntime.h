#pragma once
#include "Event.h"

namespace Magpie {

enum class ScalingState {
	Idle,
	Scaling,
	Waiting
};

class ScalingRuntime {
public:
	ScalingRuntime();
	~ScalingRuntime();

	bool Start(HWND hwndSrc, struct ScalingOptions&& options, bool force);

	void ToggleScaling(bool isWindowedMode);

	void SwitchToolbarState();

	// toClipboard 为真时将截图复制到剪贴板而不是保存为文件
	void TakeScreenshot(bool toClipboard = false);

	void Stop();

	ScalingState State() const noexcept {
		return _state.load(std::memory_order_relaxed);
	}

	// 调用者应处理线程同步
	MultithreadEvent<ScalingState> StateChanged;

private:
	void _ScalingThreadProc() noexcept;

	// 确保 _dispatcher 完成初始化
	const winrt::DispatcherQueue& _Dispatcher() noexcept;

	void _State(ScalingState value);

	std::thread _scalingThread;

	winrt::DispatcherQueue _dispatcher{ nullptr };
	std::atomic<bool> _dispatcherInitialized = false;
	// 只能在主线程访问，省下检查 _dispatcherInitialized 的开销
	bool _dispatcherInitializedCache = false;

	std::atomic<ScalingState> _state = ScalingState::Idle;
};

}
