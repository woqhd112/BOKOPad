#pragma once

namespace ComplexLibrary
{

	void* MemoryCopy(void* dest, const void* src, size_t size)
	{
		char* temp = (char*)dest;
		const char* s = (const char*)src;

		while (size--)
		{
			*temp++ = *s++;
		}

		return dest;
	}

	void* MemorySet(void* dest, int value, size_t size)
	{
		char* temp = (char*)dest;

		while (size--)
		{
			*temp++ = value;
		}

		return dest;
	}

	void* MemoryMove(void* dest, const void* src, size_t size)
	{
		char* temp = (char*)dest;
		const char* s = (const char*)src;

		if (temp <= s)
		{
			while (size--)
			{
				*temp++ = *s++;
			}
		}
		else
		{
			temp += size;
			s += size;

			while (size--)
			{
				*--temp = *--s;
			}
		}

		return dest;
	}

	int MemoryCompare(void* tgt, void* sub, size_t size)
	{
		size_t i = 0;
		char* temp1 = (char*)tgt;
		char* temp2 = (char*)sub;

		while (i < size)
		{
			if (temp1[i] < temp2[i])
			{
				return -1;
			}
			else if (temp1[i] > temp2[i])
			{
				return 1;
			}

			i++;
		}

		return 0;
	}

	void* MemoryFind(void* tgt, int value, size_t size)
	{
		size_t i = 0;
		char* temp = (char*)tgt;

		while (i < size)
		{
			if (temp[i] == (char)value)
			{
				return ((char*)tgt + i);
			}
			i++;
		}

		return ((void*)0);
	}

	void* MemoryInitialize(void* dest, size_t size)
	{
		return MemorySet(dest, 0, size);
	}

}