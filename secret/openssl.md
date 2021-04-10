
生成私钥 openssl genrsa -out private_key.pem 1024
根据私钥生成公钥文件 openssl rsa -in private_key.pem -pubout -out public_key.pem
