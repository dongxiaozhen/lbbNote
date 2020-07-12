plot(t,y) 变量是t, 因变量是y的函数 画图
hold on 表示之后执行的plot命令是在上一次的plot图上作画
xlable('time') x轴表示变量的名称是time
ylable('total') y轴表示变量的名称是time
title('file title') 给画起名字
legend('lineone','linetwo') 给图像标记名称
print -dpng xxx.png 保存图像
figure(1); plot()  方便多次画图
subplot(1,2,1) x轴1格子，y轴2格子，标记是1
axis(x1,x2,y1,y2) 设置x,y轴的坐标范围
imagesc(a) 矩阵a 可视化


hist(a) 画图

eye(3) 生成单位矩阵
randn 正态分布
pinv(a) 矩阵的逆矩阵
pi 派
a=[1;2;3]
a(:) 将所有的数据显示成一列
a(1,2)
a(:1)
a(1:)
a=[a,[1;2;3]] 增加一列
a' a矩阵的转置

c=[A B] 将B 放到A的右侧
c=[A;B] 将B 放到A的下面


size(a) 打印向量的长宽
size(a,1) 打印向量的长
size(a,2) 打印向量的宽

load file_one.dat 读取文件 file_one.dat
load file_two.dat 读取文件 file_two.dat

who 显示所有的变量
whos 显示所有变量的详细信息

clear aa 删除变量aa
clear 清理所有变量

save file.txt aa 将变量aa 保存到文件file.txt
save file.txt aa --ascii  以可读的asc形式保存数据

format long  长浮点
format short 短浮点


addpath('/tmp') 增加文件路径，寻找函数

函数
	function y=function_name(x). y 表示返回值，x代表参数
	function [y1,y2] = function_name(x) 返回多个值
