# 程序火焰图 https://github.com/pyroscope-io/pyroscope


# 流程，组建图
    go install oss.terrastruct.com/d2@latest
    echo 'x -> y -> z' > in.d2
    d2 --watch in.d2 out.svg
    
