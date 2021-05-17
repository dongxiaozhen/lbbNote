
gpg 
    --list-secret-keys      列出私钥
    --list-keys             列出密钥
    --full-generate-key     完整功能的密钥对生成
    --generate-key          生成一个新的密钥对(--full-generate-key的简洁版)
    --armor                 创建 ASCII 字符封装的输出
    -output FILE            写输出到 FILE
    --export                导出密钥
    --export-secret-keys    导出私钥
    --import                导入/合并密钥(私钥需要sudo权限)
    --fingerprint <uid>     显示指纹
    --encrypt               加密数据
    -decrypt                解密数据
    --sign                  生成一份签名(二进制)  (.asc结尾的文件)
    --clear-sign            生成一份明文签名
    --verify                验证签名


// 导出公钥
gpg --armor --output public-key.txt --export <uid>

// 导出私钥
gpg --armor --output private-key.txt --export-secret-keys


// 生成销毁
gpg --gen-revoke <uid>

// 上传公钥到服务器
gpg --send-keys <uid> --keyserver hkp://subkeys.pgp.net


// 加密文件
gpg --recipient <uid> --output demo.en.txt --encrypt demo.txt

// 解密文件
gpg --decrypt demo.en.txt 



// 生成签名
　gpg --clearsign demo.txt

// 验证签名
gpg --verify demo.txt.asc
