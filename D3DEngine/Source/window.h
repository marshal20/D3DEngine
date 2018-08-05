#pragma once
#include <string>
#include <memory>

#include "input.h"
#include "pointerutil.h"


class Window
{
public:
	struct Options
	{
		bool fullscreen;
		bool border;
		bool cursor;
		bool resizable;
		bool centered;

		int x;
		int y;
		int width; // 0 for fullwidth
		int height; // 0 for fullheight
	};

public:
	
	Window();
	Window(const Window& other) = delete;
	~Window();

	void init(const std::string& name = "UNNAMED", const Options& options = windowoptions_DEF);
	void cleanup();

	void update();
	void close();
	bool isClosed();

	void setInputSystem(InputSystem* inputsys);
	void setFullscreenState(bool enabled);

	bool HandleMessage(unsigned int umessage, unsigned int wparam);

private:
	friend class RenderDevice;
	void* getNativeHandle();

private:
	struct NativeHandle;
	std::unique_ptr<NativeHandle> m_handle;

	InputSystem* m_inputsystem = nullptr;
	Options m_options;
	bool m_closed = true;
	int m_screenWidth, m_screenHeight;

	static const Options windowoptions_DEF;
};
