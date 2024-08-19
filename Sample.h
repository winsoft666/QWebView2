#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Sample.h"

class Sample : public QMainWindow {
    Q_OBJECT

   public:
    Sample(QWidget* parent = nullptr);

   protected:
    void closeEvent(QCloseEvent* e) override;

   protected:
    void setUrl(QString url);
    void gotoUrlAndDelayDo(QString url, std::function<void()> fn);

    std::wstring WinErrorCodeToString(long errCode);
    void checkResult(HRESULT hr);
   private:
    Ui::SampleClass ui;
};
