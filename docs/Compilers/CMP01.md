# Compiler Setup 

Comparison between `.vcxproj` and `CMakeLists.txt` to manage project 

## VS2022 `.vcxproj` setup 

## CMake Setup 

CMakeLists.txt 


프로젝트 설정 시 환경과 컴파일러 설정을 매우 자세히 기록할 필요가 있습니다. 
특히 CMake를 이용해 대규모 프로젝트를 `msvc` 컴파일러 기반으로 디버깅할 경우 매우 중요합니다. 

Windows 버전 
Visual Studio 버전 
Visual Studio SDK 버전 (윈도우와 호환성이 맞아야 `.dll`) 파일 읽기 가능 

예를 들어 Windows Version 10에 SDK가 11이 설치되어 있으면 `.dll` 소스코드를 볼 수 없는 경우가 있습니다. 

MSVC 컴파일러 
cl.exe 

MSVC 링커 
link.exe 

언어 변경 

Tools (도구) -> Options (옵션) 

환경 -> 국가별 설정 

International Settings (국제 설정)



## 프로젝트 설정 / 컴파일 시 필수 확인 

디버그 모드 / 릴리즈 모드 / 커스텀 모드 

구성 속성 / 일반 

구성 속성 / 고급 

구성 속성 -> C / C++ -> 최적화 

최적화 사용 안 함 /Od 

최적화 컴파일의 종류 

/Od 

/O1 

/O2 



## VS2022 C/C++ Optimization 

Compiler Optimization 

- Custom 
- `/Od` Disabled 
- `/O1` Maximum Optimization (Favor Size)
- `/O2` Maximum Optimization (Favor Speed)
- `/Ox` Optimization (Favor Speed) 

Inline Function Expansion 

- Default 
- `/Ob0` Disabled 
- `/Ob1` Only __inline 
- `/Ob2` Any Suitable 

Microsoft Multi threading safe ANSI standard library 
C/C++ -> Code Generation -> Runtime Library 

- `/MDd` Multi-threaded Debug DLL (/MDd) 
- `/MD` Multi-threaded Release DLL (/MD)

### Default Debuggable Release Mode for Server Programming 

- Release Mode `configurationType": "Release`
- Arguments `"-DCMAKE_CXX_FLAGS_RELEASE=\"/Od /Ob1 /MD /Zi\"` 
- Debugging Enabled `DCMAKE_EXE_LINKER_FLAGS_RELEASE=\"/DEBUG\"`
- Compiler Optimization `/Od` Disabled 
- Inline Function Expansion `/Ob1` Only __inline, only `inline` declared functions are inlined  

### Debug Mode to catch 

MSVC Warning C4715 : Not all return route returns value. 
If intended to change this warning as error, setup 

- `WX` Compiler Plag, set Every Message into ERROR 
 

 Or, set some specific warning only as as Error like 

 - `/we4715` 

 Inside CMake, Compiler Setup is possible by 

 ```cmake
add_compile_options(/we4715)   # MSVC
# or for all warnings as errors
add_compile_options(/WX)
 ``` 



Release Mode 
Turn off compiler optimizations for Release mode
`inline` function expansion 

- /Od : Disable optimizations
- /Ob1 : Expand only specified `inline` functions
- /Zi : Include debugging information
- /DNDEBUG : Define `NDEBUG` for Release mode
- /MD : Use multi-threaded DLL C runtime 



목표 

- 릴리즈 모드 기본 
- 최적화 컴파일 끄기 
- 디버그 모드 컴파일로 간헐적으로 에러 파악 

vcxproj 기반 과거 설정 

VS2022 > 속성 > C/C++ > 최적화 
inline 함수 최적화 사용 / 사용 안 함(/Od) 등등 
inline 함수 확장 여기서 
_inline 만 확장 이러면 명시한 것만 inline 처리하도록 가능 __inline만 확장 (/Ob1) 

최적화 컴파일러 끈 상황에서 inline 만 확장 
C++ STL은 inline 처리가 이미 다 되어 있기 때문 

설정 기본 

- Release 모드에서  
- 최적화 컴파일러 끄고 
- inline 함수 확장 

- 이걸 CMakeList.txt에 체계적으로 적용해 보기 

## 공통 디버그/릴리즈 설정

**컴파일러가 MSVC일 경우**

if(MSVC)
  - /Od : 최적화 비활성화
  - /Ob1 : 명시된 inline만 확장
  - /Zi : 디버깅 정보 포함
  - /DNDEBUG : Release 모드 플래그
  - /MD : 멀티스레드 DLL C 런타임 사용
  set(CMAKE_CXX_FLAGS_RELEASE "/Od /Ob1 /Zi /DNDEBUG /MD")
endif()

