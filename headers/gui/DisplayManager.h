#pragma once

#include <QWidget>

class DisplayManager : public QWidget {
    Q_OBJECT

public:
    explicit DisplayManager(QWidget* parent = nullptr);
    ~DisplayManager() override = default;
};


