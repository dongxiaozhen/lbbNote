helm是k8s的包管理工具，使用helm，可以使用更为简化和系统化的方式对k8s应用进行部署、升级。


部署前

    repo: add, list, remove, update, and index chart repositories
    search: search for a keyword in charts

部署后

    install: install a chart
    list: list releases
    status: display the status of the named release
    upgrade: upgrade a release
    rollback: roll back a release to a previous revision
    uninstall: uninstall a release

自己开发chart

    lint: examine a chart for possible issues
    package: package a chart directory into a chart archive
    push: push helm chart to chartmuseum
    chart push: push helm chart to OCI repository
