#pragma once

/*
template<typename T>
class ImplPtr
{
public:
	ImplPtr()
	{
		m_ptr = new T;
	}

	ImplPtr(const ImplPtr& other)
	{
		if (m_ptr && !m_coppied)
			delete m_ptr;

		m_coppied = true;
		m_ptr = other.m_ptr;
	}

	ImplPtr(T* ptr)
	{
		if (m_ptr && !m_coppied)
			delete m_ptr;

		m_coppied = true;
		m_ptr = ptr;
	}

	~ImplPtr()
	{
		if (m_ptr && !m_coppied)
			delete m_ptr;
	}

	T *operator->()
	{ 
		return m_ptr; 
	}

	operator T*()
	{
		return m_ptr;
	}

	T* get()
	{
		return m_ptr;
	}
	
private:
	T* m_ptr = nullptr;
	bool m_coppied = false;
};
*/


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

	operator T*()
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
