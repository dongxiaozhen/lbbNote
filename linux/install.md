
1 插入u盘
2 格式化u盘
    mac 使用磁盘工具格式化
3 制作u盘启动盘
    先使用找到u盘   diskutil list 
    再卸载u盘       diskutil unmountDisk /dev/disk10
    最后制作        sudo dd if=/Users/libinbin/Downloads/ubuntu/ubuntu-22.04.1-desktop-amd64.iso of=/dev/disk10
