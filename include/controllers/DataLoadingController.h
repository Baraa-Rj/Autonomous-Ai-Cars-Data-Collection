#pragma once
#include <QObject>
#include <QString>

class DataManager;

// Single Responsibility: Handle data loading coordination and progress reporting
class DataLoadingController : public QObject {
    Q_OBJECT

public:
    explicit DataLoadingController(DataManager* dataManager, QObject* parent = nullptr);
    
    bool isLoading() const;

public slots:
    void loadData(const QString& dataDirectory);
    void cancelLoading();

signals:
    void loadingStarted();
    void loadingProgress(int percentage);
    void loadingFinished();
    void loadingError(const QString& error);

private slots:
    void onDataLoaded();
    void onDataLoadingProgress(int percentage);
    void onDataLoadingError(const QString& error);

private:
    DataManager* dataManager;
};