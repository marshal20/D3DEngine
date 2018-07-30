#pragma once

template<typename T>
class ImplPtr
{
public:
	ImplPtr()
	{
		m_ptr = new T;
	}

	~ImplPtr()
	{
		if(!m_coppied)
			delete m_ptr;
	}

	T *operator->()
	{ 
		return m_ptr; 
	}

	T* get()
	{
		return m_ptr;
	}

	void copy(T* value_ptr)
	{
		*m_ptr = *value_ptr;
		m_coppied = true;
	}
	
private:
	T* m_ptr = nullptr;
	bool m_coppied = false;
};


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
	}

	T **operator&()
	{
		return &m_ptr;
	}

	T *operator->()
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
