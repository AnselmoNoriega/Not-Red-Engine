#pragma once

#include "BlockAllocator.h"

namespace NotRed::Core
{
	template<class DataType>
	class TypedAllocator : private BlockAllocator
	{
	public:
		TypedAllocator(const char* name, size_t capacity)
			: BlockAllocator(name, sizeof(DataType), capacity)
		{
		}

		template<class... Args>
		DataType* New(Args&&... args)
		{
			DataType* instance = static_cast<DataType*>(Allocate());
			new(instance) DataType(std::forward<Args>(args)...);
			return instance;
		}

		void Delete(DataType* ptr)
		{
			if (ptr == nullptr)
			{
				return;
			}

			ptr->~DataType();
			Free(ptr);
		}
	};
}