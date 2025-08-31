#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QTimer>
#include <QHash>
#include <vector>
#include <cmath>
#include "data/GPSData.h"

class GPSMapWidget : public QWidget {
    Q_OBJECT

public:
    explicit GPSMapWidget(QWidget *parent = nullptr);
    
    void updateGPSPosition(const GPSData* gpsData);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private slots:
    void onTileDownloaded();

private:
    QVBoxLayout* layout;
    QLabel* infoLabel;
    QNetworkAccessManager* networkManager;
    QTimer* tileRequestTimer;
    
    // GPS tracking data
    struct GPSPoint {
        double latitude;
        double longitude;
        double altitude;
        double timestamp;
    };
    std::vector<GPSPoint> trajectoryPoints;
    GPSPoint currentPosition;
    bool hasCurrentPosition;
    bool followMode;
    
    // Map view parameters
    double centerLat;
    double centerLon;
    int zoomLevel;
    int mapWidth, mapHeight;
    
    // Tile management
    struct TileKey {
        int x, y, z;
        bool operator==(const TileKey& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };
    
    QHash<QString, QPixmap> tileCache;
    QHash<QString, QNetworkReply*> pendingTiles;
    
    // Mouse interaction
    bool dragging;
    QPoint lastPanPoint;
    
    // Coordinate conversion functions
    QPoint gpsToPixel(double lat, double lon) const;
    QString getTileUrl(int x, int y, int z) const;
    
    // Rendering functions
    void drawMapTiles(QPainter& painter);
    void drawTrajectory(QPainter& painter);
    void drawCurrentPosition(QPainter& painter);
    void requestVisibleTiles();
    void downloadTile(int x, int y, int z);
    
    // Tile coordinate calculation
    int long2tilex(double lon, int z) const;
    int lat2tiley(double lat, int z) const;
};