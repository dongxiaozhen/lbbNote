package main

import (
	"bytes"
	"encoding/json"
	"flag"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"path/filepath"
	"strings"

	yaml "gopkg.in/yaml.v2"
)

var flagConfig = flag.String("config", "./config/", "生成使用的配置文件")
var flagExample = flag.String("profiles", "./profiles/", "替换生成的模板文件.")
var flagOutPath = flag.String("out", "~/Library/Application Support/iTerm2/DynamicProfiles", "生成文件输出目录.")

type buildConfig struct {
	Basic    map[string]string   // 基础配置
	Services []map[string]string //覆盖配置
}

func main() {
	flag.Parse()
	filename := *flagConfig
	profileName := *flagExample

	if IsDir(filename) {
		files, failed := GetAllFileWithExt(filename, ".yaml")
		if failed {
			log.Fatalln("读取目录", filename, "错误")
		}
		if IsDir(profileName) {
			for _, v := range files {
				parse(v, profileName+"/"+strings.Replace(filepath.Base(v), ".yaml", ".json", 1))
			}
		} else {
			for _, v := range files {
				parse(v, profileName)
			}
		}
	} else {
		if IsDir(profileName) {
			log.Fatalln("不支持的选项.")
		} else {
			parse(filename, profileName)
		}
	}

}

func parse(filename, profileName string) {
	config := &buildConfig{}
	data, err := ioutil.ReadFile(filename)
	if err != nil {
		log.Fatalln("Read ", filename, " Error.", err)
	}
	// 解析结构体
	err = yaml.Unmarshal(data, &config)
	if err != nil {
		log.Fatalln("Unmarshal ", filename, " Error.", err)
	}
	profileData, err := ioutil.ReadFile(profileName)
	if err != nil {
		log.Fatalln("Read ", profileName, " Error.", err)
	}

	buf := bytes.Buffer{}
	buf.WriteString(`{
	"Profiles": [`)

	for k, v := range config.Services {
		basic := make(map[string]string) // 基础配置
		for sk, sv := range config.Basic {
			basic[sk] = sv
		}
		for sk, sv := range v {
			basic[sk] = sv
		}
		strConfig := string(profileData)
		for sk, sv := range basic {
			strConfig = strings.ReplaceAll(strConfig, "$"+sk, sv)
		}
		strConfig = strings.ReplaceAll(strConfig, "$index", fmt.Sprintf("%d", k+1))
		if k > 0 {
			buf.WriteString(`,
	`)
		}
		buf.WriteString(strConfig)
	}
	buf.WriteString(`	]
}`)

	var v interface{}
	var newData []byte
	err = json.Unmarshal(buf.Bytes(), v)
	if err == nil {
		newData, err = json.MarshalIndent(v, "", "\t")
		if err == nil {
			ioutil.WriteFile(*flagOutPath+"/"+strings.Replace(filepath.Base(filename), ".yaml", ".json", 1),
				newData, 0644)
			return
		}
	}
	dst := &bytes.Buffer{}
	err = json.Indent(dst, buf.Bytes(), "", "\t")
	if err == nil {
		ioutil.WriteFile(*flagOutPath+"/"+strings.Replace(filepath.Base(filename), ".yaml", ".json", 1),
			dst.Bytes(), 0644)
		return
	}

	log.Println("Error: ", err)
	ioutil.WriteFile(*flagOutPath+"/"+strings.Replace(filepath.Base(filename), ".yaml", ".json", 1),
		buf.Bytes(), 0644)
}

// IsDir 判断所给路径是否为文件夹
func IsDir(path string) bool {
	s, err := os.Stat(path)
	if err != nil {
		return false
	}
	return s.IsDir()
}

// IsFile 判断所给路径是否为文件
func IsFile(path string) bool {
	return !IsDir(path)
}

// GetAllFileWithExt 获取某个目录下所有ext后缀的文件
func GetAllFileWithExt(path, ext string) (files []string, failed bool) {
	return GetAllFile(path, func(file string) bool {
		if ext == "*" {
			return false
		}
		return filepath.Ext(file) != ext
	})
}

// GetAllFile 获取某个目录下所有文件
func GetAllFile(path string, filter func(string) bool) (files []string, failed bool) {
	if !IsDir(path) {
		return
	}
	rd, err := ioutil.ReadDir(path)
	if err != nil {
		log.Println("Error: 打开目录 ", path, " 出错.")
		failed = true
		return
	}
	for _, fi := range rd {
		if fi.IsDir() {
			// fmt.Printf("[%s]\n", path+"/"+fi.Name())
			nfs, failure := GetAllFile(path+fi.Name()+"/", filter)
			if !failure && len(nfs) > 1 {
				files = append(files, nfs...)
			}
		} else {
			if filter(fi.Name()) {
				continue
			}
			files = append(files, path+fi.Name())
			// fmt.Println(fi.Name())
		}
	}
	return
}
