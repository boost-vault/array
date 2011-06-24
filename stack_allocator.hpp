#include <boost/static_assert.hpp>
#include <cassert>


template<class T,std::size_t N,class BaseAllocator=std::allocator<T> >
class stack_allocator{
public:
	typedef typename BaseAllocator::pointer pointer;
	typedef typename BaseAllocator::const_pointer const_pointer;
	typedef typename BaseAllocator::reference reference;
	typedef typename BaseAllocator::const_reference const_reference;
	typedef typename BaseAllocator::value_type value_type;
	typedef typename BaseAllocator::size_type size_type;
	typedef typename BaseAllocator::difference_type difference_type;

	
	explicit stack_allocator(void *stack,BaseAllocator base=BaseAllocator())
		: memory(stack)
		, base(base){
		first_free()=static_cast<char *>(memory)+header_size;
		freeblock_next(first_free())=0;
		block_size(first_free())=size - header_size;
	}
	stack_allocator(stack_allocator const &x)
		: memory(x.memory)
		, base(base){}
	template<class U>
	stack_allocator(stack_allocator<U,N * sizeof(T) / sizeof(U),BaseAllocator> const &u);

	template<class Other>
	struct rebind{
		typedef stack_allocator<Other,N * sizeof(T) / sizeof(Other),BaseAllocator> other;
		BOOST_STATIC_ASSERT(N * sizeof(T) % sizeof(Other) == 0);
	};
	pointer address(reference x) const{ return base.address(x); }
	const_pointer address(const_reference x) const{ return base.address(x); }
	pointer allocate(size_type n){
		std::size_t const s=n*sizeof(T);
		void **current_free=&first_free();
		while(*current_free){
			if(block_size(*current_free) >= s + used_header_size){ //found large enough block
				return allocate_block(s,*current_free);
			}
			current_free=&freeblock_next(*current_free);
		}
		//no free block found:
		return base.allocate(n);
	}
	pointer allocate(size_type n,void const *){
		return allocate(n);
	}
	void deallocate(pointer p,size_type n){
		if(p >= memory && p < memory_end()){
			void *block=usedblock_memory_to_block(p);
			assert(n * sizeof(T) <= block_size(block) - used_header_size);
			freeblock_next(block)=first_free();
			first_free()=block;
		}else base.deallocate(p,n);
	}
	size_type max_size() const{ return base.max_size(); }
	size_type max_stack_size() const{
		size_type m=0;
		void *current_free=first_free();
		while(current_free){
			if(block_size(current_free) > m) m=block_size(current_free);
			current_free=freeblock_next(current_free);
		}
		return (m-used_header_size)/sizeof(T);
	}
	bool operator==(stack_allocator const &x) const{
		return this->memory == x.memory && this->base == x.base;
	}
	bool operator!=(stack_allocator const &x) const{ return !this->operator==(x); }
	void construct(pointer p,const_reference val){ base.construct(p,val); }
	void destroy(pointer p){ base.destroy(p); }

private:
	pointer allocate_block(std::size_t s,void *&previous_nextptr){
		void * const currentblock=previous_nextptr;
		if(block_size(currentblock) >= used_header_size + s + free_header_size){  //enough space to split block in 1 used and 1 free block
			void * const newblocknext=freeblock_next(currentblock);
			std::size_t const newblocksize=block_size(currentblock) - s - used_header_size;
			
			block_size(currentblock) = used_header_size + s;

			void * const newblock=static_cast<char *>(currentblock) + block_size(currentblock);
			block_size(newblock)=newblocksize;
			freeblock_next(newblock)=newblocknext;

			previous_nextptr=newblock;
		}else{
			previous_nextptr=freeblock_next(currentblock);
		}
		return static_cast<pointer>(usedblock_memory(currentblock));
	}
	static std::size_t const size=N * sizeof(T);
	static std::size_t const header_size=sizeof(void *); //first free
	static std::size_t const free_header_size=sizeof(void *)+sizeof(std::size_t); //next + size
	static std::size_t const used_header_size=sizeof(std::size_t); //size
	BOOST_STATIC_ASSERT(size > free_header_size + header_size);

	static std::size_t &block_size(void *mem){
		return *static_cast<std::size_t *>(mem);
	}
	static void *&freeblock_next(void *mem){
		return *static_cast<void **>(static_cast<void *>(static_cast<char *>(mem)+sizeof(std::size_t)));
	}
	static void *usedblock_memory(void *mem){
		return static_cast<void *>(static_cast<char *>(mem)+sizeof(std::size_t));
	}
	static void *usedblock_memory_to_block(void *mem){
		return static_cast<void *>(static_cast<char *>(mem)-sizeof(std::size_t));
	}
	void *&first_free(){
		return *static_cast<void **>(memory);
	}
	void *first_free() const{
		return *static_cast<void **>(memory);
	}
	void const *memory_end() const{
		return static_cast<char const *>(memory)+size;
	}
	void *memory;
	BaseAllocator base;
};

