// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.5
import AchtungDieKurveDesktop

Window {
    visible: true
    width: 1920
    height: 1080
    title: qsTr("Achtung Die Kurve")
    color: "#00ffff"

    LobbyScreen {
        id: lobbyScreen
    }
}
