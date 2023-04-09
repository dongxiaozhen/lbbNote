package main

import (
	"context"
	"fmt"
	"time"

	corev1 "k8s.io/api/core/v1"
	metav1 "k8s.io/apimachinery/pkg/apis/meta/v1"
	"k8s.io/apimachinery/pkg/labels"
	"k8s.io/apimachinery/pkg/runtime"
	"k8s.io/apimachinery/pkg/runtime/schema"
	"k8s.io/client-go/discovery"
	"k8s.io/client-go/discovery/cached/disk"
	"k8s.io/client-go/dynamic"
	"k8s.io/client-go/informers"
	"k8s.io/client-go/kubernetes"
	"k8s.io/client-go/kubernetes/scheme"
	"k8s.io/client-go/rest"
	"k8s.io/client-go/tools/clientcmd"
)

func main() {
	// mainClientSet()
	// mainDynamicSet()
	// mainDiscoveryClient()
	// mainCacheClient()
	// mainListWatch()
	mainSharedInformer()
}

func mainRestClient() {
	config, err := clientcmd.BuildConfigFromFlags("", "./kubeconfig")
	if err != nil {
		panic(err)
	}
	config.APIPath = "api"
	config.GroupVersion = &corev1.SchemeGroupVersion
	config.NegotiatedSerializer = scheme.Codecs

	restClient, err := rest.RESTClientFor(config)
	if err != nil {
		panic(err)
	}

	result := &corev1.PodList{}

	err = restClient.Get().Namespace("kube-system").Resource("pods").VersionedParams(&metav1.ListOptions{}, scheme.ParameterCodec).Do(context.TODO()).Into(result)
	if err != nil {
		panic(err)
	}

	for _, item := range result.Items {
		fmt.Println(item.Name, item.Kind)
	}

}

func mainClientSet() {
	config, err := clientcmd.BuildConfigFromFlags("", "./kubeconfig")
	if err != nil {
		panic(err)
	}

	clientSet, err := kubernetes.NewForConfig(config)
	if err != nil {
		panic(err)
	}

	pods, err := clientSet.CoreV1().Pods("kube-system").List(context.Background(), metav1.ListOptions{})
	// pods, err := clientSet.DiscoveryV1().EndpointSlices("kube-system").List(context.Background(), metav1.ListOptions{})
	if err != nil {
		panic(err)
	}

	for _, item := range pods.Items {
		fmt.Println(item.Name, item.Kind)
	}

}

func mainDynamicSet() {
	config, err := clientcmd.BuildConfigFromFlags("", "./kubeconfig")
	if err != nil {
		panic(err)
	}

	dynamicClient, err := dynamic.NewForConfig(config)
	if err != nil {
		panic(err)
	}

	gvr := schema.GroupVersionResource{
		Group:    "",
		Version:  "v1",
		Resource: "pods",
	}

	unStructData, err := dynamicClient.Resource(gvr).Namespace("").List(context.Background(), metav1.ListOptions{})
	if err != nil {
		panic(err)
	}

	podList := &corev1.PodList{}
	err = runtime.DefaultUnstructuredConverter.FromUnstructured(unStructData.UnstructuredContent(), podList)
	if err != nil {
		panic(err)
	}
	for _, item := range podList.Items {
		fmt.Println(item.Name, item.Kind)
	}

}

func mainDiscoveryClient() {
	config, err := clientcmd.BuildConfigFromFlags("", "./kubeconfig")
	if err != nil {
		panic(err)
	}

	discoveryClient, err := discovery.NewDiscoveryClientForConfig(config)
	if err != nil {
		panic(err)
	}

	groupList, apiResources, err := discoveryClient.ServerGroupsAndResources()
	if err != nil {
		panic(err)
	}
	for _, group := range groupList {
		fmt.Println(group.Name, group.Kind, group.APIVersion)
	}

	fmt.Println("----------")
	for _, list := range apiResources {
		gv, err := schema.ParseGroupVersion(list.GroupVersion)
		if err != nil {
			panic(err)
		}
		for _, resource := range list.APIResources {
			fmt.Println(resource.Name, "--group", gv.Group, "---version", gv.Version)
		}
	}
}

func mainCacheClient() {
	config, err := clientcmd.BuildConfigFromFlags("", "./kubeconfig")
	if err != nil {
		panic(err)
	}

	cacheClient, err := disk.NewCachedDiscoveryClientForConfig(config, "./cache/discovery", "./cache/http", time.Minute*68)
	if err != nil {
		panic(err)
	}

	groupList, apiResources, err := cacheClient.ServerGroupsAndResources()
	if err != nil {
		panic(err)
	}
	for _, group := range groupList {
		fmt.Println(group.Name, group.Kind, group.APIVersion)
	}

	fmt.Println("----------")
	for _, list := range apiResources {
		gv, err := schema.ParseGroupVersion(list.GroupVersion)
		if err != nil {
			panic(err)
		}
		for _, resource := range list.APIResources {
			fmt.Println(resource.Name, "--group", gv.Group, "---version", gv.Version)
		}
	}
}

func mainListWatch() {
	config, err := clientcmd.BuildConfigFromFlags("", "./kubeconfig")
	if err != nil {
		panic(err)
	}

	clientSet, err := kubernetes.NewForConfig(config)
	if err != nil {
		panic(err)
	}

	wtch, err := clientSet.AppsV1().Deployments("").Watch(context.Background(), metav1.ListOptions{})
	if err != nil {
		panic(err)
	}

	for {
		select {
		case e, _ := <-wtch.ResultChan():
			fmt.Println(e.Type, e.Object)
		}
	}

}

func mainSharedInformer() {
	config, err := clientcmd.BuildConfigFromFlags("", "./kubeconfig")
	if err != nil {
		panic(err)
	}

	clientSet, err := kubernetes.NewForConfig(config)
	if err != nil {
		panic(err)
	}

	sharedInformerFactory := informers.NewSharedInformerFactory(clientSet, 0)

	podInformer := sharedInformerFactory.Core().V1().Pods()
	// indexer 必须放在这里
	indexer := podInformer.Lister()

	sharedInformerFactory.Start(nil)

	sharedInformerFactory.WaitForCacheSync(nil)

	pods, err := indexer.List(labels.Everything())
	if err != nil {
		panic(err)
	}

	for _, items := range pods {
		fmt.Println(items.Name, items.Namespace)
	}

}
