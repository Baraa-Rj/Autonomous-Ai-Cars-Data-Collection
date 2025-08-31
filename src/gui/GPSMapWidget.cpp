#include "gui/GPSMapWidget.h"
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtMath>
#include <QUrl>
#include <iostream>
#include <cmath>
#include <algorithm>

GPSMapWidget::GPSMapWidget(QWidget *parent)
    : QWidget(parent)
    , layout(new QVBoxLayout(this))
    , infoLabel(new QLabel("GPS Map - Loading tiles...", this))
    , networkManager(new QNetworkAccessManager(this))
    , tileRequestTimer(new QTimer(this))
    , hasCurrentPosition(false)
    , followMode(true)
    , centerLat(32.825)
    , centerLon(34.989)
    , zoomLevel(16)
    , mapWidth(400)
    , mapHeight(300)
    , dragging(false)
{
    setMinimumSize(400, 350);
    setStyleSheet("QWidget { background-color: white; border: 1px solid #ccc; }");
    
    infoLabel->setStyleSheet("QLabel { background-color: rgba(255,255,255,0.9); padding: 5px; border-radius: 3px; font-size: 11px; }");
    infoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    infoLabel->setMaximumHeight(80);
    
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addStretch();
    layout->addWidget(infoLabel);
    
    currentPosition = {0, 0, 0, 0};
    
    tileRequestTimer->setSingleShot(true);
    tileRequestTimer->setInterval(100);
    connect(tileRequestTimer, &QTimer::timeout, this, &GPSMapWidget::requestVisibleTiles);
    
    tileRequestTimer->start();
    
}

void GPSMapWidget::updateGPSPosition(const GPSData* gpsData) {
    if (!gpsData) {
        std::cerr << "Warning: updateGPSPosition called with null GPS data" << std::endl;
        return;
    }
    
    if (std::isnan(gpsData->latitude) || std::isnan(gpsData->longitude) || 
        std::isinf(gpsData->latitude) || std::isinf(gpsData->longitude)) {
        std::cerr << "Warning: Invalid GPS coordinates: lat=" << gpsData->latitude 
                  << ", lon=" << gpsData->longitude << std::endl;
        return;
    }
    
    if (gpsData->latitude < -90.0 || gpsData->latitude > 90.0 || 
        gpsData->longitude < -180.0 || gpsData->longitude > 180.0) {
        std::cerr << "Warning: GPS coordinates out of valid range: lat=" << gpsData->latitude 
                  << ", lon=" << gpsData->longitude << std::endl;
        return;
    }
    
    currentPosition.latitude = gpsData->latitude;
    currentPosition.longitude = gpsData->longitude;
    currentPosition.altitude = gpsData->height;
    currentPosition.timestamp = gpsData->timestamp;
    hasCurrentPosition = true;
    
    {
        std::lock_guard<std::mutex> lock(trajectoryMutex);
        trajectoryPoints.push_back(currentPosition);
        
        if (trajectoryPoints.size() > 1000) {
            trajectoryPoints.erase(trajectoryPoints.begin());
        }
    }
    
    if (infoLabel) {
        size_t pointCount;
        {
            std::lock_guard<std::mutex> lock(trajectoryMutex);
            pointCount = trajectoryPoints.size();
        }
        QString info = QString("GPS Position\\nLat: %1\\nLon: %2\\nAlt: %3m\\nPoints: %4\\nZoom: %5")
            .arg(gpsData->latitude, 0, 'f', 6)
            .arg(gpsData->longitude, 0, 'f', 6)
            .arg(gpsData->height, 0, 'f', 1)
            .arg(pointCount)
            .arg(zoomLevel);
        infoLabel->setText(info);
    }
    
    if (followMode) {
        double oldCenterLat = centerLat;
        double oldCenterLon = centerLon;
        centerLat = currentPosition.latitude;
        centerLon = currentPosition.longitude;
        
        
        tileRequestTimer->start(); 
    }
    
    update(); 
}


void GPSMapWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    mapWidth = width() - 10;
    mapHeight = height() - 90; 
    
    painter.fillRect(5, 5, mapWidth, mapHeight, QColor(240, 240, 240));
    
    drawMapTiles(painter);
    drawTrajectory(painter);
    drawCurrentPosition(painter);
}

void GPSMapWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        lastPanPoint = event->pos();
        followMode = false; 
    }
}

void GPSMapWidget::mouseMoveEvent(QMouseEvent* event) {
    if (dragging) {
        QPoint delta = event->pos() - lastPanPoint;
        
        double scale = pow(2, zoomLevel);
        double latOffset = -delta.y() * 360.0 / (256.0 * scale);
        double lonOffset = -delta.x() * 360.0 / (256.0 * scale);
        
        centerLat += latOffset;
        centerLon += lonOffset;
        
        centerLat = qBound(-85.0, centerLat, 85.0);
        centerLon = fmod(centerLon + 180.0, 360.0) - 180.0;
        
        lastPanPoint = event->pos();
        tileRequestTimer->start();
        update();
    }
}

void GPSMapWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
    }
}

void GPSMapWidget::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0 && zoomLevel < 18) {
        zoomLevel++;
    } else if (event->angleDelta().y() < 0 && zoomLevel > 1) {
        zoomLevel--;
    }
    
    tileRequestTimer->start();
    update();
}

void GPSMapWidget::requestVisibleTiles() {
    const int tileSize = 256;
    const int tilesX = qMin((mapWidth / tileSize) + 2, 10); 
    const int tilesY = qMin((mapHeight / tileSize) + 2, 10); 
    
    int centerTileX = long2tilex(centerLon, zoomLevel);
    int centerTileY = lat2tiley(centerLat, zoomLevel);
    
    
    int maxTile = (1 << zoomLevel) - 1;
    if (centerTileX < 0 || centerTileX > maxTile || centerTileY < 0 || centerTileY > maxTile) {
        return;
    }
    
    for (int dx = -tilesX/2; dx <= tilesX/2; dx++) {
        for (int dy = -tilesY/2; dy <= tilesY/2; dy++) {
            int tileX = centerTileX + dx;
            int tileY = centerTileY + dy;
            
            if (tileX < 0 || tileX > maxTile || tileY < 0 || tileY > maxTile) {
                continue;
            }
            
            QString tileKey = QString("%1_%2_%3").arg(tileX).arg(tileY).arg(zoomLevel);
            
            if (!tileCache.contains(tileKey) && !pendingTiles.contains(tileKey)) {
                downloadTile(tileX, tileY, zoomLevel);
            }
        }
    }
}

void GPSMapWidget::downloadTile(int x, int y, int z) {
    QString tileKey = QString("%1_%2_%3").arg(x).arg(y).arg(z);
    QString url = getTileUrl(x, y, z);
    
    
    QNetworkRequest request{QUrl(url)};
    request.setRawHeader("User-Agent", "CarVisualizationApp/1.0");
    
    QNetworkReply* reply = networkManager->get(request);
    pendingTiles[tileKey] = reply;
    
    connect(reply, &QNetworkReply::finished, this, &GPSMapWidget::onTileDownloaded);
}

void GPSMapWidget::onTileDownloaded() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        std::cerr << "Warning: onTileDownloaded called with null reply" << std::endl;
        return;
    }
    
    QString tileKey;
    bool found = false;
    
    for (auto it = pendingTiles.begin(); it != pendingTiles.end(); ++it) {
        if (it.value() == reply) {
            tileKey = it.key();
            pendingTiles.erase(it);
            found = true;
            break;
        }
    }
    
    if (!found) {
        std::cerr << "Warning: Could not find tile key for completed download" << std::endl;
        reply->deleteLater();
        return;
    }
    
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        if (!data.isEmpty()) {
            QPixmap pixmap;
            if (pixmap.loadFromData(data)) {
                tileCache[tileKey] = pixmap;
                update(); 
            } else {
                std::cerr << "Warning: Failed to load pixmap from tile data" << std::endl;
            }
        }
    } else {
        std::cerr << "Network error downloading tile: " << reply->errorString().toStdString() << std::endl;
    }
    
    reply->deleteLater();
}

void GPSMapWidget::drawMapTiles(QPainter& painter) {
    const int tileSize = 256;
    const int tilesX = qMin((mapWidth / tileSize) + 2, 10); 
    const int tilesY = qMin((mapHeight / tileSize) + 2, 10); 
    
    int centerTileX = long2tilex(centerLon, zoomLevel);
    int centerTileY = lat2tiley(centerLat, zoomLevel);
    
    int maxTile = (1 << zoomLevel) - 1;
    if (centerTileX < 0 || centerTileX > maxTile || centerTileY < 0 || centerTileY > maxTile) {
        return; 
    }
    
    double exactTileX = (centerLon + 180.0) / 360.0 * (1 << zoomLevel);
    double exactTileY = (1.0 - log(tan(centerLat * M_PI / 180.0) + 1.0 / cos(centerLat * M_PI / 180.0)) / M_PI) / 2.0 * (1 << zoomLevel);
    
    double exactPixelX = exactTileX * tileSize;
    double exactPixelY = exactTileY * tileSize;
    
    int offsetX = mapWidth / 2 - (exactPixelX - centerTileX * tileSize);
    int offsetY = mapHeight / 2 - (exactPixelY - centerTileY * tileSize);
    
    static int offsetDebugCounter = 0;
    if (++offsetDebugCounter % 50 == 0) {
        std::cout << "Map offset: (" << offsetX << ", " << offsetY << ") for center (" << centerLat << ", " << centerLon << ")" << std::endl;
    }
    
    for (int dx = -tilesX/2; dx <= tilesX/2; dx++) {
        for (int dy = -tilesY/2; dy <= tilesY/2; dy++) {
            int tileX = centerTileX + dx;
            int tileY = centerTileY + dy;
            
            QString tileKey = QString("%1_%2_%3").arg(tileX).arg(tileY).arg(zoomLevel);
            
            if (tileCache.contains(tileKey)) {
                QPixmap tile = tileCache[tileKey];
                int drawX = 5 + offsetX + dx * tileSize;
                int drawY = 5 + offsetY + dy * tileSize;
                
                if (drawX < mapWidth + 5 && drawY < mapHeight + 5 && 
                    drawX + tileSize > 5 && drawY + tileSize > 5) {
                    painter.drawPixmap(drawX, drawY, tile);
                }
            }
        }
    }
}

void GPSMapWidget::drawTrajectory(QPainter& painter) {
    std::vector<GPSPoint> localTrajectory;
    {
        std::lock_guard<std::mutex> lock(trajectoryMutex);
        if (trajectoryPoints.size() < 2) return;
        localTrajectory = trajectoryPoints; 
    }
    
    painter.save();
    painter.setPen(QPen(QColor(0, 150, 255), 2));
    
    for (size_t i = 1; i < localTrajectory.size(); ++i) {
        QPoint p1 = gpsToPixel(localTrajectory[i-1].latitude, localTrajectory[i-1].longitude);
        QPoint p2 = gpsToPixel(localTrajectory[i].latitude, localTrajectory[i].longitude);
        
        if (abs(p1.x()) > 10000 || abs(p1.y()) > 10000 || abs(p2.x()) > 10000 || abs(p2.y()) > 10000) {
            continue;
        }
        
        if (p1.x() >= 5 && p1.x() <= mapWidth + 5 && p1.y() >= 5 && p1.y() <= mapHeight + 5 &&
            p2.x() >= 5 && p2.x() <= mapWidth + 5 && p2.y() >= 5 && p2.y() <= mapHeight + 5) {
            painter.drawLine(p1, p2);
        }
    }
    
    painter.restore();
}

void GPSMapWidget::drawCurrentPosition(QPainter& painter) {
    
    if (!hasCurrentPosition) return;
    
    if (currentPosition.latitude == 0.0 && currentPosition.longitude == 0.0) {
        return; 
    }
    
    if (std::isnan(currentPosition.latitude) || std::isnan(currentPosition.longitude) ||
        std::isinf(currentPosition.latitude) || std::isinf(currentPosition.longitude)) {
        std::cerr << "Warning: Current position has invalid coordinates" << std::endl;
        return;
    }
    
    QPoint carPos = gpsToPixel(currentPosition.latitude, currentPosition.longitude);
    
    static int debugCounter = 0;
    if (++debugCounter % 100 == 0) { 
        std::cout << "Car at GPS(" << currentPosition.latitude << ", " << currentPosition.longitude 
                  << ") -> Pixel(" << carPos.x() << ", " << carPos.y() << ")" << std::endl;
    }
    
    if (abs(carPos.x()) > 10000 || abs(carPos.y()) > 10000) {
        std::cout << "WARNING: Car position coordinates too large, skipping draw" << std::endl;
        return;
    }
    
    if (carPos.x() < -50 || carPos.x() > mapWidth + 50 || carPos.y() < -50 || carPos.y() > mapHeight + 50) {
        return;
    }
    
    painter.save();
    
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    painter.setBrush(QBrush(QColor(255, 0, 0)));
    painter.setPen(QPen(QColor(255, 255, 255), 1));
    
    int circleX = std::max(1, std::min(mapWidth - 8, carPos.x() - 4));
    int circleY = std::max(1, std::min(mapHeight - 8, carPos.y() - 4));
    painter.drawEllipse(circleX, circleY, 8, 8);
    
    painter.setPen(QPen(QColor(255, 255, 255), 1));
    int lineStartX = std::max(1, std::min(mapWidth - 1, carPos.x()));
    int lineStartY = std::max(5, std::min(mapHeight - 1, carPos.y() - 4));
    int lineEndY = std::max(1, std::min(mapHeight - 1, carPos.y() - 8));
    painter.drawLine(lineStartX, lineStartY, lineStartX, lineEndY);
    
    painter.restore();
}

QPoint GPSMapWidget::gpsToPixel(double lat, double lon) const {
    if (std::isnan(lat) || std::isnan(lon) || std::isinf(lat) || std::isinf(lon)) {
        std::cerr << "Warning: Invalid GPS coordinates (NaN/Inf): lat=" << lat << ", lon=" << lon << std::endl;
        return QPoint(mapWidth / 2, mapHeight / 2);
    }
    
    if (lat < -90.0 || lat > 90.0 || lon < -180.0 || lon > 180.0) {
        std::cerr << "Warning: GPS coordinates out of valid range: lat=" << lat << ", lon=" << lon << std::endl;
        return QPoint(mapWidth / 2, mapHeight / 2);
    }
    
    double latDiff = lat - centerLat;
    double lonDiff = lon - centerLon;
    
    double pixelsPerDegree = pow(2, zoomLevel - 10) * 1000; // Adjust the scale
    
    double exactPixelX = mapWidth / 2.0 + (lonDiff * pixelsPerDegree) + 5.0;
    double exactPixelY = mapHeight / 2.0 - (latDiff * pixelsPerDegree) + 5.0;
    
    int pixelX = std::max(-50000, std::min(50000, (int)exactPixelX));
    int pixelY = std::max(-50000, std::min(50000, (int)exactPixelY));
    
    return QPoint(pixelX, pixelY);
}

QString GPSMapWidget::getTileUrl(int x, int y, int z) const {
    return QString("https://tile.openstreetmap.org/%1/%2/%3.png").arg(z).arg(x).arg(y);
}

int GPSMapWidget::long2tilex(double lon, int z) const {
    return (int)(floor((lon + 180.0) / 360.0 * (1 << z)));
}

int GPSMapWidget::lat2tiley(double lat, int z) const {
    double latrad = lat * M_PI / 180.0;
    return (int)(floor((1.0 - asinh(tan(latrad)) / M_PI) / 2.0 * (1 << z)));
}

