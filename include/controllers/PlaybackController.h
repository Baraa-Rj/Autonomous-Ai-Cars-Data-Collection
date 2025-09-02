#pragma once
#include <QObject>
#include <functional>

class ClockManager;

class PlaybackController : public QObject {
    Q_OBJECT

public:
    explicit PlaybackController(ClockManager* clockManager, QObject* parent = nullptr);
    
    bool isPlaying() const { return playing; }
    double getPlaybackSpeed() const { return playbackSpeed; }
    void setPlaybackSpeed(double speed) { playbackSpeed = speed; }
    
public slots:
    void play();
    void pause();
    void togglePlayPause();
    void seekToPosition(double timestamp);

signals:
    void playbackStarted();
    void playbackPaused();
    void playbackEnded();
    void positionChanged(double timestamp);

private slots:
    void onTimingUpdate();

private:
    ClockManager* clockManager;
    bool playing;
    double playbackSpeed;
    std::function<void()> updateCallback;
};