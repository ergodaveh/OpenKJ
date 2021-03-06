/*
 * Copyright (c) 2013-2016 Thomas Isaac Lightburn
 *
 *
 * This file is part of OpenKJ.
 *
 * OpenKJ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BMABSTRACEAUDIOBACKEND_H
#define BMABSTRACEAUDIOBACKEND_H

#include <QObject>

class BmAbstractAudioBackend : public QObject
{
    Q_OBJECT
public:
    explicit BmAbstractAudioBackend(QObject *parent = 0);
    enum State{PlayingState=0,PausedState,StoppedState,EndOfMediaState};
    virtual int volume() {return 0;}
    virtual qint64 position() {return 0;}
    virtual bool isMuted() {return 0;}
    virtual qint64 duration() {return 0;}
    virtual BmAbstractAudioBackend::State state() {return BmAbstractAudioBackend::StoppedState;}
    virtual bool canPitchShift() {return false;}
    virtual int pitchShift() {return 0;}
    virtual bool canFade() { return false; }
    QString msToMMSS(qint64 msec);
    virtual QString backendName() {return QString("Abstract - something is horribly wrong");}
    virtual bool canDetectSilence() { return false; }
    virtual bool isSilent() { return false; }
    virtual bool canDownmix() { return false; }
    virtual bool downmixChangeRequiresRestart() { return true; }
    virtual QStringList getOutputDevices();
    virtual void setOutputDevice(int deviceIndex) {Q_UNUSED(deviceIndex);}
    virtual bool stopping() {return false;}


signals:
    void audioAvailableChanged(bool);
    void bufferStatusChanged(int);
    void durationChanged(qint64);
    void mutedChanged(bool);
    void positionChanged(qint64);
    void stateChanged(BmAbstractAudioBackend::State);
    void videoAvailableChanged(bool);
    void volumeChanged(int);
    void silenceDetected();
    void pitchChanged(int);


public slots:
    virtual void play() {}
    virtual void pause() {}
    virtual void setMedia(QString filename) {Q_UNUSED(filename);}
    virtual void setMuted(bool muted) {Q_UNUSED(muted);}
    virtual void setPosition(qint64 position) {Q_UNUSED(position);}
    virtual void setVolume(int volume) {Q_UNUSED(volume);}
    virtual void stop(bool skipFade = false) {Q_UNUSED(skipFade);}
    virtual void setPitchShift(int pitchShift) {Q_UNUSED(pitchShift);}
    virtual void fadeOut() {}
    virtual void fadeIn() {}
    virtual void setUseFader(bool fade) {Q_UNUSED(fade);}
    virtual void setUseSilenceDetection(bool enabled) {Q_UNUSED(enabled);}
    virtual void setDownmix(bool enabled) {Q_UNUSED(enabled);}


};

#endif // BMABSTRACEAUDIOBACKEND_H
