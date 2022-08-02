ffmpeg -i "TEK-067.mp4"  -f mp4 -vn TEK-067s2.mp4
ffmpeg -i "xieshang.mkv"  -f mp4 -vn xieshang2.mp4


ffmpeg
-i 指定要转换视频的源文件
-s 视频转换后视频的分辨率
-vcodec 视频转换时使用的编解码器（-codecs）
-r 视频转换换的桢率(默认25桢每秒)
-b 视频转换换的bit率
-ab 音频转换后的bit率(默认64k)
-acodec 制度音频使用的编码器（-codecs）
-ac 制定转换后音频的声道

-ar 音频转换后的采样率

-an 禁用音频

-vn 禁用视频

-acodec copy 复制音频，不转码

-vcodec copy 复制视频，不转码


ffmpeg -formats 
	D、E分别表示解复用和复用
ffmpeg -codecs
	D、E表示的解码器、编码器
ffmpeg -encoders


    -c sets the encoder (see ffmpeg -encoders)
    -c copy only copies bitstream
    -c:v sets only video encoders
    -c:a sets only audio encoders
    -an and -vn would disable audio or video streams



    ffmpeg -i bunny_1080p_60fps.mp4 -ss 00:01:24 -t 00:00:05 compressed_h264.mp4
此命令使用兔子视频作为输入 ( -i)，它寻求 ( -ss) 到时间 00:01:24，然后取 ( -t) 5s 生成输出视频。

    ffmpeg -i compressed_h264.mp4 -c copy -vn compressed_aac.aac
此命令使用压缩的兔子，它复制 ( -c copy）流，但它跳过视频流（ -vn)，有效地生成仅音频文件。

    ffmpeg -i compressed_h264.mp4 -c copy -an compressed_h264.h264
此命令的作用与上一个命令几乎相同，但它会跳过音频 ( -an) 生成仅视频文件。

    ffmpeg -i compressed_h264.mp4 -f s16le -acodec pcm_s16le raw_6ch_s16b_48khz_audio.raw
此命令采用与以前相同的输入，但现在它仅使用 ( -f s16le) 带符号的 16 位生成 pcm 音频 ( -acodec pcm_s16le).

    ffmpeg -i compressed_h264.mp4 -c:v rawvideo -pix_fmt yuv444p raw_1920p_60fps_444p_video.yuv
最后，我们正在创建一个原始 ( -c:v rawvideo) 使用像素格式的视频 ( yuv444p) 提供零压缩。 

    ffmpeg -i compressed_h264.mp4 -c:v libaom-av1 -crf 30 -b:v 2000k -cpu-used 8 -row-mt 1 compressed_av1.mp4
    ffmpeg -i compressed_h264.mp4 -c:a copy -c:v libx264 -crf 23 -maxrate 1M -bufsize 2M compressed_h264_1M.mp4
    ffmpeg -i compressed_h264.mp4 -c:a copy -c:v libx264 -crf 23 -maxrate 500k -bufsize 1M -vf scale=384:-1 compressed_h264_384x216_500k.mp4
    ffmpeg -i compressed_h264.mp4 -c copy compressed_h264.ts
    ffmpeg -hide_banner \ -i compressed_h264.mp4 \ -c:v libx264 -preset ultrafast -tune zerolatency -profile:v high \ -b:v:0 2500k -bufsize 5m -x264opts keyint=60:min-keyint=60:scenecut=-1 \ -b:v:1 1500k -s:v:0 1280:720 \ -bufsize 3m -x264opts keyint=60:min-keyint=60:scenecut=-1 \ -map 0:v:0 -map 0:v:0 \ -c:a libfdk_aac -profile:a aac_low -b:a 64k \ -map 0:a:0 \ -f hls -hls_time 1 -var_stream_map "v:0,a:0 v:1,a:0" \ -master_pl_name master.m3u8 \ -hls_segment_filename 'vs%v/file_%03d.ts' vs%v/out.m3u8 
