#ifndef QWEBVIEW2_H_
#define QWEBVIEW2_H_
#if defined(_MSVC_) && _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#include <QWidget>
#include "qwebview2_global.h"
#include "QWebView2Options.h"

class QWebView2Impl;
class QWEBVIEW2_EXPORT QWebView2 : public QWidget {
    Q_OBJECT
   public:
    QWebView2(QWidget* parent = Q_NULLPTR);
    QWebView2(const WebView2Options& opts, QWidget* parent = Q_NULLPTR);
    virtual ~QWebView2();

    static bool GetWebView2RuntimeVersion(QString& version);

   public Q_SLOTS:
    void openDevToolsWindow();

    void setUrl(const QString& url);

    QString url() const;

    void reload();

    void goBack();

    void goForward();

    void stopLoading();

    void setVirtualHostNameAndFolder(const QMap<QString, QString>& m);

    void registerSciptCallNotify(ScriptCallCallback cb);

    HRESULT postStringToWeb(const QString& str);

    HRESULT postJSONToWeb(const QString& strJSON);

    // Require ICoreWebView2_21 that introduced with WebView2 1.0.2277.86
    HRESULT executeScriptWithResult(const QString& s, ScriptResultCallback cb);

    void setNonClientRegionSupportEnabled(bool enabled);

    void setUserDataFolder(const QString& folder);

    void setCleanupUserDataFolderEnabled(bool enabled);

    bool isMute() const;

    void setMute(bool mute);

    void queryCookies(const QString& uri, QueryCookieCallback cb);

    void clearCookies();

    void showPerformanceInfo();

    void crashBrowserProcess();

    void crashRenderProcess();

    HRESULT addOrUpdateCookie(const QString& name, const QString& value, const QString& domain, const QString& path);

    WebView2Options options() const;

   Q_SIGNALS:
    void errorOccurred(long error);

    void loadUrlStarting(QString url);

    // errStatus: convert COREWEBVIEW2_WEB_ERROR_STATUS enum value to string.
    //
    void loadUrlFinished(bool success, QString url, QString errStatus);

    void fullScreenChanged(bool fullscreen);

    void profileDeleted();

    void acceleratorKeyPressed(unsigned int key);

    void childProcessCrashed(QString kind, QString reason, int exitCode, QString processDescription, QString failedModule);

   protected:
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

   protected:
    QWebView2Impl* d_ = Q_NULLPTR;
};

#endif  // !QWEBVIEW2_H_
