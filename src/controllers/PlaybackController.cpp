#include "controllers/PlaybackController.h"
#include "management/ClockManager.h"
#include <QtCore/QMetaObject>

PlaybackController::PlaybackController(ClockManager* clockManager, QObject* parent)
    : QObject(parent)
    , clockManager(clockManager)
    , playing(false)
    , playbackSpeed(1.0)
{
}

void PlaybackController::play() {
    if (playing || !clockManager) return;
    
    clockManager->setPlaybackSpeed(playbackSpeed);
    clockManager->startTiming([this]() {
        // Thread-safe callback to main thread
        QMetaObject::invokeMethod(this, "onTimingUpdate", Qt::QueuedConnection);
    }, 33); // ~30 FPS
    
    playing = true;
    emit playbackStarted();
}

void PlaybackController::pause() {
    if (!playing || !clockManager) return;
    
    clockManager->stopTiming();
    playing = false;
    emit playbackPaused();
}

void PlaybackController::togglePlayPause() {
    if (playing) {
        pause();
    } else {
        play();
    }
}

void PlaybackController::seekToPosition(double timestamp) {
    if (!clockManager) return;
    
    clockManager->setCurrentTimestamp(timestamp);
    emit positionChanged(timestamp);
}

void PlaybackController::onTimingUpdate() {
    if (!playing || !clockManager) return;
    
    double currentTime = clockManager->getCurrentTimestamp();
    
    // Check if timing has stopped due to reaching the end
    if (!clockManager->isTimingActive()) {
        playing = false;
        emit playbackEnded();
        return;
    }
    
    emit positionChanged(currentTime);
}