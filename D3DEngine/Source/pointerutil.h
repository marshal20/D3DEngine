#pragma once

template<typename T>
class InterPtr
{
public:
	InterPtr()
	{

	}

	InterPtr(T* ptr)
	{
		m_ptr = ptr;
	}

	~InterPtr()
	{
		if (m_ptr)
			m_ptr->Release();
		m_ptr = nullptr;
	}

	T **operator&()
	{
		return &m_ptr;
	}

	T *operator->()
	{
		return m_ptr;
	}

	operator T*() const
	{
		return m_ptr;
	}

	T* get()
	{
		return m_ptr;
	}

private:
	T* m_ptr = nullptr;
};
