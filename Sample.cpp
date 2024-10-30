#include "stdafx.h"
#include "Sample.h"
#include <windows.h>

Sample::Sample(QWidget* parent) :
    QMainWindow(parent) {
    ui.setupUi(this);

    resize(1024, 600);

    connect(ui.actionOpenDevTools, &QAction::triggered, this, [this]() {
        ui.widgetWebView2->openDevToolsWindow();
    });

    connect(ui.actionOpenMP4Sample, &QAction::triggered, this, [this]() {
        setUrl("http://asserts.qwebview2.com/Mp4.html");
    });

    connect(ui.actionNewWindow, &QAction::triggered, this, [this]() {
        WebView2Options opts;
        opts.builtInErrorPageEnabled = false;

        QWebView2* view = new QWebView2(opts);
        view->setVirtualHostNameAndFolder({{"asserts.qwebview2.com", "asserts"}});
        view->show();
        view->setUrl(ui.lineEditUrl->text().trimmed());
    });

    connect(ui.actionShowWebView2RuntimeVersion, &QAction::triggered, this, [this]() {
        QString rtVersion;
        bool installed = QWebView2::GetWebView2RuntimeVersion(rtVersion);
        QString msg;
        if (installed)
            msg = QString("WebView2 Runtime %1").arg(rtVersion);
        else
            msg = "WebView2 Runtime Not Install";
        QMessageBox::information(this, "Webview2 Runtime", msg);
    });

    connect(ui.actionShowPerformanceInformation, &QAction::triggered, this, [this]() {
        ui.widgetWebView2->showPerformanceInfo();
    });

    connect(ui.actionCrashBrowserProcess, &QAction::triggered, this, [this]() {
        ui.widgetWebView2->crashBrowserProcess();
    });

    connect(ui.actionCrashRenderProcess, &QAction::triggered, this, [this]() {
        ui.widgetWebView2->crashRenderProcess();
    });

    connect(ui.actionOpenHotkeyListenSample, &QAction::triggered, this, [this]() {
        setUrl("http://asserts.qwebview2.com/Hotkey.html");
    });

    connect(ui.actionOpenScriptCallNativeSmaple, &QAction::triggered, this, [this]() {
        setUrl("http://asserts.qwebview2.com/ScriptCallNative.html");
    });

    connect(ui.actionOpenNonClientRegionSupportSample, &QAction::triggered, this, [this]() {
        setUrl("http://asserts.qwebview2.com/NonClientRegionSupport.html");
    });

    connect(ui.actionOpenFullscreenSample, &QAction::triggered, this, [this]() {
        setUrl("http://asserts.qwebview2.com/Fullscreen.html");
    });

    connect(ui.actionOpenTransparentTreeSample, &QAction::triggered, this, [this]() {
        WebView2Options opts;
        opts.backgroundColor = QColor(0, 0, 0, 0);

        QWebView2* view = new QWebView2(opts);
        view->setWindowFlag(Qt::FramelessWindowHint);
        view->setObjectName("webview2");
        view->setStyleSheet("#webview2 {background: transparent; border: none;}");
        view->setVirtualHostNameAndFolder({{"asserts.qwebview2.com", "asserts"}});
        view->show();
        view->setUrl("http://asserts.qwebview2.com/TransparentTree.html");
    });

    connect(ui.actionPostStringToWeb, &QAction::triggered, this, [this]() {
        gotoUrlAndDelayDo("http://asserts.qwebview2.com/NativeCallScript.html", [this]() {
            checkResult(ui.widgetWebView2->postStringToWeb(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
        });
    });

    connect(ui.actionPostJSONToWeb, &QAction::triggered, this, [this]() {
        gotoUrlAndDelayDo("http://asserts.qwebview2.com/NativeCallScript.html", [this]() {
            QJsonObject root;
            root["date"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

            QJsonDocument doc;
            doc.setObject(root);
            QString strJSON = doc.toJson(QJsonDocument::Compact);

            checkResult(ui.widgetWebView2->postJSONToWeb(strJSON));
        });
    });

    connect(ui.actionExecuteScriptWithResult, &QAction::triggered, this, [this]() {
        gotoUrlAndDelayDo("http://asserts.qwebview2.com/NativeCallScript.html", [this]() {
            HRESULT hr = ui.widgetWebView2->executeScriptWithResult("add(1,2,3)", [this](HRESULT hr, QString result, QString exception) {
                if (SUCCEEDED(hr)) {
                    QMessageBox::information(this, "QWebView2 Sample", QString("ExecuteScriptWithResult result: %1").arg(result));
                }
                else {
                    QMessageBox::critical(this,
                                          "QWebView2 Sample",
                                          QString("ExecuteScriptWithResult failed: 0x%1 (%2), %3")
                                              .arg(hr, 8, 16, QLatin1Char('0'))
                                              .arg(QString::fromStdWString(WinErrorCodeToString(hr)))
                                              .arg(exception));
                }
            });
            checkResult(hr);
        });
    });

    connect(ui.actionCleanupUserDataFolder, &QAction::triggered, this, [this](bool checked) {
        ui.widgetWebView2->setCleanupUserDataFolderEnabled(checked);
    });

    connect(ui.actionMute, &QAction::triggered, this, [this](bool checked) {
        ui.widgetWebView2->setMute(checked);
    });

    connect(ui.actionQueryAllCookies, &QAction::triggered, this, [this](bool checked) {
        ui.widgetWebView2->queryCookies("", [this](HRESULT hr, const QVector<WebView2Cookie>& cookieList) {
            QString msg;
            if (SUCCEEDED(hr)) {
                for (int i = 0; i < cookieList.size(); i++) {
                    msg.append(QString("Name: %1, Value: %2, Domain: %3, Path: %4\n")
                                   .arg(cookieList[i].name)
                                   .arg(cookieList[i].value)
                                   .arg(cookieList[i].domain)
                                   .arg(cookieList[i].path));
                }
                QMessageBox::information(this, "QWebView2 Sample - Cookies", msg);
            }
            else {
                checkResult(hr);
            }
        });
    });

    connect(ui.actionClearCookies, &QAction::triggered, this, [this](bool checked) {
        ui.widgetWebView2->clearCookies();
    });

    connect(ui.actionAddTestCookie, &QAction::triggered, this, [this](bool checked) {
        HRESULT hr = ui.widgetWebView2->addOrUpdateCookie("Test8888", "this is a test cookie.", "html5test.co", "/");
        if (SUCCEEDED(hr)) {
            QMessageBox::information(this,
                                     "QWebView2 Sample",
                                     "Add cookie success!\nname = Test8888\nvalue = this is a test cookie.\ndomain = html5test.co\npath = /");
        }
        else {
            checkResult(hr);
        }
    });

    connect(ui.actionAboutQt, &QAction::triggered, this, [this]() {
        QMessageBox::aboutQt(this);
    });

    connect(ui.actionAboutSourceCode, &QAction::triggered, this, [this]() {
        QMessageBox::information(this, "QWebView2 Sample", "Send an email to smart7178@163.com to get the source code of QWebView2.");
    });

    connect(ui.actionQWebView2Version, &QAction::triggered, this, [this]() {
        QMessageBox::information(this,
                                 "QWebView2 Sample",
                                 QString("QWebView2 version is %1.%2.%3")
                                     .arg(QWEBVIEW2_VERSION_MAJOR)
                                     .arg(QWEBVIEW2_VERSION_MINOR)
                                     .arg(QWEBVIEW2_VERSION_PATCH));
    });

    connect(ui.pushButtonBack, &QPushButton::clicked, this, [this]() {
        ui.widgetWebView2->goBack();
    });

    connect(ui.pushButtonForward, &QPushButton::clicked, this, [this]() {
        ui.widgetWebView2->goForward();
    });

    connect(ui.pushButtonReload, &QPushButton::clicked, this, [this]() {
        ui.widgetWebView2->reload();
    });

    connect(ui.pushButtonStop, &QPushButton::clicked, this, [this]() {
        ui.widgetWebView2->stopLoading();
    });

    connect(ui.pushButtonGo, &QPushButton::clicked, this, [this]() {
        ui.widgetWebView2->setUrl(ui.lineEditUrl->text().trimmed());
    });

    connect(ui.lineEditUrl, &QLineEdit::returnPressed, this, [this]() {
        ui.widgetWebView2->setUrl(ui.lineEditUrl->text().trimmed());
    });

    ui.widgetWebView2->setVirtualHostNameAndFolder({{"asserts.qwebview2.com", "asserts"}});
    ui.widgetWebView2->registerSciptCallNotify([this](ScriptCallMode callMode, int command, const QVariant& value, CallbackToScript cbScript) -> QVariant {
        QString msg = QString("JavaScipt call native code. \nScriptCallMode: %1, Command type: %2, Value: %3")
                          .arg((int)callMode)
                          .arg(command)
                          .arg(value.toString());
        QTimer::singleShot(20, [msg, this]() {
            QMessageBox::information(this, "JavaScript Call Native Code", msg);
        });
        return QVariant();
    });

    connect(ui.widgetWebView2, &QWebView2::loadUrlStarting, this, [this](QString url) {
        qDebug() << "Load" << url << "starting...";

        ui.pushButtonReload->setEnabled(false);
        ui.pushButtonStop->setEnabled(true);
    });

    connect(ui.widgetWebView2, &QWebView2::loadUrlFinished, this, [this](bool success, QString url, bool isCatastrophic, QString errStatus) {
        ui.pushButtonReload->setEnabled(true);
        ui.pushButtonStop->setEnabled(false);

        if (success) {
            qDebug() << "Load" << url << "success!";
        }
        else {
            qDebug() << "Load" << url << "failed, errorStatus =" << errStatus;
            if (isCatastrophic) {
                QMessageBox::critical(this, "Error", QString("Load url failed!\nurl = %1\nerrorStatus = %2").arg(url).arg(errStatus));
            }
        }
    });

    connect(ui.widgetWebView2, &QWebView2::errorOccurred, this, [this](long error) {
        QMessageBox::critical(this, "Error", QString("An error occurred! error: %1").arg(error));
    });

    connect(ui.widgetWebView2, &QWebView2::fullScreenChanged, this, [this](bool fullscreen) {
        if (fullscreen)
            this->showFullScreen();
        else
            this->showNormal();
    });

    connect(ui.widgetWebView2, &QWebView2::acceleratorKeyPressed, this, [this](unsigned int key) {
        if (key == VK_F12) {
            ui.widgetWebView2->openDevToolsWindow();
        }
        else if (GetKeyState(VK_CONTROL) < 0) {
            if (key == 'R') {
                ui.widgetWebView2->reload();
            }
        }
    });

    connect(ui.widgetWebView2, &QWebView2::childProcessCrashed, this,
            [this](QString kind, QString reason, int exitCode, QString processDescription, QString failedModule) {
                QMessageBox::critical(this,
                                      "Error",
                                      QString("QWebView2 child process crashed!\nkind = %1\nreason = %2\nexitCode = %3\nprocessDescription=%4\nfailedModule=%5")
                                          .arg(kind)
                                          .arg(reason)
                                          .arg(exitCode)
                                          .arg(processDescription)
                                          .arg(failedModule));
            });

    setUrl("https://html5test.co/");
}

void Sample::closeEvent(QCloseEvent* e) {
    if (QMessageBox::question(this, "Exit", "Continue Exit?") == QMessageBox::Yes) {
        e->accept();
    }
    else {
        e->ignore();
    }
}

void Sample::setUrl(QString url) {
    ui.lineEditUrl->setText(url);
    ui.widgetWebView2->setUrl(url);
}

void Sample::gotoUrlAndDelayDo(QString url, std::function<void()> fn) {
    if (ui.widgetWebView2->url() != url) {
        setUrl(url);
        QTimer::singleShot(200, [fn, this]() { if (fn) { fn(); } });
    }
    else {
        if (fn)
            fn();
    }
}

std::wstring Sample::WinErrorCodeToString(long errCode) {
    std::wstring result;
    LPWSTR buffer = NULL;
    DWORD dwRet = FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&buffer,
        NULL,
        NULL);

    if (dwRet > 0) {
        if (buffer) {
            result = buffer;
            LocalFree(buffer);
        }
    }

    return result;
}

void Sample::checkResult(HRESULT hr) {
    if (FAILED(hr)) {
        QMessageBox::critical(this,
                              "QWebView2 Sample",
                              QString("Error: 0x%1, %2")
                                  .arg(hr, 8, 16, QLatin1Char('0'))
                                  .arg(QString::fromStdWString(WinErrorCodeToString(hr))));
    }
}
