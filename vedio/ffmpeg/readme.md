#ffmpeg
-c sets the encoder (see ffmpeg -encoders)
-c copy only copies bitstream
-c:v sets only video encoders
-c:a sets only audio encoders
-an and -vn would disable audio or video streams
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



从视屏里提取音频
    ffmpeg -i share_548123ade13265be76a1fb342fbe16c7.mp4 -vn -c:a mp3 output.mp3

截取视频
    ffmpeg -ss 01:43:10 -to 02:10:19 -i "hhd800.com@SSIS-541.mp4" -c copy  output4.mp4

转换rmkv视频
    ffmpeg -i 只有你.rmvb -c:v libx264 -strict -2 看不见的爱.mp4