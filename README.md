# himport
Simple anti-reverse-engineering technique to slow reversers from easily determining what function is being called.

## How does it work?
First quickly run the bs_hash function (or whatever hashing algorithm you are using) and acquire a hash of the function name you are targeting. Next, when you call the himport function, it will load the dll that the function resides in, and iterate over the functions. It will hash each function name and compare it to the target hash. If it matches, it will cast the function address to the function type you specified and return it.

## How to use
Very ez
```cpp
himport
    <int(__stdcall*)(HWND, LPCSTR, LPCSTR, UINT)>       // MessageBoxA function type
    (a
        "User32.dll",                                   // DLL Name
        { 0, 76, 99, 73, 119, 94, 35, 44, 42, 30, 29 }  // Hashed MessageBoxA
    )
    (
        0,                                              // hWnd
        "Hello World",                                  // lpText
        "POC",                                          // lpCaption
        0                                               // uType
    );
```
This can be one linered but it can be very hard to look at, segment it however you'd like.

## Beware kinda
One thing I have to say, I am shipping it with a trash hashing algoritm that came straight off my dome in 30 seconds, but to be fair its easier to just call the hashing algorithm on the dll then it is to reverse the hashing algorithm. If you feel like it you can easily replace the hashing algo with 

You will never completely stop the RE, only slow them down (cope lol)
