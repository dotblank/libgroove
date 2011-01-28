exists($$PWD/../qjson) {
    INCLUDEPATH = $$PWD/include
    unix:!symbian:maemo5 {
        message("Hidious workaround! The maemo5 toolchain does something wrong with -L/-l.")
        LIBS += $$OUT_PWD/../../qjson/lib/libqjson.so
    } else: LIBS += -L$$OUT_PWD/../../qjson/lib  -lqjson
} else {
    # QJson library
    CONFIG += link_pkgconfig
    PKGCONFIG += QJson
}
