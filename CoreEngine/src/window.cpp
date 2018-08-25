#include <CoreEngine/window.h>

#include "window\windowimpl.h"
#include "stringutil.h"

namespace ce
{

	Window::Window()
	{
		m_impl = std::make_unique<WindowImpl>();
	}

	Window::~Window()
	{
	}

	void Window::init(const std::string& name, const math::Vec2<int>& size)
	{
		m_impl->init(name, size);
	}

	void Window::cleanup()
	{
		m_impl->cleanup();
	}

	void Window::update()
	{
		m_impl->update();
	}

	void Window::show()
	{
		m_impl->show();
	}

	void Window::hide()
	{
		m_impl->hide();
	}

	void Window::close()
	{
		m_impl->close();
	}

	void Window::maxmize()
	{
		m_impl->maxmize();
	}

	void Window::minimize()
	{
		m_impl->minimize();
	}

	void Window::set_name(const std::string& name)
	{
		m_impl->set_name(name);
	}

	void Window::set_position(const math::Vec2<int>& position)
	{
		m_impl->set_position(position);
	}

	void Window::set_size(const math::Vec2<int>& size)
	{
		m_impl->set_size(size);
	}

	void Window::set_border(bool enable)
	{
		m_impl->set_border(enable);
	}

	void Window::set_resizable(bool enable)
	{
		m_impl->set_resizable(enable);
	}

	bool Window::is_closed() const
	{
		return m_impl->is_closed();
	}

	std::string Window::get_name() const
	{
		return m_impl->get_name();
	}

	math::Vec2<int> Window::get_position() const
	{
		return m_impl->get_position();
	}

	math::Vec2<int> Window::get_size() const
	{
		return m_impl->get_size();
	}

	math::Vec2<int> Window::get_screen_size()
	{
		return WindowImpl::get_screen_size();
	}
}
