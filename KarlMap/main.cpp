#include "mapwidget.h"
#include <QApplication>
#include <QWebEngineView>

int main(int argc, char *argv[])
{

    char ARG_DISABLE_WEB_SECURITY[] = "--disable-web-security";
    char ARG_DISABLE_SECCOMP_SECURITY[] = "--disable-seccomp-filter-sandbox";

    int newArgc = argc+1+1+1;// ARG_DISABLE_WEB_SECURITY + ARG_DISABLE_SECCOMP_SECURITY + nullptr
    char** newArgv = new char*[newArgc];
    for(int i=0; i<argc; i++) {
    newArgv[i] = argv[i];
    }
    newArgv[argc] = ARG_DISABLE_WEB_SECURITY;
    newArgv[argc+1] = ARG_DISABLE_SECCOMP_SECURITY;
    newArgv[argc+2] = nullptr;
    QApplication app(newArgc, newArgv);

    mapWidget view;
//    QWebEngineView view;


//    // 指定本地HTML文件路径
//    QString localFilePath = "file:///home/alansp/KarlPlay/KarlMap/map.html"; // 请替换为你实际的HTML文件路径
//    view.load(QUrl(localFilePath));

    view.show();

    return app.exec();
}
