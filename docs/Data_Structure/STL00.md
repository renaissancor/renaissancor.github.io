# STL container size 

```cpp
static void print_stl_size() {
printf("Operating System X86 - %zu Bits\n", sizeof(void*) * 8);
printf("sizeof(hash_set<int>)           = %zu\n", sizeof(hash_set<int>));
printf("sizeof(array<int, 13>)          = %zu\n", sizeof(array<int, 13>));
printf("sizeof(vector<bool>)            = %zu\n", sizeof(vector<bool>));
printf("sizeof(vector<int>)             = %zu\n", sizeof(vector<int>));
printf("sizeof(vector<double>)          = %zu\n", sizeof(vector<double>));
printf("sizeof(vector<string>)          = %zu\n", sizeof(vector<string>));
printf("sizeof(vector<char>)            = %zu\n", sizeof(vector<char>));
printf("sizeof(string)                  = %zu\n", sizeof(string));
printf("sizeof(list<int>)               = %zu\n", sizeof(list<int>));
printf("sizeof(stack<int>)              = %zu\n", sizeof(stack<int>));
printf("sizeof(queue<int>)              = %zu\n", sizeof(queue<int>));
printf("sizeof(deque<int>)              = %zu\n", sizeof(deque<int>));
printf("sizeof(set<int>)                = %zu\n", sizeof(set<int>));
printf("sizeof(map<int, int>)           = %zu\n", sizeof(map<int, int>));
printf("sizeof(priority_queue<int>)     = %zu\n", sizeof(priority_queue<int>));
printf("sizeof(multiset<int>)           = %zu\n", sizeof(multiset<int>));
printf("sizeof(multimap<int, int>)      = %zu\n", sizeof(multimap<int, int>));
printf("sizeof(bitset<64>)              = %zu\n", sizeof(bitset<64>));
printf("sizeof(bitset<1024>)            = %zu\n", sizeof(bitset<1024>));
printf("sizeof(unique_ptr<int>)         = %zu\n", sizeof(unique_ptr<int>));
printf("sizeof(shared_ptr<int>)         = %zu\n", sizeof(shared_ptr<int>));
printf("sizeof(function<int(int)>)      = %zu\n", sizeof(function<int(int)>));
printf("sizeof(unordered_set<int>)      = %zu\n", sizeof(unordered_set<int>));
printf("sizeof(unordered_map<int, int>) = %zu\n", sizeof(unordered_map<int, int>));
printf("sizeof(unordered_multiset<int>) = %zu\n", sizeof(unordered_multiset<int>));
printf("sizeof(unordered_multimap<int, int>) = %zu\n", sizeof(unordered_multimap<int, int>));
}
```

This function result in following for x86-32 OS. 

```txt
Operating System X86 - 32 Bits
sizeof(hash_set<int>)           = 8
sizeof(array<int, 13>)          = 52
sizeof(vector<bool>)            = 16
sizeof(vector<int>)             = 12
sizeof(vector<double>)          = 12
sizeof(vector<string>)          = 12
sizeof(vector<char>)            = 12
sizeof(string)                  = 24
sizeof(list<int>)               = 8
sizeof(stack<int>)              = 20
sizeof(queue<int>)              = 20
sizeof(deque<int>)              = 20
sizeof(set<int>)                = 8
sizeof(map<int, int>)           = 8
sizeof(priority_queue<int>)     = 16
sizeof(multiset<int>)           = 8
sizeof(multimap<int, int>)      = 8
sizeof(bitset<64>)              = 8
sizeof(bitset<1024>)            = 128
sizeof(unique_ptr<int>)         = 4
sizeof(shared_ptr<int>)         = 8
sizeof(function<int(int)>)      = 40
sizeof(unordered_set<int>)      = 32
sizeof(unordered_map<int, int>) = 32
sizeof(unordered_multiset<int>) = 32
sizeof(unordered_multimap<int, int>) = 32
``` 

This function result in following for x86-64 OS. 

```
Operating System X86 - 64 Bits
sizeof(hash_set<int>)           = 16
sizeof(array<int, 13>)          = 52
sizeof(vector<bool>)            = 32
sizeof(vector<int>)             = 24
sizeof(vector<double>)          = 24
sizeof(vector<string>)          = 24
sizeof(vector<char>)            = 24
sizeof(string)                  = 32
sizeof(list<int>)               = 16
sizeof(stack<int>)              = 40
sizeof(queue<int>)              = 40
sizeof(deque<int>)              = 40
sizeof(set<int>)                = 16
sizeof(map<int, int>)           = 16
sizeof(priority_queue<int>)     = 32
sizeof(multiset<int>)           = 16
sizeof(multimap<int, int>)      = 16
sizeof(bitset<64>)              = 8
sizeof(bitset<1024>)            = 128
sizeof(unique_ptr<int>)         = 8
sizeof(shared_ptr<int>)         = 16
sizeof(function<int(int)>)      = 64
sizeof(unordered_set<int>)      = 64
sizeof(unordered_map<int, int>) = 64
sizeof(unordered_multiset<int>) = 64
sizeof(unordered_multimap<int, int>) = 64
```

## About `std::vector`

In MSVC, automatic resize of `std::vector` ratio is `1.5`. 
However, in gcc and clang compiler, it is `2` ratio. 

```cpp
vector<int> vec; 
for (int i = 0; i < 20; ++i) {
    vec.push_back(i);
    printf("Size: %4zu, Capacity: %4zu\n", vec.size(), vec.capacity());
}
```

In MSVC Compiled Visual Studio 2022, code returns  

```txt
Size:    1, Capacity:    1
Size:    2, Capacity:    2
Size:    3, Capacity:    3
Size:    4, Capacity:    4
Size:    5, Capacity:    6
Size:    6, Capacity:    6
Size:    7, Capacity:    9
Size:    8, Capacity:    9
Size:    9, Capacity:    9
Size:   10, Capacity:   13
Size:   11, Capacity:   13
Size:   12, Capacity:   13
Size:   13, Capacity:   13
Size:   14, Capacity:   19
Size:   15, Capacity:   19
Size:   16, Capacity:   19
Size:   17, Capacity:   19
Size:   18, Capacity:   19
Size:   19, Capacity:   19
Size:   20, Capacity:   28
Size:   21, Capacity:   28
Size:   22, Capacity:   28
Size:   23, Capacity:   28
Size:   24, Capacity:   28
Size:   25, Capacity:   28
Size:   26, Capacity:   28
Size:   27, Capacity:   28
Size:   28, Capacity:   28
Size:   29, Capacity:   42
Size:   30, Capacity:   42
```

Also, vector iterators correct position is following. 

```cpp
printf("Vector   data pointer  address : %p\n", (void*)vec.data()); 
printf("Vector  begin element  address : %p\n", (void*)&*vec.begin()); 
printf("Vector    data + size  address : %p\n", (void*)((char*)vec.data() + sizeof(int) * vec.size()));
printf("Vector    end element  address : %p\n", (void*)&*(vec.end())); 
printf("Vector rbegin element  address : %p\n", (void*)&*vec.rbegin()); 
printf("Vector data + size - 1 address : %p\n", (void*)((char*)vec.data() + sizeof(int) * (vec.size() - 1)));
printf("Vector   rend element  address : %p\n", (void*)&*vec.rend()); 
printf("Vector data    -     1 address : %p\n", (void*)((char*)vec.data() - sizeof(int)));
```

This result in 

```txt 
Operating System X86 - 64 Bits
Vector   data pointer  address : 000001EAC7B06960
Vector  begin element  address : 000001EAC7B06960
Vector    data + size  address : 000001EAC7B069D8
Vector    end element  address : 000001EAC7B069D8
Vector rbegin element  address : 000001EAC7B069D4
Vector data + size - 1 address : 000001EAC7B069D4
Vector   rend element  address : 000001EAC7B0695C
Vector data    -     1 address : 000001EAC7B0695C
```

