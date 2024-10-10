#pragma once

#include <vector>
#include <string>

namespace NotRed::Core
{
	class BlockAllocator
	{
	public:
		BlockAllocator(const char* name, size_t blockSize, size_t capacity);
		virtual ~BlockAllocator();

		BlockAllocator(const BlockAllocator&) = delete;
		BlockAllocator(const BlockAllocator&&) = delete;
		BlockAllocator& operator=(const BlockAllocator&) = delete;
		BlockAllocator& operator=(const BlockAllocator&&) = delete;

		void* Allocate();
		void Free(void* ptr);

	private:
		std::string mName;
		std::vector<void*> mFreeBlocks;

		void* mData = nullptr;
		size_t mBlockSize = 0;
		size_t mCapacity = 0;
		size_t mBlocksAllocatedCurrent = 0;
		size_t mBlocksAllocatedTotal = 0;
		size_t mBlocksFreed = 0;
		size_t mBlocksHighest = 0;
	};
}