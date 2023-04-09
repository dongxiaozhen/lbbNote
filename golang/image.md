
# github.com/fogleman/gg


* https://blog.csdn.net/qq_26037391/article/details/121457209

* DrawImageAnchored方法会令图像的左上角位于 ( x − w ∗ a x , y − h ∗ a y ) (x - w * ax, y - h * ay) (x−w∗ax,y−h∗ay)，其中，w、h分别为所贴入图像的宽和长。也就是说，当指定ax=ay=0.5时，图像的居中位置会位于(x,y)，当ax=ay=1时，图像的右下角会位于(x,y)，而对于DrawImage方法而言，其ax=ay=0。




# alpha混合与porter/duff
## 
    C - 表示像素的颜色，即(RGBA)的RGB部分，C是color的缩写
    A - 表示像素的透明度，A即alpha
    s  - 表示两个混合像素的源像素，s即source
    d - 表示两个混合像素的目标像素，d即destination
    r - 表示两个像素混合后的结果，r即result
    F - 表示作用于C或A上的因子，F即factor

    有了这个符号之后，可以开始用它们来描述porter/duff混合公式了，如下，
        Cr = Cs*Fs + Cd*Fd
        Ar = As*Fs + Ad*Fd
    对于12种不同的混合方式，仅仅是Fs与Fd的取值不同
        http://www.blogjava.net/onedaylover/archive/2008/01/16/175675.html
