# install

## 修改模块下载路径
mkdir ~/.npm-global
npm config set prefix '~/.npm-global'
vim .zshrc ## export PATH=~/.npm-global/bin:$PATH
