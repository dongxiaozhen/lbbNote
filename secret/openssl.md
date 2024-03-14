
生成私钥 openssl genrsa -out private_key.pem 1024
根据私钥生成公钥文件 openssl rsa -in private_key.pem -pubout -out public_key.pem



DER = 扩展名 DER 用于二进制 DER 编码的证书。这些证书也可以用 CER 或者 CRT 作为扩展名
PEM = 扩展名 PEM 用于 ASCII (Base64) 编码（指字符编码）的各种 X.509 v3 证书。文件开始由一行 "—–BEGIN …“开始。

CRT = 扩展名 CRT 用于证书。证书可以是 DER 编码，也可以是 PEM 编码。扩展名 CER 和 CRT 几乎是同义词。

x509是一个证书格式，证书的版本信息包含有：
    证书的序列号，每个证书都有一个唯一的证书序列号；
    证书所使用的签名算法；
    证书的发行机构名称，命名规则一般采用X.500格式；
    证书的有效期，现在通用的证书一般采用UTC时间格式，它的计时范围为1950-2049;
    证书所有人的名称，命名规则一般采用X.500格式；
    证书所有人的公开密钥；
    证书发行者对证书的签名。

X509使用ASN.1进行描述，可以抽象为以下结构
    ASN.1语法可以类比为C语言中的结构体互相嵌套，其主体结构如下图所示



1. 先生成自己的私钥(xxx.key), 
    echo "generate key file..."
    openssl genrsa -out $key_file_name $rsa_size
2. 然后生成证书的请求文件(xxx.csr, 内容主要是自己的公司信息，国家信息等内容),之后这个文件就没用了
    echo "generate csr file..."
    openssl req -new -out $csr_file_name -key $key_file_name -subj "$default_subject"
3. 最后拿这两个文件去机构申请证书(xxx.crt, 包括CA机构信息，自己公司信息，自己的公钥信息)
    echo "sign crt file with ca root..."
    openssl x509 -req -in $csr_file_name -out $crt_file_name -CA $root_ca_crt_file -CAkey $root_ca_key_file -CAcreateserial -days $key_remaining_days

    3.1 也可以生成 pkcs1~pkcs12 格式信息，包括自己的证书和私钥，最好使用密码加码(--passout 这样别人拿到文件，没有密码也使用不了), 这种格式一般是给客户生成证书使用的
        echo "generate p12 file..."
        openssl pkcs12 -export -clcerts -in $crt_file_name -inkey $key_file_name -out $p12_file_name --passout pass:$p12_install_password


