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

#ifndef DATABASEUPDATETHREAD_H
#define DATABASEUPDATETHREAD_H

#include <QThread>
#include <QStringList>

class DbUpdateThread : public QThread
{
    Q_OBJECT
public:
    explicit DbUpdateThread(QObject *parent = 0);
    void run();
    QString path() const;
    void setPath(const QString &path);

signals:
    
public slots:

private:
    QString m_path;
    QStringList *findMediaFiles(QString directory);
    
};

#endif // DATABASEUPDATETHREAD_H
