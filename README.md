# media-market
media operation example about opencv, ffmpeg etc;

all example was place to dir=examples

all resource which used by examples was place to dir=resources

cmake dir create for cmake module; opencv module place in here

# 中文blog

For record the thread of learning, I write some blog in Chinese.
You can find this blog from these link
[./examples/convert-cv-mat-to-av-frame.cpp](https://blog.csdn.net/pengjian444/article/details/120920125)

# build
## NOTICE!

Because no suitable cmake module config file which can help to find ffmpeg on machine
at present, I write absolute path of ffmpeg include and libs. Like below in [CMakeLists.txt](./CMakeLists.txt)
```cmake
include_directories(
        /opt/homebrew/Cellar/ffmpeg/4.4_2/include
        ${OpenCV_INCLUDE_DIRS})

link_directories(/opt/homebrew/Cellar/ffmpeg/4.4_2/lib)
```
If you want to build this project, please replace according to your actual path.

