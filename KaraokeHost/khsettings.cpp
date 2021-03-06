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

#include "khsettings.h"
#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QStandardPaths>

KhSettings::KhSettings(QObject *parent) :
    QObject(parent)
{
    QCoreApplication::setOrganizationName("OpenKJ");
    QCoreApplication::setOrganizationDomain("OpenKJ.org");
    QCoreApplication::setApplicationName("KaraokeHost");
    settings = new QSettings(this);
}

bool KhSettings::cdgWindowFullscreen()
{
    return settings->value("cdgWindowFullscreen", false).toBool();
}

void KhSettings::setCdgWindowFullscreen(bool fullScreen)
{
    settings->setValue("cdgWindowFullscreen", fullScreen);
    emit cdgWindowFullscreenChanged(fullScreen);
}


bool KhSettings::showCdgWindow()
{
    return settings->value("showCdgWindow", false).toBool();
}

void KhSettings::setShowCdgWindow(bool show)
{
    settings->setValue("showCdgWindow", show);
    emit cdgShowCdgWindowChanged(show);
}

void KhSettings::setCdgWindowFullscreenMonitor(int monitor)
{
    settings->setValue("cdgWindowFullScreenMonitor", monitor);
}

int KhSettings::cdgWindowFullScreenMonitor()
{
    //We default to the highest mointor present, by default, rather than the primary display.  Seems to make more sense
    //and will help prevent people from popping up a full screen window in front of the main window and getting confused.
    return settings->value("cdgWindowFullScreenMonitor", QApplication::desktop()->screenCount() - 1).toInt();
}

void KhSettings::saveWindowState(QWidget *window)
{
    settings->beginGroup(window->objectName());
    settings->setValue("size", window->size());
    settings->setValue("pos", window->pos());
    settings->endGroup();
}

void KhSettings::restoreWindowState(QWidget *window)
{

    settings->beginGroup(window->objectName());
    if (settings->contains("size") && settings->contains("pos"))
    {
        window->resize(settings->value("size", QSize(640, 480)).toSize());
        window->move(settings->value("pos", QPoint(100, 100)).toPoint());
    }
    settings->endGroup();
}

void KhSettings::saveColumnWidths(QTreeView *treeView)
{
    settings->beginGroup(treeView->objectName());
    settings->setValue("headerState", treeView->header()->saveState());
    settings->setValue("hiddenSections", treeView->header()->hiddenSectionCount());
    settings->setValue("sections", treeView->header()->count());
    settings->endGroup();
}

void KhSettings::saveColumnWidths(QTableView *tableView)
{
    settings->beginGroup(tableView->objectName());
    for (int i=0; i < tableView->horizontalHeader()->count(); i++)
    {
        settings->beginGroup(QString::number(i));
        settings->setValue("size", tableView->horizontalHeader()->sectionSize(i));
        settings->setValue("hidden", tableView->horizontalHeader()->isSectionHidden(i));
        settings->endGroup();
    }
    settings->endGroup();
}

void KhSettings::restoreColumnWidths(QTreeView *treeView)
{
    settings->beginGroup(treeView->objectName());
    if ((settings->contains("headerState")) && (settings->value("hiddenSections").toInt() == treeView->header()->hiddenSectionCount()) && (settings->value("sections").toInt() == treeView->header()->count()))
        treeView->header()->restoreState(settings->value("headerState").toByteArray());
    settings->endGroup();
}

void KhSettings::restoreColumnWidths(QTableView *tableView)
{
    settings->beginGroup(tableView->objectName());
    QStringList headers = settings->childGroups();
    for (int i=0; i < headers.size(); i++)
    {
        settings->beginGroup(headers.at(i));
        int section = headers.at(i).toInt();
        bool hidden = settings->value("hidden", false).toBool();
        int size = settings->value("size", 0).toInt();
        tableView->horizontalHeader()->resizeSection(section, size);
        tableView->horizontalHeader()->setSectionHidden(section, hidden);
        settings->endGroup();
    }
    settings->endGroup();
}

void KhSettings::saveSplitterState(QSplitter *splitter)
{
    settings->beginGroup(splitter->objectName());
    settings->setValue("splitterState", splitter->saveState());
    settings->endGroup();
}

void KhSettings::restoreSplitterState(QSplitter *splitter)
{
    settings->beginGroup(splitter->objectName());
    if (settings->contains("splitterState"))
        splitter->restoreState(settings->value("splitterState").toByteArray());
    settings->endGroup();
}

void KhSettings::setTickerFont(QFont font)
{
    settings->setValue("tickerFont", font.toString());
    emit tickerFontChanged();
}

QFont KhSettings::tickerFont()
{
    QFont font;
    font.fromString(settings->value("tickerFont", QApplication::font().toString()).toString());
    return font;
}

int KhSettings::tickerHeight()
{
    return settings->value("tickerHeight", 25).toInt();
}

void KhSettings::setTickerHeight(int height)
{
    settings->setValue("tickerHeight", height);
    emit tickerHeightChanged(height);
}

int KhSettings::tickerSpeed()
{
    return settings->value("tickerSpeed", 50).toInt();
}

void KhSettings::setTickerSpeed(int speed)
{
    settings->setValue("tickerSpeed", speed);
    emit tickerSpeedChanged();
}

QColor KhSettings::tickerTextColor()
{
    return settings->value("tickerTextColor", QApplication::palette().foreground().color()).value<QColor>();
}

void KhSettings::setTickerTextColor(QColor color)
{
    settings->setValue("tickerTextColor", color);
    emit tickerTextColorChanged();
}

QColor KhSettings::tickerBgColor()
{
    return settings->value("tickerBgColor", QApplication::palette().background().color()).value<QColor>();
}

void KhSettings::setTickerBgColor(QColor color)
{
    settings->setValue("tickerBgColor", color);
    emit tickerBgColorChanged();
}

bool KhSettings::tickerFullRotation()
{
    return settings->value("tickerFullRotation", true).toBool();
}

void KhSettings::setTickerFullRotation(bool full)
{
    settings->setValue("tickerFullRotation", full);
    emit tickerOutputModeChanged();
}

int KhSettings::tickerShowNumSingers()
{
    return settings->value("tickerShowNumSingers", 10).toInt();
}

void KhSettings::setTickerShowNumSingers(int limit)
{
    settings->setValue("tickerShowNumSingers", limit);
    emit tickerOutputModeChanged();
}

void KhSettings::setTickerEnabled(bool enable)
{
    settings->setValue("tickerEnabled", enable);
    emit tickerEnableChanged();
}

bool KhSettings::tickerEnabled()
{
    return settings->value("tickerEnabled", false).toBool();
}

bool KhSettings::requestServerEnabled()
{
    return settings->value("requestServerEnabled", false).toBool();
}

void KhSettings::setRequestServerEnabled(bool enable)
{
    settings->setValue("requestServerEnabled", enable);
}

QString KhSettings::requestServerUrl()
{
    return settings->value("requestServerUrl", "").toString();
}

void KhSettings::setRequestServerUrl(QString url)
{
    settings->setValue("requestServerUrl", url);
}

QString KhSettings::requestServerUsername()
{
    return settings->value("requestServerUsername","").toString();
}

void KhSettings::setRequestServerUsername(QString username)
{
    settings->setValue("requestServerUsername", username);
}

QString KhSettings::requestServerPassword()
{
    return settings->value("requestServerPassword", "").toString();
}

void KhSettings::setRequestServerPassword(QString password)
{
    settings->setValue("requestServerPassword", password);
}

bool KhSettings::requestServerIgnoreCertErrors()
{
    return settings->value("requestServerIgnoreCertErrors", false).toBool();
}

void KhSettings::setRequestServerIgnoreCertErrors(bool ignore)
{
    settings->setValue("requestServerIgnoreCertErrors", ignore);
}

bool KhSettings::audioUseFader()
{
    return settings->value("audioUseFader", true).toBool();
}

void KhSettings::setAudioUseFader(bool fader)
{
    settings->setValue("audioUseFader", fader);
}

int KhSettings::audioVolume()
{
    return settings->value("audioVolume", 50).toInt();
}

void KhSettings::setAudioVolume(int volume)
{
    settings->setValue("audioVolume", volume);
}

QString KhSettings::cdgDisplayBackgroundImage()
{
    return settings->value("cdgDisplayBackgroundImage", "").toString();
}

void KhSettings::setCdgDisplayBackgroundImage(QString imageFile)
{
    settings->setValue("cdgDisplayBackgroundImage", imageFile);
    emit cdgBgImageChanged();
}

bool KhSettings::audioDownmix()
{
    return settings->value("audioDownmix", false).toBool();
}

void KhSettings::setAudioDownmix(bool downmix)
{
    settings->setValue("audioDownmix", downmix);
}

bool KhSettings::audioDetectSilence()
{
    return settings->value("audioDetectSilence", false).toBool();
}

void KhSettings::setAudioDetectSilence(bool enabled)
{
    settings->setValue("audioDetectSilence", enabled);
}

QString KhSettings::audioOutputDevice()
{
    return settings->value("audioOutputDevice", 0).toString();
}

void KhSettings::setAudioOutputDevice(QString device)
{
    settings->setValue("audioOutputDevice", device);
}

int KhSettings::audioBackend()
{
    return settings->value("audioBackend", 0).toInt();
}

void KhSettings::setAudioBackend(int index)
{
    settings->setValue("audioBackend", index);
    emit audioBackendChanged(index);
}

QString KhSettings::recordingContainer()
{
    return settings->value("recordingContainer", "ogg").toString();
}

void KhSettings::setRecordingContainer(QString container)
{
    settings->setValue("recordingContainer", container);
    emit recordingSetupChanged();
}

QString KhSettings::recordingCodec()
{
    return settings->value("recordingCodec", "audio/vorbis").toString();
}

void KhSettings::setRecordingCodec(QString codec)
{
    settings->setValue("recordingCodec", codec);
    emit recordingSetupChanged();
}

QString KhSettings::recordingInput()
{
    return settings->value("recordingInput", "undefined").toString();
}

void KhSettings::setRecordingInput(QString input)
{
    settings->setValue("recordingInput", input);
    emit recordingSetupChanged();
}

QString KhSettings::recordingOutputDir()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    return settings->value("recordingOutputDir", defaultPath).toString();
}

void KhSettings::setRecordingOutputDir(QString path)
{
    settings->setValue("recordingOutputDir", path);
    emit recordingSetupChanged();
}

bool KhSettings::recordingEnabled()
{
    return settings->value("recordingEnabled", false).toBool();
}

void KhSettings::setRecordingEnabled(bool enabled)
{
    settings->setValue("recordingEnabled", enabled);
    emit recordingSetupChanged();
}

QString KhSettings::recordingRawExtension()
{
    return settings->value("recordingRawExtension", QString()).toString();
}

void KhSettings::setRecordingRawExtension(QString extension)
{
    settings->setValue("recordingRawExtension", extension);
}

int KhSettings::cdgVOffset()
{
    return settings->value("cdgVOffset", 0).toInt();
}

void KhSettings::setCdgVOffset(int pixels)
{
    settings->setValue("cdgVOffset", pixels);
    emit cdgVOffsetChanged(pixels);
}

int KhSettings::cdgHOffset()
{
    return settings->value("cdgHOffset", 0).toInt();
}

void KhSettings::setCdgHOffset(int pixels)
{
    settings->setValue("cdgHOffset", pixels);
    emit cdgHOffsetChanged(pixels);
}

int KhSettings::cdgVSizeAdjustment()
{
    return settings->value("cdgVSizeAdjustment", 0).toInt();
}

void KhSettings::setCdgVSizeAdjustment(int pixels)
{
    settings->setValue("cdgVSizeAdjustment", pixels);
    emit cdgVSizeAdjustmentChanged(pixels);
}

int KhSettings::cdgHSizeAdjustment()
{
    return settings->value("cdgHSizeAdjustment", 0).toInt();
}

void KhSettings::setCdgHSizeAdjustment(int pixels)
{
    settings->setValue("cdgHSizeAdjustment", pixels);
    emit cdgHSizeAdjustmentChanged(pixels);
}
