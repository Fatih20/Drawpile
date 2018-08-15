/*
   Drawpile - a collaborative drawing program.

   Copyright (C) 2006-2018 Calle Laakkonen

   Drawpile is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Drawpile is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Drawpile.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef JOINDIALOG_H
#define JOINDIALOG_H

#include <QDialog>

class Ui_JoinDialog;

class ServerDiscoveryModel;
class SessionListingModel;

class QSortFilterProxyModel;

namespace dialogs {

class JoinDialog : public QDialog
{
	Q_OBJECT
public:
	explicit JoinDialog(const QUrl &defaultUrl, QWidget *parent=nullptr);
	~JoinDialog();

	//! Get the host address
	QString getAddress() const;

	//! Get the username
	QString getUserName() const;

	//! Should the session be recorded from the beginning
	bool recordSession() const;

	//! Get the join parameters encoded as an URL
	QUrl getUrl() const;

	//! Restore settings from configuration file
	void restoreSettings();

	//! Store settings in configuration file
	void rememberSettings() const;

private slots:
	void addressChanged(const QString &addr);
	void refreshListing();

private:
	void resolveRoomcode(const QString &roomcode, const QStringList &servers);
	void setListingError(const QString &message);

	Ui_JoinDialog *m_ui;
	QSortFilterProxyModel *m_filteredSessions;
	SessionListingModel *m_sessions;
	ServerDiscoveryModel *m_localServers;
};

}

#endif

