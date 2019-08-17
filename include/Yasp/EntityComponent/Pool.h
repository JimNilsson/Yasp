#ifndef YASP_POOL_H
#define YASP_POOL_H

namespace yasp
{
	template <typename T>
	class Pool
	{
	public:
		Pool();
		~Pool();

		void Insert(const T& item);


	private:
		T* elements;
		size_t capacity;
		size_t count;

		inline static size_t STARTING_CAPACITY = 512;
		inline static size_t CAPACITY_INCREMENT = 512;

		size_t* freeIndices;
		size_t freeIndicesCapacity;
		size_t freeIndicesCount;

		void Expand();
	};

	template<typename T>
	inline Pool<T>::Pool() : capacity(Pool<T>::STARTING_CAPACITY), count(0), freeIndicesCapacity(Pool<T>::STARTING_CAPACITY), freeIndicesCount(0)
	{
		elements = new T[capacity];
		freeIndices = new size_t[capacity];
	}

	template<typename T>
	inline Pool<T>::~Pool()
	{
		delete[] elements;
		delete[] freeIndices;
	}

	template<typename T>
	inline void Pool<T>::Insert(const T& item)
	{
		if (freeIndicesCount)
		{
			size_t index = freeIndices[--freeIndicesCount];
			elements[index] = item;
		}
		else
		{
			if (count >= capacity)
			{
				Expand();
			}
			elements[count++] = item;
		}
	}

	template<typename T>
	inline void Pool<T>::Expand()
	{
		 
	}
}

#endif
