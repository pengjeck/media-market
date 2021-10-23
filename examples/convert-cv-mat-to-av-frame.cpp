//
// Created by pengjian05 on 2021/10/23.
//
#include<string>
#include <iostream>
#include <opencv2/opencv.hpp>

#ifdef __cplusplus
extern "C" {
#endif
#include <libavformat/avformat.h>
#include <libavutil/error.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavutil/pixfmt.h>
#include <libavutil/timestamp.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif


static bool readAVPixelFormatId(const cv::Mat &mat, AVPixelFormat &format) {
    int type = mat.type();
    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar channel = 1 + (type >> CV_CN_SHIFT);

    std::cout << "type=" << type << ", depth=" << (int) depth
              << ", channel num=" << (int) channel << std::endl;

    if (depth == CV_8U) {
        if (channel == 3) {
            format = AVPixelFormat::AV_PIX_FMT_BGR24;
            return true;
        } else if (channel == 4) {
            format = AVPixelFormat::AV_PIX_FMT_BGRA;
            return true;
        }
    }
    return false;
}

static std::shared_ptr<AVFrame> convertToAVFrame(
        cv::Mat &mat,
        const AVPixelFormat &target_format) {
    AVPixelFormat src_format = AVPixelFormat::AV_PIX_FMT_NONE;
    if (!readAVPixelFormatId(mat, src_format)) {
        std::cout << "read av pixel src_format id failed." << std::endl;
        return nullptr;
    }

    std::shared_ptr<AVFrame> frame(
            av_frame_alloc(), [](auto p) { av_frame_free(&p); });

    frame->format = target_format;
    frame->width = mat.cols;
    frame->height = mat.rows;
    int ret = av_frame_get_buffer(frame.get(), 32);
    if (ret < 0) {
        std::cout << "Could not allocate the video frame data." << std::endl;
        return nullptr;
    }

    ret = av_frame_make_writable(frame.get());
    if (ret < 0) {
        std::cout << "Av frame make writable failed." << std::endl;
        return nullptr;
    }

    SwsContext *swsctx = sws_getContext(
            frame->width,
            frame->height,
            src_format,
            frame->width,
            frame->height,
            target_format,
            SWS_BILINEAR,
            nullptr,
            nullptr,
            nullptr);

    const int stride[4] = {static_cast<int>(mat.step[0])};

    int scale_res = sws_scale(
            swsctx,
            &mat.data,
            stride,
            0,
            mat.rows,
            frame->data,
            frame->linesize);
    std::cout << "sws scale res=" << scale_res << std::endl;

    return frame;
}

static std::shared_ptr<AVFrame> read_frame(
        const std::string &file_name,
        const AVPixelFormat &target_format) {
    cv::Mat mat = cv::imread(file_name, cv::IMREAD_UNCHANGED);
    return convertToAVFrame(mat, target_format);
}

int main() {
    AVPixelFormat target_pix_fmt = AV_PIX_FMT_YUVA420P;
    const std::shared_ptr<AVFrame> &frame = read_frame(
            "../resources/background_earth_landscape.jpg", target_pix_fmt);
    std::cout << "target image width=" << frame->width
              << ", image height=" << frame->height << std::endl;
    return 1;
}